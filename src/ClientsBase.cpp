#include "ClientsBase.h"
#include <iostream>

ClientsBase::ClientsBase()
{}

void ClientsBase::add(std::string id, std::string ip, int portUDP, int portTCP)
{
    ClientInfo info;
    info.ip = ip;
    info.portUDP = portUDP;
    info.portTCP = portTCP;
    info.time = time(NULL);
    clientsMap[id] = info;
    std::cout << "ID:" << id << " IP: " << ip << ":"<< std::to_string(portUDP) << ":"<< std::to_string(portTCP) <<std::endl;
}

void ClientsBase::update(std::string id, std::string ip, int port)
{
    remove(id);
    add(id, ip, port, 0);
}

ClientInfo ClientsBase::get(std::string id)
{
    auto iter = clientsMap.find(id);
    if (iter != clientsMap.end())
        return iter->second;
    ClientInfo i;
    i.ip = "";
    return i;
}

void ClientsBase::remove(std::string id)
{
    clientsMap.erase(id);
}

ClientsBase::~ClientsBase()
{}
