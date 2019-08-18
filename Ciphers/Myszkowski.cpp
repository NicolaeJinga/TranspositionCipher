#include "Myszkowski.h"
#include "../Memory.cpp"

Myszkowski::Myszkowski()
{
	m_key = "TOOMATOR";
	m_keyTBS = m_key;
}

Myszkowski::~Myszkowski()
{
	Memory::DestroyMatrix(m_matrix, m_matrixSize.y);
}

void Myszkowski::Order()
{
	bool allTrue = false;
	int pos;
	int fin = 0;
	while (!allTrue)
	{
		allTrue = true;
		//get first character
		for (int i = 0; i < m_keySize; ++i)
		{
			if (!m_columnNumbersChecked[i])
			{
				m_char = m_key[i];
				pos = i;
				break;
			}
		}
		//search for min character
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
		//search for characters equal to min
		for (int i = 0; i < m_keySize; ++i)
		{
			if (m_char == m_key[i] && !m_columnNumbersChecked[i])
			{
				m_columnNumbers[i] = fin;
				m_columnNumbersChecked[i] = true;
			}
		}
		fin++;
		//condition to end while
		for (int i = 0; i < m_keySize; ++i)
		{
			if (!m_columnNumbersChecked[i])
			{
				allTrue = false;
			}
		}
	}
	memset(m_columnNumbersChecked, 0, sizeof(bool)*m_keySize);
}

Myszkowski::ColPosAndTimes Myszkowski::GetColPos(int nr)
{
	m_cpat.ms_times = 0;
	m_cpat.ms_pos.clear();
	for (int i = 0; i < m_matrixSize.x; ++i)
	{
		if (nr == m_columnNumbers[i])
		{
			m_cpat.ms_times++;
		}
	}
	for (int i = 0; i < m_keySize; ++i)
	{
		if (nr == m_columnNumbers[i])
		{
			m_cpat.ms_pos.insert(m_cpat.ms_pos.end(), i);
		}
	}
	return m_cpat;
}

sf::String Myszkowski::Encrypt(sf::String input)
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
					{
						m_matrix[i][j].insert(0, m_inputString[m_posInLength]);
					}
				}
				else
				{
					m_matrix[i][j].insert(0, "*");
				}
				m_posInLength++;
			}
		}
		int keySize = 0;
		ColPosAndTimes temp;
		//while (keySize < m_keySize)
		{
			for (int k = 0; k < m_keySize; ++k)
			{
				temp = GetColPos(k);
				if (temp.ms_times == 1)
				{
					//column traversal
					for (int i = 0; i < m_matrixSize.y; ++i)
					{
						m_encryptedString.insert(m_encryptedString.getSize(), m_matrix[i][*temp.ms_pos.begin()]);
					}
				}
				else if (temp.ms_times > 1)
				{
					//row traversal
					for (int i = 0; i < m_matrixSize.y; ++i)
					{
						for (int j = 0; j < temp.ms_times; ++j)
						{
							m_encryptedString.insert(m_encryptedString.getSize(), m_matrix[i][temp.ms_pos[j]]);
						}
					}
				}
				keySize += temp.ms_times;
				if (keySize >= m_keySize)
					break;
			}
		}
		ClearAddedNulls(m_encryptedString);
	}

	return m_encryptedString;
}

sf::String Myszkowski::Decrypt(sf::String input)
{
	ReInit(input, m_decryptedString);
	if (m_inputSize > 0)
	{
		ColPosAndTimes temp;
		int keySize = 0;
		for (int k = 0; k < m_keySize; ++k)
		{
			temp = GetColPos(k);
			
			if (temp.ms_times == 1)
			{
				for (int i = 0; i < m_matrixSize.y; ++i)
				{
					if (m_posInLength < m_inputSize)
					{
						m_matrix[i][*temp.ms_pos.begin()].insert(0, m_inputString[m_posInLength]);
					}
					else
					{
						m_matrix[i][*temp.ms_pos.begin()].insert(0, "*");
					}
					m_posInLength++;
				}
			}
			else if (temp.ms_times > 1)
			{
				for (int i = 0; i < m_matrixSize.y; ++i)
				{
					for (int j = 0; j < temp.ms_times; ++j)
					{
						if (m_posInLength < m_inputSize)
						{
							m_matrix[i][temp.ms_pos[j]].insert(0, m_inputString[m_posInLength]);
						}
						else
						{
							m_matrix[i][temp.ms_pos[j]].insert(0, "*");
						}
						m_posInLength++;
					}
				}
			}
			
			keySize += temp.ms_times;
			if (keySize >= m_keySize)
				break;
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