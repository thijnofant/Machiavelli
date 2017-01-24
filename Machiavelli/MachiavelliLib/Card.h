#pragma once
enum CardColour
{
	Yellow = 1,
	Blue = 2,
	Green = 3,
	Red = 4,
	Purple = 5
};

const map<CardColour, string> CardColourEnumToString = {
	{Yellow, "geel"}, 
	{Blue, "blauw"}, 
	{Green, "groen"}, 
	{Red, "rood"}, 
	{Purple, "lila"}
};
const map<string, CardColour> CardColourStringToEnum = { 
	{ CardColourEnumToString.at(Yellow), Yellow },
	{ CardColourEnumToString.at(Blue), Blue },
	{ CardColourEnumToString.at(Green), Green },
	{ CardColourEnumToString.at(Red), Red },
	{ CardColourEnumToString.at(Purple), Purple } 
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
	Card(): cost(0), pointValue(0), colour(CardColour::Purple), text(""), destructable(true)
	{}

	Card(string const name, int const cost, int const pointValue, CardColour const colour, string const text = "", bool const destructable = true);
	~Card();
	friend std::ostream& operator<<(std::ostream& is, const Card& obj);
	friend std::istream& operator>>(std::istream& is, Card& obj);

	string GetName() const;
	int GetCost() const;
	int GetValue() const;
	CardColour GetColour() const;
};

