#include <iostream>
#include <regex>
#include "Server.h"

Server::Server()
{
    config.setDefault();
}

int Server::connectDB()
{
    char *server = (char* )"localhost";
    char *user = (char* )"scream";
    char *password = (char* )"19960302";
    char *database = (char* )"scream";
    mySQLconn = mysql_init(NULL);

    if (!mysql_real_connect(mySQLconn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(mySQLconn));
        return -1;
    }
    return 0;
}

int Server::init(int port)
{
    int ret;

    ret = connectDB();
    if (ret < 0)
        return ret;

    serverTCP = socket(AF_INET, SOCK_STREAM, 0);
    if (serverTCP < -1) {
        perror("Socket Error");
        return -1;
    }
    serverTCPAddress = new sockaddr_in;
    serverTCPAddress->sin_family = AF_INET;
    serverTCPAddress->sin_addr.s_addr = htonl(INADDR_ANY);
    serverTCPAddress->sin_port = htons(port);

    ret = bind(serverTCP, (sockaddr* )serverTCPAddress, sizeof(*serverTCPAddress));
    if (ret < 0) {
        perror("Bind Error");
        return -2;
    }
    listen(serverTCP, 10);
    std::cout << "Inited!" << std::endl;
    return 0;
}

void Server::listening()
{
    buf = new char[BUF_SIZE];
    socklen_t clientLen = sizeof(*clientTCPAddress);
    clientTCPAddress = new sockaddr_in;

    while (1) {
        memset(buf, 0, BUF_SIZE);
        std::cout << "Wait" << std::endl;
        clientTCP = accept(serverTCP, (sockaddr* ) clientTCPAddress, &clientLen);
        int n = recv(clientTCP, buf, BUF_SIZE, 0);
        if (n > 0) {
            MessageInfo info = parse(buf);
            switch (info.type) {
                case NEW_CON: {
                    newConnection(info);
                    break;
                }
                case FRIEND:
                    break;
                case GET_CL: {
                    std::string tempstr;
                    ClientInfo clinfo = clientsBase.get(info.param[0]);
                    if (clinfo.ip.empty())

                        tempstr = "NOCLN";
                    else
                        tempstr = "CLADDR: " + clinfo.ip + ":" + std::to_string(clinfo.portUDP) + ":" + std::to_string(clinfo.portTCP);
                        send(clientTCP, tempstr.c_str(), tempstr.length(), 0);
                        std::cout << "Sended data " << tempstr.c_str() << std::endl;
                    break;
                }
                case QUIT: {
                    if (checkUser(info.param[0], info.param[1])) {
                        clientsBase.remove(info.param[0]);
                    }
                    break;
                }
            }

        }
        ::close(clientTCP);
    }
}

void Server::newConnection(MessageInfo info)
{
    if (checkUser(info.param[0], info.param[1])) {
        int clientUDP = socket(AF_INET, SOCK_DGRAM, 0);
        sockaddr_in clUDPaddr;
        clUDPaddr.sin_family = AF_INET;
        clUDPaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        clUDPaddr.sin_port = clientTCPAddress->sin_port;
        socklen_t clUDPlen = sizeof(clUDPaddr);
        bind(clientUDP, (sockaddr* )&clUDPaddr, sizeof(clUDPaddr));

        std::cout << clUDPaddr.sin_addr.s_addr << " : " <<  ntohs(clUDPaddr.sin_port) << std::endl;

        std::string tempID(info.param[0]);
        std::string tempString = "OK. UDP EXP " + std::to_string(ntohs(clUDPaddr.sin_port));
        send(clientTCP, tempString.c_str(), tempString.length(), 0);

        recvfrom(clientUDP, buf, BUF_SIZE, 0, (sockaddr* ) &clUDPaddr, &clUDPlen);

        tempString = "OK. UDP " + std::to_string(ntohs(clUDPaddr.sin_port));
        send(clientTCP, tempString.c_str(), tempString.length(), 0);

        clientsBase.add(tempID, inet_ntoa(clUDPaddr.sin_addr), ntohs(clUDPaddr.sin_port), ntohs(clientTCPAddress->sin_port));

        std::cout << "Regestrated: " + tempID + " port: " +  std::to_string(ntohs(clUDPaddr.sin_port))<< std::endl;
    } else {
        std::string tempString = "PASSINCORRECT";
        send(clientTCP, tempString.c_str(), tempString.length(), 0);
    };
}
void Server::close()
{
    shutdown(serverTCP, SHUT_RDWR);
    ::close(serverTCP);
}

bool Server::checkUser(std::string login, std::string password)
{
    int ret = false;
    std::string query = "SELECT password FROM users WHERE login='" + login + "';";
    if (mysql_query(mySQLconn, query.c_str())) {
        fprintf(stderr, "%s\n", mysql_error(mySQLconn));
        return ret;
    }
    mySQLres = mysql_use_result(mySQLconn);

    mySQLrow = mysql_fetch_row(mySQLres);

    if ((mySQLrow) != nullptr) {
        if (!password.compare(std::string(mySQLrow[0])))
            ret = true;
    }
    mysql_free_result(mySQLres);
    return ret;
}
MessageInfo Server::parse(char* b)
{
    MessageInfo info;
    info.type = 0;
    std::string str(b);
    std::regex pattern("([^ ]*): ?([^ ]*) ?(.*)");
    std::smatch res;
    regex_search(str, res, pattern);
    std::string tempstr = res[1].str();
    if(tempstr.length() > 0)
    {
        if(!tempstr.compare("LOGIN")) {
            info.type = NEW_CON;
            info.param = new std::string[2];
            info.param[0] = res[2].str();
            info.param[1] = res[3].str();
        }
        if(!tempstr.compare("CLNID")) {
            info.type = GET_CL;
            info.param = new std::string[1];
            info.param[0] = res[2].str();
        }
        if(!tempstr.compare("RECON")) {
            info.type = RE_CON;
        }
        if(!tempstr.compare("QUIT")) {
            info.type = QUIT;
            info.param = new std::string[2];
            info.param[0] = res[2].str();
            info.param[1] = res[3].str();
        }
    }
    return info;
}

Server::~Server()
{
    //dtor
}
