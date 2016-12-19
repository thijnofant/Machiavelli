#include "stdafx.h"
#include "MachiavelliClient.h"
#include <thread>

int __WAITTIME__ = 1000; //Note: wait time in run

MachiavelliClient::MachiavelliClient(std::shared_ptr<IHostConnection> connection):connection{connection}, running{true}, token{-1}
{
}

MachiavelliClient::~MachiavelliClient()
{
}

void MachiavelliClient::Run()
{
	string status;
	string playerName;
	std::cout << "Enter player name" << endl;
	std::cin >> playerName;

	token = connection->Connect(playerName);

	while (running)
	{
		string newStatus = connection->GetGameStatus(token);
		if (status != newStatus)
		{
			status = newStatus;
			cout << status << endl;
		}
		if (connection->IsItMyTurn(token))
		{
			//get commands
			auto commands = connection->GetCommands(token);
			commands.push_back("exit");

			//check for new messages issues by get commands
			cout << connection->GetMessages(token) << "\n";
			
			for (auto i = 0; i < commands.size(); i++)
			{
				cout << std::to_string(i) << ": " << commands[i] << "  ";
			}
			cout << endl;

			int inCmd = -1;
			while (true)
			{
				cin >> inCmd;
				if (inCmd >= 0 && inCmd <= commands.size() - 1)
				{
					if (commands[inCmd] == "exit")
					{
						running = false;
						break;
					}
					else
					{
						connection->SendMessageToHost(token, commands[inCmd]);
						break;
					}
				}
				else
				{
					cout << "The entered value wasn't a command available at this time. Please enter the numeric value before the command." << endl;
				}
			}
		}

		else
			std::this_thread::sleep_for(std::chrono::milliseconds(__WAITTIME__));
	}
}
