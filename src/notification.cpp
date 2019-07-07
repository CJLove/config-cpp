#include "notification.h"

namespace ConfigCpp {
    
Notification::Notification(const Event& event, std::string path, std::chrono::steady_clock::time_point time)
    : m_event(event), m_path(std::move(path)), m_time(time) 
{}

}  // namespace ConfigCpp