#ifndef CONFIG_H
#define CONFIG_H
#include <map>
#include <string>

class config
{
public:
    static config* getSingleton();
    void test();
    const char* getValue(const char* key);
    void  setValue(const char* key, const char* value);
    int load_config_file(char* msg, int& len);

private:
    config()=default;

private:
    static config instance;
    std::map<std::string, std::string> m_config;
};

#endif // CONFIG_H
