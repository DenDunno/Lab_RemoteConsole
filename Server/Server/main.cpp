#include "Server.h"

using namespace std;

const char* IP = "192.168.1.100";
const int PORT = 1028;


int main(int argc, char* argv[]) 
{
	Server server;

	server.SetSockaddr(IP, PORT);
	server.ConnectToClient();
	server.SendCommands();

	system("pause");
	return 0;
}

