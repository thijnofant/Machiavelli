#include "stdafx.h"
#include "CardGenerator.h"
#include "GameSession.h"


CardGenerator::CardGenerator()
{
}


CardGenerator::~CardGenerator()
{
}

deque<Card> CardGenerator::CreateAndGetAllCards(GameSession* session)
{
	deque<Card> r_vector;
	//Rode kaarten
	{
		AddCardXTimes(Card{ "Wachttoren",1,1,CardColour::Red }, r_vector, 3);
		AddCardXTimes(Card{ "Kerker",2,2,CardColour::Red }, r_vector, 3);
		AddCardXTimes(Card{ "Toernooiveld",3,3,CardColour::Red }, r_vector, 3);
		AddCardXTimes(Card{ "Vesting",5,5,CardColour::Red }, r_vector, 2);
	}

	//Blauwe kaarten
	{
		AddCardXTimes(Card{ "Tempel",1,1,CardColour::Blue }, r_vector, 3);
		AddCardXTimes(Card{ "Kerk",2,2,CardColour::Blue }, r_vector, 3);
		AddCardXTimes(Card{ "Abdij",3,3,CardColour::Blue }, r_vector, 3);
		AddCardXTimes(Card{ "Kathedraal",5,5,CardColour::Blue }, r_vector, 2);
	}

	//Groene kaarten
	{
		AddCardXTimes(Card{ "Taveerne",1,1,CardColour::Green }, r_vector, 5);
		AddCardXTimes(Card{ "Markt",2,2,CardColour::Green }, r_vector, 4);
		AddCardXTimes(Card{ "Gildehuis",2,2,CardColour::Green }, r_vector, 3);
		AddCardXTimes(Card{ "Handelshuis",3,3,CardColour::Green }, r_vector, 3);
		AddCardXTimes(Card{ "Haven",4,4,CardColour::Green }, r_vector, 3);
		AddCardXTimes(Card{ "Raadhuis",5,5,CardColour::Green }, r_vector, 2);
	}

	//Gele kaarten
	{
		AddCardXTimes(Card{ "Jachtslot",3,3,CardColour::Yellow }, r_vector, 5);
		AddCardXTimes(Card{ "Slot",4,4,CardColour::Yellow }, r_vector, 4);
		AddCardXTimes(Card{ "Paleis",5,5,CardColour::Yellow }, r_vector, 3);
	}

	//Paarse kaarten
	{
		AddCardXTimes(Card{ "Hof der wonderen",2,2,CardColour::Purple , "Bij het tellen van overwinningspunten mag je het Hof der wonderen als een gebouw met een kleur naar keuze beschouwen." }, r_vector, 1);
		AddCardXTimes(Card{ "Verdedigingstoren",3,3,CardColour::Purple , "De verdedigingstoren kan niet door de condottiere worden vernietigd", false }, r_vector, 2);
		AddCardXTimes(Card{ "Kerkhof",5,5,CardColour::Purple , "Als de Condottiere een gebouw vernietigd mag je 1 geldstuk betalen om dat gebouw in je hand te nemen. Je mag dit niet doen als je zelf de condotitiere bent." }, r_vector, 1);
		AddCardXTimes(Card{ "Laboratorium",5,5,CardColour::Purple , "Je mag tijdens je beurt eenmaal handkaarten afleggen en daarvoor 1 goudstuk uit de vooraad nemen." }, r_vector, 1);
		AddCardXTimes(Card{ "Observatorium",5,5,CardColour::Purple , "Als je in je inkomsten fase voor kaarten kiest mag je er 3 trekken waarvan je er 1 houdt en 2 aflegd(uitzondering bibliotheek)" }, r_vector, 1);
		AddCardXTimes(Card{ "Smederij",5,5,CardColour::Purple , "Je mag op elk moment tijdens je beurt eenmaal 3 goudstukken betalen om 2 kaarten van de gedekte stapel te trekken." }, r_vector, 1);
		AddCardXTimes(Card{ "Bibliotheek",6,6,CardColour::Purple , "Als je in je inkomstenfase voor kaarten kiest, mag je 2 kaarten houden." }, r_vector, 1);
		AddCardXTimes(Card{ "School voor magiers",6,6,CardColour::Purple , "Tijdens je inkomstenfase neemt de school voor magiers een kleur naar keuze aan. Je krijgt 1 extra goudstuk als je de Koning, Prediker, Koopman of Condottiere bent." }, r_vector, 1);
		AddCardXTimes(Card{ "Universiteit",6,8,CardColour::Purple , "Dit prestigieuze gebouw kost 6 goudstukken en levert aan het einde van het spel 8 punten op." }, r_vector, 1);
		AddCardXTimes(Card{ "Drakenburcht",6,8,CardColour::Purple , "Dit prestigieuze gebouw kost 6 goudstukken en levert aan het einde van het spel 8 punten op." }, r_vector, 1);
	}
	return r_vector;
}

void CardGenerator::AddCardXTimes(Card card, deque<Card>& deck, int amountOfTimes)
{
	for (size_t i = 0; i < amountOfTimes; i++)
	{
		deck.push_back(card);
	}
}

deque<Card> CardGenerator::BuildDeckFromFile(string fileName)
{
	std::ifstream is(fileName);
	deque<Card> _deck;
	Card newCard;
	//NOTE: if failbit set, does not enter while.
	while (is >> newCard)
	{
		if (is.good() || is.eof())
		{
			_deck.push_back(newCard);
		}
		//NOTE: skip card if bad line and continue.
		else
		{
			is.setstate(std::ios::goodbit);
		}
	}

	is.close();
	return _deck;
}

void CardGenerator::BuildFileFromCards(string fileName, deque<Card> deck)
{
	std::ofstream os(fileName);

	for (auto card : deck)
	{
		os << card << '\n';
	}
	os.close();
}
