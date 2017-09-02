/* Game Engine by Carl Mesias using 'SDL2 Graphics Library'
 *
 * 4-19-2017
 * Finish Player.h class by also updating input events
 * Add inheritance of LTexture.h into Player.h
 *
 *
 *
 *
 */

#include "Game.h"
#include "Engine/SplashScreen.h"
#include "Engine/MainMenu.h"
#include "Engine/CustomizeCharacter.h"
#include "Engine/ActSelection.h"
#include "Engine/PlayGame.h"
#include "Engine/TestRoom.h"
#include "Engine/ThankYou.h"
#include "Engine/Credits.h"
#include "Engine/HowTo.h"

#include <stdlib.h>
#include <stdio.h>
#include "time.h"

// TODO [ ] - get input from player so they can input their names
// TODO [ ] - make pause menu

// Initialize
void Game::Init() {
	if (_gameState != Uninitialized)
		return;

	// Default level selection
	levelToLoad = 1;

	// Load Video settings from file
	loadVideoCFG();

	// Initialize Video
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);

	//printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
   // printf("The names of the joysticks are:\n");

	// Create window
	gWindow.create("CEngine V2", RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN );//| SDL_WINDOW_RESIZABLE );

	// Create renderer for window
	gRenderer = gWindow.createRenderer();
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Set render size
	SDL_RenderSetLogicalSize(gRenderer,screenWidth,screenHeight);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);
	TTF_Init();

	//SDL_Init(SDL_INIT_EVERYTHING);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
		Mix_GetError());
	}

	// Set Game-State to ShowSplash()
	_gameState = Game::ShowingSplash;

	//SDL_MaximizeWindow(gWindow.getWindow());
	//SDL_SetWindowFullscreen(gWindow.getWindow(), SDL_TRUE);


}

// Load
void Game::Load() {

	// Load Audio settings from file
	loadAudioCFG();

	// load audio
	LoadAudioFiles();

	// Apply audio configurations
	applyMasterAudioCFG();

	// load fonts
	gFont 	= TTF_OpenFont("fonts/Viga-Regular.ttf", 18);
	gFont13 = TTF_OpenFont("fonts/Viga-Regular.ttf", 13);
	gFont26 = TTF_OpenFont("fonts/Viga-Regular.ttf", 26);
}

// Free
void Game::Free() {
	// free audio
	FreeAudioFiles();

	// free textures
	gText.free();

	// free fonts
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;
}

// Start Game
void Game::Start() {

	srand(time(0));

	Init();
	Load();

	// While loop
	while (!IsExiting()) {
		GameLoop();
	}

	Free();

	// Close upon exit
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Exit Game
bool Game::IsExiting() {
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}

// Loop Game
void Game::GameLoop()
{
	switch (_gameState)
	{
		case Game::ShowingSplash:							// Splash Screen
			{
				ShowSplashScreen();
				break;
			}
		case Game::ShowingMenu:								// Menu
			{
				ShowMenu();
				break;
			}
		case Game::ShowingCustomizeCharacter:				// Customize Character
			{
				ShowCustomizeCharacter(gWindow, gRenderer);
				break;
			}
		case Game::ShowingActSelection:						// Play Game
			{
				ShowActSelectionScreen(gWindow, gRenderer);
				break;
			}
		case Game::ShowingPlayGame:							// Play Game
			{
				ShowPlayGame(levelToLoad);
				break;
			}
		case Game::ShowingTestRoom:							// Test Room
			{
				ShowTestRoom(gWindow, gRenderer, levelToLoad);
				break;
			}
		case Game::ShowingThankYouPage:							// Test Room
			{
				ShowThankYouPage(gWindow, gRenderer);
				break;
			}
		case Game::ShowingCredits:							// Test Room
			{
				ShowCredits(gWindow, gRenderer);
				break;
			}
		case Game::Exiting:									// Quit
			{
				IsExiting();
				break;
			}
		case Game::OptionScene:								// Test
			{
				//
				break;
			}
		case Game::CreditScene:								// Test
			{
				ShowCredits(gWindow, gRenderer);
				break;
			}
		case Game::ShowingHowToPlay:								// Test
			{
				ShowHowToPlay(gWindow, gRenderer);
				break;
			}
		case Game::Uninitialized:							// Test
			{
				//
				break;
			}
	}
}


// Splash Screen
void Game::ShowSplashScreen() {
	// Create Splash Screen
	SplashScreen splashScreen;
	// Show Main Menu
	SplashScreen::SplashResult result;
	splashScreen.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case SplashScreen::Play:					// Play (Next scene is Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case SplashScreen::Nothing:
		//
		break;
	case SplashScreen::Exit:					// Quit
		_gameState = Game::Exiting;
		break;
	}
}

// Splash Screen
void Game::ShowMenu() {
	// Fade in Menu Music
	Mix_FadeInMusic( sAmbientMusic, -1, 50);
	// Create Main Menu
	MainMenu mainMenu;
	// Show Main Menu
	MainMenu::MenuResult result;
	mainMenu.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case MainMenu::NewGame:					// Menu returned 'Play', NewGame()
		//_gameState = Game::ShowingPlayGame;
		//_gameState = Game::ShowingActSelection;
		//_gameState = Game::ShowingCustomizeCharacter;
		//_gameState = Game::ShowingActSelection;
		_gameState = Game::ShowingTestRoom;
		//_gameState = Game::ShowingThankYouPage;
		//_gameState = Game::ShowingPlayGame;
		//_gameState = Game::ShowingPlayGame;
		break;
	case MainMenu::HowToPlay:				// Menu returned 'Load', LoadGame()
		_gameState = Game::ShowingHowToPlay;
		break;
	case MainMenu::Options:					// Menu returned 'Options', Options()
	//	_gameState = Game::OptionScene;
		break;
	case MainMenu::Credits:					// Menu returned 'Credits', Credits()
		_gameState = Game::CreditScene;
		break;
	case MainMenu::Nothing:
		//
		break;
	case MainMenu::Exit:					// Menu returned 'Exit', Quit()
		_gameState = Game::Exiting;
		break;
	}
}

/* Customize Character */
void Game::ShowCustomizeCharacter(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Create Main Menu
	CustomizeCharacter customizeCharacter;
	// Show Main Menu
	CustomizeCharacter::Result result;
	customizeCharacter.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case CustomizeCharacter::StartGame:			// Start Game
		_gameState = Game::ShowingActSelection;
		break;
	case CustomizeCharacter::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case CustomizeCharacter::Nothing:			// Nothing
		//
		break;
	case CustomizeCharacter::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* New Game: Choose Act # to Play */
void Game::ShowActSelectionScreen(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Create Main Menu
	ActSelection actSelection;
	// Show Main Menu
	ActSelection::Result result;
	actSelection.Show(gWindow, gRenderer, result, levelToLoad);
	// Do something on Main Menu return
	switch(result)
	{
	case ActSelection::Back:				// Back (to Main Menu)
		//_gameState = Game::ShowingCustomizeCharacter;
		_gameState = Game::ShowingMenu;
		break;
	case ActSelection::StartGame:			// Start Game
		_gameState = Game::ShowingPlayGame;
		break;
	case ActSelection::Nothing:				// Nothing
		//
		break;
	case ActSelection::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* Play Game */
void Game::ShowPlayGame(int levelToLoad) {
	// Stop menu music
	Mix_HaltMusic();
	// Fade in Menu Music
	Mix_FadeInMusic( sStrangeMusic, -1, 50);
	// Create Main Menu
	PlayGame game1;
	// Show Main Menu
	PlayGame::Result result;
	game1.Show(gWindow, gRenderer, result, levelToLoad);
	// Do something on Main Menu return
	switch(result)
	{
	case PlayGame::ShowingThankYouPage:				// Back (to Main Menu)
		_gameState = Game::ShowingThankYouPage;
		break;
	case PlayGame::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case PlayGame::Nothing:				// Nothing
		//
		break;
	case PlayGame::StartGame:			// Start Game
		//_gameState = Game::PlayGameScene;
		break;
	case PlayGame::ShowingMenu:			// Nothing
		//
		break;
	case PlayGame::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* Test Room */
void Game::ShowTestRoom(LWindow &gWindow, SDL_Renderer *gRenderer, int &levelToLoad) {
	// Play Room Music
	Mix_PlayMusic( sElement, -1);
	// Create Main Menu
	TestRoom customizeCharacter;
	// Show Main Menu
	TestRoom::TestResult result;
	customizeCharacter.Show(gWindow, gRenderer, result, levelToLoad);
	// Do something on Main Menu return
	switch(result)
	{
	case TestRoom::StartGame:			// Start a certain scene/level
		_gameState = Game::ShowingPlayGame;
		break;
	case TestRoom::Nothing:				// Nothing
		//
		break;
	case TestRoom::Exit:				// Exit Game
		Mix_HaltMusic();
		_gameState = Game::Exiting;
		break;
	case TestRoom::Back:				// Back
		Mix_HaltMusic();
		_gameState = Game::ShowingMenu;
		break;
	}
}

void Game::ShowThankYouPage(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Play Room Music
	Mix_PlayMusic( sRelaxingInterlude, -1);
	// Create Main Menu
	ThankYou thankYou;
	// Show Main Menu
	ThankYou::ThankYouResult result;
	thankYou.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case ThankYou::StartGame:			// Start a certain scene/level
		//
		break;
	case ThankYou::Nothing:				// Nothing
		//
		break;
	case ThankYou::Exit:				// Exit Game
		Mix_HaltMusic();
		_gameState = Game::Exiting;
		break;
	case ThankYou::Back:				// Back
		Mix_HaltMusic();
		_gameState = Game::ShowingMenu;
		break;
	}
}

void Game::ShowCredits(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Play Room Music
	Mix_PlayMusic( sRelaxingInterlude, -1);
	// Create Main Menu
	Credits credits;
	// Show Main Menu
	Credits::CreditsResult result;
	credits.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case Credits::StartGame:			// Start a certain scene/level
		//
		break;
	case Credits::Nothing:				// Nothing
		//
		break;
	case Credits::Exit:				// Exit Game
		Mix_HaltMusic();
		_gameState = Game::Exiting;
		break;
	case Credits::Back:				// Back
		Mix_HaltMusic();
		_gameState = Game::ShowingMenu;
		break;
	}
}

void Game::ShowHowToPlay(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Play Room Music
	Mix_PlayMusic( sRelaxingInterlude, -1);
	// Create Main Menu
	HowTo howTo;
	// Show Main Menu
	HowTo::HowToResult result;
	howTo.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case HowTo::StartGame:			// Start a certain scene/level
		//
		break;
	case HowTo::Nothing:				// Nothing
		//
		break;
	case HowTo::Exit:				// Exit Game
		Mix_HaltMusic();
		_gameState = Game::Exiting;
		break;
	case HowTo::Back:				// Back
		Mix_HaltMusic();
		_gameState = Game::ShowingMenu;
		break;
	}
}




Game::GameState Game::_gameState = Uninitialized;



