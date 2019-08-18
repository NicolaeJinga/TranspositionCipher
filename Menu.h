#pragma once

#include "ButtonSystem.h"
#include "TextBoxSystem.h"
#include "Ciphers/BaseCipher.h"
#include "Ciphers/RailFence.h"
#include "Ciphers/Route.h"
#include "Ciphers/Columnar.h"
#include "Ciphers/Double.h"
#include "Ciphers/Myszkowski.h"
#include "Ciphers/Galois.h"

class Menu
{
public:
	sf::RenderWindow m_window;
	sf::Event m_event;
	int m_currentState;
	
	ButtonSystem* m_buttonSystem;
	sf::String* m_buttonStrings;
	int m_nrButtons;
	ButtonSystem* m_submitButtonSystem;
	sf::String* m_submitButtonStrings;
	int m_submitNrButtons;

	Button* m_clearInputButton;
	float m_clearInputButtonCooldown = 0.2f;
	float m_clearInputButtonElapsedTime = 0.0f;

	Button* m_moveTextToInputButton;
	float m_moveTextToInputButtonCooldown = 0.1f;
	float m_moveTextToInputButtonElapsedTime = 0.0f;

	Button* m_resetToDefaultStringButton;
	float m_resetToDefaultStringButtonCooldown = 0.2f;
	float m_resetToDefaultStringButtonElapsedTime = 0.0f;

	sf::String m_defaultInputString;

	sf::String m_clipBoardString;
	//these 2 bools block the weird characters from displaying
	bool m_copiedString;
	bool m_pastedString;

	TextBoxSystem* m_textBoxSystem;
	sf::String* m_textBoxStrings;
	int m_nrTextBoxes;
	int m_currentTextBox;
	int m_inputLimit;

	enum ETextBox
	{
		NOTEXTBOX = -1,
		INPUT,
		OUTPUT,
		KEY,
		TEXTBOX_COUNT
	};

	enum ESubmit
	{
		NOSUBMIT = -1,
		ENCRYPT = 0,
		DECRYPT
	};

	enum ECipher
	{
		RAILFENCE = 0,
		ROUTE,
		COLUMNAR,
		DOUBLE,
		MYSZKOWSKI,
		GALOIS,
		CIPHERS_COUNT
	};

	BaseCipher* m_ciphers[CIPHERS_COUNT];
	BaseCipher* m_currentCipher;
	//ints to handle when a cipher is changed
	//(another cipher button is pressed)
	int m_currentCipherButtonPressed;
	int m_lastCipherButtonPressed;

	//ints to handle when submitButtons changed
	int m_currentSubmitButtonPressed;
	int m_lastSubmitButtonPressed;

	sf::String m_inputString;
	sf::String m_encryptedString;
	sf::String m_decryptedString;

	sf::Text m_inputText;
	sf::Text m_encryptedText;
	sf::Text m_decryptedText;

	sf::Font m_font;

	bool m_maxInputString;
	bool m_firstTimeCrypt = true;

	bool m_keyPressed[sf::Keyboard::KeyCount];
	bool m_textEntered = false;
	sf::String m_lastInput;
	sf::String m_lastCipherKey;

	sf::Texture m_bgTexture;
	sf::Sprite m_bgSprite;

	enum EStates
	{
		MENU = 0,
		STATES_COUNT
	};
	Menu();
	~Menu();
	void Init();
	void InitGUISystems();
	void InitCiphers();
	void DeInit();
	bool OnKeyPress(sf::Keyboard::Key key);
	void Input();
	void InputTextBox();
	void GetOutput(BaseCipher* cipher);
	void SetCurrentState(EStates state);
	void SoloButtonsUpdate(float dt);
	void Update(float dt);
	void Draw();
};