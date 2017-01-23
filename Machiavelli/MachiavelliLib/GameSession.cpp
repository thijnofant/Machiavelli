#include "stdafx.h"
#include "GameSession.h"
#include "CardGenerator.h"
#include "PickingCharactersPhase.h"
#include "ExecutingPhase.h"
#include "NotPlayingPhase.h"
#include "LocalHost.h"


GameSession::GameSession(int amountOfPlayers) : 
	amountOfMoneyInBank{ 30 }, 
	amountOfPlayers{ amountOfPlayers }, 
	gameOver{ false },
	deck(CardGenerator::BuildDeckFromFile("../cards.txt"))
{
//	deck = CardGenerator::CreateAndGetAllCards(this);

	this->ShuffleDeck();	
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

shared_ptr<Player> GameSession::GetPlayer(string userName) const
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

vector<shared_ptr<Player>> GameSession::GetPlayers() const
{
	return players;
}

string GameSession::GetStatus()
{
	if (!IsFull())
	{
		return "\r\n Waiting for more players to join...\r\n";
	}

	std::stringstream status;

	status << "\r\n  Amount of money left in bank: " << amountOfMoneyInBank << "\r\n";

	//for debug print all cards in deck
	/*for (auto card : deck)
	{
		status << card.GetName() << " ";
	}*/

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
		map<shared_ptr<Player>, int> scores;
		for (auto player : players)
		{
			scores[player] = 0;

			int numberOfDiferentCollors = 0 + (player->GetAmountOfColourInVillage(Yellow) >= 1 ? 0 : 1) + (player->GetAmountOfColourInVillage(Blue) >= 1 ? 0 : 1) +
				(player->GetAmountOfColourInVillage(Green) >= 1 ? 0 : 1) + (player->GetAmountOfColourInVillage(Red) >= 1 ? 0 : 1) +
				(player->GetAmountOfColourInVillage(Purple) >= 1 ? 0 : 1);
			if(player->HasCardInVillage("Hof der Wonderen"))
				numberOfDiferentCollors++;

			if (numberOfDiferentCollors >= 5)
			{
				scores[player] += 3;
			}

			for (auto card : player->GetVillage())
			{
				scores[player] += card.GetValue();
			}

			if (player->WasFirstToEight()) {
				scores[player] += 4;
			}
			else if (player->GetVillage().size() >= 8)
			{
				scores[player] += 2;
			}
		}

		shared_ptr<Player> highestScoringPlayer;
		int highestScore = 0;
		for (auto pair : scores)
		{
			if (pair.second >= highestScore)
			{
				highestScore = pair.second;
				highestScoringPlayer = pair.first;
			}
			SendAllPlayersMessage(pair.first->GetPlayerName() + " scored " + to_string(pair.second) + " points.");
		}

		SendAllPlayersMessage(highestScoringPlayer->GetPlayerName() + "Won");
		SendAllPlayersMessage("If you want to play again please reconnect to the server.");
		currentPhase = std::make_unique<NotPlayingPhase>();
		return;
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

deque<Card> GameSession::DrawCards(int amount)
{
	deque<Card> reVector;
	for (int i = 0; i < amount; i++)
	{
		Card reCard = deck[0];
		deck.pop_front();
		reVector.push_back(reCard);
	}
	return reVector;
}

void GameSession::AddCardsToDeck(deque<Card> cards)
{
	for (auto card : cards)
	{
		deck.push_back(card);
	}
}

void GameSession::ShuffleDeck()
{
	std::shuffle(std::begin(deck), std::end(deck), Util::GetRandomEngine());
}

std::ostream& operator<<(std::ostream& os, const GameSession& obj)
{
	//session info
	string firstLine = to_string(obj.amountOfMoneyInBank) + ";" + to_string(obj.amountOfPlayers) + ";" + (obj.gameOver ? "1" : "0") + ";" + to_string(obj.GetCurrentPlayer()->GetToken());
	string fileName = "";
	os << firstLine << '\n';

	//deck
	fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "deck" + LocalHost::Extension;
	os << fileName << '\n';
	CardGenerator::BuildFileFromCards(fileName, obj.deck);

	//discard pile
	fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "discard" + LocalHost::Extension;
	os << fileName << '\n';
	CardGenerator::BuildFileFromCards(fileName, obj.discardPile);

	//players
	for (auto player : obj.players)
	{
		fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "player" + to_string(player->GetToken()) + LocalHost::Extension;
		os << fileName << '\n';
		ofstream playerOStream(fileName);
		playerOStream << *player;
		playerOStream.close();

	}

	//gamestate
	fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "phase" + LocalHost::Extension;
	os << fileName;
	ofstream gamePhaseOStream(fileName);
	gamePhaseOStream << obj.currentPhase->ToString();
	gamePhaseOStream.close();

	return os;
}

std::istream& operator>>(std::istream& is, GameSession& obj)
{
	string line = "";

	getline(is, line);

	string segment;
	std::vector<std::string> seglist;
	std::stringstream temp(line);
	while (getline(temp, segment, ';'))
	{
		seglist.push_back(segment);
	}

	obj.amountOfMoneyInBank = stoi(seglist[0]);
	obj.amountOfPlayers = stoi(seglist[1]);
	obj.gameOver = seglist[2] == "1";
	int currentPlayerToken = stoi(seglist[3]);


	//GetDeck
	getline(is, line);
	obj.deck = CardGenerator::BuildDeckFromFile(line);

	//GetDiscardPile
	getline(is, line);
	obj.discardPile = CardGenerator::BuildDeckFromFile(line);

	//GetPlayers
	for (int i = 0; i < obj.amountOfPlayers; i++)
	{
		getline(is, line);

		shared_ptr<Player> player = std::make_shared<Player>();
		std::ifstream playerFileStream(line);
		playerFileStream >> *player;

		obj.players.push_back(player);

		playerFileStream.close();
	}

	//GetPhase
	{
		getline(is, line);
		std::ifstream sessionFileStream(line);

		string sessionFileFirstLine;
		getline(sessionFileStream, sessionFileFirstLine);

		if (sessionFileFirstLine == "NotPlaying")
		{
			obj.currentPhase = std::make_unique<NotPlayingPhase>();
		}
		else if (sessionFileFirstLine == "PickingCharacters")
		{
			auto tempphase = new PickingCharactersPhase();
			sessionFileStream >> *tempphase;
			obj.currentPhase.reset(tempphase);
		}
		else if (sessionFileFirstLine == "Excecuting")
		{
			auto tempphase = new ExecutingPhase();
			sessionFileStream >> *tempphase;
			obj.currentPhase.reset(tempphase);
		}
		sessionFileStream.close();
	}


	obj.SetCurrentPlayer(obj.GetPlayer(currentPlayerToken));
	return is;
}
