#include "Config.h"

Config::Config()
{
    log = "./log.log";
}

Config::Config(std::string str)
{
    file.open(str);
}

void Config::setDefault()
{
    log = "./log.log";
}

void Config::readConfig()
{
    std::string temp;
    while (!file.eof()) {
        file >> temp;
        parseCongig(temp);
    }
}

std::string Config::getLog()
{
    return log;
}

std::set<std::string> Config::getFriends() const
{
    return friends;
}

void Config::parseCongig(std::string str)
{
    std::regex pattern("([^ ]*) ?([^ ]*)");
    std::smatch res;
    regex_search(str, res, pattern);
    if (!res[1].str().compare("LOG_FILE"))
        log = res[2].str();
    if (!res[1].str().compare("FRIEND"))
        friends.insert(res[2].str());
}

Config::~Config()
{
    //dtor
}
