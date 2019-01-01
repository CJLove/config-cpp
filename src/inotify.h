#pragma once

#include <assert.h>
#include <atomic>
#include <chrono>
#include <errno.h>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <thread>
#include <time.h>
#include <vector>
#include <thread>

#include "notification.h"
#include "filesystemEvent.h"

#define MAX_EVENTS 4096
#define MAX_EPOLL_EVENTS 10
#define EVENT_SIZE (sizeof(inotify_event))

namespace ConfigCpp {

using EventObserver = std::function<void(Notification)>;

class Inotify {
public:
    Inotify();
    ~Inotify();

    void watchFile(const std::string &file);
    void unwatchFile(const std::string &file);
    void onEvent(Event event, EventObserver observer);
    void onUnexpectedEvent(EventObserver observer);
    void setEventTimeout(std::chrono::milliseconds eventTimeout, std::function<void(FileSystemEvent)> onEventTimeout);

    void stop();
    void start();
    bool hasStopped();

    void run();

private:
    bool getNextEvent(FileSystemEvent &event);
    std::string wdToPath(int wd);
    bool isOnTimeout(const std::chrono::steady_clock::time_point &eventTime);
    void removeWatch(int wd);
    ssize_t readEventsIntoBuffer(std::vector<uint8_t> &eventBuffer);
    void readEventsFromBuffer(uint8_t *buffer, int length, std::vector<FileSystemEvent> &events);
    void filterEvents(std::vector<FileSystemEvent> &events, std::queue<FileSystemEvent> &eventQueue);
    void sendStopSignal();
    bool isDirectory(const std::string &path);
    bool exists(const std::string &path);

private:
    int m_error;
    std::chrono::milliseconds m_eventTimeout;
    std::chrono::steady_clock::time_point m_lastEventTime;
    uint32_t m_eventMask;
    uint32_t m_threadSleep;
    std::queue<FileSystemEvent> m_eventQueue;
    std::map<int, std::string> m_directoryMap;
    std::map<std::string, int> m_watchMap;
    int m_inotifyFd;
    std::atomic<bool> m_stopped;
    int m_epollFd;
    epoll_event m_inotifyEpollEvent;
    epoll_event m_stopPipeEpollEvent;
    epoll_event m_epollEvents[MAX_EPOLL_EVENTS];
    std::thread m_thread;

    std::function<void(FileSystemEvent)> m_onEventTimeout;
    std::vector<uint8_t> m_eventBuffer;

    int m_stopPipeFd[2];
    const int m_pipeReadIdx;
    const int m_pipeWriteIdx;

    std::map<Event, EventObserver> m_eventObserver;
    EventObserver m_unexpectedEventObserver;
};

}  // namespace ConfigCpp