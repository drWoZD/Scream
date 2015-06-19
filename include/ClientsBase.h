#ifndef CLIENTSBASE_H
#define CLIENTSBASE_H
#include <map>
#include <time.h>

typedef struct
{
    std::string ip;
    int portTCP;
    int portUDP;
    time_t time;
} ClientInfo;

class ClientsBase
{
    std::map<std::string, ClientInfo> clientsMap;
    public:
        ClientsBase();
        void add(std::string , std::string , int, int);
        void update(std::string , std::string , int);
        ClientInfo get(std::string );
        void remove(std::string );
        virtual ~ClientsBase();
    protected:
    private:
};

#endif // CLIENTSBASE_H
