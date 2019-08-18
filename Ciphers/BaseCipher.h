#pragma once

#include "SFML/Graphics.hpp"
#include "../TextBox.h"

class BaseCipher
{
public:
	sf::String m_inputString;
	sf::String m_encryptedString;
	sf::String m_decryptedString;
	sf::String m_outputString;

	sf::String m_char; // local in every function for every cipher

	sf::String m_keyTBS;
	int m_keyInputLimit = 25;
	bool m_keyInputLimitReached = false;

public:
	virtual sf::String Encrypt(sf::String input) = 0;
	virtual sf::String Decrypt(sf::String input) = 0;

	virtual void ClearAddedNulls(sf::String& crypt);
};