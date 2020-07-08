#include "gtest/gtest.h"

#include "tomlHandler.h"

static const std::string tomlLiteral = R"(top-string = "bob"
top-int = 10
top-bool = true
top-double =1.1234
top-array = [ "item1", "item2", "item3" ]
nested-array = [
    [ "nested-item1-subitem1", "nested-item1-subitem2"],
    [ "nested-item2-subitem1", "nested-item2-subitem2"]
]

[top-table]
key1 = "value1"
key2 = "value2"
key3 = "value3"

# Nested table
[nested-table.key1]

key1-subkey1 = "value1-1"
key1-subkey2 = "value1-2"

[nested-table.key2]

key2-subkey1 = "value2-1"
key2-subkey2 = "value2-2"
)";

static const std::string tomlBadLiteral = R"(top-string = "bob"
top-int = 10
top-bool = true
top-double =1.1234
top-array = [ "item1", "item2", "item3" 
nested-array = [
    [ "nested-item1-subitem1", "nested-item1-subitem2"],
    [ "nested-item2-subitem1", "nested-item2-subitem2"]
]

[top-table]
key1 = "value1"
key2 = "value2"
key3 = "value3"

# Nested table
[nested-table.key1]

key1-subkey1 = "value1-1"
key1-subkey2 = "value1-2"

[nested-table.key2]

key2-subkey1 = "value2-1"
key2-subkey2 = "value2-2"
)";

TEST(TomlHandlerTest, IsSet) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;
    ConfigCpp::TomlHandler h(tomlLiteral, defaults, args);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-table"), true);
    EXPECT_EQ(h.IsSet("top-array"), true);

    // Nested keys
    EXPECT_EQ(h.IsSet("top-table.key1"), true);
    EXPECT_EQ(h.IsSet("top-table.no-key"), false);

    EXPECT_EQ(h.IsSet("nested-table.key1.key1-subkey1"), true);
    EXPECT_EQ(h.IsSet("nested-table.key1.no-key"), false);

    // Repeat the top-level keys to ensure we haven't messed with the toml structure
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-table"), true);
    EXPECT_EQ(h.IsSet("top-array"), true);

}

TEST(TomlHandlerTest, NewDefaults) {
    ConfigCpp::Values defaults;
    // Add 4 top-level defaults
    defaults.push_back(ConfigCpp::Value("def-bool", false));
    defaults.push_back(ConfigCpp::Value("def-int", 123));
    defaults.push_back(ConfigCpp::Value("def-double", 2.345));
    defaults.push_back(ConfigCpp::Value("def-string", "defaultStringVal"));
    // Add default for value that will be read in from the config
    defaults.push_back(ConfigCpp::Value("top-int", 555));
    // 

    ConfigCpp::Values args;
    ConfigCpp::TomlHandler h(tomlLiteral, defaults, args);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-table"), true);
    EXPECT_EQ(h.IsSet("top-array"), true);
    EXPECT_EQ(h.IsSet("top-int"), true);

    // Value from config takes precedence over default value
    EXPECT_EQ(h.GetInt("top-int"), 10);

    // Nested keys
    EXPECT_EQ(h.IsSet("top-table.key1"), true);
    EXPECT_EQ(h.IsSet("top-table.no-key"), false);

    EXPECT_EQ(h.IsSet("nested-table.key1.key1-subkey1"), true);
    EXPECT_EQ(h.IsSet("nested-table.key1.no-key"), false);

    // Repeat the top-level keys to ensure we haven't messed with the toml structure
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-table"), true);
    EXPECT_EQ(h.IsSet("top-array"), true);

    // Values added as defaults
    EXPECT_EQ(h.IsSet("def-bool"), true);
    EXPECT_EQ(h.GetBool("def-bool"), false);
    EXPECT_EQ(h.IsSet("def-int"), true);
    EXPECT_EQ(h.GetInt("def-int"), 123);
    EXPECT_EQ(h.IsSet("def-double"), true);
    EXPECT_EQ(h.GetDouble("def-double"), 2.345);
    EXPECT_EQ(h.IsSet("def-string"), true);
    EXPECT_EQ(h.GetString("def-string"), "defaultStringVal");
    EXPECT_EQ(h.IsSet("def-nokey"), false);

    std::cout << "Config data with defaults:\n" << h.GetConfig() << "\n";
}


TEST(TomlHandlerTest, CmdLineArgs) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;
    // Add 4 top-level arguments
    args.push_back(ConfigCpp::Value("def-bool", false));
    args.push_back(ConfigCpp::Value("def-int", 123));
    args.push_back(ConfigCpp::Value("def-double", 2.345));
    args.push_back(ConfigCpp::Value("def-string", "defaultStringVal"));
    // Add arguments superceding those read in from the config
    args.push_back(ConfigCpp::Value("top-int", 555));
    args.push_back(ConfigCpp::Value("top-string", "cmdLineString"));
    args.push_back(ConfigCpp::Value("top-double", 555.555));

    ConfigCpp::TomlHandler h(tomlLiteral, defaults, args);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-table"), true);
    EXPECT_EQ(h.IsSet("top-array"), true);
    EXPECT_EQ(h.IsSet("top-int"), true);

    // Values added as defaults
    EXPECT_EQ(h.IsSet("def-bool"), true);
    EXPECT_EQ(h.GetBool("def-bool"), false);
    EXPECT_EQ(h.IsSet("def-int"), true);
    EXPECT_EQ(h.GetInt("def-int"), 123);
    EXPECT_EQ(h.IsSet("def-double"), true);
    EXPECT_EQ(h.GetDouble("def-double"), 2.345);
    EXPECT_EQ(h.IsSet("def-string"), true);
    EXPECT_EQ(h.GetString("def-string"), "defaultStringVal");
    EXPECT_EQ(h.IsSet("def-nokey"), false);

    EXPECT_EQ(h.GetInt("top-int"), 555);
    EXPECT_EQ(h.GetString("top-string"), "cmdLineString");
    EXPECT_EQ(h.GetDouble("top-double"), 555.555);
}


TEST(TomlHandlerTest, Get) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;
    ConfigCpp::TomlHandler h(tomlLiteral, defaults, args);

    EXPECT_EQ(h.GetBool("top-bool"), true);
    EXPECT_EQ(h.GetBool("no-key"), false);
    // Type conflict - Implicit conversion
    EXPECT_EQ(h.GetInt("top-bool"), 1);

    EXPECT_EQ(h.GetInt("top-int"), 10);
    EXPECT_EQ(h.GetInt("no-key"), 0);
    // Type conflict - revert to default
    EXPECT_EQ(h.GetBool("top-int"), false);

    EXPECT_EQ(h.GetString("top-string"), "bob");
    EXPECT_EQ(h.GetString("no-key"), "");
    // Type conflicts - revert to default
    EXPECT_EQ(h.GetInt("top-string"), 0);
    EXPECT_EQ(h.GetString("top-int"), "");

    EXPECT_EQ(h.GetDouble("top-double"), 1.1234);
    EXPECT_EQ(h.GetDouble("no-key"), 0.0);
    // Type conflict - implicit conversion
    EXPECT_EQ(h.GetInt("top-double"), 1);
    EXPECT_EQ(h.GetDouble("top-int"), 10.0);

    EXPECT_EQ(h.GetString("nested-table.key1.key1-subkey1"), "value1-1");
}

TEST(TomlHandlerTest, InvalidToml) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;

    try {
        ConfigCpp::TomlHandler h(tomlBadLiteral, defaults, args);

        FAIL() << "expected std::runtime_error";

    } catch (std::runtime_error &e) {

    } catch (...) {
        FAIL() << "expected std::runtime_error";
    }
}