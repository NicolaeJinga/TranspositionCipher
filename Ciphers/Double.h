#pragma once

#include "BaseCipher.h"
#include "Columnar.h"

class Double : public BaseCipher
{
	Columnar* m_columnar;
	sf::String m_key1;
	sf::String m_key2;

public:
	static bool m_nullKey;
	
	Double();
	~Double();
	sf::String Encrypt(sf::String input);
	sf::String Decrypt(sf::String input);
	void InitKeys();
};