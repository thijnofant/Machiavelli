#include "stdafx.h"
#include "LocalHost.h"
#include <memory>

std::shared_ptr<GameSession> LocalHost::GetSessionWithPlayer(int token)
{
	for (auto session : sessions)
	{
		if (session->ContainsPlayer(token))
			return session;
	}
	return nullptr;
}

shared_ptr<GameSession> LocalHost::GetSessionWithPlayer(string playerName)
{
	for (auto session : sessions)
	{
		if (session->ContainsPlayer(playerName) && !(session->IsGameOver()))
			return session;
	}
	return nullptr;
}

shared_ptr<GameSession> LocalHost::GetFirstWaitingSession()
{
	for (auto session : sessions)
	{
		if (!session->IsFull())
			return session;
	}
	return nullptr;
	
}

LocalHost::LocalHost(): lastToken{0}
{
	LoadGameSessions("../Saves", "sessions.sav");
	//todo load saved sessions
}


LocalHost::~LocalHost()
{
}

int LocalHost::Connect(string playerName)
{
	auto session = GetSessionWithPlayer(playerName);

	if (session != nullptr) //Session with this player excists
	{
		return session->GetPlayer(playerName)->GetToken();		
	}
	session = GetFirstWaitingSession();
	if (session!= nullptr) //A session is waiting for a seccond player
	{
		auto newPlayer = std::make_shared<Player>(lastToken++, playerName, false);
		session->AddPlayer(newPlayer, session);
		return newPlayer->GetToken();
	}

	//Note geen sessies, maak nieuwe
	session = std::make_shared<GameSession>();
	session->SetPhase(GamePhases::NotPlaying, session);
	auto newPlayer = std::make_shared<Player>(lastToken++, playerName, true);
		
	sessions.push_back(session);
	session->AddPlayer(newPlayer, session);
	return newPlayer->GetToken();
}

bool LocalHost::IsItMyTurn(int token)
{	
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->IsItMyTurn(token, session);
	}
	return false;
}

vector<string> LocalHost::GetCommands(int token)
{
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->GetActions(token, session);
	}
	return vector<string>();
}

bool LocalHost::SendMessageToHost(int token, string message)
{
	cout << "host recieved: \"" << message << "\"." << endl;

	if (IsItMyTurn(token))
	{
		auto session = GetSessionWithPlayer(token);
		if (session != nullptr)
		{
			return session->HandleAction(token, message, session);
			//todo save session
		}
	}
	return false;
}

bool LocalHost::Disconect(int token)
{
	return true;
}

string LocalHost::GetMessages(int token)
{
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->GetPlayer(token)->GetMessages();
	}
	return "";
}

void LocalHost::LoadGameSessions(string folder, string fileName)
{	
	std::ifstream is(folder + "/" + fileName);

	string line = "";
	while (std::getline(is, line))
	{
		shared_ptr<GameSession> session = std::make_shared<GameSession>();
		std::ifstream sessionfilestream(folder + "/" + line);
		sessionfilestream >> *session;

		this->sessions.push_back(session);
		
		sessionfilestream.close();
	}
	is.close();
}

string LocalHost::GetGameStatus(int token)
{
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->GetStatus(token);
	}
	return "Je bent op dit moment niet verbonden met een sessie. Start de gameclient opnieuw op.";
}

