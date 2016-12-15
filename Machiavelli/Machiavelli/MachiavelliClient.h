#pragma once
#include "IHostConnection.h"
#include <memory>

class MachiavelliClient
{
private:
	bool running;
	std::shared_ptr<IHostConnection> connection;
	int token;
public:
	MachiavelliClient(std::shared_ptr<IHostConnection> connection);
	~MachiavelliClient();
	void Run();
};

