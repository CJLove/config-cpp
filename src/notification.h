#pragma once
#include "event.h"
#include <chrono>
#include <string>

namespace ConfigCpp {

class Notification {
  public:
    Notification(
        const Event& event,
        const std::string& path,
        std::chrono::steady_clock::time_point time);

  public:
    const Event m_event;
    const std::string m_path;
    const std::chrono::steady_clock::time_point m_time;
};

}