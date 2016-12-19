#pragma once
#include "IHostConnection.h"

class MachiavelliClient
{
private:
	shared_ptr<IHostConnection> connection;
	bool running;
	int token;
public:
	MachiavelliClient(std::shared_ptr<IHostConnection> connection);
	~MachiavelliClient();
	void Run();
};

