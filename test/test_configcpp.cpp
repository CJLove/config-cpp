#include "gtest/gtest.h"

#include "config-cpp/config-cpp.h"

#include <atomic>
#include <fstream>

static std::atomic<int> modifyCount;

static void onModify(ConfigCpp::ConfigCpp &config) {
    config.ReadInConfig();

    std::cout << "onModify():\n" << config.GetConfigData() << "\n";
    modifyCount++;
}

#if defined(JSON_SUPPORT)
TEST(ConfigCppTest, JsonUpdate) {
    int argc = 0;
    char **argv = nullptr;

    // Create minimal json config file
    unlink("/tmp/config.json");
    unlink("/tmp/config.yaml");
    std::ofstream f("/tmp/config.json");
    f << "{}";
    f.close();
    f.flush();
    modifyCount = 0;

    ConfigCpp::ConfigCpp config(argc, argv);
    config.SetConfigName("config");
    config.AddConfigPath("/tmp");
    EXPECT_EQ(config.ReadInConfig(), true);
    config.OnConfigChange(onModify);
    config.WatchConfig();

    std::ofstream ff("/tmp/config.json");
    ff << "{ \"new\": 10 }";
    ff.close();

    sleep(2);
    EXPECT_EQ(modifyCount, 1);

    EXPECT_EQ(config.ReadInConfig(), true);
    EXPECT_EQ(config.GetInt("new"), 10);

    unlink("/tmp/config.json");
}

TEST(ConfigCppTest, JsonDefaults) {
    int argc = 0;
    char **argv = nullptr;
    ConfigCpp::ConfigCpp config(argc, argv);

    config.SetConfigName("config");
    config.AddConfigPath("../inputs/json");
    config.AddConfigPath("../../inputs/json");

    // Create new defaults
    config.SetDefault("def-bool", true);
    config.SetDefault("def-int", 23);
    config.SetDefault("def-string", "mystring");
    config.SetDefault("def-string2", std::string("mystring2"));
    config.SetDefault("def-double", 5.555);

    // Set default which is overridden from config
    config.SetDefault("top-int", 555);

    EXPECT_EQ(config.ReadInConfig(), true);

    EXPECT_EQ(config.GetBool("def-bool"), true);
    EXPECT_EQ(config.GetInt("def-int"), 23);
    EXPECT_EQ(config.GetString("def-string"), "mystring");
    EXPECT_EQ(config.GetDouble("def-double"), 5.555);

    EXPECT_EQ(config.GetInt("top-int"), 10);
}

TEST(ConfigCppTest, Json) {
    int argc = 0;
    char **argv = nullptr;
    ConfigCpp::ConfigCpp config(argc, argv);

    config.SetConfigName("config");
    config.AddConfigPath("../inputs/json");
    config.AddConfigPath("../../inputs/json");

    EXPECT_EQ(config.ReadInConfig(), true);

    EXPECT_EQ(config.GetInt("top-int"), 10);
    EXPECT_EQ(config.GetString("top-string"), "bob");
    EXPECT_EQ(config.GetBool("top-bool"), true);
    EXPECT_EQ(config.GetDouble("top-double"), 1.234);

    EXPECT_EQ(config.IsSet("top-int"), true);
    EXPECT_EQ(config.IsSet("not-there"), false);

    EXPECT_EQ(config.GetBool("not-there"), false);
    EXPECT_EQ(config.GetInt("not-there"), 0);
    EXPECT_EQ(config.GetDouble("not-there"), 0.0);
    EXPECT_EQ(config.GetString("not-there"), "");

    EXPECT_EQ(config.GetString("top-dict.key1"), "value1");
    EXPECT_EQ(config.GetString("nested-dict.key1.key1-subkey2"), "value1-2");

    std::string data = config.GetConfigData();
    EXPECT_TRUE(data.size() > 0);

    EXPECT_EQ(config.GetConfigType(), ConfigCpp::ConfigType::JSON);
}

TEST(ConfigCppTest, JsonCmdLineArgs) {
    int argc = 7;
    const char *argv[] = {"test", "--top-string", "cmdLineString", "--top-int", "555", "--top-double", "555.555"};
    ConfigCpp::ConfigCpp config(argc, (char **)argv);

    config.SetConfigName("config");
    config.AddConfigPath("../inputs/json");
    config.AddConfigPath("../../inputs/json");
    config.AddBoolOption("b,top-bool", "Bool option");
    config.AddIntOption("t,top-int", "Integer option");
    config.AddStringOption("s,top-string", "String option");
    config.AddDoubleOption("d,top-double", "Double option");
    EXPECT_EQ(config.ReadInConfig(), true);

    // Note: can't override a boolean flag set to TRUE in config file via command-line argument
    EXPECT_EQ(config.GetInt("top-int"), 555);
    EXPECT_EQ(config.GetString("top-string"), "cmdLineString");
    EXPECT_EQ(config.GetDouble("top-double"), 555.555);
}

TEST(ConfigCppTest, NoJson) {
    int argc = 0;
    char **argv = nullptr;
    ConfigCpp::ConfigCpp config(argc, argv);

    config.SetConfigName("other");
    config.AddConfigPath("../inputs/json");
    config.AddConfigPath("../../inputs/json");

    EXPECT_EQ(config.ReadInConfig(), false);
}
#endif

#if defined(YAML_SUPPORT)
TEST(ConfigCppTest, YamlUpdate) {
    int argc = 0;
    char **argv = nullptr;

    // Create minimal json config file
    unlink("/tmp/config.json");
    unlink("/tmp/config.yaml");
    std::ofstream f("/tmp/config.yaml");
    f << "---";
    f.close();
    f.flush();
    modifyCount = 0;

    ConfigCpp::ConfigCpp config(argc, argv);
    config.SetConfigName("config");
    config.AddConfigPath("/tmp");
    EXPECT_EQ(config.ReadInConfig(), true);
    config.OnConfigChange(onModify);
    config.WatchConfig();

    std::ofstream ff("/tmp/config.yaml");
    ff << "---\n";
    ff << "new: 10";
    ff.close();

    sleep(2);
    EXPECT_EQ(modifyCount, 1);

    EXPECT_EQ(config.ReadInConfig(), true);
    EXPECT_EQ(config.GetInt("new"), 10);
    unlink("/tmp/config.yaml");
}

TEST(ConfigCppTest, YamlDefaults) {
    int argc = 0;
    char **argv = nullptr;
    ConfigCpp::ConfigCpp config(argc, argv);

    config.SetConfigName("config");
    config.AddConfigPath("../inputs/yaml");
    config.AddConfigPath("../../inputs/yaml");

    // Create new defaults
    config.SetDefault("def-bool", true);
    config.SetDefault("def-int", 23);
    config.SetDefault("def-string", "mystring");
    config.SetDefault("def-string2", std::string("mystring2"));
    config.SetDefault("def-double", 5.555);

    // Set default which is overridden from config
    config.SetDefault("top-int", 555);

    EXPECT_EQ(config.ReadInConfig(), true);

    EXPECT_EQ(config.GetBool("def-bool"), true);
    EXPECT_EQ(config.GetInt("def-int"), 23);
    EXPECT_EQ(config.GetString("def-string"), "mystring");
    EXPECT_EQ(config.GetDouble("def-double"), 5.555);

    EXPECT_EQ(config.GetInt("top-int"), 10);
}

TEST(ConfigCppTest, Yaml) {
    int argc = 0;
    char **argv = nullptr;
    ConfigCpp::ConfigCpp config(argc, argv);

    config.SetConfigName("config");
    config.AddConfigPath("../inputs/yaml");
    config.AddConfigPath("../../inputs/yaml");

    EXPECT_EQ(config.ReadInConfig(), true);

    EXPECT_EQ(config.GetInt("top-int"), 10);
    EXPECT_EQ(config.GetString("top-string"), "bob");
    EXPECT_EQ(config.GetBool("top-bool"), true);
    EXPECT_EQ(config.GetDouble("top-double"), 1.234);

    EXPECT_EQ(config.GetBool("not-there"), false);
    EXPECT_EQ(config.GetInt("not-there"), 0);
    EXPECT_EQ(config.GetDouble("not-there"), 0.0);
    EXPECT_EQ(config.GetString("not-there"), "");

    EXPECT_EQ(config.GetInt("not-there"), 0);
    EXPECT_EQ(config.GetString("top-dict.key1"), "value1");
    EXPECT_EQ(config.GetString("nested-dict.key1.key1-subkey2"), "value1-2");

    std::string data = config.GetConfigData();
    EXPECT_TRUE(data.size() > 0);

    EXPECT_EQ(config.GetConfigType(), ConfigCpp::ConfigType::YAML);
}

TEST(ConfigCppTest, YamlCmdLineArgs) {
    int argc = 7;
    const char *argv[] = {"test", "--top-string", "cmdLineString", "--top-int", "555", "--top-double", "555.555"};
    ConfigCpp::ConfigCpp config(argc, (char **)argv);

    config.SetConfigName("config");
    config.AddConfigPath("../inputs/yaml");
    config.AddConfigPath("../../inputs/yaml");
    config.AddBoolOption("b,top-bool", "Bool option");
    config.AddIntOption("t,top-int", "Integer option");
    config.AddStringOption("s,top-string", "String option");
    config.AddDoubleOption("d,top-double", "Double option");
    EXPECT_EQ(config.ReadInConfig(), true);

    // Note: can't override a boolean flag set to TRUE in config file via command-line argument
    EXPECT_EQ(config.GetInt("top-int"), 555);
    EXPECT_EQ(config.GetString("top-string"), "cmdLineString");
    EXPECT_EQ(config.GetDouble("top-double"), 555.555);
}

TEST(ConfigCppTest, NoYaml) {
    int argc = 0;
    char **argv = nullptr;
    ConfigCpp::ConfigCpp config(argc, argv);

    config.SetConfigName("other");
    config.AddConfigPath("../inputs/yaml");
    config.AddConfigPath("../../inputs/yaml");

    EXPECT_EQ(config.ReadInConfig(), false);
}
#endif