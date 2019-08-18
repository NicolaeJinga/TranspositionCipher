#pragma once
#include "BaseCipher.h"

class Route : public BaseCipher
{
	bool m_clockwise;
	sf::Vector2i m_corner;
	sf::Vector2i m_matrixSize = sf::Vector2i(0,0);
	sf::String** m_matrix = nullptr;

	int m_inputSize;
	int m_posInLength = 0;

	~Route();
	sf::Vector2i GetMatrixSize(int inputSize);
	void ReInit();
public:
	Route();
	sf::String Encrypt(sf::String input);
	sf::String Decrypt(sf::String input);
};
