#include "RailFence.h"
#include "../Memory.cpp"

RailFence::RailFence(int key)
{
	m_key = key;
	m_vectors = new sf::String[m_key];
	m_stream << m_key;
	m_keyTBS = m_stream.str();
}

RailFence::~RailFence()
{
	Memory::DestroyArray(m_vectors);
}

void RailFence::ResetVars(sf::String in, sf::String& crypt)
{
	Memory::DestroyArray(m_vectors);
	m_string.clear();
	if (m_keyTBS.getSize() > 2)
	{
		m_keyTBS = "99";
	}
	for (unsigned int i = 0; i < m_keyTBS.getSize(); ++i)
	{
		if (m_keyTBS[i] < '0' || m_keyTBS[i] > '9')
		{
			m_keyTBS = "99";
			break;
		}
	}
	if (m_keyTBS.getSize() == 0)
	{
		m_keyTBS = '1';
	}
	m_string = m_keyTBS;
	m_key = std::stoi(m_string);
	m_stream.str("");
	m_stream << m_key;
	m_keyTBS = m_stream.str();

	m_vectors = new sf::String[m_key];
	for (int i = 0; i < m_key; ++i)
	{
		m_vectors[i].clear();
	}
	m_currentVector = 0;
	m_direction = true;
	m_char = "";
	m_inputString = in;
	crypt.clear();
}

sf::String RailFence::Encrypt(sf::String input)
{
	ResetVars(input, m_encryptedString);
	if (m_key > 1)
	{
		while (!m_inputString.isEmpty())
		{
			m_char = m_inputString.substring(0, 1);
			m_inputString.erase(0, 1);

			m_vectors[m_currentVector].insert(m_vectors[m_currentVector].getSize(), m_char);
			if (m_direction)
			{
				m_currentVector++;
			}
			else
			{
				m_currentVector--;
			}
			if (m_currentVector == m_key - 1)
			{
				m_direction = false;
			}
			if (m_currentVector == 0)
			{
				m_direction = true;
			}
		}
		for (int i = 0; i < m_key; ++i)
		{
			m_encryptedString.insert(m_encryptedString.getSize(), m_vectors[i]);
		}
	}
	else
	{
		m_encryptedString = input;
	}
	return m_encryptedString;
}

sf::String RailFence::Decrypt(sf::String input)
{
	ResetVars(input, m_decryptedString);
	
	int pos = 0;
	int vec = 0;
	int inputSize = m_inputString.getSize();
	if (m_key > 1)
	{
		for (int i = 0; i < m_key; ++i)
		{
			for (int j = 0; j < inputSize; ++j)
			{
				m_vectors[i].insert(j, " ");
			}
		}
		while (!m_inputString.isEmpty())
		{
			while (pos < inputSize)
			{
				if (m_currentVector == vec)
				{
					m_char = m_inputString.substring(0, 1);
					m_inputString.erase(0, 1);
					m_vectors[m_currentVector].insert(pos, m_char);
				}
				if (m_direction)
				{
					m_currentVector++;
					pos++;
				}
				else
				{
					m_currentVector--;
					pos++;
				}
				if (m_currentVector == m_key - 1)
				{
					m_direction = false;
				}
				if (m_currentVector == 0)
				{
					m_direction = true;
				}
			}
			vec++;
			pos = 0;
			m_direction = true;
			m_currentVector = 0;
		}

		pos = 0;
		m_currentVector = 0;
		m_direction = true;
		while (pos < inputSize)
		{
			m_decryptedString.insert(m_decryptedString.getSize(), m_vectors[m_currentVector][pos]);
			if (m_direction)
			{
				m_currentVector++;
				pos++;
			}
			else
			{
				m_currentVector--;
				pos++;
			}
			if (m_currentVector == m_key - 1)
			{
				m_direction = false;
			}
			if (m_currentVector == 0)
			{
				m_direction = true;
			}
		}
	}
	else
	{
		m_decryptedString = input;
	}

	return m_decryptedString;
}