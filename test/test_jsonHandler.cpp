#include "gtest/gtest.h"

#include "jsonHandler.h"

static const std::string jsonLiteral = R"({
    "top-string": "bob",
    "top-int": 10,
    "top-bool": true,
    "top-double": 1.1234,
    "top-list": [ "item1", "item2", "item3" ],
    "top-dict": { "key1": "value1", "key2": "value2", "key3": "value3" },
    "nested-list": [
        [ "nested-item1-subitem1", "nested-item1-subitem2" ],
        [ "nested-item2-subitem1", "nested-item2-subitem2" ]
    ],
    "nested-dict": {
        "key1": {
            "key1-subkey1": "value1-1",
            "key1-subkey2": "value1-2"
        },
        "key2": {
            "key2-subkey1": "value2-1",
            "key2-subkey2": "value2-2"
        }
    }

}
)";

static const std::string jsonBadLiteral = R"({
    "top-string": "bob",
    "top-int": 10,
    "top-bool": true,
    "top-double": 1.1234,
    "top-list": [ "item1", "item2", "item3" ],
    "top-dict": { "key1": "value1", "key2": "value2", "key3": "value3" },
    "nested-list": [
        [ "nested-item1-subitem1", "nested-item1-subitem2" ],
        [ "nested-item2-subitem1", "nested-item2-subitem2" ]
    ],
    "nested-dict": {
        "key1": {
            "key1-subkey1": "value1-1",
            "key1-subkey2": "value1-2"
        },
        "key2": {
            "key2-subkey1": "value2-1"
            "key2-subkey2": "value2-2"
        }
    }

}
)";

TEST(JsonHandlerTest, IsSet) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;
    ConfigCpp::JsonHandler h(jsonLiteral, defaults, args);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-dict"), true);
    EXPECT_EQ(h.IsSet("top-list"), true);

    // Nested keys
    EXPECT_EQ(h.IsSet("top-dict.key1"), true);
    EXPECT_EQ(h.IsSet("top-dict.no-key"), false);

    EXPECT_EQ(h.IsSet("nested-dict.key1.key1-subkey1"), true);
    EXPECT_EQ(h.IsSet("nested-dict.key1.no-key"), false);

    // Repeat the top-level keys to ensure we haven't messed with the json structure
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-dict"), true);
    EXPECT_EQ(h.IsSet("top-list"), true);
}

TEST(JsonHandlerTest, Get) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;
    ConfigCpp::JsonHandler h(jsonLiteral, defaults, args);

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

    EXPECT_EQ(h.GetString("nested-dict.key1.key1-subkey1"), "value1-1");
}

TEST(JsonHandlerTest, CmdLineArgs) {
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

    ConfigCpp::JsonHandler h(jsonLiteral, defaults, args);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-dict"), true);
    EXPECT_EQ(h.IsSet("top-list"), true);
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

TEST(JsonHandlerTest, NewDefaults) {
    ConfigCpp::Values defaults;
    // Add 4 top-level defaults
    defaults.push_back(ConfigCpp::Value("def-bool", false));
    defaults.push_back(ConfigCpp::Value("def-int", 123));
    defaults.push_back(ConfigCpp::Value("def-double", 2.345));
    defaults.push_back(ConfigCpp::Value("def-string", "defaultStringVal"));
    // Add default for value that will be read in from the config
    defaults.push_back(ConfigCpp::Value("top-int", 555));
    // // Add nested defaults to existing json objects
    // defaults.push_back(ConfigCpp::Value("top-dict.key4",1234));
    // defaults.push_back(ConfigCpp::Value("nested-dict.key2.key2-subkey3","another default"));

    ConfigCpp::Values args;
    ConfigCpp::JsonHandler h(jsonLiteral, defaults, args);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-dict"), true);
    EXPECT_EQ(h.IsSet("top-list"), true);
    EXPECT_EQ(h.IsSet("top-int"), true);

    // Value from config takes precedence over default value
    EXPECT_EQ(h.GetInt("top-int"), 10);

    // Nested keys
    EXPECT_EQ(h.IsSet("top-dict.key1"), true);
    EXPECT_EQ(h.IsSet("top-dict.no-key"), false);

    EXPECT_EQ(h.IsSet("nested-dict.key1.key1-subkey1"), true);
    EXPECT_EQ(h.IsSet("nested-dict.key1.no-key"), false);

    // Repeat the top-level keys to ensure we haven't messed with the json structure
    EXPECT_EQ(h.IsSet("top-string"), true);
    EXPECT_EQ(h.IsSet("no-key"), false);
    EXPECT_EQ(h.IsSet("top-dict"), true);
    EXPECT_EQ(h.IsSet("top-list"), true);

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

    // // Nested values added as defaults
    // EXPECT_EQ(h.IsSet("top-dict.key4"),true);
    // EXPECT_EQ(h.GetInt("top-dict.key4"),1234);
    // EXPECT_EQ(h.IsSet("nested-dict.key2.key2-subkey3"),true);
    // EXPECT_EQ(h.GetString("nested-dict.key2.key2-subkey3"),"another default");

    std::cout << "Config data with defaults:\n" << h.GetConfig() << "\n";
}

TEST(JsonHandlerTest, InvalidJson) {
    ConfigCpp::Values defaults;
    ConfigCpp::Values args;

    try {
        ConfigCpp::JsonHandler h(jsonBadLiteral, defaults, args);

        FAIL() << "expected std::runtime_error";

    } catch (std::runtime_error &e) {

    } catch (...) {
        FAIL() << "expected std::runtime_error";
    }
}