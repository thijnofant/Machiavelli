#pragma once
class Util
{
	static bool randomInitialized;
	static std::default_random_engine generator;
public:
	Util();
	~Util();

	/**
	This function returns a random integer between min and max (inclusive)
	*/
	static int GetRandomNumber(int const min, int const max);

	static std::default_random_engine GetRandomEngine();
};

