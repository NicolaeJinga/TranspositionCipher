#pragma once
#include "BaseCipher.h"

class Galois : public BaseCipher
{
	int m_vi[27][3] = { { 0, 0, 0 },{ 0, 1, 0 },{ 1, 0 ,0 },{ 0, 1, 2 },{ 1, 2, 0 },{ 2, 1, 2 },{ 1, 1, 1 },{ 1, 2, 2 },{ 2, 0, 2 },
	{ 0, 1, 1 },{ 1, 1, 0 },{ 1, 1, 2 },{ 1, 0, 2 },{ 0, 0, 2 },{ 0, 2, 0 },{ 2, 0, 0 },{ 0, 2, 1 },{ 2, 1, 0 },
	{ 1, 2, 1 },{ 2, 2, 2 },{ 2, 1, 1 },{ 1, 0, 1 },{ 0, 2, 2 },{ 2, 2, 0 },{ 2, 2, 1 },{ 2, 0, 1 },{ 0, 0, 1 } };
	int  m_fi[27] = { 0, 3, 9, 5, 15, 23, 13, 17, 20, 4, 12, 14, 11, 2, 6, 18, 7, 21, 16, 26, 22, 10, 8, 24 ,25, 19, 1 };
	char m_ci[27] = { ' ', 'C', 'I', 'E', 'O', 'W', 'M', 'Q', 'T', 'D', 'L', 'N', 'K', 'B', 'F', 'R', 'G', 'U',
		'P', 'Z', 'V', 'J', 'H', 'X', 'Y', 'S', 'A' };
	struct cipherElem
	{
		int v[3];
		int f;
		char c;
	};
	std::vector<cipherElem> vElems;
	int a[40];
	int b[40];
	int PolynomialToPower(int* v);
	int* PowerToPolynomial(int p);
	char EncryptLetter(char letter, int pos);
	char DecryptLetter(char letter, int pos);
public:
	Galois();
	void ReInit(sf::String in, sf::String& crypt);
	sf::String Encrypt(sf::String input);
	sf::String Decrypt(sf::String input);
};
