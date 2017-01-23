#pragma once

class CharacterGenerator
{
public:
	CharacterGenerator();
	~CharacterGenerator();
	static map<int,string> GetCharactersFromFile(string fileName)
	{
		map<int, string> reMap;
		std::ifstream is(fileName);

		string line;

		while(getline(is, line))
		{
			string segment;
			std::vector<std::string> seglist;
			std::stringstream temp(line);
			while (getline(temp, segment, ';'))
			{
				reMap[stoi(seglist[0])] = seglist[1];
			}
		}

		is.close();
	}
};

