#pragma once
#include "IHostConnection.h"
class LocalHost : public IHostConnection
{

public:
	LocalHost();
	~LocalHost();
	int Conect(string playerName) override;
	bool IsItMyTurn(int token) override;
	vector<string> GetCommands() override;
	bool SendMessage(int token, string message) override;
	bool Disconect() override;
};

