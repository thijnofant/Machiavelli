#pragma once
#include "GamePhase.h"

class ExecutingPhase: public GamePhase
{
private:
	int charToKill = 0;
	int charToRob = 0;
	int amountBuiltThisTurn = 0;

	bool characterActionUsed = false;
	bool currentlyBuilding = false;

	bool selectingCharacter = false;

	bool tradingCards = false;

	bool discardingCardsFromBuffer = false;

	bool condottiereSelectingPlayer = false;
	bool condottiereSelectingBuilding = false;
	int selectedCharacterToken = false;

	bool magierTrading = false;
	bool buildingsSafeFromCondottiere = false;
	bool smederijUsed = false;
	bool laboratoriumUsed = false;
	bool laboratoriumDiscarding = false;

	deque<Card> cardBuffer;

public:
	ExecutingPhase(){ subPhase = 0; }
	ExecutingPhase(shared_ptr<GameSession> session);
	~ExecutingPhase();
	bool HandleAction(int token, string message, shared_ptr<GameSession> session) override;
	vector<string> GetActions(int token, shared_ptr<GameSession> session) override;

	void HandTurnToNextCharacter(shared_ptr<GameSession> session);

	inline bool WasCharKilled(int character) const
	{
		return character == charToKill;
	}
	inline bool WasCharRobbed(int character) const
	{
		return character == charToRob;
	}

	bool IsItMyTurn(int token, shared_ptr<GameSession> session) override;


	//stream functions
	friend std::istream& operator>>(std::istream& is, ExecutingPhase& obj);
	string ToString() override;
};

