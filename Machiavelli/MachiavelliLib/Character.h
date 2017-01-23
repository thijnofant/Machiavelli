#pragma once

enum Character
{
	Moordenaar = 1,
	Dief = 2,
	Magier = 3,
	Koning = 4,
	Prediker = 5,
	Koopman = 6,
	Bouwmeester = 7,
	Condottiere = 8,
};

static map<Character, string> characterEnumToString = { 
	{Moordenaar,"Moordenaar"},
	{ Dief,"Dief" },
	{ Magier,"Magier" },
	{ Koning,"Koning" },
	{ Prediker,"Prediker" },
	{ Koopman,"Koopman" },
	{ Bouwmeester,"Bouwmeester" },
	{ Condottiere,"Condottiere" }, 
};
static map<string, Character> characterStringToEnum = { 
	{ characterEnumToString.at(Moordenaar), Moordenaar },
	{ characterEnumToString.at(Dief), Dief },
	{ characterEnumToString.at(Magier), Magier },
	{ characterEnumToString.at(Koning) ,Koning },
	{ characterEnumToString.at(Prediker),Prediker },
	{ characterEnumToString.at(Koopman),Koopman },
	{ characterEnumToString.at(Bouwmeester),Bouwmeester },
	{ characterEnumToString.at(Condottiere),Condottiere },
};

