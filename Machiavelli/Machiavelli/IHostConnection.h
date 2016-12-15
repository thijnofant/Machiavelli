#pragma once
class IHostConnection
{
public:
	virtual ~IHostConnection(){}
	virtual int Conect(string playerName) = 0;
	virtual bool IsItMyTurn(int token) = 0;
	virtual vector<string> GetCommands() = 0;
	virtual bool SendMessage(int token, string message) = 0;
	virtual bool Disconect() = 0;
};

