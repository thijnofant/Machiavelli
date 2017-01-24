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
	Player(int const token, string const playerName, bool const isKing);
	~Player();

	int GetToken() const;
	string GetPlayerName() const;
	bool IsKing() const;
	int GetAmountOfCoins() const;
	bool HasCharacter(Character const character) const;
	std::vector<Character> GetCharacters() const;
	std::deque<Card> GetHand() const;
	std::deque<Card> GetVillage() const;	
	void SendMessage(string const message);
	string GetMessages();
	string GetStatus() const;
	void ClearCharacters();
	void Givecharacter(Character character);
	void GiveMoney(int const amount);
	int SpendMoney(int const amount);
	void GiveCards(deque<Card> const card);
	int BuildCard(string const cardName);

	bool HasCardInVillage(string const cardName) const;
	int GetAmountOfColourInVillage(CardColour const colour) const;

	void SetIsKing(bool const isKing);
	void SetWasFirstToEight(bool const isFirst);
	bool WasFirstToEight() const;
	void ClearHand();
	Card GetCard(string const buildingName);
	Card DestroyBuildingFromVilage(string const buildingName);
	void DiscardCardWithNameFromHand(string const name);


	//stream functions
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);
	friend std::istream& operator>>(std::istream& is, Player& obj);
};

