#include "Menu.h"
#include "Memory.cpp"

using namespace Memory;

Menu::Menu()
{
	m_window.create(sf::VideoMode(1600, 900), "Transposition Cipher");
	m_currentState = EStates::MENU;
	Button::LoadFont();
	m_maxInputString = false;
	memset(m_keyPressed, 0, sf::Keyboard::KeyCount*sizeof(bool));
	if (!m_font.loadFromFile("arial.ttf"))
	{
		printf("Error loading arial.ttf in Menu::Menu()");
	}
	m_inputText.setFont(m_font);
	m_encryptedText.setFont(m_font);
	m_decryptedText.setFont(m_font);

	m_inputText.setPosition(200, 200);
	m_inputText.setCharacterSize(32);
	m_encryptedText.setPosition(200, 400);
	m_encryptedText.setCharacterSize(32);
	m_encryptedText.setFillColor(sf::Color::Red);
	m_decryptedText.setPosition(200, 400);
	m_decryptedText.setCharacterSize(32);
	m_decryptedText.setFillColor(sf::Color::Green);
	
	m_currentCipherButtonPressed = -1;
	m_lastCipherButtonPressed = -1;

	m_defaultInputString = "Am fost descoperiti. Fugiti!";
	
	InitGUISystems();

	m_clipBoardString = "";
	m_copiedString = false;
	m_pastedString = false;

	m_currentTextBox = -1;
	m_inputLimit = 40;
	m_firstTimeCrypt = true;
	m_currentSubmitButtonPressed = -1;
	m_lastSubmitButtonPressed = -1;

	m_bgTexture.loadFromFile("bg169.jpg");
	m_bgSprite.setTexture(m_bgTexture);
	m_bgSprite.setColor(sf::Color(255, 255, 255, 80));
}

Menu::~Menu()
{
	DeInit();
}

void Menu::Init()
{
	InitCiphers();
}

void Menu::InitGUISystems()
{
	//ciphers button system
	m_nrButtons = 6;
	m_buttonSystem = new ButtonSystem;
	m_buttonStrings = new sf::String[m_nrButtons];
	char* buttonStrings[] = { "Rail Fence", "Route", "Columnar", "Double", "Myszkowski", "Galois" };
	for (int i = 0; i < m_nrButtons; ++i)
	{
		m_buttonStrings[i] = buttonStrings[i];
	}
	m_buttonSystem->Init(sf::Vector2f(1270, 130), sf::Vector2f(30, 30), sf::Vector2f(300, 100), m_nrButtons, m_buttonStrings);

	//submit button system (encrypt and decrypt buttons)
	m_submitNrButtons = 2;
	m_submitButtonSystem = new ButtonSystem;
	m_submitButtonStrings = new sf::String[m_submitNrButtons];
	char* submitButtonStrings[] = { "Encrypt", "Decrypt" };
	for (int i = 0; i < m_submitNrButtons; ++i)
	{
		m_submitButtonStrings[i] = submitButtonStrings[i];
	}
	m_submitButtonSystem->Init(sf::Vector2f(400, 700), sf::Vector2f(100, 100), sf::Vector2f(300, 100), m_submitNrButtons, m_submitButtonStrings, false);

	//TextBoxSystem
	m_nrTextBoxes = 3;
	m_textBoxSystem = new TextBoxSystem;
	m_textBoxStrings = new sf::String[m_nrTextBoxes];
	char* textBoxStrings[] = { "Input:", "Output:", "Key:" };
	for (int i = 0; i < m_nrTextBoxes; ++i)
	{
		m_textBoxStrings[i] = textBoxStrings[i];
	}
	m_textBoxSystem->Init(sf::Vector2f(200, 200), sf::Vector2f(0, 150), sf::Vector2f(1000, 50), m_nrTextBoxes, m_textBoxStrings);
	m_textBoxSystem->m_textBoxes[INPUT].SetDisplayString(m_defaultInputString);
	m_lastInput = m_textBoxSystem->m_textBoxes[INPUT].m_string;

	m_textBoxSystem->m_textBoxes[KEY].SetPosition(sf::Vector2f(450, 550));
	m_textBoxSystem->m_textBoxes[KEY].SetSize(sf::Vector2f(600, 50));
	m_textBoxSystem->m_textBoxes[KEY].SetInputLimit(25);

	//clearInput Button
	m_clearInputButton = new Button;
	sf::Vector2f pos = m_textBoxSystem->m_textBoxes[INPUT].GetPosition();
	m_clearInputButton->Init(sf::Vector2f(pos.x-70,pos.y), sf::Vector2f(50, 50), 'X', false);

	//resetToDefaultString Button
	m_resetToDefaultStringButton = new Button;
	m_resetToDefaultStringButton->Init(sf::Vector2f(pos.x - 140, pos.y), sf::Vector2f(50, 50), '<', false, sf::Vector2f(-3.0f, -5.0f));

	//moveTextToInput Button
	m_moveTextToInputButton = new Button;
	pos = m_textBoxSystem->m_textBoxes[OUTPUT].GetPosition();
	m_moveTextToInputButton->Init(sf::Vector2f(pos.x, pos.y - 115), sf::Vector2f(50, 50), '^', false, sf::Vector2f(-1.0f,-2.0f));
}

void Menu::InitCiphers()
{
	m_ciphers[RAILFENCE]	= new RailFence(3);
	m_ciphers[ROUTE]		= new Route;
	m_ciphers[COLUMNAR]		= new Columnar;
	m_ciphers[DOUBLE]		= new Double;
	m_ciphers[MYSZKOWSKI]	= new Myszkowski;
	m_ciphers[GALOIS]		= new Galois;
}

void Menu::DeInit()
{
	Destroy(m_buttonSystem);
	DestroyArray(m_buttonStrings);
	for (int i = 0; i < CIPHERS_COUNT; ++i)
	{
		Destroy(m_ciphers[i]);
	}
	Destroy(m_submitButtonSystem);
	DestroyArray(m_submitButtonStrings);
	Destroy(m_textBoxSystem);
	DestroyArray(m_textBoxStrings);
}

bool Menu::OnKeyPress(sf::Keyboard::Key key)
{
	if (!sf::Keyboard::isKeyPressed(key) && m_keyPressed[key])
	{
		m_keyPressed[key] = false;
	}
	if (sf::Keyboard::isKeyPressed(key) && !m_keyPressed[key])
	{
		m_keyPressed[key] = true;
		return true;
	}
	return false;
}

void Menu::InputTextBox()
{
	if (m_textBoxSystem->GetString().getSize() >= (size_t)m_textBoxSystem->GetInputLimit())
	{
		m_textBoxSystem->m_textBoxes[m_currentTextBox].m_maxInputChars = true;
	}

	if (m_textBoxSystem->m_textBoxes[KEY].m_maxInputChars)
	{
		m_textBoxSystem->m_textBoxes[KEY].m_maxInputChars = false;
	}

	for (int i = 0; i < CIPHERS_COUNT; ++i)
	{
		if (m_ciphers[i]->m_keyTBS.getSize() >= (size_t)m_ciphers[i]->m_keyInputLimit)
		{
			m_ciphers[i]->m_keyInputLimitReached = true;
		}
	}

	//gets the input from the keyboard
	if (m_currentState == MENU
		&& m_event.type == sf::Event::TextEntered
		&& m_textBoxSystem->GetString().getSize() < (size_t)m_textBoxSystem->GetInputLimit()
		&& !m_textBoxSystem->m_textBoxes[m_currentTextBox].m_maxInputChars
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)
		&& !m_copiedString
		&& !m_pastedString)
	{
		m_textBoxSystem->GetString().insert(m_textBoxSystem->GetString().getSize(), m_event.text.unicode);
		if (m_textBoxSystem->m_textBoxes[KEY].IsPressed() && m_currentCipher!=nullptr && !m_currentCipher->m_keyInputLimitReached)
		{
			m_currentCipher->m_keyTBS.insert(m_currentCipher->m_keyTBS.getSize(), m_event.text.unicode);
		}
	}
	//filters out backspace, ctrl+c, ctrl+v and does their known action
	else if (m_event.type == sf::Event::KeyPressed)
	{
		m_copiedString = false;
		m_pastedString = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && m_textBoxSystem->GetString().getSize() > 0)
		{
			m_textBoxSystem->GetString().erase(m_textBoxSystem->GetString().getSize() - 1);
			m_textBoxSystem->m_textBoxes[m_currentTextBox].m_maxInputChars = false;
			if (m_textBoxSystem->m_textBoxes[KEY].IsPressed() && m_currentCipher!=nullptr)
			{
				m_currentCipher->m_keyTBS.erase(m_currentCipher->m_keyTBS.getSize() - 1);
				m_currentCipher->m_keyInputLimitReached = false;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				m_copiedString = true;
				m_clipBoardString = m_textBoxSystem->GetString();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
			{
				m_pastedString = true;
				if (m_clipBoardString != "")
				{
					m_textBoxSystem->GetString() = m_clipBoardString;
				}
			}
		}
	}
}

void Menu::SetCurrentState(EStates state)
{
	m_currentState = state;
}

void Menu::SoloButtonsUpdate(float dt)
{
	//clear input button
	m_clearInputButton->Update(dt, m_window);
	if (m_clearInputButton->IsPressed())
	{
		if (m_clearInputButtonElapsedTime == 0.0f && m_textBoxSystem->m_textBoxes[INPUT].m_string.getSize() > 0)
		{
			m_textBoxSystem->m_textBoxes[INPUT].m_string = "";
			m_textBoxSystem->m_textBoxes[INPUT].m_maxInputChars = false;
		}
		m_clearInputButtonElapsedTime += dt;
		if (m_clearInputButtonElapsedTime >= m_clearInputButtonCooldown)
		{
			m_clearInputButton->ResetPress();
			m_clearInputButtonElapsedTime = 0.0f;
		}
	}

	//moveTextToInput Button
	m_moveTextToInputButton->Update(dt, m_window);
	if (m_moveTextToInputButton->IsPressed())
	{
		if (m_moveTextToInputButtonElapsedTime == 0.0f && m_textBoxSystem->m_textBoxes[OUTPUT].m_string.getSize() > 0)
		{
			m_textBoxSystem->m_textBoxes[INPUT].m_string = m_textBoxSystem->m_textBoxes[OUTPUT].m_string;
		}
		m_moveTextToInputButtonElapsedTime += dt;
		if (m_moveTextToInputButtonElapsedTime >= m_moveTextToInputButtonCooldown)
		{
			m_moveTextToInputButton->ResetPress();
			m_moveTextToInputButtonElapsedTime = 0.0f;
		}
	}

	//reset to default string
	m_resetToDefaultStringButton->Update(dt, m_window);
	if (m_resetToDefaultStringButton->IsPressed())
	{
		if (m_resetToDefaultStringButtonElapsedTime == 0.0f)
		{
			m_textBoxSystem->m_textBoxes[INPUT].m_string = m_defaultInputString;
		}
		m_resetToDefaultStringButtonElapsedTime += dt;
		if (m_resetToDefaultStringButtonElapsedTime >= m_resetToDefaultStringButtonCooldown)
		{
			m_resetToDefaultStringButton->ResetPress();
			m_resetToDefaultStringButtonElapsedTime = 0.0f;
		}
	}
}

void Menu::Update(float dt)
{
	m_textBoxSystem->Update(dt, m_window);
	//displays the strings in all the textboxes
	for (int i = 0; i < TEXTBOX_COUNT; ++i)
	{
		m_textBoxSystem->m_textBoxes[i].SetDisplayString(m_textBoxSystem->m_textBoxes[i].m_string);
	}
	//displays string in the current text box.
	/*if (m_currentTextBox != NOTEXTBOX)
	{
		m_textBoxSystem->m_textBoxes[m_currentTextBox].SetDisplayString(m_textBoxSystem->m_textBoxes[m_currentTextBox].m_string);
	}*/

	if (m_currentCipher != nullptr)
	{
		if (Double::m_nullKey == true)
		{
			m_submitButtonSystem->m_buttons[m_submitButtonSystem->GetToggledButton()].ResetPress();
		}
		if (m_currentCipher->m_keyTBS.getSize() == 0)
		{
			m_submitButtonSystem->m_buttons[m_submitButtonSystem->GetToggledButton()].ResetPress();
		}
		m_textBoxSystem->m_textBoxes[KEY].SetDisplayString(m_currentCipher->m_keyTBS);
	}

	m_buttonSystem->Update(dt, m_window);
	m_submitButtonSystem->Update(dt, m_window);

	m_currentCipherButtonPressed = m_buttonSystem->GetToggledButton();
	m_currentSubmitButtonPressed = m_submitButtonSystem->GetToggledButton();
	m_currentTextBox = m_textBoxSystem->GetToggledTextBox();

	SoloButtonsUpdate(dt);

	//encrypt decrypt buttons: last pressed
	if (m_currentSubmitButtonPressed != m_lastSubmitButtonPressed)
	{
		m_firstTimeCrypt = true;
		m_lastSubmitButtonPressed = m_currentSubmitButtonPressed;
	}

	//sets m_currentCipher to the respective selected button cipher
	if (m_currentCipherButtonPressed != m_lastCipherButtonPressed)
	{
		m_firstTimeCrypt = true;
		if (m_currentCipherButtonPressed != -1)
		{
			m_currentCipher = m_ciphers[m_buttonSystem->GetToggledButton()];
		}
		else
		{
			m_currentCipher = nullptr;
		}
		m_lastCipherButtonPressed = m_currentCipherButtonPressed;
	}

	//encryption and decryption of currentCipher
	//if (m_event.type == sf::Event::TextEntered || m_firstTimeCrypt)
	if (m_textBoxSystem->m_textBoxes[INPUT].m_string != m_lastInput || m_firstTimeCrypt || m_currentCipher->m_keyTBS != m_lastCipherKey)
	{
		if (m_currentCipher != nullptr)
		{
			if (m_submitButtonSystem->GetToggledButton() == ENCRYPT)
			{
				m_encryptedString = m_currentCipher->Encrypt(m_textBoxSystem->m_textBoxes[INPUT].m_string);
				m_textBoxSystem->m_textBoxes[OUTPUT].SetDisplayString(m_encryptedString);
				m_firstTimeCrypt = false;
			}
			if (m_submitButtonSystem->GetToggledButton() == DECRYPT)
			{
				m_decryptedString = m_currentCipher->Decrypt(m_textBoxSystem->m_textBoxes[INPUT].m_string);
				m_textBoxSystem->m_textBoxes[OUTPUT].SetDisplayString(m_decryptedString);
				m_firstTimeCrypt = false;
			}
		}
	}
	m_lastInput = m_textBoxSystem->m_textBoxes[INPUT].m_string;
	if (m_currentCipher != nullptr)
	{
		m_lastCipherKey = m_currentCipher->m_keyTBS;
	}
}

void Menu::Draw()
{
	m_window.draw(m_bgSprite);
	//systems
	m_buttonSystem->Draw(m_window);
	m_textBoxSystem->Draw(m_window);
	m_submitButtonSystem->Draw(m_window);
	//action buttons
	m_clearInputButton->Draw(m_window);
	m_moveTextToInputButton->Draw(m_window);
	m_resetToDefaultStringButton->Draw(m_window);
}