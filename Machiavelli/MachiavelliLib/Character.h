#pragma once
#include <map>

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

static std::map<Character, string> chararacterEnumToString = { {Moordenaar,"Moordenaar"},{ Dief,"Dief" },{ Magier,"Magier" },{ Koning,"Koning" },{ Prediker,"Prediker" },{ Koopman,"Koopman" },{ Bouwmeester,"Bouwmeester" },{ Condottiere,"Condottiere" }, };
static std::map<string, Character> chararacterStringToEnum = { {"Moordenaar", Moordenaar },{"Dief", Dief },{"Magier", Magier },{ "Koning" ,Koning },{ "Prediker",Prediker },{ "Koopman",Koopman },{ "Bouwmeester",Bouwmeester },{ "Condottiere",Condottiere }, };


