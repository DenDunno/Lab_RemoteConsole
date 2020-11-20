#include "Client.h"
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <filesystem>

using namespace std;

Client::Client()
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}
}


void Client::SetSockaddr(const char* ip, const int port)
{
	_addr.sin_addr.s_addr = inet_addr(ip);
	_addr.sin_port = htons(port);
	_addr.sin_family = AF_INET;
}


void Client::ConnectToServer()
{
	_connectionWithServer = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(_connectionWithServer, (SOCKADDR*)&_addr, sizeof(_addr)) != 0)
		cout << "Error: failed connect to server.\n";

	else
		cout << "Connected!\n\n";
}


void Client::ReceiveCommands()
{
	char message[128];
	string command;
	string data;

	do
	{
		recv(_connectionWithServer, message, sizeof(message), NULL);

		ParseCommand(message, command, data);

		if (_commands.find(command) != _commands.end())
		{
			callBack = "OK\n";
			(this->*_commands[command]) (data); // вызываем соотвествующий метод по ключу
		}

		else
			callBack = "Eror. Invalid input\n";

		callBack.push_back('\0');
		send(_connectionWithServer, callBack.c_str(), callBack.size(), NULL);

		command.clear();
		data.clear();
		callBack.pop_back(); // убираем нулевой символ (нужно для правильного сранениея string и const char*)

	} while (callBack != "exit");
}


void Client::ParseCommand(char* message, string& command, string& data)
{
	int spaceIndex = find(message, message + strlen(message) - 1, ' ') - message;

	// если не было пробела, (команды exit,cls,who) прибаляем 1 (чтобы правильно прочитать команду)
	if (spaceIndex == strlen(message) - 1) 
		spaceIndex++;

	for (int i = 0; i < spaceIndex; ++i)
		command += tolower(message[i]);

	for (int i = spaceIndex + 1; i < strlen(message); ++i)
		data += message[i];
}


void Client::Echo(string data)
{
	cout << data << endl;
}


void Client::Type(string data)
{
	ifstream inFile;
	string inFileText = "\n";

	inFile.open(data);

	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			string temp;
			getline(inFile, temp);
			inFileText += temp + '\n'; // отправляем одну крупную строку
		}

		callBack = inFileText + "OK\n";
	}

	else
		callBack = "Invalid path\n";
}


void Client::Exit(string data)
{
	callBack = "exit";
}


void Client::Clear(std::string data)
{
	system("CLS");
	cout << "Connected!\n\n";
	callBack = "cls";
}


void Client::ShowWho(std::string data)
{
	callBack =  "Hello :)\n"
				"Denis Bondar K-24\n"
				"Remote comsole\n"
				"Variant 3\n"
				"OK\n";
}


void Client::MD(std::string data)
{
	try 
	{
		if (filesystem::create_directories(data))
			callBack = "Catalog created\nOK\n";

		else
			throw EXCEPTION_READ_FAULT;
	}

	catch(...) { callBack = "Eror\n"; }
}





