#pragma once
#include "Character.h"
#include "Card.h"
#include <queue>

class GameSession;

class Player
{
private:
	int token = -1;
	string playerName = "";
	bool isKing = false;
	bool firstToEight = false;
	int amountOfCoins = 0;
	std::vector<Character> characters;
	std::deque<Card> hand;
	std::deque<Card> village;
	std::queue<string> messages;

public:
	Player(){};
	Player(int token, string playerName, bool isKing);
	~Player();

	int GetToken() const;
	string GetPlayerName() const;
	bool IsKing() const;
	int GetAmountOfCoins() const;
	bool HasCharacter(Character character);
	std::vector<Character> GetCharacters() const;
	std::deque<Card> GetHand() const;
	std::deque<Card> GetVillage() const;	
	void SendMessage(string message);
	string GetMessages();
	string GetStatus();
	void ClearCharacters();
	void Givecharacter(Character character);
	void GiveMoney(int amount);
	void SpendMoney(int amount);
	void GiveCards(deque<Card> card);
	int BuildCard(string cardName);

	bool HasCardInVillage(string cardName);
	int GetAmountOfColourInVillage(CardColour colour);

	void SetIsKing(bool isKing);
	void SetWasFirstToEight(bool isFirst);
	bool WasFirstToEight() const;
	void ClearHand();
	void DestroyBuildingFromVilage(string buildingName);
	void DiscardCardWithNameFromHand(string name);


	//stream functions
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);
	friend std::istream& operator>>(std::istream& is, Player& obj);
};

