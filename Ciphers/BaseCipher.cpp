#include "BaseCipher.h"

void BaseCipher::ClearAddedNulls(sf::String& crypt)
{
	int i = crypt.getSize() - 1;
	while (crypt[i] == '*')
	{
		crypt.erase(i, 1);
		--i;
	}
}