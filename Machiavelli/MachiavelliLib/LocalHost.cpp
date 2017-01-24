#include "stdafx.h"
#include "LocalHost.h"
#include <memory>

const string LocalHost::Folder = "../Saves/";
const string LocalHost::SaveFile = "sessions.sav";
string LocalHost::CurrentExportingSessionName = "";
const string LocalHost::Extension = ".sav";

std::shared_ptr<GameSession> LocalHost::GetSessionWithPlayer(int const token) const
{
	for (auto session : sessions)
	{
		if (session->ContainsPlayer(token))
			return session;
	}
	return nullptr;
}

shared_ptr<GameSession> LocalHost::GetSessionWithPlayer(string const playerName) const
{
	for (auto session : sessions)
	{
		if (session->ContainsPlayer(playerName) && !(session->IsGameOver()))
			return session;
	}
	return nullptr;
}

shared_ptr<GameSession> LocalHost::GetFirstWaitingSession() const
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
	LoadGameSessions(LocalHost::Folder, LocalHost::SaveFile);
}


LocalHost::~LocalHost()
{
}

int LocalHost::Connect(string const playerName)
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

bool LocalHost::IsItMyTurn(int const token) const
{	
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->IsItMyTurn(token, session);
	}
	return false;
}

vector<string> LocalHost::GetCommands(int const token)
{
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->GetActions(token, session);
	}
	return vector<string>();
}

bool LocalHost::SendMessageToHost(int const token, string const message)
{
	cout << "host recieved: \"" << message << "\"." << endl;

	if (IsItMyTurn(token))
	{
		auto session = GetSessionWithPlayer(token);
		if (session != nullptr)
		{
			if(session->HandleAction(token, message, session))
			{
				SaveGame(LocalHost::Folder, LocalHost::SaveFile);
				return true;
			}
			return false;
		}
	}
	return false;
}

bool LocalHost::Disconect(int const token)
{
	return true;
}

string LocalHost::GetMessages(int const token)
{
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->GetPlayer(token)->GetMessages();
	}
	return "";
}

void LocalHost::SaveGame(string const folder, string const fileName)
{
	std::ofstream os(folder + fileName);

	os << to_string(lastToken) << '\n';

	for (size_t i = 0; i < sessions.size(); i++)
	{
		LocalHost::CurrentExportingSessionName = "session" + to_string(i);
		std::ofstream sessionOs(folder + LocalHost::CurrentExportingSessionName + ".sav");
		os << LocalHost::CurrentExportingSessionName + ".sav" << '\n';
		sessionOs << *sessions[i];
		sessionOs.close();
	}
	os.close();
}

void LocalHost::LoadGameSessions(const string folder, const string fileName)
{	
	std::ifstream is(folder + fileName);

	string line = "";
	getline(is, line);
	lastToken = stoi(line);

	while (std::getline(is, line))
	{
		std::ifstream sessionfilestream(folder + line);

		try
		{
			shared_ptr<GameSession> session = std::make_shared<GameSession>();
			sessionfilestream >> *session;

			this->sessions.push_back(session);
		}
		catch(...)
		{
			
		}
		
		sessionfilestream.close();
	}
	is.close();
}

string LocalHost::GetGameStatus(int const token) const
{
	auto session = GetSessionWithPlayer(token);
	if (session != nullptr)
	{
		return session->GetStatus(token);
	}
	return "Je bent op dit moment niet verbonden met een sessie. Start de gameclient opnieuw op.";
}

