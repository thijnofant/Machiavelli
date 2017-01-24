#include "stdafx.h"
#include "PickingCharactersPhase.h"
#include "Util.h"


PickingCharactersPhase::PickingCharactersPhase()
{
	availableCharacters.push_back(Character::Moordenaar);
	availableCharacters.push_back(Character::Dief);
	availableCharacters.push_back(Character::Magier);
	availableCharacters.push_back(Character::Koning);
	availableCharacters.push_back(Character::Prediker);
	availableCharacters.push_back(Character::Koopman);
	availableCharacters.push_back(Character::Bouwmeester);
	availableCharacters.push_back(Character::Condottiere);

	std::shuffle(std::begin(availableCharacters), std::end(availableCharacters), Util::GetRandomEngine());
}

PickingCharactersPhase::~PickingCharactersPhase()
{
}

bool PickingCharactersPhase::HandleAction(int const token, string const message, shared_ptr<GameSession> session)
{
	if (subPhase == 1 || subPhase == 2 || subPhase == 3 || subPhase == 4)
	{
		auto player = session->GetCurrentPlayer();

		//player gets to pick character
		if (subsubPhase == 1)
		{
			if (subPhase == 1|| subPhase == 2)
				player->ClearCharacters();

			player->Givecharacter(characterStringToEnum[message]);
			availableCharacters.erase(std::remove(availableCharacters.begin(), availableCharacters.end(), characterStringToEnum[message]), availableCharacters.end());
			player->SendMessage("You picked the " + message);
			subsubPhase++;

			if (subPhase == 1)
			{
				subPhase++;
				subsubPhase = 1;
				session->NextPlayer();
			}
			return true;
		}
		//player gets to throw away character
		else if (subsubPhase == 2)
		{
			availableCharacters.erase(std::remove(availableCharacters.begin(), availableCharacters.end(), characterStringToEnum[message]), availableCharacters.end());
			player->SendMessage("You discarded the " + message);
						
			session->NextPlayer();

			if (subPhase == 4)
			{
				session->SetPhase(GamePhases::Executing, session);
			}
				

			subsubPhase = 1;
			subPhase++;
			return true;
		}
	}
	return false;
}

vector<string> PickingCharactersPhase::GetActions(int const token, shared_ptr<GameSession> session)
{
	vector<string> re_vector;
	if (session->GetAmountOfPlayers() == 2)
	{
		//king gets to pick character
		if (subPhase == 1)
		{
			if (subsubPhase == 1)
			{
				auto player = session->GetKing();
				session->SetCurrentPlayer(player);
								
				auto pickedChar = availableCharacters[Util::GetRandomNumber(0, availableCharacters.size() - 1)];
				availableCharacters.erase(std::remove(availableCharacters.begin(), availableCharacters.end(), pickedChar), availableCharacters.end());

				player->SendMessage("The " + characterEnumToString[pickedChar] + " was discarded");
				player->SendMessage("Choose a character:");

				for (auto available_character : availableCharacters)
				{
					re_vector.push_back(characterEnumToString[available_character]);
				}
			}
		}
		//p2 gets to pick character
		else if (subPhase == 2 || subPhase == 3 || subPhase == 4)
		{
			//player gets to pick character
			if(subsubPhase == 1)
			{
				auto player = session->GetCurrentPlayer();
				player->SendMessage("Choose a character:");

				for (auto available_character : availableCharacters)
				{
					re_vector.push_back(characterEnumToString[available_character]);
				}
			}
			//player gets to throw away character
			else if (subsubPhase == 2)
			{
				auto player = session->GetCurrentPlayer();
				player->SendMessage("Choose a character to discard:");

				for (auto available_character : availableCharacters)
				{
					re_vector.push_back(characterEnumToString[available_character]);
				}
			}
		}
	}
	return re_vector;
}

bool PickingCharactersPhase::IsItMyTurn(int const token, shared_ptr<GameSession> session) const
{
	return session->GetCurrentPlayer() == session->GetPlayer(token);
}

string PickingCharactersPhase::ToString() const
{
	std::stringstream stream;
	stream << "PickingCharacters" << '\n';
	stream << to_string(subPhase) << ';' << to_string(subsubPhase) << '\n';

	//available characters
	for (auto character : availableCharacters)
	{
		stream << characterEnumToString.at(character) << '\n';
	}

	return stream.str();
}

std::istream& operator>>(std::istream& is, PickingCharactersPhase& obj)
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

	//subPhase;subsubPhase
	{
		obj.subPhase = stoi(seglist[0]);
		obj.subsubPhase = stoi(seglist[1]);
	}

	//vector<Character> availableCharacters;
	obj.availableCharacters.clear();
	while(getline(is ,line))
	{
		if (line != "")
		{
			obj.availableCharacters.push_back(characterStringToEnum.at(line));
		}
	}

	return is;
}
