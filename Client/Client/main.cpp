#include "Client.h"


using namespace std;


const char* IP = "192.168.1.100";
const int PORT = 1028;



int main(int argc, char* argv[])
{
	Client client;
	
	client.SetSockaddr(IP, PORT);
	client.ConnectToServer();
	client.ReceiveCommands();

	return 0;
}







