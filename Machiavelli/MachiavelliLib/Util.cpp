#include "stdafx.h"
#include "Util.h"

bool Util::randomInitialized = false;
std::default_random_engine Util::generator;

Util::Util()
{
}


Util::~Util()
{
}

int Util::GetRandomNumber(int min, int max)
{
	if (min > max)
	{
		return 0;
	}

	if (!randomInitialized)
	{
		generator = std::default_random_engine();
		generator.seed(time(nullptr));
		randomInitialized = true;
	}
	std::uniform_int_distribution<int> Distribution(min, max);
	return Distribution(generator);
}

std::default_random_engine Util::GetRandomEngine()
{
	if (!randomInitialized)
	{
		generator = std::default_random_engine();
		generator.seed(time(nullptr));
		randomInitialized = true;
	}
	return generator;
}
