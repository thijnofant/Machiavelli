#pragma once
#include "Card.h"

class GameSession;

class CardGenerator
{
public:
	//static deque<Card> CreateAndGetAllCards(GameSession* session);
	//static void AddCardXTimes(Card card, deque<Card> &deck, int amountOfTimes);

	static deque<Card> BuildDeckFromFile(string const fileName);
	static void BuildFileFromCards(string const fileName, deque<Card> const deck);
};

