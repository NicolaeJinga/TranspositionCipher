#include "Galois.h"
#include<vector>

//NOTE: everything should be % 27 and not mod % 26, because having %26, we skip the 26th slot which in this case is 'A'
// and therefore we cannot ever decrypt 'A'

Galois::Galois()
{
	m_keyTBS = "Not available";
	Galois::cipherElem elem;
	
	for (int i = 0; i < 27; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			elem.v[j] = m_vi[i][j];
		}
		elem.f = m_fi[i];
		elem.c = m_ci[i];
		vElems.insert(vElems.end(), elem);
	}
	for (int i = 0; i < 27; ++i)
	{
		//printf("t^%d = (%d, %d, %d); f = %d; c = %c\n", i, vElems[i].v[0], vElems[i].v[1], vElems[i].v[2], vElems[i].f, vElems[i].c);
	}
	a[0] = 16;
	a[1] = 6;
	b[0] = 13;
	b[1] = 10;
	//compute all a and b
	for (int i = 2; i < 40; ++i)
	{
		int vr[3] = { 0, 0, 0 };
		for (int j = 0; j < 3; ++j)
		{
			vr[j] = (m_vi[a[i - 2]][j] + m_vi[a[i - 1]][j]) % 3;
		}
		a[i] = PolynomialToPower(vr);

		b[i] = (b[i - 1] + b[i - 2]-1) % 26 + 1;
	}
	for (int i = 0; i < 40; ++i)
	{
		//printf("a[%d] = %d  ;  b[%d] = %d\n", i, a[i], i, b[i]);
	}
	/*int q[3] = { 8,7,9 };
	printf("\n");
	printf("%d", PolynomialToPower(q));
	printf("\n");
	int* z = PowerToPolynomial(35);
	printf("%d,%d,%d", z[0], z[1], z[2]);*/
}

void Galois::ReInit(sf::String in, sf::String& crypt)
{
	m_keyTBS = "Not available";
	m_inputString = in;
	for (int i = 0; (size_t)i < m_inputString.getSize(); ++i)
	{
		if (m_inputString[i] >= 97)
		{
			m_inputString[i] -= 32;
		}
	}
	crypt.clear();
}

int Galois::PolynomialToPower(int* v)
{
	for (int i = 0; i < 3; ++i)
	{
		v[i] = v[i] % 3;
	}
	for (int i = 0; i < 27; ++i)
	{
		if (vElems[i].v[0] == v[0] && vElems[i].v[1] == v[1] && vElems[i].v[2] == v[2])
		{
			return i;
		}
	}
	return -1;
}

int* Galois::PowerToPolynomial(int p)
{
	p = p % 27;
	return vElems[p].v;
}

char Galois::EncryptLetter(char letter, int pos)
{
	//variables used for step by step calculations
	int p; //power
	int* pp; //power (to) polynomial
	int* bpoly = PowerToPolynomial(b[pos]); //b in polynomial form
	int appb[3]; //after pp+b
	for (int i = 0; i < 27; ++i)
	{
		if (m_ci[i] == letter)
		{
			//do ai*xi
			p = (i + a[pos] - 1) % 26 + 1;
			//convert ai from power to polynomial
			pp = PowerToPolynomial(p);
			//do ai+bi
			for (int j = 0; j < 3; ++j)
			{
				appb[j] = (pp[j] + bpoly[j]) % 3;
			}
			//lookup in m_vi where pp is and get it's corresponding m_ci
			for (int j = 0; j < 27;++j)
			{
				if (m_vi[j][0] == appb[0] && m_vi[j][1] == appb[1] && m_vi[j][2] == appb[2])
				{
					return m_ci[j];
				}
			}
		}
	}
	return '#';
}

char Galois::DecryptLetter(char letter, int pos)
{
	//variables used for step by step calculations
	int* bpoly = PowerToPolynomial(b[pos]); //b in polynomial form
	int ymbi[3]; //y-bi
	int* ypoly; // y in polynomial form
	int p;//power of ymbi
	int x; //decrypted letter
	for (int i = 0; i < 27; ++i)
	{
		if (m_ci[i] == letter)
		{
			ypoly = PowerToPolynomial(i);
			for (int j = 0; j < 3; ++j)
			{
				ymbi[j] = (((ypoly[j] - bpoly[j]) % 3) + 3) % 3;
			}
			p = PolynomialToPower(ymbi);
			if (p == 0)
			{
				x = 0;
			}
			else if ((((p - a[pos]) % 26) + 26) % 26 == 0)
			{
				x = 26;
			}
			else
			{
				x = (((p - a[pos]) % 26) + 26) % 26;
			}
			return m_ci[x];
		}
	}
	return '#';
}

sf::String Galois::Encrypt(sf::String input)
{
	ReInit(input, m_encryptedString);
	for (int i = 0; (size_t)i < m_inputString.getSize(); ++i)
	{
		m_encryptedString.insert(m_encryptedString.getSize(), EncryptLetter(m_inputString[i], i));
	}
	return m_encryptedString;
}

sf::String Galois::Decrypt(sf::String input)
{
	ReInit(input, m_decryptedString);
	for (int i = 0; (size_t)i < m_inputString.getSize(); ++i)
	{
		m_decryptedString.insert(m_decryptedString.getSize(), DecryptLetter(m_inputString[i], i));
	}
	return m_decryptedString;
}