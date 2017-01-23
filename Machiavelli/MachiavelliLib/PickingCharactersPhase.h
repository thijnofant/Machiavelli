#pragma once
#include "GamePhase.h"
class PickingCharactersPhase :
	public GamePhase
{
private:
	vector<Character> availableCharacters;
public:
	PickingCharactersPhase();
	~PickingCharactersPhase();
	bool HandleAction(int token, string message, shared_ptr<GameSession> session) override;
	vector<string> GetActions(int token, shared_ptr<GameSession> session) override;
	bool IsItMyTurn(int token, shared_ptr<GameSession> session) override;



	//stream functions
	friend std::istream& operator>>(std::istream& is, PickingCharactersPhase& obj);
	string ToString() override;
};

