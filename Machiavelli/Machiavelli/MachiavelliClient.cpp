#include "stdafx.h"
#include "MachiavelliClient.h"
#include <iostream>

MachiavelliClient::MachiavelliClient(std::shared_ptr<IHostConnection> connection):connection{connection}, running{true}
{
}

MachiavelliClient::~MachiavelliClient()
{
}

void MachiavelliClient::Run()
{
	//todo ask for playername
	string playername;
	std::cin >> playername;
	
	//todo log into server with playername and get a client token for identification
	token = connection->Conect(playername);

	while(running)
	{
		//todo check for updated information
		//todo ask server if it is players turn to do something
		if(connection->IsItMyTurn(token))
		{
			//todo ask server for possible actions
			//todo check for user input
			//todo send user input to server for further handling
		}
	}
}
