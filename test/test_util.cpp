#include "gtest/gtest.h"

#include "util.h"

TEST(UtilTest, SplitString) {
    std::string key1{"key"};
    auto result1 = ConfigCpp::split(key1, '.');

    EXPECT_EQ(result1.size(), 1);
    EXPECT_EQ(result1[0], "key");

    std::string key2{"key.subkey"};
    auto result2 = ConfigCpp::split(key2, '.');

    EXPECT_EQ(result2.size(), 2);
    EXPECT_EQ(result2[0], "key");
    EXPECT_EQ(result2[1], "subkey");

    std::string key3{"key.subkey.subsubkey"};
    auto result3 = ConfigCpp::split(key3, '.');

    EXPECT_EQ(result3.size(), 3);
    EXPECT_EQ(result3[0], "key");
    EXPECT_EQ(result3[1], "subkey");
    EXPECT_EQ(result3[2], "subsubkey");
}

TEST(UtilTest, Prefix) {
    std::string key1{"key"};
    auto result1 = ConfigCpp::prefix(key1);
    EXPECT_EQ(result1, "key");

    std::string key2{"key.subkey"};
    auto result2 = ConfigCpp::prefix(key2);
    EXPECT_EQ(result2, "key");

    std::string key3{"key.subkey.subsubkey"};
    auto result3 = ConfigCpp::prefix(key3);
    EXPECT_EQ(result3, "key.subkey");
}

TEST(UtilTest, Symlink) {
    std::string key1{"/etc/config/config.json"};
    auto result1 = ConfigCpp::symlinkName(key1);
    EXPECT_EQ(result1, "/etc/config/..data");

    std::string key2{"config/config.json"};
    auto result2 = ConfigCpp::symlinkName(key2);
    EXPECT_EQ(result2, "config/..data");
}

TEST(UtilTest, NormalizePath) {
    std::string path1 = "/path/to/config";
    auto result1 = ConfigCpp::normalizePath(path1);
    EXPECT_EQ(result1, "/path/to/config/");

    std::string path2 = "/path/to/config/";
    auto result2 = ConfigCpp::normalizePath(path2);
    EXPECT_EQ(result2, "/path/to/config/");
}

TEST(UtilTest, LongOption) {
    std::string option1 = "t,top-int";
    auto result1 = ConfigCpp::longOption(option1);
    EXPECT_EQ(result1, "top-int");

    std::string option2 = "top-string";
    auto result2 = ConfigCpp::longOption(option2);
    EXPECT_EQ(result2, "top-string");
}