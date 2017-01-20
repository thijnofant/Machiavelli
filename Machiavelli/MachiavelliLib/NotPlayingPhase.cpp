#include "stdafx.h"
#include "NotPlayingPhase.h"



NotPlayingPhase::NotPlayingPhase()
{
}


NotPlayingPhase::~NotPlayingPhase()
{
}

bool NotPlayingPhase::HandleAction(int token, string message, shared_ptr<GameSession> session)
{
	return false;
}

vector<string> NotPlayingPhase::GetActions(int token, shared_ptr<GameSession> session)
{
	vector<string> reVec;
	return  reVec;
}

bool NotPlayingPhase::IsItMyTurn(int token, shared_ptr<GameSession> session)
{
	return false;
}
