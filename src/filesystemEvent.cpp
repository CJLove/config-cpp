#include "filesystemEvent.h"

#include <sys/inotify.h>

namespace ConfigCpp {

    FileSystemEvent::FileSystemEvent(): m_wd(0), m_mask(0) {}

FileSystemEvent::FileSystemEvent(const int wd, uint32_t mask, const std::string& path,
                                 const std::chrono::steady_clock::time_point& eventTime)
    : m_wd(wd), m_mask(mask), m_path(path), m_eventTime(eventTime) {}

FileSystemEvent::~FileSystemEvent() {}

}  // namespace ConfigCpp