#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <map>
#include <string>
#pragma warning(disable: 4996)

class Client
{

private:

	std::string callBack;

	SOCKADDR_IN _addr;

	SOCKET _connectionWithServer;
	int _sizeofaddr = sizeof(_addr);

	std::map<std::string, void (Client::*) (std::string)> _commands // по ключу вызываем соответсвующий метод
	{
		{"echo" , &Client::Echo },
		{"type" , &Client::Type },
		{"exit" , &Client::Exit },
		{"cls"  , &Client::Clear },
		{"who"  , &Client::ShowWho },
		{"md"   , &Client::MD },
	};

	void ParseCommand(char* message, std::string& command, std::string& data);
	void Echo(std::string data);
	void Type(std::string data);
	void Exit(std::string data);
	void Clear(std::string data);
	void ShowWho(std::string data);
	void MD(std::string data);

public:

	Client();

	void SetSockaddr(const char* ip, const int port);
	void ConnectToServer();
	void ReceiveCommands();
};
