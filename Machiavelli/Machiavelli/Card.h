#pragma once
enum CardColour
{
	Yellow = 1,
	Blue = 2,
	Green = 3,
	Red = 4,
	Purple = 5,
};

class Card
{
private:
	int cost;
	int pointValue;
	//todo effect
	CardColour colour;

public:
	Card();
	~Card();
};

