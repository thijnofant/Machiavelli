#pragma once
enum CardColour
{
	Yellow = 1,
	Blue = 2,
	Green = 3,
	Red = 4,
	Purple = 5
};

class Card
{
private:
	int cost;
	int pointValue;	
	CardColour colour;
	string name;
	string text;
	bool destructable;

public:
	Card(string name, int cost, int pointValue, CardColour colour, string text = "", bool destructable = true);
	~Card();

	string GetName();
};

