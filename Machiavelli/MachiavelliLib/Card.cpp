#include "stdafx.h"
#include "Card.h"

Card::Card(string name, int cost, int pointValue, CardColour colour, string text, bool destructable) : cost{ cost }, pointValue{ pointValue }, colour{ colour }, name{ name }, text{ text }, destructable{ destructable }
{
}

Card::~Card()
{
}

string Card::GetName()
{
	return name;
}
