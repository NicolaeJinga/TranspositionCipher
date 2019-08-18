#pragma once
#include <sstream>
#include "BaseCipher.h"

class RailFence : public BaseCipher
{
	int m_key;
	sf::String* m_vectors;
	bool m_direction = true;
	int m_currentVector = 0;

	std::stringstream m_stream;
	std::string m_string;

	void ResetVars(sf::String in, sf::String& crypt);
public:
	RailFence(int key);
	~RailFence();
	sf::String Encrypt(sf::String input);
	sf::String Decrypt(sf::String input);
};