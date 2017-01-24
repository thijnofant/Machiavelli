#include "stdafx.h"
#include "Card.h"

Card::Card(string const name, int const cost, int const pointValue, CardColour const colour, string const text, bool const destructable) : 
cost{ cost }, pointValue{ pointValue }, colour{ colour }, name{ name }, text{ text }, destructable{ destructable }
{
}

Card::~Card()
{
}

string Card::GetName() const
{
	return name;
}

int Card::GetCost() const
{
	return cost;
}

int Card::GetValue() const
{
	return pointValue;
}

CardColour Card::GetColour() const
{
	return colour;
}

std::ostream& operator<<(std::ostream& os, const Card& obj)
{
	os << obj.name << ';';
	os << to_string(obj.cost) << ';';
	os << to_string(obj.pointValue) << ';';
	os << CardColourEnumToString.at(obj.colour) << ';';
	os << obj.text << ';';
	os << (obj.destructable ? "1" : "0");
	return os;
}

std::istream& operator>>(std::istream& is, Card& obj)
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


