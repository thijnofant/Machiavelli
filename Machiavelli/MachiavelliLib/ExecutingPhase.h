#pragma once
#include "GamePhase.h"

class ExecutingPhase: public GamePhase
{
private:
	int charToKill;
	int charToRob;
	int amountBuiltThisTurn;

	bool characterActionUsed;
	bool currentlyBuilding ;

public:
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
};

