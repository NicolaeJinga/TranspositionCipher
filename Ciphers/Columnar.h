#pragma once
#include "BaseCipher.h"

class Columnar : public BaseCipher
{
protected:
	sf::String m_key;
	int m_keySize;
	sf::String** m_matrix;
	sf::Vector2i m_matrixSize = sf::Vector2i(0, 0);
	int* m_columnNumbers;
	bool* m_columnNumbersChecked;

	int m_inputSize;
	int m_posInLength;

	bool m_guard = false;
	
public:
	int GetHeight();
	void Guard(bool guard);
	virtual void ReInit(sf::String in, sf::String& crypt);
	virtual sf::Vector2i GetMatrixSize(int inputSize, int keySize);
	virtual void Order();
	virtual int GetColumnPos(int nr);

	Columnar();
	~Columnar();
	virtual void SetKey(sf::String& key);
	virtual sf::String Encrypt(sf::String input);
	virtual sf::String Decrypt(sf::String input);
};
