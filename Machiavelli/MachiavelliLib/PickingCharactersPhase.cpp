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

bool PickingCharactersPhase::HandleAction(int token, string message, shared_ptr<GameSession> session)
{
	if (subPhase == 1 || subPhase == 2 || subPhase == 3 || subPhase == 4)
	{
		//player gets to pick character
		if (subsubPhase == 1)
		{
			auto player = session->GetCurrentPlayer();
			if (subPhase == 1|| subPhase == 2)
				player->ClearCharacters();

			player->Givecharacter(chararacterStringToEnum[message]);
			availableCharacters.erase(std::remove(availableCharacters.begin(), availableCharacters.end(), chararacterStringToEnum[message]), availableCharacters.end());
			subsubPhase++;

			if (subPhase == 1)
			{
				subPhase++;
				subsubPhase = 1;
				session->NextPlayer();
			}
		}
		//player gets to throw away character
		else if (subsubPhase == 2)
		{
			availableCharacters.erase(std::remove(availableCharacters.begin(), availableCharacters.end(), chararacterStringToEnum[message]), availableCharacters.end());
						
			session->NextPlayer();

			if (subPhase == 4)
			{
				session->SetPhase(GamePhases::Executing, session);
			}
				

			subsubPhase = 1;
			subPhase++;
		}
	}
	return false;
}

vector<string> PickingCharactersPhase::GetActions(int token, shared_ptr<GameSession> session)
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

				player->SendMessage("The " + chararacterEnumToString[pickedChar] + " was discarded");
				player->SendMessage("Choose a character:");

				for (auto available_character : availableCharacters)
				{
					re_vector.push_back(chararacterEnumToString[available_character]);
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
					re_vector.push_back(chararacterEnumToString[available_character]);
				}
			}
			//player gets to throw away character
			else if (subsubPhase == 2)
			{
				auto player = session->GetCurrentPlayer();
				player->SendMessage("Choose a character to discard:");

				for (auto available_character : availableCharacters)
				{
					re_vector.push_back(chararacterEnumToString[available_character]);
				}
			}
		}
	}
	return re_vector;
}

bool PickingCharactersPhase::IsItMyTurn(int token, shared_ptr<GameSession> session)
{
	return session->GetCurrentPlayer() == session->GetPlayer(token);
}
