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
	bool HandleAction(int const token, string const message, shared_ptr<GameSession> session) override;
	vector<string> GetActions(int token, shared_ptr<GameSession> session) override;
	bool IsItMyTurn(int const token, shared_ptr<GameSession> session) const override;



	//stream functions
	friend std::istream& operator>>(std::istream& is, PickingCharactersPhase& obj);
	string ToString() const override;
};

