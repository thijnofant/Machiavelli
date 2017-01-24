#pragma once
#include "stdafx.h"
#include "Card.h"
#include "Player.h"
#include "Character.h"
class GamePhase;

enum GamePhases
{
	NotPlaying = 0,
	PickingCharacters = 1,
	Executing = 2,
};
const map<GamePhases, string> GamePhaseEnumToString = { {NotPlaying,"Not playing"},{PickingCharacters,"Picking Characters"},{Executing,"Executing"} };
const map<string, GamePhases> GamePhaseStringToEnum = { { GamePhaseEnumToString.at(NotPlaying), NotPlaying},{ GamePhaseEnumToString.at(PickingCharacters) , PickingCharacters },{ GamePhaseEnumToString.at(Executing) , Executing } };

class GameSession
{
private:
	int amountOfMoneyInBank;
	std::deque<Card> deck;
	std::deque<Card> discardPile;
	vector<shared_ptr<Player>> players;
	int amountOfPlayers;
	bool gameOver;

	shared_ptr<Player> currentPlayer;
	unique_ptr<GamePhase> currentPhase;

public:
	 GameSession(int amountOfPlayers = 2);
	~GameSession();
	bool IsFull() const;
	shared_ptr<Player> GetPlayer(int const token) const;
	shared_ptr<Player> GetPlayer(string const playerName) const;
	vector<shared_ptr<Player>> GetPlayers() const;
	string GetStatus() const;
	string GetStatus(int const token) const;
	bool ContainsPlayer(int const token) const;
	bool ContainsPlayer(string const playerName) const;
	bool AddPlayer(shared_ptr<Player> newPlayer, shared_ptr<GameSession> session);
	bool HandleAction(int const token, string const message, shared_ptr<GameSession> session) const;
	vector<string> GetActions(int const token, shared_ptr<GameSession> session) const;
	bool IsItMyTurn(int const token, shared_ptr<GameSession> session) const;
	void SetPhase(GamePhases const phase, shared_ptr<GameSession> session);
	shared_ptr<Player> GetCurrentPlayer() const;
	void SetCurrentPlayer(shared_ptr<Player> player);
	int GetAmountOfPlayers() const;
	shared_ptr<Player> GetKing() const;
	shared_ptr<Player> GetPlayerWithCharacter(Character const character) const;
	void MakeKing(int const token);
	void MakeKing(string const playerName);
	int TakeMoney(int amount);
	void GiveMoney(int const amount);
	void NextPlayer();
	bool IsGameOver() const;
	void SetGameOver(bool const isGameOver);
	void SendAllPlayersMessage(string const message);
	deque<Card> DrawCards(int const amount);
	void AddCardsToDeck(deque<Card> cards);
	void ShuffleDeck();


	//stream functions
	friend std::ostream& operator<<(std::ostream& os, const GameSession& obj);
	friend std::istream& operator>>(std::istream& is, GameSession& obj);
};

