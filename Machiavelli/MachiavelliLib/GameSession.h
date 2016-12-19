#pragma once
#include <vector>
#include "Card.h"
#include "Player.h"
#include "Character.h"
class GamePhase;

enum GamePhases
{
	PickingCharacters = 1,
	Executing = 2,
};
class GameSession: public std::enable_shared_from_this<GameSession>
{
private:
	int amountOfMoneyInBank;
	vector<Card> deck;
	vector<Card> discardPile;
	vector<Character> characters;
	vector<Character> characterDiscardPile;
	vector<shared_ptr<Player>> players;
	int amountOfPlayers;
	bool gameOver;

	shared_ptr<Player> currentPlayer;
	unique_ptr<GamePhase> currentPhase;
	int phaseStep;

public:
	 GameSession(int amountOfPlayers = 2);
	~GameSession();
	bool IsFull() const;
	shared_ptr<Player> GetPlayer(int token);
	shared_ptr<Player> GetPlayer(string playerName);
	string GetStatus();
	string GetStatus(int token);
	bool ContainsPlayer(int token);
	bool ContainsPlayer(string playerName);
	bool AddPlayer(shared_ptr<Player> newPlayer);
	bool HandleAction(int token, string message, shared_ptr<GameSession> session);
	vector<string> GetActions(int token, shared_ptr<GameSession> session);
	bool IsItMyTurn(int token, shared_ptr<GameSession> session);
	void SetPhase(GamePhases phase, shared_ptr<GameSession> session);
	shared_ptr<Player> GetCurrentPlayer();
	void SetCurrentPlayer(shared_ptr<Player> player);
	int GetAmountOfPlayers() const;
	shared_ptr<Player> GetKing();
	shared_ptr<Player> GetPlayerWithCharacter(Character character);
	void MakeKing(int token);
	void MakeKing(string playerName);
	int TakeMoney(int amount);
	void GiveMoney(int amount);
	void NextPlayer();
	bool IsGameOver() const;
	void SetGameOver(bool isGameOver);
};

