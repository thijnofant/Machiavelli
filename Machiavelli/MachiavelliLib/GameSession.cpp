#include "stdafx.h"
#include "GameSession.h"
#include "CardGenerator.h"
#include "PickingCharactersPhase.h"
#include "ExecutingPhase.h"


GameSession::GameSession(int amountOfPlayers) : amountOfPlayers{ amountOfPlayers }, amountOfMoneyInBank{ 30 }, gameOver{ false }
{
	deck = CardGenerator::CreateAndGetAllCards(this);
	std::shuffle(std::begin(deck), std::end(deck), Util::GetRandomEngine());
}

GameSession::~GameSession()
{
}

shared_ptr<Player> GameSession::GetPlayer(int token)
{
	for (auto player : players)
	{
		if (player->GetToken() == token)
		{
			return player;
		}
	}
	return nullptr;
}

shared_ptr<Player> GameSession::GetPlayer(string userName)
{
	for (auto player : players)
	{
		if (player->GetPlayerName() == userName)
		{
			return player;
		}
	}
	return nullptr;
}

string GameSession::GetStatus()
{
	std::stringstream status; 

	status << "Amount of money left in bank: " << amountOfMoneyInBank << "\r\n";

	//temp for debug print all cards in deck
	for (auto card : deck)
	{
		status << card.GetName() << " ";
	}
	status << "\r\n";

	for (auto player : players)
	{
		status << player->GetPlayerName() << "'s vilage:";

		for (auto card : player->GetVillage())
		{
			status << " " << card.GetName();
		}
		status << "\r\n";
	}

	return status.str();
}

string GameSession::GetStatus(int token)
{	
	auto player = GetPlayer(token);
	
	if (player != nullptr)
	{
		return GetStatus() + player->GetStatus();
	}

	return GetStatus();
	}

bool GameSession::ContainsPlayer(int token)
{
	for (auto player : players)
	{
		if (player->GetToken() == token)
		{
			return true;
		}
	}
	return false;
}

bool GameSession::ContainsPlayer(string playerName)
{
	for (auto player : players)
	{
		if(player->GetPlayerName() == playerName)
		{
			return true;
		}
	}
	return false;
}

bool GameSession::AddPlayer(shared_ptr<Player> newPlayer)
{	
	if (!IsFull())
	{
		if (players.empty())
		{
			SetCurrentPlayer(newPlayer);
		}

		this->players.push_back(newPlayer);
		return true;
	}

	return false;
}

bool GameSession::HandleAction(int token, string message, shared_ptr<GameSession> session)
{
	return currentPhase->HandleAction(token, message, session);
}

vector<string> GameSession::GetActions(int token, shared_ptr<GameSession> session)
{
	return currentPhase->GetActions(token, session);
}

bool GameSession::IsItMyTurn(int token, shared_ptr<GameSession> session)
{
	return currentPhase->IsItMyTurn(token, session);
}

bool GameSession::IsFull() const
{
	return players.size() >= amountOfPlayers;
}

void GameSession::SetPhase(GamePhases phase, shared_ptr<GameSession> session)
{
	cout << "switching phases" << endl;

	//todo if game over end game

	switch (phase)
	{
	case PickingCharacters:
		currentPhase = std::make_unique<PickingCharactersPhase>();
		break;
	case Executing: 
		currentPhase = std::make_unique<ExecutingPhase>(session);
		break;
	default: break;
	}
	

}

shared_ptr<Player> GameSession::GetCurrentPlayer()
{
	return currentPlayer;
}

void GameSession::SetCurrentPlayer(shared_ptr<Player> player)
{
	currentPlayer = player;
}

int GameSession::GetAmountOfPlayers() const
{
	return amountOfPlayers;
}

shared_ptr<Player> GameSession::GetKing()
{
	for (auto player : players)
	{
		if(player->IsKing())
		{
			return player;
		}
	}
	return nullptr;
}

shared_ptr<Player> GameSession::GetPlayerWithCharacter(Character character)
{
	for (auto player : players)
	{
		if (player->HasCharacter(character))
		{
			return player;
		}
	}
	return nullptr;
}

void GameSession::MakeKing(int token)
{
	for (auto player : players)
	{
		player->SetIsKing(player->GetToken() == token);
	}
}

void GameSession::MakeKing(string playerName)
{
	for (auto player : players)
	{
		player->SetIsKing(player->GetPlayerName() == playerName);
	}
}

int GameSession::TakeMoney(int amount)
{
	if (amount > amountOfMoneyInBank)
	{
		amount = 0;
	}
	amountOfMoneyInBank -= amount;
	return amount;
}

void GameSession::GiveMoney(int amount)
{
	amountOfMoneyInBank += amount;
}

void GameSession::NextPlayer()
{
	auto it = std::find(players.begin(), players.end(), currentPlayer);
	if (it == players.end())
	{
		// name not in vector
	}
	else
	{
		auto index = std::distance(players.begin(), it);
		index++;
		if (index >= players.size())
		{
			index = 0;
		}
		currentPlayer = players[index];
	}
}

bool GameSession::IsGameOver() const
{
	return gameOver;
}

void GameSession::SetGameOver(bool isGameOver)
{
	gameOver = isGameOver;
}
