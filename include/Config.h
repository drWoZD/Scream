#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <fstream>
#include <regex>
#include <set>

class Config
{
    std::ifstream         file;
    std::string           log;
    std::set<std::string> friends;
    public:
        Config();
        Config(std::string );
        void setDefault();
        void readConfig();
        std::string getLog();
        std::set<std::string> getFriends() const;
        virtual ~Config();
    protected:
    private:
        void parseCongig(std::string );
};

#endif // CONFIG_H
