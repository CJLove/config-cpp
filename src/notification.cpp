#include "notification.h"

namespace ConfigCpp {
    
Notification::Notification(const Event& event, const std::string& path, std::chrono::steady_clock::time_point time)
    : m_event(event), m_path(path), m_time(time) {}

}  // namespace ConfigCpp