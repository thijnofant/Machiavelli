#include "stdafx.h"
#include "GameSession.h"
#include "CardGenerator.h"
#include "PickingCharactersPhase.h"
#include "ExecutingPhase.h"
#include "NotPlayingPhase.h"


GameSession::GameSession(int amountOfPlayers) : 
	amountOfMoneyInBank{ 30 }, 
	amountOfPlayers{ amountOfPlayers }, 
	gameOver{ false },
	deck(CardGenerator::BuildDeckFromFile("../cards.txt"))
{
//	deck = CardGenerator::CreateAndGetAllCards(this);
	
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
	if (!IsFull())
	{
		return "\r\n Waiting for more players to join...\r\n";
	}

	std::stringstream status;

	status << "\r\n  Amount of money left in bank: " << amountOfMoneyInBank << "\r\n";

	//todo temp for debug print all cards in deck
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
		status << ". Vilage size: " << std::to_string(player->GetVillage().size()) << " \r\n";
	}

	return status.str();
}

string GameSession::GetStatus(int token)
{	
	if (!IsFull())
	{
		return GetStatus();
	}

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

bool GameSession::AddPlayer(shared_ptr<Player> newPlayer, shared_ptr<GameSession> session)
{	
	if (!IsFull())
	{
		if (players.empty())
		{
			SetCurrentPlayer(newPlayer);
		}

		this->players.push_back(newPlayer);
		newPlayer->GiveMoney(this->TakeMoney(2));
		if (IsFull())
		{
			SetPhase(PickingCharacters, session);
		}
		return true;
	}
	return false;
}

bool GameSession::HandleAction(int token, string message, shared_ptr<GameSession> session) const
{
	return currentPhase->HandleAction(token, message, session);
}

vector<string> GameSession::GetActions(int token, shared_ptr<GameSession> session) const
{
	return currentPhase->GetActions(token, session);
}

const bool GameSession::IsItMyTurn(int token, shared_ptr<GameSession> session) const
{
	if (gameOver)
	{
		return true;
	}
	return currentPhase->IsItMyTurn(token, session);
}

bool GameSession::IsFull() const
{
	return players.size() >= amountOfPlayers;
}

void GameSession::SetPhase(GamePhases phase, shared_ptr<GameSession> session)
{
	cout << "switching to phase " << GamePhaseEnumToString.at(phase) << endl;

	if (gameOver)
	{
		//todo end game
		//todo tally scores
		//todo als speler Hof der wonderen heeft vraag om welke kleur deze moet worden
		//todo message players with scores
		//todo go to final phase
	}

	switch (phase)
	{
	case NotPlaying:
		currentPhase = std::make_unique<NotPlayingPhase>();
		break;
	case PickingCharacters:
		SendAllPlayersMessage("\r\nTime to pick characters.");
		currentPhase = std::make_unique<PickingCharactersPhase>();
		break;
	case Executing: 
		SendAllPlayersMessage("\r\nAll characters chosen, time to play.");
		currentPhase = std::make_unique<ExecutingPhase>(session);
		break;
	default: break;
	}
	

}

shared_ptr<Player> GameSession::GetCurrentPlayer() const
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

void GameSession::SendAllPlayersMessage(string message)
{
	for (auto player : players)
	{
		player->SendMessage(message);
	}
}

vector<Card> GameSession::DrawCards(int amount)
{
	vector<Card> reVector;
	for (int i = 0; i < amount; i++)
	{
		Card reCard = deck[0];
		deck.erase(deck.begin());
		reVector.push_back(reCard);
	}
	return reVector;
}
