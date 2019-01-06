#include "gtest/gtest.h"

#include "inotify.h"

#include <fstream>
#include <unistd.h>

using namespace ConfigCpp;

void createFile(const std::string &file) {
    std::ofstream f(file);
    f << "Blah";
    f.close();
}

void createSymlink(const std::string &file) { symlink(file.c_str(), "/tmp/testfile"); }

void modifyFile() {
    std::ofstream f("/tmp/testfile", std::ios_base::app);
    f << "MoreBlah";
    f.close();
}

void accessFile() {
    std::ifstream f("/tmp/testfile");
    std::stringstream stream;
    stream << f.rdbuf();
}

void removeFile(const std::string &file) { unlink(file.c_str()); }

static std::atomic<int> createCount;
static std::atomic<int> modifyCount;
static std::atomic<int> accessCount;
static std::atomic<int> removeCount;
static std::atomic<int> unexpectedCount;

static void onUnexpected(Notification note) {
    std::cout << "onUnexpected() event " << note.m_event << " for " << note.m_path << "\n";
    unexpectedCount++;
}

static void onCreate(Notification note) {
    std::cout << "onCreate() event " << note.m_event << " for " << note.m_path << "\n";
    createCount++;
}

static void onModify(Notification note) {
    std::cout << "onModify() event " << note.m_event << " for " << note.m_path << "\n";
    modifyCount++;
}

static void onAccess(Notification note) {
    std::cout << "onAccess() event " << note.m_event << " for " << note.m_path << "\n";
    accessCount++;
}

static void onRemove(Notification note) {
    std::cout << "onRemove() event " << note.m_event << " for " << note.m_path << "\n";
    removeCount++;
}

TEST(InotifyTest, File) {
    Inotify note;
    createCount = 0;
    modifyCount = 0;
    accessCount = 0;
    removeCount = 0;

    removeFile("/tmp/testfile");

    createFile("/tmp/testfile");

    note.watchFile("/tmp/testfile");
    note.onEvent(Event::create, onCreate);
    note.onEvent(Event::modify, onModify);
    note.onEvent(Event::access, onAccess);
    note.onEvent(Event::remove_self | Event::remove, onRemove);
    note.onUnexpectedEvent(onUnexpected);
    note.start();

    modifyFile();
    sleep(2);

    accessFile();
    sleep(2);

    removeFile("/tmp/testfile");
    sleep(2);

    EXPECT_EQ(modifyCount, 1);
    EXPECT_EQ(accessCount, 1);
    EXPECT_EQ(removeCount, 1);
    EXPECT_TRUE(unexpectedCount > 0);
    note.stop();
}

TEST(InotifyTest, BadFile) {
    Inotify note;
    createCount = 0;

    // Test ignoring watch on file which isn't there
    removeFile("/tmp/testfile");
    note.watchFile("/tmp/testfile");
    note.onEvent(Event::create, onCreate);

    EXPECT_EQ(createCount, 0);
    note.stop();
}

TEST(InotifyTest, Symlink) {
    Inotify note;
    createCount = 0;
    modifyCount = 0;
    accessCount = 0;
    removeCount = 0;

    removeFile("/tmp/testlink");

    createFile("/tmp/testlink");

    createSymlink("/tmp/testlink");

    note.watchFile("/tmp/testfile");
    note.onEvent(Event::create, onCreate);
    note.onEvent(Event::modify, onModify);
    note.onEvent(Event::access, onAccess);
    note.onEvent(Event::remove_self | Event::remove, onRemove);
    note.start();

    modifyFile();
    sleep(2);

    accessFile();
    sleep(2);

    removeFile("/tmp/testlink");
    sleep(2);

    EXPECT_EQ(modifyCount, 1);
    EXPECT_EQ(accessCount, 1);
    EXPECT_EQ(removeCount, 1);

    removeFile("/tmp/testfile");

    note.unwatchFile("/tmp/testfile");

    EXPECT_EQ(note.hasStopped(), false);

    note.stop();
}