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

TEST(JsonHandlerTest, IsSet) {
    ConfigCpp::JsonHandler h(jsonLiteral);

    // Top-level keys
    EXPECT_EQ(h.IsSet("top-string"),true);
    EXPECT_EQ(h.IsSet("no-key"),false);
    EXPECT_EQ(h.IsSet("top-dict"),true);
    EXPECT_EQ(h.IsSet("top-list"),true);

    // Nested keys
    EXPECT_EQ(h.IsSet("top-dict.key1"),true);
    EXPECT_EQ(h.IsSet("top-dict.no-key"),false);

    EXPECT_EQ(h.IsSet("nested-dict.key1.key1-subkey1"),true);
    EXPECT_EQ(h.IsSet("nested-dict.key1.no-key"),false);

    // Repeat the top-level keys to ensure we haven't messed with the yaml structure
    EXPECT_EQ(h.IsSet("top-string"),true);
    EXPECT_EQ(h.IsSet("no-key"),false);
    EXPECT_EQ(h.IsSet("top-dict"),true);
    EXPECT_EQ(h.IsSet("top-list"),true);

}

TEST(JsonHandlerTest, Get) {
    ConfigCpp::JsonHandler h(jsonLiteral);

    EXPECT_EQ(h.GetBool("top-bool"),true);
    EXPECT_EQ(h.GetBool("no-key"),false);
    // Type conflict - Implicit conversion
    EXPECT_EQ(h.GetInt("top-bool"),1);

    EXPECT_EQ(h.GetInt("top-int"),10);
    EXPECT_EQ(h.GetInt("no-key"),0);
    // Type conflict - revert to default
    EXPECT_EQ(h.GetBool("top-int"),false);

    EXPECT_EQ(h.GetString("top-string"),"bob");
    EXPECT_EQ(h.GetString("no-key"),"");
    // Type conflicts - revert to default
    EXPECT_EQ(h.GetInt("top-string"),0);
    EXPECT_EQ(h.GetString("top-int"),"");

    EXPECT_EQ(h.GetDouble("top-double"),1.1234);
    EXPECT_EQ(h.GetDouble("no-key"),0.0);
    // Type conflict - implicit conversion
    EXPECT_EQ(h.GetInt("top-double"),1);
    EXPECT_EQ(h.GetDouble("top-int"),10.0);

    EXPECT_EQ(h.GetString("nested-dict.key1.key1-subkey1"),"value1-1");

}