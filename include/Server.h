#ifndef SERVER_H
#define SERVER_H
#include "ClientsBase.h"
#include "Config.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mysql.h>

#define NEW_CON      1
#define RE_CON       2
#define GET_CL       3
#define FRIEND       4
#define ALIEN_FRIEND 5
#define QUIT         6
#define BUF_SIZE 256

typedef struct
{
    std::string* param;
    int type;
} MessageInfo;

class Server
{
    char*        buf;
    int          serverTCP;
    int          clientTCP;
    sockaddr_in* serverTCPAddress;
    sockaddr_in* clientTCPAddress;
    ClientsBase  clientsBase;

    Config config;

    MYSQL*       mySQLconn;
    MYSQL_RES*   mySQLres;
    MYSQL_ROW    mySQLrow;
    public:
        Server();
        int init(int );
        void listening();
        void close();
        virtual ~Server();
    protected:
    private:
        int connectDB();
        void newConnection(MessageInfo );
        void getClient(MessageInfo );
        void findWithFriends(std::string );
        bool checkUser(std::string, std::string );
        MessageInfo parse(char* );
};

#endif // SERVER_H
