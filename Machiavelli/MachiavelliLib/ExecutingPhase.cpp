#include "stdafx.h"
#include "ExecutingPhase.h"
#include "CardGenerator.h"
#include "LocalHost.h"

ExecutingPhase::ExecutingPhase()
{
	subPhase = 0;
}

bool ExecutingPhase::WasCharKilled(int character) const
{
	return character == charToKill;
}

bool ExecutingPhase::WasCharRobbed(int character) const
{
	return character == charToRob;
}

ExecutingPhase::ExecutingPhase(shared_ptr<GameSession> session) : amountBuiltThisTurn{ 0 }, characterActionUsed{ false }, currentlyBuilding(false), selectingCharacter{ false }, discardingCardsFromBuffer{false}, condottiereSelectingPlayer{false}, magierTrading{false}, buildingsSafeFromCondottiere{false}
{
	subPhase = 0;
	HandTurnToNextCharacter(session);
}


ExecutingPhase::~ExecutingPhase()
{
}

bool ExecutingPhase::HandleAction(int token, string message, shared_ptr<GameSession> session)
{
	auto currentPlayer = session->GetCurrentPlayer();

	if (currentlyBuilding)
	{
		int cost = currentPlayer->BuildCard(message);
		if(cost != -1)
		{
			session->GiveMoney(cost);
			amountBuiltThisTurn++;
			session->SendAllPlayersMessage(currentPlayer->GetPlayerName() + " has built a " + message + "in his village.");
		}
		currentlyBuilding = false;
	}

	if (message == "Draw Cards")
	{	
		int drawAmount;
		if(currentPlayer->HasCardInVillage("Observatorium"))
		{
			session->SendAllPlayersMessage(currentPlayer->GetPlayerName() + " has drawn an additional card due to the effect of their Observatorium.");
			drawAmount = 3;
		}
		else
		{
			session->SendAllPlayersMessage(currentPlayer->GetPlayerName() + " has drawn 2 cards.");
			drawAmount = 2;
		}
		currentPlayer->HasCardInVillage("Observatorium") ? 3 : 2;
		cardBuffer = session->DrawCards(drawAmount);
		discardingCardsFromBuffer = true;

		int amountOfCardsToDiscard = cardBuffer.size() - (currentPlayer->HasCardInVillage("Bibliotheek") ? 2 : 1);
		if (amountOfCardsToDiscard >= 0)
		{
			currentPlayer->SendMessage("Select " + to_string(amountOfCardsToDiscard) + " card(s) to discard.");
		}
		return true;
	}

	if (discardingCardsFromBuffer)
	{
		auto it = std::find_if(cardBuffer.begin(), cardBuffer.end(), [message](Card a)->bool { return a.GetName() == message; });

		if (it != cardBuffer.end()) {
			cardBuffer.erase(it);
			session->SendAllPlayersMessage(currentPlayer->GetPlayerName() + " has discarded a card.");
		}

		int maxSize = currentPlayer->HasCardInVillage("Bibliotheek") ? 2 : 1;
		if (cardBuffer.size() <= maxSize)
		{
			currentPlayer->GiveCards(cardBuffer);
			cardBuffer.clear();
			subsubPhase++;
			discardingCardsFromBuffer = false;
		}

		return true;
	}

	if (message == "Take Money")
	{
		int moneyTaken = session->TakeMoney(2);
		currentPlayer->GiveMoney(moneyTaken);
		session->SendAllPlayersMessage(currentPlayer->GetPlayerName() + " took "+ std::to_string(moneyTaken) +" gold coins from the bank.");
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
	
	if (selectingCharacter)
	{
		if (subPhase == Moordenaar)
		{
			charToKill = characterStringToEnum[message];
			session->SendAllPlayersMessage("The moordenaar plans to kill the " + message + " he will not get a turn.");
		}
		if (subPhase == Dief)
		{
			charToRob = characterStringToEnum[message];
			session->SendAllPlayersMessage("The dief sneaked up on the " + message + " and stole all their money.");
		}
		selectingCharacter = false;
		return true;
	}

	if (message == "Moordenaar")
	{
		characterActionUsed = true;
		selectingCharacter = true;
		return true;
	}
	if(message == "Dief")
	{
		characterActionUsed = true;
		selectingCharacter = true;
		return true;
	}
	if (message == "Magier")
	{
		characterActionUsed = true;
		magierTrading = true;
		currentPlayer->SendMessage("Select who to trade with.");
		
		return true;
	}

	if (magierTrading)
	{
		if (message == "Deck")
		{
			currentPlayer->GetHand().size();
			cardBuffer = session->DrawCards(currentPlayer->GetHand().size());
			session->AddCardsToDeck(currentPlayer->GetHand());
			session->ShuffleDeck();
			currentPlayer->ClearHand();	
			currentPlayer->GiveCards(cardBuffer);
			cardBuffer.clear();
			magierTrading = false;
			session->SendAllPlayersMessage("The magier used some magic to tradeall of his cards with the deck.");
			return true;
		}
		if (session->GetPlayer(message))
		{
			auto tradingPartner = session->GetPlayer(message);
			cardBuffer = tradingPartner->GetHand();
			tradingPartner->ClearHand();
			tradingPartner->GiveCards(currentPlayer->GetHand());
			currentPlayer->ClearHand();
			currentPlayer->GiveCards(cardBuffer);
			cardBuffer.clear();
			magierTrading = false;
			session->SendAllPlayersMessage("The magier used some dark magic to trade all of his cards with those of " + message + ".");
			return true;
		}
		
		return false;
	}

	if (message == "Koning")
	{
		characterActionUsed = true;
		session->MakeKing(currentPlayer->GetToken());

		int moneyTaken = currentPlayer->HasCardInVillage("School voor magiërs") ? 1 : 0;
		moneyTaken += currentPlayer->GetAmountOfColourInVillage(Yellow);

		currentPlayer->GiveMoney(session->TakeMoney(moneyTaken));
		currentPlayer->SendMessage("You got " + std::to_string(moneyTaken) + " coins from the Koning's effect.");
		session->SendAllPlayersMessage("All hail the new Koning " + currentPlayer->GetPlayerName()+ ".");
		return true;
	}
	if (message == "Prediker")
	{
		characterActionUsed = true;

		int moneyTaken = currentPlayer->HasCardInVillage("School voor magiërs") ? 1 : 0;
		moneyTaken += currentPlayer->GetAmountOfColourInVillage(Blue);
		currentPlayer->GiveMoney(session->TakeMoney(moneyTaken));
		currentPlayer->SendMessage("You got " + std::to_string(moneyTaken) + " coins from the Prediker's effect.");
		session->SendAllPlayersMessage("The prediker calls upon the higher powers to protect his village from the condottiere.");
		buildingsSafeFromCondottiere = true;
		return true;
	}
	if (message == "Koopman")
	{
		characterActionUsed = true;
		int moneyTaken = currentPlayer->HasCardInVillage("School voor magiërs") ? 1 : 0;
		moneyTaken += currentPlayer->GetAmountOfColourInVillage(Green) + 1;
		currentPlayer->GiveMoney(session->TakeMoney(moneyTaken));
		currentPlayer->SendMessage("You got " + std::to_string(moneyTaken) + " coins from the Koopman's effect.");
		return true;
	}
	if (message == "Bouwmeester")
	{
		characterActionUsed = true;
		auto cards = session->DrawCards(2);
		session->GetCurrentPlayer()->GiveCards(cards);
		return true;
	}
	if (message == "Condottiere")
	{
		characterActionUsed = true;
		int moneyTaken = currentPlayer->HasCardInVillage("School voor magiërs") ? 1 : 0;
		moneyTaken += currentPlayer->GetAmountOfColourInVillage(Red);
		currentPlayer->GiveMoney(session->TakeMoney(moneyTaken));
		currentPlayer->SendMessage("You got " + std::to_string(moneyTaken) + " coins from the Condottiere's effect.");

		if (buildingsSafeFromCondottiere)
		{
			session->SendAllPlayersMessage("The prediker's prayers were answered and all the buildings in his village were saved from being destroyed by the condotierre.");
			return true;
		}

		condottiereSelectingPlayer = true;
		return true;
	}

	if (condottiereSelectingPlayer)
	{
		if (session->GetPlayer(message))
		{
			selectedCharacterToken = session->GetPlayer(message)->GetToken();
			condottiereSelectingPlayer = false;
			condottiereSelectingBuilding = true;
			return true;
		}
	}

	if (condottiereSelectingBuilding)
	{
		condottiereSelectingBuilding = false;
		cardBuffer.push_back(session->GetPlayer(selectedCharacterToken)->DestroyBuildingFromVilage(message));
		session->SendAllPlayersMessage("The condottiere destroyed the " + message + " from "+ session->GetPlayer(selectedCharacterToken)->GetPlayerName() +"'s village.");

		for (auto player: session->GetPlayers())
		{
			if (player->HasCardInVillage("Kerkhof"))
			{
				kerkhofActive = true;
				return true;
			}
		}

		cardBuffer.clear();
		return true;
	}

	if (kerkhofActive)
	{
		if (message == "No")
		{
			cardBuffer.clear();
			kerkhofActive = false;
			return true;
		}

		int amountSpent = session->GetPlayer(token)->SpendMoney(1);
		if (amountSpent >= 1 )
		{
			session->GetPlayer(token)->GiveCards(cardBuffer);
			session->SendAllPlayersMessage(session->GetPlayer(token)->GetPlayerName() + " raised the card that the condotitiere just destroyed back from the dead with their Kerkhof and took it in their hands.");
		}
		else
		{
			session->GetPlayer(token)->SendMessage("You have insuficient funds to use the magic powers of the Karkhof.");
		}

		session->GiveMoney(amountSpent);
		cardBuffer.clear();
		kerkhofActive = false;
		return true;
	}

	if (message == "Smederij" && currentPlayer->HasCardInVillage("Smederij"))
	{
		if (currentPlayer->GetAmountOfCoins() >= 3)
		{
			session->GiveMoney(3); 
			currentPlayer->SpendMoney(3);
			currentPlayer->GiveCards(session->DrawCards(2));
			smederijUsed = true;
			session->SendAllPlayersMessage("The smith produced 2 cards for "+currentPlayer->GetPlayerName()+".");
		}
		else
		{
			currentPlayer->SendMessage("You currently don't have the 3 gold needed to spend to use this card.");
		}
		return true;
	}

	if (laboratoriumDiscarding)
	{
		currentPlayer->DiscardCardWithNameFromHand(message);
		currentPlayer->GiveMoney(session->TakeMoney(1));
		laboratoriumDiscarding = false;
		laboratoriumUsed = true;
		session->SendAllPlayersMessage("The alchemists in " + currentPlayer->GetPlayerName() + "'s Labratorium tried to turn one of his cards into gold... and succeeded!");
	}

	if (message == "Laboratorium" && currentPlayer->HasCardInVillage("Laboratorium"))
	{
		if(currentPlayer->GetHand().size() >= 1)
		{
			currentPlayer->SendMessage("Pick a Card to discard");
			laboratoriumDiscarding = true;
		}
		else
		{
			currentPlayer->SendMessage("You need at least 1 card in your hand to use this card.");
			laboratoriumDiscarding = false;
		}
		return true;
	}

	return false;
}

vector<string> ExecutingPhase::GetActions(int token, shared_ptr<GameSession> session)
{	
	vector<string> re_vector;

	if (kerkhofActive)
	{
		auto player = session->GetPlayer(token);
		player->SendMessage("Do you want to use the Kerkhof to spend 1 coin and take the destroyed card into your hand?");		
		re_vector.push_back("Yes");
		re_vector.push_back("No");
		return re_vector;
	}


	auto currentPlayer = session->GetCurrentPlayer();
	

	if (subsubPhase == 1)
	{
		if (!discardingCardsFromBuffer)
		{
			re_vector.push_back("Draw Cards");
			re_vector.push_back("Take Money");
		}
		else
		{
			int maxSize = currentPlayer->HasCardInVillage("Bibliotheek") ? 2:1;
			if (cardBuffer.size() <= maxSize )
			{
				currentPlayer->GiveCards(cardBuffer);
				cardBuffer.clear();
				subsubPhase++;
				discardingCardsFromBuffer = false;
			}
			else
			{
				for (auto card : cardBuffer)
				{
					re_vector.push_back(card.GetName());
				}
			}
		}
	}

	if (subsubPhase == 2)
	{
		if (currentlyBuilding)
		{
			for (auto card : session->GetCurrentPlayer()->GetHand())
			{
				re_vector.push_back(card.GetName());
			}
		}
		else {
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

	if (selectingCharacter)
	{
		if( false )
		{
			re_vector.push_back("Moordenaar");
		}

		if( subPhase == Moordenaar )
		{
			re_vector.push_back("Dief");
		}

		re_vector.push_back("Magier");
		re_vector.push_back("Koning");
		re_vector.push_back("Prediker");
		re_vector.push_back("Koopman");
		re_vector.push_back("Bouwmeester");
		re_vector.push_back("Condotiere");
	}

	if (magierTrading || condottiereSelectingPlayer)
	{
		for (auto player : session->GetPlayers())
		{
			if (!(player->GetToken() == currentPlayer->GetToken()))
			{
				re_vector.push_back(player->GetPlayerName());
			}
		}
		if( magierTrading )
			re_vector.push_back("Deck");
	}

	if (condottiereSelectingBuilding)
	{
		if (session->GetPlayer(selectedCharacterToken)->GetVillage().size() >= 8)
		{
			session->SendAllPlayersMessage("You can't destroy a building from a village with 8 or more buildings");
			condottiereSelectingBuilding = false;
		}

		for (auto building : session->GetPlayer(selectedCharacterToken)->GetVillage())
		{
			re_vector.push_back(building.GetName());
		}
	}

	if (currentPlayer->HasCardInVillage("Smederij") && !smederijUsed)
	{
		re_vector.push_back("Smederij");
	}

	if (currentPlayer->HasCardInVillage("Laboratorium") && !laboratoriumUsed)
	{
		re_vector.push_back("Laboratorium");
	}	

	if (laboratoriumDiscarding)
	{
		for (auto card : session->GetCurrentPlayer()->GetHand())
		{
			re_vector.push_back(card.GetName());
		}
	}

	re_vector.push_back("End Turn");
	return re_vector;
}

void ExecutingPhase::HandTurnToNextCharacter(shared_ptr<GameSession> session)
{
	//check for game over
	{
		auto player = session->GetCurrentPlayer();
		if (player->GetVillage().size() >= 8)
		{
			if (!session->IsGameOver())
			{
				player->SetWasFirstToEight(true);
				session->SetGameOver(true);
			}
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
	currentlyBuilding = false;

	selectingCharacter = false;

	tradingCards = false;

	discardingCardsFromBuffer = false;

	condottiereSelectingPlayer = false;
	condottiereSelectingBuilding = false;
	selectedCharacterToken = 0;

	magierTrading = false;
	smederijUsed = false;
	laboratoriumUsed = false;
	laboratoriumDiscarding = false;
	kerkhofActive = false;

	cardBuffer.clear();

	auto player = session->GetPlayerWithCharacter(static_cast<Character>(subPhase));
	bool done = (player != nullptr && !WasCharKilled(subPhase));

	while (!done)
	{
		subPhase++;
		if (subPhase == 9)
		{
			session->SetCurrentPlayer(session->GetKing());
			session->SetPhase(PickingCharacters, session);
			return;
		}

		if (WasCharKilled(subPhase))
		{
			done = false;
			continue;
		}

		player = session->GetPlayerWithCharacter(static_cast<Character>(subPhase));	
		done = player != nullptr;
	}

	if (WasCharRobbed(subPhase))
	{
		int amountOfMoney = player->GetAmountOfCoins();
		player->SpendMoney(amountOfMoney);
		player->SendMessage("The thief stole " + std::to_string(amountOfMoney) + " from you.");
		auto dief = session->GetPlayerWithCharacter(Dief);
		dief->GiveMoney(amountOfMoney);
		dief->SendMessage("You stole " + std::to_string(amountOfMoney) + ".");
	}	

	session->SetCurrentPlayer(player);
}

bool ExecutingPhase::IsItMyTurn(int token, shared_ptr<GameSession> session)
{
	if (!kerkhofActive)
		return  session->GetPlayerWithCharacter(static_cast<Character>(subPhase)) == session->GetPlayer(token);

	return (session->GetPlayer(token)->HasCardInVillage("Kerkhof") && (session->GetPlayer(token) != session->GetCurrentPlayer()));
}

string ExecutingPhase::ToString()
{
	std::stringstream stream;
	stream << "Excecuting" << '\n';

	stream <<
		to_string(subPhase) << ';' <<
		to_string(subsubPhase) << ';' <<
		to_string(charToKill) << ';' <<
		to_string(charToRob) << ';' <<
		to_string(amountBuiltThisTurn) << ';' <<
		(characterActionUsed ? 1 : 0) << ';' <<
		(currentlyBuilding ? 1 : 0) << ';' <<
		(selectingCharacter ? 1 : 0) << ';' <<
		(tradingCards ? 1 : 0) << ';' <<
		(discardingCardsFromBuffer ? 1 : 0) << ';' <<
		(condottiereSelectingPlayer ? 1 : 0) << ';' <<
		(condottiereSelectingBuilding ? 1 : 0) << ';' <<
		(selectedCharacterToken ? 1 : 0) << ';' <<
		(magierTrading ? 1 : 0) << ';' <<
		(buildingsSafeFromCondottiere ? 1 : 0) << ';' <<
		(smederijUsed ? 1 : 0) << ';' <<
		(laboratoriumUsed ? 1 : 0) << ';' <<
		(kerkhofActive ? 1 : 0) << ';' <<
		(laboratoriumDiscarding ? 1 : 0) << '\n';

	string fileName = LocalHost::Folder + LocalHost::CurrentExportingSessionName + "phasecardbuffer" + LocalHost::Extension;
	CardGenerator::BuildFileFromCards(fileName, cardBuffer);
	stream << fileName;

	return stream.str();
}

std::istream& operator>>(std::istream& is, ExecutingPhase& obj)
{
	string line;

	//subPhase;subsubPhase;charToKill;charToRob;amountBuiltThisTurn;characterActionUsed;currentlyBuilding;selectingCharacter;tradingCards;discardingCardsFromBuffer;condottiereSelectingPlayer;condottiereSelectingBuilding;selectedCharacterToken;magierTrading;buildingsSafeFromCondottiere;smederijUsed;laboratoriumUsed;laboratoriumDiscarding
	{
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

		obj.subPhase = stoi(seglist[0]);
		obj.subsubPhase = stoi(seglist[1]);
		obj.charToKill = stoi(seglist[2]);
		obj.charToRob = stoi(seglist[3]);
		obj.amountBuiltThisTurn = stoi(seglist[4]);
		obj.characterActionUsed = seglist[5] == "1";
		obj.currentlyBuilding = seglist[6] == "1";
		obj.selectingCharacter = seglist[7] == "1";
		obj.tradingCards = seglist[8] == "1";
		obj.discardingCardsFromBuffer = seglist[9] == "1";
		obj.condottiereSelectingPlayer = seglist[10] == "1";
		obj.condottiereSelectingBuilding = seglist[11] == "1";
		obj.selectedCharacterToken = seglist[12] == "1";
		obj.magierTrading = seglist[13] == "1";
		obj.buildingsSafeFromCondottiere = seglist[14] == "1";
		obj.smederijUsed = seglist[15] == "1";
		obj.laboratoriumUsed = seglist[16] == "1";
		obj.kerkhofActive = seglist[17] == "1";
		obj.laboratoriumDiscarding = seglist[18] == "1";
	}

	//cardbuffer
	getline(is, line);
	obj.cardBuffer = CardGenerator::BuildDeckFromFile(line);

	return is;
}
