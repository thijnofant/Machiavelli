#include "stdafx.h"
#include "Player.h"
#include "CardGenerator.h"

Player::Player(int token, string playerName, bool isKing): token{token}, playerName{playerName}, isKing{isKing}, firstToEight(false), amountOfCoins{0}
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

bool Player::HasCharacter(Character character)
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

void Player::SendMessage(string message)
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

string Player::GetStatus()
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

void Player::GiveMoney(int amount)
{
	amountOfCoins += amount;
}

void Player::SpendMoney(int amount)
{
	amountOfCoins -= amount;
}

void Player::GiveCards(deque<Card> cards)
{
	for (auto card : cards)
	{
		hand.push_back(card);
	}
}

int Player::BuildCard(string cardName)
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
				return false;
			}

			SpendMoney(card.GetCost());

			village.push_back(card);

			DiscardCardWithNameFromHand(cardName);

			return card.GetCost();
		}
	}
	return -1;
}

bool Player::HasCardInVillage(string cardName)
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

int Player::GetAmountOfColourInVillage(CardColour colour)
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

void Player::SetIsKing(bool isKing)
{
	this->isKing = isKing;
}

void Player::SetWasFirstToEight(bool isFirst)
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

void Player::DestroyBuildingFromVilage(string buildingName)
{
	auto it = std::find_if(village.begin(), village.end(), [buildingName](Card a)->bool { return a.GetName() == buildingName; });

	if (it != village.end())
	{
		village.erase(it);
	}
}

void Player::DiscardCardWithNameFromHand(string name)
{
	auto it = std::find_if(hand.begin(), hand.end(), [name](Card a)->bool { return a.GetName() == name; });

	if (it != hand.end())
	{
		hand.erase(it);
	}
}

std::ostream& operator<<(std::ostream& os, const Player& obj)
{
	//todo implement this function
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
	//todo GetCharacters
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
