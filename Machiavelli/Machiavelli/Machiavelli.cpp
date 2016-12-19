// Machiavelli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MachiavelliClient.h"
#include "LocalHost.h"


int main()
{
	std::shared_ptr<IHostConnection> connection = std::make_shared<LocalHost>();
	MachiavelliClient client{ connection };
	client.Run();
    return 0;
}

