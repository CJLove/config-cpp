#include "gtest/gtest.h"

#include "notification.h"

using namespace ConfigCpp;

TEST(NotificationTest, Basics) {
    auto now = std::chrono::steady_clock::now();
    Notification note(Event::open, "/path/to/file", now);

    EXPECT_EQ(note.m_event, Event::open);
    EXPECT_EQ(note.m_path, "/path/to/file");
    EXPECT_EQ(note.m_time, now);
}