#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <string.h>
#include "partie.h"

#pragma comment (lib, "ws2_32.lib")

void main()
{
	Partie l_partie;
	int l_i = 0;
	bool EnCours = false;
	//
	sockaddr_in l_client;
	int clientsize = 0;
	//
	WSADATA WSdata;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &WSdata);
	if (wsok != 0)
	{
		std::cout << "Erreur winsock " << std::endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET)
	{
		std::cout << "probleme socket" << std::endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54888);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;


	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	clientsize = sizeof(l_client);

	while (true)
	{
		fd_set copy = master;


		
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);


		

		for (int i = 0; i < socketCount; i++)
		{
			std::cout << i << std::endl;
			

			SOCKET sock = copy.fd_array[i];


			if (sock == listening)
			{
				//SOCKET client = accept(listening, nullptr, nullptr);

				SOCKET client = accept(listening, (sockaddr*)&l_client, &clientsize);
				FD_SET(client, &master);

				char host[NI_MAXHOST];
				char service[NI_MAXHOST];

				char* liste[NI_MAXHOST];

				ZeroMemory(host, NI_MAXHOST);
				ZeroMemory(service, NI_MAXHOST);

				getnameinfo((sockaddr*)&l_client, sizeof(l_client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
				

				std::cout << host << " est connecte sur le port " << service << std::endl;

				if (l_i < 1)
				{
					send(client, "en attente d'un autre joueur.... ", 4096, 0);
				}
				else
				{
					send(client, "ca commence", 4096, 0);
				}
				liste[l_i] = service;


				l_i++;
				


				
			
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				
				int bytesIn = 4096;

				char buf1[4096];
				ZeroMemory(buf1, 4096);

				recv(sock, buf1, 4096, 0);

				if (l_i >= 2)
				{
					send(sock, "Ca commence", 4096, 0);
					l_i = 0;
				}

				if ((strcmp(buf1, "first") == 0)) // && (EnCours == false) && (l_i >= 2))
				{
					char buf2[4096];
					int l_tirage = l_partie.tirage();

					switch (l_tirage)
					{
					case 0:
						send(sock, "0", 4096, 0);
						break;
					case 1:
						send(sock, "1", 4096, 0);
						break;
					}

					EnCours == true;
				}



				if ((buf1[0] == 's') && (buf[1] == 'c'))
				{
					if (l_partie.EstCordonneValide((int)buf1[2], (int)buf1[2] == true))
					{
						send(sock, "o", 4096, 0);
						std::cout << "ca marche";
					}
					else
					{
						send(sock, "n", 4096, 0);
					}
					
				}
				


				if (bytesIn <= 0)
				{
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{

					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							
							std::ostringstream ss;
							ss << "Socket #" << sock << ": " << buf << std::endl;
							//std::cout << "Socket #" << sock << ": " << buf << std::endl;
							std::string strOut = ss.str();

				
						//	send(outSock, buf, bytesIn, 0);

							


													
						}

					}
				}


			}
		
		}
		
	}



	WSACleanup();

	system("pause");
}


