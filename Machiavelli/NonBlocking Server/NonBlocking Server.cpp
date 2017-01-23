// NonBlocking Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//
//  NonBlocking Server.cpp
//  socketexample
//
//  Created by Bob Polis on 16/09/14.
//  Revised by Jeroen de Haas on 22/11/2016
//	Revised by Thijn van Dijk on 10/12/2016
//  Copyright (c) 2014 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#include <thread>
#include <iostream>
#include <exception>
#include <memory>
#include <utility>
#include <chrono>
using namespace std;

#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"
#include "LocalHost.h"
#include "User.h"
#include "ClientInfo.h"

namespace machiavelli {
	const int tcp_port{ 1080 };
	const string prompt{ "machiavelli> " };
}

static bool serverRunning = true;
const int __WAITTIME__ = 1000; //Note: wait time in run
static Sync_queue<ClientCommand> commandQueue;

void consume_command(shared_ptr<IHostConnection> host) // runs in its own thread
{
	/*
	try {
		while (serverRunning) {
			ClientCommand command{ commandQueue.get() }; // will block here unless there are still command objects in the queue
			if (auto clientInfo = command.get_client_info().lock()) {
				auto &client = clientInfo->get_socket();
				auto &user = clientInfo->get_user();
				try {
					if (!host->SendMessageToHost(user.GetToken(), command.get_cmd()))
					{
						client << user.get_name() << ", you entered: '" << command.get_cmd() << "', but I'll ignore that for now.\r\n" << machiavelli::prompt;
					}
				}
				catch (const exception& ex) {
					cerr << "*** exception in consumer thread for user " << user.get_name() << ": " << ex.what() << '\n';
					if (client.is_open()) {
						client.write("Sorry, something went wrong during handling of your request.\r\n");
					}
				}
				catch (...) {
					cerr << "*** exception in consumer thread for user " << user.get_name() << '\n';
					if (client.is_open()) {
						client.write("Sorry, something went wrong during handling of your request.\r\n");
					}
				}
			}
		}
	}
	catch (...) {
		cerr << "consume_command crashed\n";
	}
	*/
}

std::shared_ptr<ClientInfo> init_client_session(Socket client) {
	client.write("Welcome to Server 1.0! To quit, type 'quit'.\r\n");
	client.write("Please enter your name?\r\n");
	client.write(machiavelli::prompt);
	string name;
	bool done{ false };
	while (!done) {
		done = client.readline([&name](std::string input) {
			name = input;
		});
	}
	return make_shared<ClientInfo>(move(client), User{ name });
}

void handle_client(Socket client, shared_ptr<IHostConnection> host) // this function runs in a separate thread
{
	try {
		auto client_info = init_client_session(move(client));
		auto &socket = client_info->get_socket();
		auto &user = client_info->get_user();
		socket << "Welcome, " << user.get_name() << ", have fun playing our game!\r\n" << machiavelli::prompt;

		user.SetToken(host->Connect(user.get_name()));

		string status;
		bool threadRunning = true;

		while (serverRunning && threadRunning) { // game loop
			try {

				string messages = host->GetMessages(user.GetToken());
				if (messages != "")
				{
					socket << messages << "\r\n";
				}

				//todo haal deze status hier weg messages is the way to go
				string newStatus = host->GetGameStatus(user.GetToken());
				if (status != newStatus)
				{
					status = newStatus;
					socket << status << "\r\n";
				}

				if (host->IsItMyTurn(user.GetToken()))
				{
					auto commands = host->GetCommands(user.GetToken());
					commands.push_back("exit");
					commands.push_back("quit server");

					//check for new messages issued during by getcommands
					messages = host->GetMessages(user.GetToken());
					if (messages != "")
					{
						socket << messages << "\r\n";
					}

					for (auto i = 0; i < commands.size(); i++)
					{
						socket << std::to_string(i) << ": " << commands[i] << "  ";
					}
					socket << "\r\n" << machiavelli::prompt;

					int inCmd = -1;
					while (true)
					{
						string cmd;
						if (socket.readline([&cmd](std::string input) { cmd = input; })) {
							cerr << '[' << socket.get_dotted_ip() << " (" << socket.get_socket() << ") " << user.get_name() << "] " << cmd << "\r\n";
						}
						if (cmd == "")
						{
							continue;
						}

						inCmd = stoi(cmd);

						if (inCmd >= 0 && inCmd <= commands.size() - 1)
						{
							if (commands[inCmd] == "exit")
							{
								threadRunning = false;
								break;
							}
							else if (commands[inCmd] == "quit server")
							{
								serverRunning = false;
								break;
							}
							else
							{
								cerr << '[' << socket.get_dotted_ip() << " (" << socket.get_socket() << ") " << user.get_name() << "] " << commands[inCmd] << "\r\n";
								//commandQueue.put(commands[inCmd]);

								host->SendMessageToHost(user.GetToken(), commands[inCmd]);
								break;
							}
						}
						else
						{
							socket << "The entered value isn't a command available at this time. Please enter the numeric value in front of the command." << "\r\n";
						}

					}
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(__WAITTIME__));
				}
			}
			catch (const exception& ex) {
				socket << "ERROR: " << ex.what() << "\r\n";
			}
			catch (...) {
				socket.write("ERROR: something went wrong during handling of your request. Sorry!\r\n");
			}
		}
	}
	catch (std::exception &ex) {
		cerr << "handle_client " << ex.what() << "\n";
	}
	catch (...) {
		cerr << "handle_client crashed\n";
	}
}

int main(int argc, const char * argv[])
{
	shared_ptr<IHostConnection> host = make_shared<LocalHost>();

	// start command consumer thread
	vector<thread> all_threads;
	all_threads.emplace_back(consume_command, host);

	// create a server socket
	ServerSocket server{ machiavelli::tcp_port };

	try {
		cerr << "server listening" << '\n';
		while (serverRunning) {
			// wait for connection from client; will create new socket
			server.accept([&all_threads, host](Socket client) {
				std::cerr << "Connection accepted from " << client.get_dotted_ip() << "\n";
				all_threads.emplace_back(handle_client, move(client), host);
			});
			this_thread::sleep_for(chrono::milliseconds(100));
		}
	}
	catch (const exception& ex) {
		cerr << ex.what() << ", resuming..." << '\n';
	}
	catch (...) {
		cerr << "problems, problems, but: keep calm and carry on!\n";
	}

	for (auto &t : all_threads) {
		t.join();
	}

	return 0;
}

