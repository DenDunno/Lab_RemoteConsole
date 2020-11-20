#include "Server.h"
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <ctime>

using namespace std;


Server::Server()
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}

	_outFile.open(_savePath);
}


void Server::SetSockaddr(const char* ip, const int port)
{
	_addr.sin_addr.s_addr = inet_addr(ip);
	_addr.sin_port = htons(port);
	_addr.sin_family = AF_INET;
}


void Server::ConnectToClient()
{
	_sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(_sListen, (SOCKADDR*)&_addr, sizeof(_addr));

	listen(_sListen, SOMAXCONN);
	
	_connectionWithClient = accept(_sListen, (SOCKADDR*)&_addr, &_sizeOfAddr);

	CheckConnection(_connectionWithClient);
}


void Server::ShowStartMessage()
{
	system("CLS");

	char msg[256] =
		"Client connected. \n\n"
		"Commands you can use: \n"
		"who \n"
		"echo \n"
		"type \n"
		"md \n"
		"cls \n"
		"exit \n";

	cout << msg << endl;
}


void Server::CheckConnection(SOCKET connectionWithClient)
{
	if (!connectionWithClient)
	{
		cout << "Error" << endl;;
		exit(1);
	}

	else
		ShowStartMessage();
}


void Server::GetCallBack(string& message)
{
	char callBack[512];

	while (true)
	{
		recv(_connectionWithClient, callBack, sizeof(callBack), NULL);
		_systemLog.AnswerFromClient = callBack;

		// callBack содержит 'e' ,'x' ,'i' , 't' , '\0' и дальше куча мусора, поэтому сравнение ограниченное
		if (strncmp(callBack, "exit", 4) != 0)
			cout << callBack << endl;

		else
			break;
		
		if (strncmp(callBack, "cls" , 3) == 0)
			ShowStartMessage();

		WriteToSystemLog();
	}

	shutdown(_connectionWithClient, 2);
	exit(1); // выход з метода прослушивания означает конец программы
}


void Server::WriteToSystemLog()
{
	time_t now = time(0);
	_systemLog.Time = ctime(&now);
	
	_outFile << _systemLog.Time;
	_outFile << _systemLog.MessageToClient << endl;
	_outFile << _systemLog.AnswerFromClient << endl;
	_outFile << endl << endl;
}


void Server::SendCommands()
{
	string message;
	thread getCallBack(&Server::GetCallBack , this ,ref(message));
	
	do
	{
		getline(cin, message);
		message.push_back('\0');

		send(_connectionWithClient, message.c_str(), message.size(), NULL);
		_systemLog.MessageToClient = message;

		message.pop_back(); // нужно для правильного сравнения string и const char*

	} while (true);

	getCallBack.join();
}





