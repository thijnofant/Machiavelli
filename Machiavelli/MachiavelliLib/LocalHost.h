#pragma once
#include "IHostConnection.h"
#include "GameSession.h"
#include <memory>

class LocalHost : public IHostConnection
{
private: 
	vector<shared_ptr<GameSession>> sessions;
	shared_ptr<GameSession> GetSessionWithPlayer(int token);
	shared_ptr<GameSession> GetSessionWithPlayer(string playerName);
	shared_ptr<GameSession> GetFirstWaitingSession();
	int lastToken;

public:
	LocalHost();
	~LocalHost();
	int Connect(string playerName) override;
	bool IsItMyTurn(int token) override;
	string GetGameStatus(int token) override;
	vector<string> GetCommands(int token) override;
	bool SendMessageToHost(int token, string message) override;
	bool Disconect(int token) override;
	string GetMessages(int token) override;
	void SaveGame(string folder, string fileName);
	void LoadGameSessions(const string folder, const string fileName);

	const static string LocalHost::Folder;
	const static string LocalHost::SaveFile;
	static string LocalHost::CurrentExportingSessionName;
	const static string LocalHost::Extension;
};

