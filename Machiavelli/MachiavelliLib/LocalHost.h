#pragma once
#include "IHostConnection.h"
#include "GameSession.h"
#include <memory>

class LocalHost : public IHostConnection
{
private: 
	vector<shared_ptr<GameSession>> sessions;
	shared_ptr<GameSession> GetSessionWithPlayer(int const token) const;
	shared_ptr<GameSession> GetSessionWithPlayer(string const playerName) const;
	shared_ptr<GameSession> GetFirstWaitingSession() const;
	int lastToken;

public:
	LocalHost();
	~LocalHost();
	int Connect(string const playerName) override;
	bool IsItMyTurn(int const token) const override;
	string GetGameStatus(int const token) const override;
	vector<string> GetCommands(int const token) override;
	bool SendMessageToHost(int const token, string const message) override;
	bool Disconect(int const token) override;
	string GetMessages(int const token) override;
	void SaveGame(string folder, string fileName);
	void LoadGameSessions(const string folder, const string fileName);

	const static string LocalHost::Folder;
	const static string LocalHost::SaveFile;
	static string LocalHost::CurrentExportingSessionName;
	const static string LocalHost::Extension;
};

