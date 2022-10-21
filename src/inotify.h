#pragma once

#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <ctime>
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
#include <vector>

#include "filesystemEvent.h"
#include "notification.h"

constexpr int MAX_EVENTS = 4096;
constexpr int MAX_EPOLL_EVENTS = 10;
constexpr ssize_t EVENT_SIZE = (sizeof(inotify_event));
constexpr ssize_t EVENT_PAD = 16;

namespace ConfigCpp {

using EventObserver = std::function<void(Notification)>;

class Inotify {
public:
    Inotify();
    Inotify(const Inotify &rhs) = delete;
    Inotify(Inotify &&rhs) noexcept = delete;

    ~Inotify();

    Inotify &operator=(const Inotify &rhs) = delete;
    Inotify &operator=(Inotify &&rhs) = delete;

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
    void readEventsFromBuffer(uint8_t *buffer, ssize_t length, std::vector<FileSystemEvent> &events);
    void filterEvents(std::vector<FileSystemEvent> &events, std::queue<FileSystemEvent> &eventQueue);
    void sendStopSignal();
    bool exists(const std::string &path);

private:
    int m_error = 0;
    std::chrono::milliseconds m_eventTimeout;
    std::chrono::steady_clock::time_point m_lastEventTime;
    uint32_t m_eventMask;
    std::queue<FileSystemEvent> m_eventQueue;
    std::map<int, std::string> m_directoryMap;
    std::map<std::string, int> m_watchMap;
    int m_inotifyFd = 0;
    std::atomic_bool m_stopped = ATOMIC_VAR_INIT(false);
    int m_epollFd;
    epoll_event m_inotifyEpollEvent{};
    epoll_event m_stopPipeEpollEvent{};
    std::array<epoll_event, MAX_EPOLL_EVENTS> m_epollEvents;
    std::thread m_thread;

    std::function<void(FileSystemEvent)> m_onEventTimeout;
    std::vector<uint8_t> m_eventBuffer;

    std::array<int, 2> m_stopPipeFd{};
    const int m_pipeReadIdx = 0;
    const int m_pipeWriteIdx = 1;

    std::map<Event, EventObserver> m_eventObserver;
    EventObserver m_unexpectedEventObserver;
};

}  // namespace ConfigCpp