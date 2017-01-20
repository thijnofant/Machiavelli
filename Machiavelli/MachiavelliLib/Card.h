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

	Card(string name, int cost, int pointValue, CardColour colour, string text = "", bool destructable = true);
	~Card();
	friend std::ostream& operator<<(std::ostream& is, const Card& obj);

	friend std::istream& operator>>(std::istream& is, Card& obj)
	{
		string line;
		getline(is, line);

		if (line.empty())
		{
			is.setstate(std::ios::badbit);
			return is;
		}

		string segment;
		std::vector<std::string> seglist;
		std::stringstream temp(line);
		while (getline(temp, segment, ';'))
		{
			seglist.push_back(segment);
		}

		obj.name = seglist[0];
		obj.cost = stoi(seglist[1]);
		obj.pointValue = stoi(seglist[2]);
		obj.colour = CardColourStringToEnum.at(seglist[3]);
		obj.text = seglist[4];
		obj.destructable = seglist[5] == "1";

		return is;
	}

	string GetName() const;
	int GetCost() const;
	CardColour GetColour() const;
};

inline std::ostream& operator<<(std::ostream& os, const Card& obj)
{
	// stream obj's data into os
	return os;
}



