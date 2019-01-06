#include "gtest/gtest.h"

#include "event.h"

using namespace ConfigCpp;

TEST(EventTest, ContainsEvent) {

    Event e = Event::open | Event::access;

    EXPECT_EQ(containsEvent(e, Event::access), true);

    EXPECT_EQ(containsEvent(e, Event::close), false);

    EXPECT_EQ(containsEvent(e, e), true);

    Event e2 = (Event::open | Event::close);

    Event e3 = (e2 & Event::open);

    EXPECT_EQ(containsEvent(e3, Event::open), true);

    EXPECT_EQ(containsEvent(e3, Event::close), false);
}

TEST(EventTest, Iostream) {
    std::stringstream stream;
    stream << Event::all;
    EXPECT_EQ(
        stream.str(),
        "access attrib close_write close_nowrite create remove remove_self modify move_self moved_from moved_to open close ");
}
