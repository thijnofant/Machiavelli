#pragma once
class IHostConnection
{
public:
	virtual ~IHostConnection(){}
	virtual int Connect(string playerName) = 0;
	virtual bool IsItMyTurn(int token) = 0;
	virtual vector<string> GetCommands(int token) = 0;
	virtual bool SendMessageToHost(int token, string message) = 0;
	virtual bool Disconect(int token) = 0;
	virtual string GetGameStatus(int token) = 0;
	virtual string GetMessages(int token) = 0;
};

