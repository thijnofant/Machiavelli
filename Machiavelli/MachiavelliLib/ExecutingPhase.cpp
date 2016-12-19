#include "stdafx.h"
#include "ExecutingPhase.h"

ExecutingPhase::ExecutingPhase(shared_ptr<GameSession> session) : amountBuiltThisTurn{ 0 }, currentlyBuilding(false)
{
	subPhase = 0;
	HandTurnToNextCharacter(session);
}


ExecutingPhase::~ExecutingPhase()
{
}

bool ExecutingPhase::HandleAction(int token, string message, shared_ptr<GameSession> session)
{
	auto player = session->GetCurrentPlayer();

	if (currentlyBuilding)
	{
		for (auto card : player->GetHand())
		{
			if (message == card.GetName())
			{
				//todo build deze kaart en haal hem uit de hand
				currentlyBuilding = false;
				amountBuiltThisTurn++;
			}
		}
	}

	if (message == "Draw Cards")
	{	
		//todo draw cards from deck
		subsubPhase++;
		return true;
	}
	if (message == "Take Money")
	{
		int moneyTaken = session->TakeMoney(2);
		player->GiveMoney(moneyTaken);
		player->SendMessage("You took " + std::to_string(moneyTaken) + " coins from the bank.");
		subsubPhase++;
		return true;
	}
	if (message == "Build")
	{
		currentlyBuilding = true;
		return true;
	}
	if (message == "End Turn")
	{
		this->HandTurnToNextCharacter(session);
		return true;
	}

	if (message == "Moordenaar")
	{
		characterActionUsed = true;
		return true;
		//todo zorg voor een manier om te vragen om characters om te doden
	}
	if(message == "Dief")
	{
		//todo zorg voor een manier om te vragen om characters om te bestelen
		return true;
	}
	if (message == "Magier")
	{
		//todo voer magier uit
		return true;
	}
	if (message == "Koning")
	{
		//todo voer Koning uit
		return true;
	}
	if (message == "Prediker")
	{
		//todo voer Prediker uit
		return true;
	}
	if (message == "Koopman")
	{
		//todo voer Koopman uit
		return true;
	}
	if (message == "Bouwmeester")
	{
		//todo voer Bouwmeester uit
		return true;
	}
	if (message == "Condottiere")
	{
		//todo voer Condottiere uit
		//todo kies een gebouw om te vernietigen
		//todo controleer of dat gebouw vernietiegd kan worden
		return true;
	}

	//todo kaart effecten
	//todo implement this
	return true;
}

vector<string> ExecutingPhase::GetActions(int token, shared_ptr<GameSession> session)
{
	//todo return a list of commands that can be excecuted by the player at this point
	vector<string> re_vector;

	if (subsubPhase == 1)
	{
		re_vector.push_back("Draw Cards");
		re_vector.push_back("Take Money");
	}
	if (subsubPhase == 2)
	{
		if (currentlyBuilding)
		{
			for (auto card : session->GetCurrentPlayer()->GetHand())
			{
				re_vector.push_back(card.GetName());
			}
			
			//todo loop door hand heen en vind een kaart met een naam die klopt met de message
			amountBuiltThisTurn++;
		}
		else {
			//todo test
			//kijk of speler nog mag bouwen
			if (subPhase == Bouwmeester && amountBuiltThisTurn < 3)
			{
				re_vector.push_back("Build");
			}
			else if (amountBuiltThisTurn < 1)
			{
				re_vector.push_back("Build");
			}
		}
	}	

	//Note als character niet dood is en nog niet zijn actie heeft uitgevoerd kan hij actie nog uitvoeren
	//todo moordenaar werkt niet?
	if (!WasCharKilled(subPhase) && !characterActionUsed)
		{
		switch (subPhase)
		{
		case Moordenaar:
			re_vector.push_back("Moordenaar");
			break;
		case Dief:
			re_vector.push_back("Dief");
			break;
		case Magier:
			re_vector.push_back("Magier");
			break;
		case Koning:
			re_vector.push_back("Koning");
			session->MakeKing(token);
			break;
		case Prediker:
			re_vector.push_back("Prediker");
			break;
		case Koopman:
			re_vector.push_back("Koopman");
			break;
		case Bouwmeester:
			re_vector.push_back("Bouwmeester");
			break;
		case Condottiere:
			re_vector.push_back("Condottiere");
			break;
		}
	}

	re_vector.push_back("End Turn");
	return re_vector;

	//todo implement this
}

void ExecutingPhase::HandTurnToNextCharacter(shared_ptr<GameSession> session)
{
	//check for game over
	{
		auto player = session->GetCurrentPlayer();
		if (player->GetVillage().size() >= 8)
		{
			session->SetGameOver(true);
		}
	}
	if (subPhase == 8)
	{
		session->SetCurrentPlayer(session->GetKing());
		session->SetPhase(PickingCharacters, session);
		return;
	}

	subPhase++;
	subsubPhase = 1;
	amountBuiltThisTurn = 0;
	characterActionUsed = false;

	auto player = session->GetPlayerWithCharacter(static_cast<Character>(subPhase));
	bool done = player != nullptr;

	while (!done)
	{
		subPhase++;
		if (subPhase == 9)
		{
			session->SetCurrentPlayer(session->GetKing());
			session->SetPhase(PickingCharacters, session);
			return;
		}
		player = session->GetPlayerWithCharacter(static_cast<Character>(subPhase));
		if (false/*todo test for char death*/)
		{
			done = false;
			continue;
		}
		done = player != nullptr;
	}

	session->SetCurrentPlayer(player);
}

bool ExecutingPhase::IsItMyTurn(int token, shared_ptr<GameSession> session)
{
	return  session->GetPlayerWithCharacter(static_cast<Character>(subPhase)) == session->GetPlayer(token);
}
