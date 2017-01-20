#include "stdafx.h"
#include "Player.h"


Player::Player(int token, string playerName, bool isKing): token{token}, playerName{playerName}, isKing{isKing}, amountOfCoins{0}
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

std::vector<Card> Player::GetHand() const
{
	return hand;
}

std::vector<Card> Player::GetVillage() const
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
		status << " " << chararacterEnumToString[character];
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

void Player::GiveCards(vector<Card> cards)
{
	for (auto card : cards)
	{
		hand.push_back(card);
	}
}

bool Player::BuildCard(string cardName)
{
	for (auto card : hand)
	{
		if (cardName == card.GetName())
		{
			if (card.GetCost() > this->amountOfCoins)
			{
				SendMessage("Insuficient funds to build the " + card.GetName());
				return false;
			}

			this->amountOfCoins -= card.GetCost();

			village.push_back(card);
			return true;

			//todo remove from hand!
			
			//vector<Card> reVector;
			//deck.erase(deck.begin());
			//reVector.push_back(reCard);
		}
	}
	return false;
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
