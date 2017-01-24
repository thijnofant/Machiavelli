#pragma once
#include "stdafx.h"
#include "GameSession.h"

class GamePhase
{
protected:
	int subPhase;
	int subsubPhase;
public:
	virtual ~GamePhase()
	{
	}

	GamePhase() : subPhase{ 1 }, subsubPhase{ 1 } {}
	virtual bool HandleAction(int const token, string const message, shared_ptr<GameSession> session) = 0;
	virtual vector<string> GetActions(int const token, shared_ptr<GameSession> session) = 0;
	virtual bool IsItMyTurn(int const token, shared_ptr<GameSession> session) const = 0;
	virtual string ToString() const = 0;
};

