#include "Columnar.h"
#include "../Memory.cpp"

Columnar::Columnar()
{
	m_key = "ZEBRAS";
	m_keyTBS = m_key;
	m_guard = false;
}

Columnar::~Columnar()
{
	Memory::DestroyMatrix(m_matrix, m_matrixSize.y);
}

void Columnar::Guard(bool guard)
{
	m_guard = guard;
}

void Columnar::ReInit(sf::String in, sf::String& crypt)
{
	Memory::DestroyMatrix(m_matrix, m_matrixSize.y);

	if (!m_guard)
	{
		if (m_keyTBS.getSize() == 0)
		{
			m_keyTBS = "Min 1 char";
		}
		m_key = m_keyTBS;
	}

	m_inputString = in;
	m_inputSize = m_inputString.getSize();
	crypt.clear();
	m_keySize = m_key.getSize();
	if (m_inputSize > 0)
	{
		m_matrixSize = GetMatrixSize(m_inputSize, m_keySize);
		m_matrix = new sf::String*[m_matrixSize.y];
		for (int i = 0; i < m_matrixSize.y; ++i)
		{
			m_matrix[i] = new sf::String[m_matrixSize.x];
			m_matrix[i]->clear();
		}
		m_posInLength = 0;
	}

	Memory::DestroyArray(m_columnNumbers);
	Memory::DestroyArray(m_columnNumbersChecked);
	m_columnNumbers = new int[m_keySize];
	m_columnNumbersChecked = new bool[m_keySize];
	memset(m_columnNumbersChecked, 0, sizeof(bool)*m_keySize);
	Order();
}

void Columnar::Order()
{
	int fin = 0;
	int pos;
	while (fin < m_keySize)
	{
		for (int i = 0; i < m_keySize; ++i)
		{
			if (!m_columnNumbersChecked[i])
			{
				m_char = m_key[i];
				pos = i;
				break;
			}
		}
		for (int i = 0; i < m_keySize; ++i)
		{
			if (m_key[i] < m_char && !m_columnNumbersChecked[i])
			{
				m_char = m_key[i];
				pos = i;
			}
		}
		m_columnNumbers[pos] = fin;
		m_columnNumbersChecked[pos] = true;
		fin++;
	}
}

sf::Vector2i Columnar::GetMatrixSize(int inputSize, int keySize)
{
	sf::Vector2i size = sf::Vector2i(keySize, (int)(ceil)((float)inputSize / (float)keySize));
	return size;
}

void Columnar::SetKey(sf::String& key)
{
	m_key = key;
}

int Columnar::GetColumnPos(int nr)
{
	for (int i = 0; i < m_matrixSize.x; ++i)
	{
		if (nr == m_columnNumbers[i])
		{
			return i;
		}
	}
	return -1;
}

sf::String Columnar::Encrypt(sf::String input)
{
	ReInit(input, m_encryptedString);
	if (m_inputSize > 0)
	{
		for (int i = 0; i < m_matrixSize.y; ++i)
		{
			for (int j = 0; j < m_matrixSize.x; ++j)
			{
				if (m_posInLength < m_inputSize)
				{
					m_matrix[i][j].insert(0, m_inputString[m_posInLength]);
				}
				else
				{
					m_matrix[i][j].insert(0, "*");
				}
				m_posInLength++;
			}
		}

		for (int j = 0; j < m_matrixSize.x; ++j)
		{
			for (int i = 0; i < m_matrixSize.y; ++i)
			{
				m_encryptedString.insert(m_encryptedString.getSize(), m_matrix[i][GetColumnPos(j)]);
			}
		}

		ClearAddedNulls(m_encryptedString);
	}
	
	return m_encryptedString;
}

sf::String Columnar::Decrypt(sf::String input)
{
	ReInit(input, m_decryptedString);
	if (m_inputSize > 0)
	{
		for (int j = 0; j < m_matrixSize.x; ++j)
		{
			for (int i = 0; i < m_matrixSize.y; ++i)
			{
				if (m_posInLength < m_inputSize)// && !((i > (m_inputSize % m_keySize)) && (j > (m_inputSize/m_keySize))))
				{
					m_matrix[i][GetColumnPos(j)].insert(0, m_inputString[m_posInLength]);
				}
				else
				{
					m_matrix[i][GetColumnPos(j)].insert(0, "*");
				}
				m_posInLength++;
			}
		}

		for (int i = 0; i < m_matrixSize.y; ++i)
		{
			for (int j = 0; j < m_matrixSize.x; ++j)
			{
				m_decryptedString.insert(m_decryptedString.getSize(), m_matrix[i][j]);
			}
		}

		ClearAddedNulls(m_decryptedString);
	}

	return m_decryptedString;
}