#pragma once
#include <vector>
#include "Card.h"
#include "Player.h"
#include "Character.h"

class GameSession
{
private:
	int amountOfMoneyInBank;
	vector<Card> deck;
	vector<Card> discardPile;
	vector<Character> characters;
	vector<Character> characterDiscardPile;
	vector<Player> players;
	int currentCharacterTurn;
public:
	GameSession();
	~GameSession();
};

