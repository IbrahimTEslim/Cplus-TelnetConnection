// Telnet_Connection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <string>
#include <iostream>
#include <WS2tcpip.h>

using namespace std;

string user, pass, comm;
SOCKET server;


void main()
{
	system("title Telnet Connection");
	system("cls");
	system("color 09");


	int iR;
	std::string IP;
	std::cout << "Telnet IP: ";
	getline(std::cin, IP);
	std::cout << std::endl;

	u_long ip = 0;
	u_short port = 23;
	u_long* ptr = &ip;

	std::cout << "Telnet port: ";
	cin >> iR;
	// std::cout << std::endl;
	cin.ignore();
	cout << "\n\nInsert User name: ";
	getline(cin, user);

	cout << "Insert password: ";
	getline(cin, pass);


	if ((iR > SHRT_MAX) || (iR < 0)) {
		std::cout << "Wrong port number, using default." << std::endl;
	}
	else {
		port = iR;
	}

	WSADATA wsaData;
	iR = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iR != 0) { return; }
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET) { WSACleanup(); return; }

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Broadcast locally
	addr.sin_port = htons(port); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	std::cout << "Connecting..." << std::endl;
	if ((connect(server, (SOCKADDR*)&addr, sizeof(addr))) == SOCKET_ERROR) {
		std::cout << "Couldn't connect." << std::endl;
		WSACleanup();
		Sleep(2000);
		return;
	}

	cout << "\n\nInsert the command: ";
	getline(cin, comm);

	int bRecv = 0;
	char buff[2048];

	bRecv = recv(server, buff, sizeof(buff), 0);
	if ((bRecv == SOCKET_ERROR) || (bRecv == 0)) {
		std::cout << "Disconnected from the server." << std::endl;
		WSACleanup();
		return;
	}

	buff[bRecv] = 0;
	// When it connects successfully, a message appears on the screen from server
	std::cout << buff << std::endl;

	memset(buff, 0, sizeof(buff));

	user = user + "\r\n";
	pass = pass + "\r\n";
	comm = comm + "\r\n";

	iR = send(server, user.c_str(), user.length(), 0);

	while (true)
	{
		bRecv = recv(server, buff, sizeof(buff), 0);
		if (buff != 0)
			break;
	}

	iR = send(server, pass.c_str(), pass.length(), 0);

	while (true)
	{
		bRecv = recv(server, buff, sizeof(buff), 0);
		if (buff[2] == 'B' && buff[3] == 'a' && buff[4] == 'd') {
			cout << "Wrong Username\pass inputs!!\nReenter.\n\n";
			return;
		}
		else if (buff != 0)
			break;
	}

	iR = send(server, comm.c_str(), comm.length(), 0);

	while (true)
	{
		bRecv = recv(server, buff, sizeof(buff), 0);
		if (buff != 0)
			break;
	}

	if (iR == SOCKET_ERROR)
	{
		std::cout << "Couldn't send data." << std::endl; WSACleanup(); return;
		while (true) {
			bRecv = recv(server, buff, sizeof(buff), 0);
			if ((bRecv == SOCKET_ERROR) || (bRecv == 0)) {
				std::cout << "Disconnected from the server." << std::endl;
				break;
			}
			buff[bRecv] = 0;
			std::cout << buff << std::endl;
		}
	}

	WSACleanup();
	return;
}