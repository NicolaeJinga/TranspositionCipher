#include "Double.h"
#include "../Memory.cpp"

Double::Double()
{
	m_columnar = new Columnar;
	m_key1 = "DUPLICATE";
	m_key2 = "ZEBRAS";
	m_keyTBS = m_key1 + '/' + m_key2;
	m_nullKey = false;
}

Double::~Double()
{
	Memory::Destroy(m_columnar);
}

void Double::InitKeys()
{
	bool b = true;
	m_key1.clear();
	m_key2.clear();
	for (unsigned int i = 0; i < m_keyTBS.getSize(); ++i)
	{
		if (m_keyTBS[i] == '/' && b)
		{
			b = false;
			for (unsigned int j = 0; j < i; ++j)
			{
				m_key1.insert(m_key1.getSize(), " ");
				m_key1[j] = m_keyTBS[j];
			}
			int k = 0;
			for (unsigned int j = i + 1; j < m_keyTBS.getSize(); ++j)
			{
				m_key2.insert(m_key2.getSize(), " ");
				m_key2[k] = m_keyTBS[j];
				k++;
			}
		}
		m_nullKey = false;
	}
	if (m_key1.getSize() == 0 || m_key2.getSize() == 0)
	{
		m_key1 = "TWO";
		m_key2 = "KEYS";
		m_nullKey = true;
	}
	m_keyTBS = m_key1 + "/" + m_key2;
	//printf("%s\n%s\n", m_key1.toAnsiString().c_str(), m_key2.toAnsiString().c_str());
}

sf::String Double::Encrypt(sf::String input)
{
	InitKeys();
	m_columnar->Guard(true);
	m_columnar->SetKey(m_key1);
	sf::String firstEncrypt = m_columnar->Encrypt(input);
	m_columnar->SetKey(m_key2);
	m_encryptedString = m_columnar->Encrypt(firstEncrypt);
	m_columnar->Guard(false);
	return m_encryptedString;
}

sf::String Double::Decrypt(sf::String input)
{
	InitKeys();
	m_columnar->Guard(true);
	m_columnar->SetKey(m_key2);
	sf::String firstDecrypt = m_columnar->Decrypt(input);
	m_columnar->SetKey(m_key1);
	m_decryptedString = m_columnar->Decrypt(firstDecrypt);
	m_columnar->Guard(false);
	return m_decryptedString;
}

bool Double::m_nullKey = false;