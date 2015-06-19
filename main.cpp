#include <iostream>
#include <fcntl.h>
#include <signal.h>
#include "Server.h"
#include "Config.h"

using namespace std;

Server server;

void kill(int sig)
{
    server.close();
    exit(0);
}

int main()
{
    if (server.init(9734))
        exit(-1);
    signal(SIGINT, kill);
    Config config("/etc/scream");
    config.readConfig();
    int pid = fork();
    std::cout << pid << std::endl;
    switch (pid) {
        case 0:
            server.listening();
            break;
        case -1:
            perror("Fork Error");
            exit(1);
            break;
    }

}
