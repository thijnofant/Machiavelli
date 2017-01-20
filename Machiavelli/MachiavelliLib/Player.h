#pragma once
#include "Character.h"
#include "Card.h"
#include <queue>

class Player
{
private:
	int token;
	string playerName;
	bool isKing;
	int amountOfCoins;
	std::vector<Character> characters;
	std::vector<Card> hand;
	std::vector<Card> village;
	std::queue<string> messages;

public:
	Player(int token, string playerName, bool isKing);
	~Player();

	int GetToken() const;
	string GetPlayerName() const;
	bool IsKing() const;
	int GetAmountOfCoins() const;
	bool HasCharacter(Character character);
	std::vector<Character> GetCharacters() const;
	std::vector<Card> GetHand() const;
	std::vector<Card> GetVillage() const;	
	void SendMessage(string message);
	string GetMessages();
	string GetStatus();
	void ClearCharacters();
	void Givecharacter(Character character);
	void GiveMoney(int amount);
	void SpendMoney(int amount);
	void GiveCards(vector<Card> card);
	bool BuildCard(string cardName);

	bool HasCardInVillage(string cardName);
	int GetAmountOfColourInVillage(CardColour colour);

	void SetIsKing(bool isKing);
};

