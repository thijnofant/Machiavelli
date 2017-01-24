#include "stdafx.h"
#include "Player.h"
#include "CardGenerator.h"
#include "LocalHost.h"

Player::Player(int const token, string const playerName, bool const isKing): 
token{token}, playerName{playerName}, isKing{isKing}, firstToEight(false), amountOfCoins{0}
{
}


Player::~Player()
{
}

int Player::GetToken() const
{
	return token;
}

string Player::GetPlayerName() const
{
	return playerName;
}

bool Player::IsKing() const
{
	return isKing;
}

int Player::GetAmountOfCoins() const
{
	return amountOfCoins;
}

bool Player::HasCharacter(Character const character)  const
{
	for (auto playersCharacters : characters)
	{
		if (playersCharacters == character)
		{
			return true;
		}
	}
	return false;
}

std::vector<Character> Player::GetCharacters() const
{
	return characters;
}

std::deque<Card> Player::GetHand() const
{
	return hand;
}

std::deque<Card> Player::GetVillage() const
{
	return village;
}

void Player::SendMessage(string const message)
{
	messages.push(message);
}

string Player::GetMessages()
{
	std::stringstream status;
	while (!messages.empty())
	{
		status << messages.front() << "\r\n";
		messages.pop();
	}
	string reval = status.str();
	reval = reval.substr(0, reval.size() - 1);
	return reval;
}

string Player::GetStatus() const
{
	std::stringstream status;

	status << "You have " << amountOfCoins << " coins. \r\n";
	status << "Your hand contains:";
	for (auto card : hand)
	{
		status << " " << card.GetName();
	}
	status << "\r\n";
	status << "Your characters are:";
	for (auto character : characters)
	{
		status << " " << characterEnumToString[character];
	}

	return status.str();
}

void Player::ClearCharacters()
{
	characters.clear();
}

void Player::Givecharacter(Character character)
{
	characters.push_back(character);
}

void Player::GiveMoney(int const amount)
{
	amountOfCoins += amount;
}

int Player::SpendMoney(int amount)
{
	if (amount > amountOfCoins)
	{
		amount = amountOfCoins;
	}
	amountOfCoins -= amount;
	return amount;
}

void Player::GiveCards(deque<Card> const cards)
{
	for (auto card : cards)
	{
		hand.push_back(card);
	}
}

int Player::BuildCard(string const cardName)
{
	
	if (HasCardInVillage(cardName))
	{
		SendMessage("You already have a copy of " + cardName + " in your village.");
		return -1;
	}

	for (auto card : hand)
	{
		if (cardName == card.GetName())
		{
			if (card.GetCost() > this->amountOfCoins)
			{
				SendMessage("Insuficient funds to build the " + card.GetName());
				return -1;
			}

			SpendMoney(card.GetCost());

			village.push_back(card);

			DiscardCardWithNameFromHand(cardName);

			return card.GetCost();
		}
	}
	return -1;
}

bool Player::HasCardInVillage(string const cardName) const
{
	for (auto building : village)
	{
		if( building.GetName() == cardName)
		{
			return true;
		}
	}
	return false;
}

int Player::GetAmountOfColourInVillage(CardColour const colour) const
{
	int amount = 0;
	for (auto building : village)
	{
		if (building.GetColour() == colour)
		{
			amount++;
		}
	}
	return amount;
}

void Player::SetIsKing(bool const isKing)
{
	this->isKing = isKing;
}

void Player::SetWasFirstToEight(bool const isFirst)
{
	this->firstToEight = isFirst;
}

bool Player::WasFirstToEight() const
{
	return firstToEight;
}

void Player::ClearHand()
{
	hand.clear();
}

Card Player::GetCard(string const buildingName)
{
	return *std::find_if(village.begin(), village.end(), [buildingName](Card a)->bool { return a.GetName() == buildingName; });
}

Card Player::DestroyBuildingFromVilage(string const buildingName)
{
	Card reval = GetCard(buildingName);

	auto it = std::find_if(village.begin(), village.end(), [buildingName](Card a)->bool { return a.GetName() == buildingName; });

	if (it != village.end())
	{
		village.erase(it);
	}

	return reval;
}

void Player::DiscardCardWithNameFromHand(string const name)
{
	auto it = std::find_if(hand.begin(), hand.end(), [name](Card a)->bool { return a.GetName() == name; });

	if (it != hand.end())
	{
		hand.erase(it);
	}
}

std::ostream& operator<<(std::ostream& os, const Player& obj)
{
	os <<
		to_string(obj.token) << ';' <<
		obj.playerName << ';' <<
		(obj.isKing ? 1 : 0) << ';' <<
		(obj.firstToEight ? 1 : 0) << ';' <<
		to_string(obj.amountOfCoins) << '\n';

	string fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "player" + to_string(obj.GetToken()) + "hand" + LocalHost::Extension;
	CardGenerator::BuildFileFromCards(fileName, obj.hand);
	os << fileName << '\n';

	fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "player" + to_string(obj.GetToken()) + "village" + LocalHost::Extension;
	CardGenerator::BuildFileFromCards(fileName, obj.village);
	os << fileName << '\n';

	for (auto character : obj.characters)
	{
		os << characterEnumToString.at(character) << '\n';
	}

	return os;
}

std::istream& operator>>(std::istream& is, Player& obj)
{
	string line = "";
	getline(is, line);

	string segment;
	std::vector<std::string> seglist;
	std::stringstream temp(line);
	while (getline(temp, segment, ';'))
	{
		seglist.push_back(segment);
	}

	//token;playerName;isKing;firstToEight;amountOfCoins;
	obj.token = stoi(seglist[0]);
	obj.playerName = seglist[1];
	obj.isKing = seglist[2] == "1";
	obj.firstToEight = seglist[3] == "1";
	obj.amountOfCoins = stoi(seglist[4]);

	//GetHand
	getline(is, line);
	obj.hand = CardGenerator::BuildDeckFromFile(line);

	//GetVillage
	getline(is, line);
	obj.village = CardGenerator::BuildDeckFromFile(line);

	//GetCharacters
	while(getline(is, line))
	{
		if (line != "")
		{
			obj.characters.push_back(characterStringToEnum.at(line));
		}
	}

	is.setstate(std::ios::goodbit);
	return is;
}
