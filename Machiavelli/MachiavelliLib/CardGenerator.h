#pragma once
#include "Card.h"

class GameSession;

class CardGenerator
{
public:
	CardGenerator();
	~CardGenerator();
	static vector<Card> CreateAndGetAllCards(GameSession* session);
	static void AddCardXTimes(Card card, vector<Card> &deck, int amountOfTimes);

	static vector<Card> BuildDeckFromFile(string fileName);
};

