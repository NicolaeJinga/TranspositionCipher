#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Menu.h"

int main()
{
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Menu menu;
	menu.Init();
	sf::Clock clock;
	float dt = 0.0f;
	while (menu.m_window.isOpen())
	{
		while (menu.m_window.pollEvent(menu.m_event))
		{
			if (menu.m_event.type == sf::Event::Closed)
				menu.m_window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				menu.m_window.close();
			if(menu.m_textBoxSystem->GetToggledTextBox() != menu.NOTEXTBOX)
				menu.InputTextBox();
		}
		dt = clock.restart().asSeconds();
		menu.Update(dt);
		menu.m_window.clear();
		menu.Draw();
		menu.m_window.display();
	}
	
	return 0;
}