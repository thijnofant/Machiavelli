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
	vector<Card> deck; //todo kijk naar dequeu
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
	bool AddPlayer(shared_ptr<Player> newPlayer, shared_ptr<GameSession> session);
	bool HandleAction(int token, string message, shared_ptr<GameSession> session) const;
	vector<string> GetActions(int token, shared_ptr<GameSession> session) const;
	const bool IsItMyTurn(int token, shared_ptr<GameSession> session) const;
	void SetPhase(GamePhases phase, shared_ptr<GameSession> session);
	shared_ptr<Player> GetCurrentPlayer() const;
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
	void SendAllPlayersMessage(string message);
	vector<Card> DrawCards(int amount);
};

