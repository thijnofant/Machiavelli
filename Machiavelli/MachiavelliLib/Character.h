#pragma once

//todo haal deze uit een file
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

//todo maak dit iets mooier
static map<Character, string> characterEnumToString = { {Moordenaar,"Moordenaar"},{ Dief,"Dief" },{ Magier,"Magier" },{ Koning,"Koning" },{ Prediker,"Prediker" },{ Koopman,"Koopman" },{ Bouwmeester,"Bouwmeester" },{ Condottiere,"Condottiere" }, };
static map<string, Character> characterStringToEnum = { {"Moordenaar", Moordenaar },{"Dief", Dief },{"Magier", Magier },{ "Koning" ,Koning },{ "Prediker",Prediker },{ "Koopman",Koopman },{ "Bouwmeester",Bouwmeester },{ "Condottiere",Condottiere }, };


