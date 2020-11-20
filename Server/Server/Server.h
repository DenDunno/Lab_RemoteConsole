#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <fstream>
#include <string>
#pragma warning(disable: 4996)


struct SystemLog
{
	std::string Time;
	std::string MessageToClient;
	std::string AnswerFromClient;
};


class Server
{
private:
	
	std::ofstream _outFile;
	std::string _savePath = "..\\..\\SystemLog.txt";
	SystemLog _systemLog;

	SOCKADDR_IN _addr;
	int _sizeOfAddr = sizeof(_addr);
	
	SOCKET _sListen;
	SOCKET _connectionWithClient;

	void ShowStartMessage();
	void CheckConnection(SOCKET _connectionWithClient);
	void GetCallBack(std::string& message);
	void WriteToSystemLog();

public:

	Server();

	void SetSockaddr(const char* ip, const int port);
	void ConnectToClient();
	void SendCommands();
};