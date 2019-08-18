#include "Route.h"
#include "../Memory.cpp"
#include <assert.h>

Route::Route()
{
	m_keyTBS = "Not available";
}

Route::~Route()
{
	Memory::DestroyMatrix(m_matrix, m_matrixSize.y);
}

void Route::ReInit()
{
	Memory::DestroyMatrix(m_matrix, m_matrixSize.y);
	m_keyTBS = "Not available";
	m_inputSize = m_inputString.getSize();
	if (m_inputSize > 0)
	{
		m_matrixSize = GetMatrixSize(m_inputSize);
		m_matrix = new sf::String*[m_matrixSize.y];
		for (int i = 0; i < m_matrixSize.y; ++i)
		{
			m_matrix[i] = new sf::String[m_matrixSize.x];
			m_matrix[i]->clear();
		}
		m_posInLength = 0;
	}
}

sf::Vector2i Route::GetMatrixSize(int inputSize)
{
	for (int i = 0; i <= inputSize; ++i)
	{
		if (i*i >= inputSize)
			return sf::Vector2i(i, i);
		if (i*(i + 1) >= inputSize)
			return sf::Vector2i(i, i + 1);
	}
	return sf::Vector2i(0, 0);
}

sf::String Route::Encrypt(sf::String input)
{
	m_inputString = input;
	m_encryptedString.clear();
	ReInit();
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

		for (int j = m_matrixSize.x - 1; j >= 0; --j)
		{
			for (int i = m_matrixSize.y - 1; i >= 0; --i)
			{
				m_encryptedString.insert(m_encryptedString.getSize(), m_matrix[i][j]);
			}
		}
		ClearAddedNulls(m_encryptedString);
	}

	return m_encryptedString;
}

sf::String Route::Decrypt(sf::String input)
{
	m_inputString = input;
	m_decryptedString.clear();
	ReInit();
	if (m_inputSize > 0)
	{
		for (int j = m_matrixSize.x - 1; j >= 0; --j)
		{
			for (int i = m_matrixSize.y - 1; i >= 0; --i)
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