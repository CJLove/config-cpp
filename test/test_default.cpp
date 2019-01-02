#include "gtest/gtest.h"

#include "default.h"

TEST(DefaultTest, DefaultType) {
    ConfigCpp::DefaultValue def1("aaa", true);
    EXPECT_EQ(def1.m_key, "aaa");
    EXPECT_EQ(def1.m_bool, true);
    EXPECT_EQ(def1.m_type, ConfigCpp::DefaultValue::BOOL);

    ConfigCpp::DefaultValue def2("bbb", (int)55);
    EXPECT_EQ(def2.m_key, "bbb");
    EXPECT_EQ(def2.m_int, 55);
    EXPECT_EQ(def2.m_type, ConfigCpp::DefaultValue::INT);

    ConfigCpp::DefaultValue def3("ccc", (double)1.1234);
    EXPECT_EQ(def3.m_key, "ccc");
    EXPECT_EQ(def3.m_double, 1.1234);
    EXPECT_EQ(def3.m_type, ConfigCpp::DefaultValue::DOUBLE);

    ConfigCpp::DefaultValue def4("ddd", "stringValue");
    EXPECT_EQ(def4.m_key, "ddd");
    EXPECT_EQ(def4.m_string, "stringValue");
    EXPECT_EQ(def4.m_type, ConfigCpp::DefaultValue::STRING);

    std::string str{"stringValue"};
    ConfigCpp::DefaultValue def5("eee", str);
    EXPECT_EQ(def5.m_key, "eee");
    EXPECT_EQ(def5.m_string, "stringValue");
    EXPECT_EQ(def5.m_type, ConfigCpp::DefaultValue::STRING);

    ConfigCpp::DefaultValue copy1(def1);
    EXPECT_EQ(copy1.m_key, "aaa");
    EXPECT_EQ(copy1.m_bool, true);
    EXPECT_EQ(copy1.m_type, ConfigCpp::DefaultValue::BOOL);
}
