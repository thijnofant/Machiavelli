#pragma once
#include "GamePhase.h"
class NotPlayingPhase : public GamePhase
{
public:
	NotPlayingPhase();
	~NotPlayingPhase();
	bool HandleAction(int const token, string const message, shared_ptr<GameSession> session) override;
	vector<string> GetActions(int token, shared_ptr<GameSession> session) override;
	bool IsItMyTurn(int const token, shared_ptr<GameSession> session) const override;

	//stream functions
	string ToString() const override;
};

