#pragma once
#include "Card.h"

class GameSession;

class CardGenerator
{
public:
	CardGenerator();
	~CardGenerator();
	static deque<Card> CreateAndGetAllCards(GameSession* session);
	static void AddCardXTimes(Card card, deque<Card> &deck, int amountOfTimes);

	static deque<Card> BuildDeckFromFile(string fileName);
};

