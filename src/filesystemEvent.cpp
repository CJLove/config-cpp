#include "filesystemEvent.h"

#include <sys/inotify.h>

namespace ConfigCpp {

FileSystemEvent::FileSystemEvent(const int wd, uint32_t mask, std::string path,
                                 const std::chrono::steady_clock::time_point& eventTime)
    : m_wd(wd), m_mask(mask), m_path(std::move(path)), m_eventTime(eventTime) 
    
{}

}  // namespace ConfigCpp