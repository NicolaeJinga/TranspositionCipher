#pragma once
#include "BaseCipher.h"
#include "Columnar.h"

class Myszkowski : public Columnar
{
	struct ColPosAndTimes
	{
		int ms_times;
		std::vector<int> ms_pos;
	};
	ColPosAndTimes m_cpat;
	ColPosAndTimes GetColPos(int nr);
	void Order();

public:
	Myszkowski();
	~Myszkowski();
	sf::String Encrypt(sf::String input);
	sf::String Decrypt(sf::String input);
};
