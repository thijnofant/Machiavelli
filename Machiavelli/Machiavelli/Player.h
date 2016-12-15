#pragma once
#include "Character.h"
#include "Card.h"

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
public:
	Player();
	~Player();
	
};

