/*
 * PlayGame.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */


#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LTexture.h"
#include "PlayGame.h"
#include "Particle.h"

// TODO [VERY USEFUL] - if you initialize a font at a decent size but render it half
//						of its original width and height, it will look much better
//						than initializing it at a smaller value


// TODO [ ] - fix players class so that we can easily create more players
// TODO [ ] - research how servers and clients talk to each other
// TODO [ ] - make it multiplayer

// TODO [ ] - make it so that when a grenade explodes, an Enemy will get hurt by it if its in the blast radius

/* Particless.h */
// TODO [ ] - create a new particle type, one that is part of a grenade particle and separate from a star particle

/* Level Loadinig? */
// TODO [ ] - create level save interface so we can save with "level1.map" or something
// TODO [ ] - make a door entry to go into another level (or Room)

/* Monster.h */
// TODO [ ] - create a monster class so we can spawn monsters

/* Player.h */
// TODO [ ] - Create a sort of "Gun Barrel", to cast spells so all we need to do is input the details of the bullet (spell)

// BIG TODO - make a StatusEffects class (buffs and debuffs)

// TODO - if  Levels are not loading properly its because they dont have the destructible trait saved to them (level 6 and level8 work)


void PlayGame::Create(SDL_Renderer *gRenderer) {

}

void PlayGame::Initialize(Particle &part, Particle particles[]) {
	// Game mode VS Editor mode Variables
    camlock 			= true;
    debug 				= false;
	editor	 			= false;
	// Upon entry
	place_type 			= 0;
	clampSize 			= 16;
	quit 				= false;
	leftClick 			= false;
	shift 				= false;
	camx 				= 0;
	camy 				= 0;
	frame 				= 0;
	dimSize 			= 5;
	dimDir 				= 1;
    cap 				= true;
    playerStateLevel 	= 1;
	int i = 0;
	// tile texture clips
	int startY = 0;
	for (int h = 0; h < tb.tilesHeight; h++) {
		for (int w = 0; w < tb.tilesWidth; w++) {
			rTiles[i].x = 0 + w * 16;
			rTiles[i].y = startY + h * 16;
			rTiles[i].w = 16;
			rTiles[i].h = 16;
			i++;
		}
	}
	// door texture clips
	startY = 128;
	for (int h = 0; h < 4; h++) {
		for (int w = 0; w < 4; w++) {
			rTiles[i].x = 0 + w * 32;
			rTiles[i].y = startY + h * 32;
			rTiles[i].w = 32;
			rTiles[i].h = 32;
			i++;
		}
	}
	// castle texture clips
	startY = 256;
	for (int h = 0; h < 21; h++) {
		for (int w = 0; w < 8; w++) {
			rTiles[i].x = 0 + w * 16;
			rTiles[i].y = startY + h * 16;
			rTiles[i].w = 16;
			rTiles[i].h = 16;
			i++;
		}
	}
	// Opened door, closed door, spawn tile
	rDoor[0] = {0, 0, 16, 16};
	rDoor[1] = {16, 0, 16, 16};
	rDoor[2] = {32, 0, 16, 16};
	// Cursors
	i = 0;
	for (int h = 0; h < 2; h++) {
		for (int w = 0; w < 8; w++) {
			rMouse[i].x = 0 + w * 10;
			rMouse[i].y = 0 + h * 10;
			rMouse[i].w = 10;
			rMouse[i].h = 10;
			i++;
		}
	}
	// Xbox buttons
	i = 0;
	for (int h = 0; h < 4; h++) {
		for (int w = 0; w < 4; w++) {
			rXboxButtons[i].x = 0 + w * 16;
			rXboxButtons[i].y = 0 + h * 16;
			rXboxButtons[i].w = 16;
			rXboxButtons[i].h = 16;
			i++;
		}
	}
	// Spell icon clips
	for (int i = 0; i < 4; i++) {
		rSpellIcons[i].x = 0 + i * 16;
		rSpellIcons[i].y = 0;
		rSpellIcons[i].w = 16;
		rSpellIcons[i].h = 16;
	}
	// GUI clips
	rGUI[0] = {448, 180, 84, 64};
	rGUI[1] = {533, 184, 8, 56};
	rGUI[2] = {542, 184, 8, 56};
	rGUI[3] = {551, 182, 32, 60};
	// Health bar clip
	rHealthBar[0] = {592, 186, 8, 12};
	rHealthBar[1] = {601, 186, 8, 12};
	// Mana bar clip
	rManaBar[0] = {592, 206, 8, 12};
	rManaBar[1] = {601, 206, 8, 12};
	// Green bar clip
	rGreenGar[0] = {592, 226, 8, 12};
	rGreenGar[1] = {601, 226, 8, 12};

	// Initialize
	// Items
	obj.Init(item);
    // Zombies
	mon.Init(monster);
	// Particles
	part.init(particles);
	// Spawners
	spaw.init(spawner);
	// Spawners
	vf.Init(vfx);
	// Player
	//player.Init();
	///player.SetName("player1");
	// Text
	tex.init(text);
	// Tiles
	tl.Init(tile);
	// Collision Tiles
	//tc.init(tilec);
	// Tilebar
	tb.Init(tilebar);
	tb.SpawnMultiple(tilebar);
}


/*
void PlayGame::saveCFG(std::string fileName){
	std::ofstream fileSettings;
	fileSettings.open( fileName.c_str() );
		std::stringstream tempss;
		tempss << MASTER_VOL << " "
			   << MUSIC_VOL  << " "
			   << SFX_VOL 	 << " "
			   << RESOLUTION << " "
			   << FULLSCREEN << " "
			   << VSYNC;
		fileSettings << tempss.str().c_str();
	fileSettings.close();
}

void PlayGame::loadCFG(std::string fileName){
	std::fstream fileSettings( fileName.c_str() );
	if (fileSettings.is_open()) {
		fileSettings >> MASTER_VOL >> MUSIC_VOL >> SFX_VOL >> RESOLUTION >> FULLSCREEN >> VSYNC;
		std::cout << "Loaded config.cfg" << std::endl;
	}else{
		std::cout << "Error opening config.cfg." << std::endl;
		std::cout << "\tCreating default config.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/config.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created config.cfg." << std::endl;
			newConfigFile << "100 128 128 2 2 2";
		}else{
			std::cout << "\t\tError creating config.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default config settings." << std::endl;
			MASTER_VOL			= 100;
			MUSIC_VOL			= 128;
			SFX_VOL				= 128;
			RESOLUTION			= 2;
			FULLSCREEN			= 2;
			VSYNC				= 2;
		}
		newConfigFile.close();
	}
	fileSettings.close();

	// Set Audio Settings
	Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sLazer, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sAtariBoom, SFX_VOL*(MASTER_VOL*0.01));

	// Set Video Settings
	gWindow.applySettings(RESOLUTION, FULLSCREEN, VSYNC);
}*/

// Load
void PlayGame::Load(LWindow &gWindow, SDL_Renderer *gRenderer, Particle &part, Particle particles[]) {

	// Load Video settings from file
	loadVideoCFG();

	// Load Audio settings from file
	loadAudioCFG();

	// Load audio files
	LoadAudioFiles();

	// Apply audio configurations
	applyMasterAudioCFG();

 	// load textures
	gCharacters.loadFromFile(gRenderer, "resource/gfx/ByJoseLuisPeiroLima/player-mages.png");
	gMonsters.loadFromFile(gRenderer, "resource/gfx/By Scott Matott/monsters.png");
	gDoor.loadFromFile(gRenderer, 	"resource/gfx/By Scott Matott/door.png");
	gBG.loadFromFile(gRenderer, 		"resource/gfx/test.png");
	gCircles.loadFromFile(gRenderer, 	"resource/gfx/ranges.png");
	rCircles[0] = {0,0,16,16};
	rCircles[1] = {16,0,16,16};

	// set texture clips
	// Character clips
	for (int i=0; i<10; i++) {
		SDL_Rect tempClip = {i*16, 0, 16, 16};
		rGreenMage.push_back( tempClip );

		tempClip = {i*16, 16, 16, 16};
		rBlueMage.push_back( tempClip );

		tempClip = {i*16, 32, 16, 16};
		rWhiteMage.push_back( tempClip );
	}
	// Monster clips
	for (int i=0; i<9; i++) {
		SDL_Rect tempClip = {i*16, 0, 16, 16};
		rSkeleton.push_back( tempClip );
	}

	// load fonts
	gFont 	= TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 18);
	gFont13 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 13);
	gFont26 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 26);

	// load particle textures
	gLightBG.loadFromFile(gRenderer, "resource/gfx/light_bg.png");
	gLight.loadFromFile(gRenderer, "resource/gfx/light.png");
	gMouse.loadFromFile(gRenderer, "resource/gfx/cursors.png");
	gXboxButtons.loadFromFile(gRenderer, "resource/gfx/buttons.png");
	gSpellIcons.loadFromFile(gRenderer, "resource/gfx/spell-icons.png");
	gRect.loadFromFile(gRenderer, "resource/gfx/rect.png");
	gGUI.loadFromFile(gRenderer, "resource/gfx/By Buch/ui_big_pieces.png");

	//Load texture target
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	// load media for other classes
	player1.setTexture( gCharacters, rWhiteMage );
	part.load(gRenderer);
	//player.Load(gRenderer);
	spaw.load(gRenderer);
	vf.Load(gRenderer);
	tb.Load(gRenderer);
	tl.Load(gRenderer);
	obj.Load(gRenderer);
	mon.Load(gRenderer);

	// Character.cpp initializations
	player1.setParam(gRenderer, "player1", 100, 64, 64, 16, 16);
	player1.setJoyStick(0);
	player1.target.x = &tile[0].x;
	player1.target.y = &tile[0].y;
	player1.target.id = 0;
	player1.spell.push_back( Spell("Star Scorch") );
	player1.spell.push_back( Spell("Blaze") );
	player1.spell.push_back( Spell("Inferno") );
	player1.spell.push_back( Spell("Fireball") );
	//std::vector<Character>::iterator it;
	//for (it = characters.begin(); it != characters.end(); it++) {
	//}

	// Apply video configurations
	//applyVideoCFG(gWindow);
}

void PlayGame::Free(Particle &part) {
	// free textures
	gText.free();
	gBG.free();
	gCircles.free();
	gMouse.free();
	gXboxButtons.free();
	gCharacters.free();
	gMonsters.free();
	gSpellIcons.free();
	gRect.free();

	// free fonts
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;

	// Free audio files
	FreeAudioFiles();

	// free media from other classes
	part.free();
	player1.Free();
	//player.Free();
	spaw.free();
	vf.Free();
	tl.Free();
	obj.Free();
	mon.Free();
}

void PlayGame::Show(LWindow &gWindow, SDL_Renderer *gRenderer, PlayGame::Result &result, int levelToLoad) {

	// Particles
	static Particle part;
	static Particle particles[8000];

	// Creations
	Create(gRenderer);

	// Initialize
	Initialize(part,particles);

	//fri.Spawn(frigate, 0, 2, map.x+tl.levelWidth - 620 - 180, map.y + tl.levelHeight/2 - 280/2, 180.0, 0.0, "Team1");

	// Load resources
	Load(gWindow, gRenderer, part,particles);
	LoadLevel(levelToLoad, part, particles);
	playerStateLevel = levelToLoad;

	//CreateInitializations();

    // Play Music, looped
	//Mix_FadeInMusic(sAmbientMusic, -1, 0);

	// Updates from different classes
	/*Updates.push_back ( );

	// Renders from different classes
	Updates.push_back ( );*/



	// While loop
	while (!quit) {

		// Start FPS cap
		fps.start();

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Close program
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free(part);
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {

					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_q:
						if (!editor) {
							ResetLevel(part, particles);
						}
						break;
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					case SDLK_LCTRL:
						ctrl = true;
						break;
					case SDLK_RCTRL:
						ctrl = true;
						break;
					case SDLK_h:
						debug = (!debug);
						break;
					case SDLK_y:
						camlock = (!camlock);
						break;
					case SDLK_p:
						if (editor) {
							editor = false;
							camlock = true;
						}else{
							editor = true;
							camlock = false;
						}
						break;
					case SDLK_m:
						if (editor) {
							std::stringstream mapSizeSS;
							mapSizeSS << GetInput(gWindow, gRenderer, quit, "Enter Keys required and Level Size (i.e.: 3 128 128): ");
							// Check if the Editor entered any numbers
							if (mapSizeSS.str().size() > 0) {
								mapSizeSS >> tl.requiredKeys >> tl.levelWidth >> tl.levelHeight;
							}
						}
						break;
					case SDLK_s:
						if (editor && !shift) {
							// Editor visual message
							std::stringstream tempss;
							tempss << "Saving level data...";
							tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());

							// Save Data for spawn coordinates
							std::stringstream SpawnCoordinatesData;
							SpawnCoordinatesData << spawnX << " " << spawnY << " " << doorX << " " << doorY;

							// Save Data for Tiles
							std::stringstream TileSaveData;
							TileSaveData << tl.SaveData(tile);

							// Save Data for Collision Tiles
							//std::stringstream CollisionTileSaveData;
							//CollisionTileSaveData << tc.saveTiles(tilec);

							// Save Data for Items
							std::stringstream ItemSaveData;
							ItemSaveData << obj.SaveData(item);

							// Save Data for Items
							std::stringstream MonsterSaveData;
							MonsterSaveData << mon.SaveData(monster);

							// Go to saving interface
							SaveLevel(gWindow, gRenderer, quit,
									  SpawnCoordinatesData.str().c_str(),
									  TileSaveData.str().c_str(),
									  ItemSaveData.str().c_str(),
									  MonsterSaveData.str().c_str());
							// Editor visual message
							tempss.str(std::string());
							tempss << "Finished saving.";
							tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());
						}
						break;
					case SDLK_ESCAPE:
						start(gWindow, gRenderer);
						break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyDown(event.key.keysym.sym, part, particles);
					}
					/* Player Controls */
					else{
						// Player key down
						player1.OnKeyDown(event.key.keysym.sym);
						//player.OnKeyDown(player, event.key.keysym.sym);
					}	// end editor check
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_LCTRL:
						ctrl = false;
						break;
					case SDLK_RCTRL:
						ctrl = false;
						break;
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyUp(event.key.keysym.sym);
					}
					/* Player Controls */
					else{
						// Player Keys Released, categorized
						player1.OnKeyUp(event.key.keysym.sym);
						//player.OnKeyUp(player, event.key.keysym.sym);
					} // end editor
				}
				//////////////////////////////////////

				/////////////////////////////////////

				// Update Player click state
				if (!editor) {
					// Player Mouse Click state
					///player.mouseClickState(player, event);

					// Update Xbox 360 controls
					player1.updateJoystick(event);

					// Player mouse clicks
					player1.MouseClick(event);
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				mouseReleased(event);

				// Customize Character results
				switch (optionsResult)  {
				case Back:				// Exit to Main Menu
					quit = true;
					break;
				case Nothing:
					//
					break;
				case StartGame:
					//
					break;
				case ShowingMenu:
					//
					break;
				case Exit:				// Exit Desktop
					result = Exit;
					quit = true;
					break;
				}

				// Customize Character results
				/*switch (result)  {
					case Back:				// Back (to Main Menu)
						quit = true;
						break;
					case Nothing:			// Nothing

						break;
					case StartGame:			// Start Game
						quit = true;
						break;
				}*/
			}
		}

		// update everything
		Update(gWindow, gRenderer, part, particles);

		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		//------------------------------------ Draw on Scene here ------------------------------------//
		//Set our Texture as our render target
		gTargetTexture.setAsRenderTarget(gRenderer);
		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render foreground
			RenderFG(gRenderer, gWindow);

			// Render items
			Render(gRenderer, gWindow, part, particles);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );
		//------------------------------------ Draw on Scene here ------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render lights
			RenderLights(gRenderer, part, particles);

			/// Render Scene
			// If in editor mode, turn off lights (which is actually just setting our Scene Texture from "ADD to "BLEND" blend mode)
			if (debug) {
				gTargetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
			}else{
				gTargetTexture.setBlendMode(SDL_BLENDMODE_MOD);
			}
			gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight, NULL, 0.0);

			/// Render Debug
			RenderDebug(gRenderer, part, particles);

			/// Render Text
			RenderText(gRenderer, gWindow, part, particles);

			/// Render Editor GUI
			RenderUI(gRenderer);

			/// Render Player GUI
			RenderGUI(gRenderer);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}
	// Free everything
	Free(part);
}

// Update everything
void PlayGame::Update(LWindow &gWindow, SDL_Renderer *gRenderer, Particle &part, Particle particles[]) {
	// Variable limits
	if (tl.layer < 0) {
		tl.layer = 0;
	}
	if (tl.layer > 6) {
		tl.layer = 0;
	}
	if (tl.id < 0) {
		tl.id = tb.uniqueTiles;
	}
	if (tl.id > tb.uniqueTiles) {
		tl.id = 0;
	}
	/*if (tc.type < 0) {
		tc.type = 0;
	}
	if (tc.type > 2) {
		tc.type = 0;
	}
	if (tc.layer > 2) {
		tc.layer = 0;
	}*/
	if (obj.id > 56) {
		obj.id = 0;
	}
	if (mon.type < 0) {
		mon.type = 0;
	}
	if (mon.type > mon.maxTypes) {
		mon.type = mon.maxTypes;
	}
	if (place_type > 3) {
		place_type = 0;
	}

	// Get mouse position
	SDL_GetMouseState(&mx, &my);

	/* get render width and height
	 * but (mostly used to get actual render height)
	 */
	int renderW = 0;
	int renderHDummy = 0;
	SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
	int en = renderW * 0.4375;
	int renderH = renderW - en;

	// Grid like coordinates of mouse
	// get new mouse coordinates based on render size, and actual screen size
	mx = (screenWidth*mx)/gWindow.getWidth();			// New mouse coordinates, no relation to camera
	my = (screenHeight*my)/gWindow.getHeight();			// New mouse coordinates, no relation to camera

	//int mex = (1280*mx)/gWindow.getWidth();
	//int mey = (720*my)/gWindow.getHeight();
	int oldMX = mx+camx;								// New mouse coordinates, relation to camera
	int oldMY = my+camy;								// New mouse coordinates, relation to camera
	int clampSize = 16;									// Magnet pixel size
	int remainderW = oldMX % clampSize;
	int remainderH = oldMY % clampSize;
	int newMx = mx - remainderW;						// New mouse coordinates, locked on x32 coordinates
	int newMy = my - remainderH;						// New mouse coordinates, locked on x32 coordinates
	newMx = mx - remainderW;						// New mouse coordinates, locked on x32 coordinates
	newMy = my - remainderH;						// New mouse coordinates, locked on x32 coordinates

	// Editor mouse coordinates
	if (ctrl) {
		mouseX = mx;
		mouseY = my;
	}else{
		mouseX = newMx;
		mouseY = newMy;
	}

	// Update Items
	obj.Update(item, mouseX+camx, mouseY+camy, mx+camx, my+camy, camx, camy);

	// Update tiles
	tl.Update(tile, gWindow, mouseX+camx, mouseY+camy, mx+camx, my+camy, camx, camy, &rTiles[0]);

	// Update collision tiles
	//tc.update(tilec, mouseX+camx, mouseY+camy, mx+camx, my+camy, camx, camy, screenWidth, screenHeight);

	// Update Tile bar
	tb.Update(tilebar, gWindow, mx, my, camx, camy);

	// Editor
	if (editor) {
		if (leftClick) {
			/* If not on Tile-bar, place other tiles */
			if (!tb.touching) {
				if (editor) {
					if (place_type == 0) {
						tl.SpawnMultiple(tile, mouseX, mouseY, camx, camy, &rTiles[0]);
					}else if (place_type == 1) {
					//	tc.spawn(tilec, mouseX, mouseY, camx, camy);
					}else if (place_type == 2) {
						obj.Spawn(item, mouseX+camx, mouseY+camy, 16, 16);
					}else if (place_type == 3) {
						mon.SpawnMultiple(monster, mouseX, mouseY, camx, camy);
					}
				}
			}else{
				tb.Select(tilebar, tl.id);
			}
		}
		if (rightClick) {
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {
				if (editor) {
					if (place_type == 0) {
						tl.Remove(tile, 1);
					}else if (place_type == 1) {
					//	tc.remove(tilec, 1);
					}else if (place_type == 2) {
						obj.Remove(item, 1);
					}else if (place_type == 3) {
						mon.Remove(monster, 1);
					}
				}
			}else{
				tb.Select(tilebar, tl.id);
			}
		}
	}

	// Update Particles
	part.Update(particles, 0, 0, tl.levelWidth, tl.levelHeight, camx, camy,
				player1.getCenterX(), player1.getCenterY(),
				sLazer);
	part.updateStarParticles(particles, 0, 0, tl.levelWidth, tl.levelHeight);
	part.updateBulletParticles(particles, 0, 0, tl.levelWidth, tl.levelHeight);

	// Update Spawner
	spaw.update(spawner, player1.getCenterX(), player1.getCenterY(), mx, my, camx, camy);

	// Update VFX
	vf.Update(vfx, mx, my, camx, camy, part, particles);

	// Check collision between Tile & Player
	checkCollisionTilePlayer();

	// Check collision between Tilec & Player
	checkCollisionTilecPlayer();

	// Update Player
	player1.Update(part, particles, mx+camx, my+camy, sLazer);

	// Update Player
	//player.Update(particles, part,
	//		  	  tl, tile,
	//			  mx, my, camx, camy,
	//			  gWindow, gRenderer,
	//			  gText, gFont26, {255,255,255},
	//			  sAtariBoom, sLazer, sGrenade,
	//			  sGrenadePickup, sPistolReload);

	// update Monsters
	mon.EditorUpdate(monster, mouseX+camx, mouseY+camy, mx+camx, my+camy, camx, camy);

	// If Player is alive
	if (player1.isAlive && !editor) {

		// Check if Enemy has vision of Player
		checkEnemyPlayerVision(gRenderer);

		// update Monsters
		mon.Update(monster, part, particles, sLazer, camx, camy,
				   player1.getCenterX(), player1.getCenterY());

		// Monster and Player collision
		checkCollisionMonsterPlayer();

		// Monster will find nearest Tile that will get to the Player
		UpdateMonsterPlayer(gRenderer);
	}

	// Collision, particle & monster
	checkCollisionParticleMonster(part, particles);

	// Collision, tile & monster
	checkCollisionTileMonster();

	// Update Enemy Particle & Player collision check
	checkCollisionParticlePlayer(part, particles);

	// Collision: Grenade Particle & Monster
	checkCollisionGrenadePlayer(part, particles);

	// Check collision between Items & Player
	checkCollisionItemPlayer();

	// Collision: Particle & Collision Tiles
	checkCollisionParticleTile(part, particles);

	// Update Asteroids: Wave re-spawn
	spawnAsteroidsNow2();

	// Update Player manually
	UpdatePlayer(part, particles);

	// Handle collision of items and Level Size
	ClampObjectsToLevelSize(part, particles);

	// Damage text: for monster
	tex.update(text);

	// Update camera
	if (camLeft) {
		camx -= 3;
	}
	if (camRight) {
		camx += 3;
	}
	if (camUp) {
		camy -= 3;
	}
	if (camDown) {
		camy += 3;
	}

	// center camera on target
	if (camlock) {
		// If Level is smaller than Render size, center camera on center of Level,
		// otherwise it follows the Player, and also is bounded on the Level Size
		if (tl.levelWidth <= 270 || tl.levelHeight <= 152) {
			camx = 0 + tl.levelWidth / 2 - screenWidth/2;
			camy = 0 + tl.levelHeight / 2 - screenHeight/2;
		}else{
			float angleTowardsMouse = atan2((my+camy) - player1.y-player1.h/2, (mx+camx) - player1.x-player1.w/2);
			angleTowardsMouse = angleTowardsMouse * (180/M_PI);
			if (angleTowardsMouse < 0) { angleTowardsMouse = 360 - (-angleTowardsMouse); }
			float radians   = (3.1415926536/180)*(angleTowardsMouse);
			float Cos 		= floor(cos(radians)*100+0.05)/100;
			float Sin 		= floor(sin(radians)*100+0.05)/100;

			// Camera target distance
			float bmx, bmy;
			bmx  = player1.getCenterX()-screenWidth/2;
			bmy  = player1.getCenterY()-screenHeight/2;
			float distance = sqrt((bmx - camx) * (bmx - camx)+
								  (bmy - camy) * (bmy - camy));

			// Camera target
			float vX, vY;
			if (distance > 0.5){
				vX 	= 2 * (10*distance/600) * (bmx - camx) / distance;
				vY 	= 2 * (10*distance/600) * (bmy - camy) / distance;
			}
			camx += vX;
			camy += vY;

			// Camera bounds
			if( camx < 0 ){
				camx = 0;
			}
			if( camy < 0 ){
				camy = 0;
			}
			if( camx+screenWidth > tl.levelWidth ){
				camx = tl.levelWidth-screenWidth ;
			}
			if( camy+screenHeight  > tl.levelHeight ){
				camy = tl.levelHeight-screenHeight ;
			}
		}
	}
}

void PlayGame::RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow) {
	//gBG.render(gRenderer, map.x-camx, map.y-camy, tl.levelWidth, tl.levelHeight);
}

void PlayGame::Render(SDL_Renderer *gRenderer, LWindow &gWindow, Particle &part, Particle particles[]) {

	// Render particles
	//part.renderStarParticle(particles, camx, camy, 1, gRenderer);

	// Render Tiles
	tl.Render(gRenderer, tile, 0, camx, camy);
	tl.Render(gRenderer, tile, 1, camx, camy);

	// Render spawn point and exit door
	{
		// Render Player spawn point
		gDoor.render(gRenderer, spawnX-camx, spawnY-camy, 16, 16, &rDoor[2]);
		/*SDL_Rect tempRect = {spawnX-camx, spawnY-camy, 16, 16};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 200, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);*/

		// Render Exit door
		if (player1.collectedKeys >= tl.requiredKeys) {
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 16, 16, &rDoor[1]);
		}else{
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 16, 16, &rDoor[0]);
		}
	}

	// Render spawner
	spaw.render(spawner, camx, camy, gRenderer);

	// Render VFX
	//vf.Render(vfx, camx, camy, gRenderer);

	// Render Items
	obj.Render(gRenderer, item, camx, camy);

	// Render particles
	part.RenderBulletsBehind(gRenderer, particles,
							camx, camy, 1,
							player1.getCenterY()+6);

	// Render monsters
	mon.RenderBehind(gRenderer, monster, camx, camy, player1.y, player1.h);

	// Render Player
	player1.Render(gRenderer, camx, camy);

	// Render our player
	//player.Render(mx, my, camx, camy, gWindow,
	//			gRenderer,
	//			gFont13, gFont26,
	//			{255,255,255}, part.count, gText);

	// Render monsters
	mon.RenderInFront(gRenderer, monster, camx, camy, player1.y, player1.h);

	// Render particles
	part.RenderBulletsFront(gRenderer, particles,
							camx, camy, 1,
							player1.getCenterY()+6);

	// Render tile, appliances
	tl.Render(gRenderer, tile, 2, camx, camy);
	tl.Render(gRenderer, tile, 3, camx, camy);
	tl.Render(gRenderer, tile, 4, camx, camy);
	tl.Render(gRenderer, tile, 5, camx, camy);
	tl.Render(gRenderer, tile, 6, camx, camy);
}

void PlayGame::RenderBreak(SDL_Renderer *gRenderer, int layer) {
	/*for (int i = 0; i < tl.max; i++) {
		if (tile[i].alive && tile[i].screen){
			// Render all tiles
			if (!tl.hideOtherLayers) {
				if (layer == tile[i].layer) {
					// Center Door Tile
					if (tile[i].id > 63 && tile[i].id < 80) {

					}
					// Render every other tile normally
					else{
						// If destructible, render destructible Texture on Tiles
						if (tile[i].destructible) {
							tl.gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[tile[i].hits]);
						}
					}
				}
			}
		}
	}*/
}

void PlayGame::RenderLights(SDL_Renderer *gRenderer, Particle &part, Particle particles[]) {

	// Handle dim effect
	if (dimSize > 20) {
		dimDir = dimDir * -1;
	}
	else if (dimSize < 1) {
		dimDir = dimDir * -1;
	}

	dimSize += 0.6 * (dimDir * 0.6);

	// Render default lighting background (sets the darkness level
	gLightBG.setAlpha(29);
	gLightBG.setColor(255,255,255);
	gLightBG.setBlendMode(SDL_BLENDMODE_ADD);
	gLightBG.render( gRenderer, 0, 0, screenWidth, screenHeight);

	// Render Player lighting
	/*dimTimer += 15;
	if (dimTimer > 60) {
		dimTimer = 0;
		dimSize = 32;
	}*/

	// Spawn light
	gLight.setAlpha(255);
	gLight.setColor(255,255,255);
	gLight.setBlendMode(SDL_BLENDMODE_ADD);
	gLight.render( gRenderer, spawnX+16/2-16/2-dimSize/2-camx, spawnY+16/2-16/2-dimSize/2-camy, 16 + dimSize, 16 + dimSize);

	// Door light
	gLight.setAlpha(255);
	gLight.setColor(255,255,255);
	gLight.setBlendMode(SDL_BLENDMODE_ADD);
	gLight.render( gRenderer, doorX+16/2-16/2-dimSize/2-camx , doorY+16/2-16/2-dimSize/2-camy, 16 + dimSize, 16 + dimSize);

	// Player light
	gLight.setAlpha(255);
	gLight.setColor(255,255,255);
	gLight.setBlendMode(SDL_BLENDMODE_ADD);
	gLight.render( gRenderer, player1.getCenterX()-100/2-dimSize/2-camx, player1.getCenterY()-100/2-dimSize/2-camy, 100 + dimSize, 100 + dimSize);
	//gLight.render( gRenderer, player1.getCenterX()-128/2-dimSize/2-camx, player1.getCenterY()-128/2-dimSize/2-camy, 128 + dimSize, 128 + dimSize);

	// Monster lights
	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {
			gLight.setAlpha(255);
			gLight.setColor(180,20,180);
			gLight.setBlendMode(SDL_BLENDMODE_ADD);
			int lightW = 32 + 4 + dimSize;
			int lightH = 32 + 4 + dimSize;
			gLight.render( gRenderer, monster[i].x+monster[i].w/2-lightW/2-camx, monster[i].y+monster[i].h/2-lightH/2-camy, lightW, lightH);
		}
	}

	// Particle lights
	for (int i = 0; i < part.max; i++) {
		if (particles[i].alive) {
			if (particles[i].damage > 0) {
				for (int j = 0; j < 10; j++) {
					gLight.setAlpha(200);
					gLight.setColor(particles[i].color.r, particles[i].color.g, particles[i].color.b);
					gLight.setBlendMode(SDL_BLENDMODE_ADD);
					gLight.render( gRenderer, particles[i].x2-64/2-camx, particles[i].y2-64/2-camy, 64, 64);
					//gLight.setBlendMode(SDL_BLENDMODE_ADD);
					//gLight.render( gRenderer, particles[i].x2-80/2-camx, particles[i].y2-80/2-camy, 80, 80);
				}
			}else{
				gLight.setAlpha(particles[i].alpha);
				gLight.setColor(particles[i].color.r, particles[i].color.g, particles[i].color.b);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				gLight.render( gRenderer, particles[i].x2-(4/2)*7-camx, particles[i].y2-(4/2)*7-camy, 4*7, 4*7, NULL, particles[i].angle);

			}
		}
	}

	// Item lights
	for (int i = 0; i < obj.max; i++) {
		if (item[i].alive) {
			gLight.setAlpha(255);
			gLight.setBlendMode(SDL_BLENDMODE_ADD);
			if (item[i].id == 0) {
				gLight.setColor(189, 203, 197);
				int newWidth = item[i].w*1.2 + dimSize;
				int newHeight = item[i].h*1.2 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			else if (item[i].id == 7 || item[i].id == 8) {
				gLight.setColor(255, 255, 0);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			else if (item[i].id == 10) {
				gLight.setColor(9, 159, 220);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			// Mana pot
			else if (item[i].id == 25) {
				gLight.setColor(108, 80, 225);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			// Health pot
			else if (item[i].id == 26) {
				gLight.setColor(100, 255, 100);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			// left-side Torch
			else if (item[i].id == 31) {
				gLight.setColor(222, 222, 100);
				int newWidth = item[i].w*4.2 + dimSize;
				int newHeight = item[i].h*4.2 + dimSize;
				gLight.render( gRenderer, item[i].x2 - newWidth/2 - 5 - camx,
										  item[i].y2 - newHeight/2 - camy,
										  newWidth, newHeight);

			}
			// right-side Torch
			else if (item[i].id == 32) {
				gLight.setColor(222, 222, 100);
				int newWidth = item[i].w*4.2 + dimSize;
				int newHeight = item[i].h*4.2 + dimSize;
				gLight.render( gRenderer, item[i].x2 - newWidth/2 + 5 - camx,
										  item[i].y2 - newHeight/2 - camy,
										  newWidth, newHeight);

			}
		}
	}
}

void PlayGame::RenderDebug(SDL_Renderer *gRenderer, Particle &part, Particle particles[])
{
	// If debugging, show debug info
	if (debug) {
		// Character debug
		SDL_Rect tempRect = {player1.x-camx, player1.y-camy, player1.w, player1.h};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		// Particle debug
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				if (particles[i].damage > 0) {
					// Original box, untouched
					SDL_Rect tempRect = {particles[i].x-camx, particles[i].y-camy, particles[i].w, particles[i].h};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);

						std::stringstream tempss;
						tempss << particles[i].bounces;
						gText.setAlpha(255);
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
						gText.render(gRenderer,particles[i].x-camx, particles[i].y-camy,
								gText.getWidth()/5, gText.getHeight()/5);
				}


				// Render circle
				/*gCircle.setColor(255,255,255);
				gCircle.setAlpha(180);
				gCircle.render(gRenderer, particles[i].x-camx,
						particles[i].y-camy,8, 8);*/
			}
		}

		// Monster Debug
		mon.RenderDebug(gRenderer, monster, camx, camy);
		for (int i = 0; i < mon.max; i++) {
			if (monster[i].alive){
				// Render Monster sight range
				SDL_Rect rRect = {0,0,16,16};
				/*gCircles.setAlpha(100);
				gCircles.render(gRenderer, monster[i].x2-monster[i].sightRange-camx,
								monster[i].y2-monster[i].sightRange-camy,
								monster[i].sightRange*2,
								monster[i].sightRange*2, &rRect);
				// Render Monster attack range
				rRect = {16,0,16,16};
				gCircles.setAlpha(100);
				gCircles.render(gRenderer, monster[i].x2-monster[i].atkRange-camx,
								monster[i].y2-monster[i].atkRange-camy,
								monster[i].atkRange*2,
								monster[i].atkRange*2, &rRect);*/
				// Render Monster hasVision on target
				if (monster[i].alive && monster[i].follow){
					// Render Monster hasVision on target
					if (monster[i].hasVision) {
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
						SDL_RenderDrawLine(gRenderer, monster[i].x+monster[i].w/2-camx, monster[i].y+monster[i].h/2-camy,
												      player1.getCenterX()-camx, player1.getCenterY()-camy);
					}else{
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
						SDL_RenderDrawLine(gRenderer, monster[i].x+monster[i].w/2-camx, monster[i].y+monster[i].h/2-camy,
												      player1.getCenterX()-camx, player1.getCenterY()-camy);
					}
				}
			}
		}

		// Collision Tile debug
		//tc.render(gRenderer, tilec, camx, camy);

		// Render Map Box
		tempRect = {0-camx, 0-camy, tl.levelWidth, tl.levelHeight};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
		//player1.RenderDebug(gRenderer, camx, camy, gFont);
	}	// end debug
}

void PlayGame::RenderHand(SDL_Renderer *gRenderer) {
	if (debug){
		if (place_type == 0) {
			// Render Tile debug
			tl.RenderDebug(gRenderer, tile, mouseX, mouseY, mx, my, camx, camy, &rTiles[0], tb.tilesWidth);
		}else if (place_type == 1) {
/*
			// Render tile in hand
			SDL_Color color;
			int tcWidth;
			int tcHeight;
			if (tc.type == 0) {
				color = {0, 0, 255};
				tcWidth = 16;
				tcHeight = 13;
			}else if (tc.type == 1) {
				color = {255, 0, 255};
				tcWidth = 16;
				tcHeight = 16;
			}else if (tc.type == 2) {
				color = {255, 255, 0};
				tcWidth = 16;
				tcHeight = 16;
			}
			for (int j = 0; j < tc.multiW; j++) {
				for (int h = 0; h < tc.multiH; h++) {
					//SDL_Rect tempr = {newMx, newMy, 32*multiW, 32*multiH};
					SDL_Rect tempr = {mouseX+j*tc.tilew, mouseY+h*tc.tileh, tcWidth, tcHeight};
					SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}*/
		}else if (place_type == 2) {
			// Render Item in Hand
			// Render mouse coordinates snapped to grid
			obj.gItem.setAlpha(110);
			obj.gItem.render(gRenderer, mouseX, mouseY, 16, 16, &obj.rClips[obj.id]);
			SDL_Rect tempRect = {mouseX, mouseY, 16, 16};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}
	}
}

// Render text
void PlayGame::RenderText(SDL_Renderer *gRenderer, LWindow &gWindow, Particle &part, Particle particles[]) {

	/* Render Player Ammo */
	std::stringstream tempss;
	/*std::stringstream tempss;
	tempss << "camx: " << camx << ", camy: " << camy;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.ammoClip * 10, screenHeight - 20 - 100, gText.getWidth(), gText.getHeight());
*/
	// Render Player Ammo
	/*tempss.str(std::string());
	tempss << player.health << "/" << player.maxHealth;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.maxHealth * 5, 50, gText.getWidth(), gText.getHeight());

	// Render Player Health
	tempss.str(std::string());
	tempss << player.ammo << "/" << player.ammoClip;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.ammoClip * 10, 50 + 48 + 8, gText.getWidth(), gText.getHeight());

	// Render Player Ammo
	tempss.str(std::string());
	tempss << player.grenades << "/" << player.grenadesMax;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.grenadesMax * 36, 50 + 96 + 3, gText.getWidth(), gText.getHeight());*/

	// Render map
	//map.render(gRenderer, camx, camy);

	// Tile debug text info
	/*for (int i = 0; i < tl.max; i++) {
		if (tile[i].alive && tile[i].destructible) {
			std::stringstream tempss;
						tempss << tile[i].hits;
						gText.setAlpha(255);
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
						gText.render(gRenderer, tile[i].x - camx, tile[i].y - camy,gText.getWidth()/4, gText.getHeight()/4);
		}
	}*/


	for (double j=0; j<player1.spell.size(); j += 1) {
		std::stringstream tempss;
		/*tempss  << player1.spell[j].activate 		 << " "
				<< player1.spell[j].cooldown 		 << " "
				<< player1.spell[j].cooldownTimer    << " "
				<< player1.spell[j].baseCooldown     << " "
				<< player1.spell[j].currentDuration  << " "
				<< player1.spell[j].maxDuration;*/
		tempss  << player1.casting		 << " "  << player1.frame		 << " "  << player1.frameTimer		 << " ";
		gText.setAlpha(255);
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
		gText.render(gRenderer, player1.x - camx,
				player1.y-gText.getHeight()/2 - camy,
				gText.getWidth()/4, gText.getHeight()/4);
	}

	// Particle debug text info
	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {

			/*
			std::stringstream tempss;
			tempss <<monster[i].increAngle;
			gText.setAlpha(255);
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
			gText.render(gRenderer, monster[i].x - camx,
					monster[i].y-gText.getHeight()/2 - camy,
					gText.getWidth()/4, gText.getHeight()/4);
					*/

			/*tempss.str(std::string());
			tempss << "currentDuration: " << monster[i].currentDuration
					<< " maxDuration: " 	  << monster[i].maxDuration
					<< " attack: " << monster[i].attack
					<< " cooldown: " << monster[i].cooldown
					<< " cooldownTimer: " << monster[i].cooldownTimer;
			gText.setAlpha(255);
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont, 195);
			gText.render(gRenderer, monster[i].x - camx,
					monster[i].y - camy,
					gText.getWidth()/4, gText.getHeight()/4);*/

			for (double j=0; j<monster[i].spell.size(); j += 1) {

				/*std::stringstream tempss;
				tempss <<monster[i].spell[j].increAngle << " " <<monster[i].spell[j].increAngleMax;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				gText.render(gRenderer, monster[i].x - camx,
						monster[i].y-gText.getHeight()/2 - camy,
						gText.getWidth()/4, gText.getHeight()/4);*/

				// Spawn Spell as a particle
			/*	p_dummy.spawnParticleAngle(particle, monster[i].tag, monster[i].spell[j].type,
						monster[i].x2 - monster[i].spell[j].size/2,
						monster[i].y2 - monster[i].spell[j].size/2,
						monster[i].spell[j].size, monster[i].spell[j].size,
						monster[i].angle + k * (monster[i].spell[j].scope/monster[i].spell[j].projectiles), monster[i].spell[j].speed,
						monster[i].spell[j].damage,
						monster[i].spell[j].color, 1,
						monster[i].spell[j].dir, monster[i].spell[j].dirSpe,
						monster[i].spell[j].alpha, monster[i].spell[j].alphaSpe,
						monster[i].spell[j].deathTimer, monster[i].spell[j].deathTimerSpe,
						monster[i].spell[j].sizeDeath, monster[i].spell[j].sizeDeathSpe,
						monster[i].spell[j].decay, monster[i].spell[j].decaySpe,
						monster[i].spell[j].trail, monster[i].spell[j].trailRate, monster[i].spell[j].trailColor,
						monster[i].spell[j].trailMinSize, monster[i].spell[j].trailMaxSize);*/
			}






		}
	}

	// Editor debug menu
	if (debug) {

		// Render collision tile debug
		/*for (int i = 0; i < tc.max; i++) {
			if (tilec[i].alive){
				if (tilec[i].mouse) {
					SDL_Rect tempr = {tilec[i].x+tilec[i].w/2-4-camx, tilec[i].y+tilec[i].h/2-4-camy, 8, 8};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
					std::stringstream tempss;
					tempss << tilec[i].layer;
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
					gText.setAlpha(255);
					gText.render(gRenderer, tilec[i].x - camx, tilec[i].y - camy, gText.getWidth()/4, gText.getHeight()/4);
				}
				if (tilec[i].mouseBox) {
					SDL_Rect tempr = {tilec[i].x+4 - camx, tilec[i].y+4 - camy, tilec[i].w-8, tilec[i].h-8};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}*/

		// Render hand debug info
		std::stringstream tempss;
		tempss << "place_type: "  << place_type   << ", id: " 		 << tl.id
			   << ", layer: "     << tl.layer << ", editor: " 	 << editor
			   << ", tl.multiW: " << tl.multiW    << ", tl.multiH: " << tl.multiH
			   << ", tc.multiH: " << tl.multiH
			   << ", tl.tilew: "  << tl.tilew     << ", tl.tileh: "  << tl.tileh
			   << ", tl.Count: "  << tl.tileCount
			   << ", obj.count: " << obj.count << ", part.count: " << part.count;
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont, 200);
		gText.setAlpha(255);
		gText.render(gRenderer, 0+screenWidth-gText.getWidth()/4, 0, gText.getWidth()/4, gText.getHeight()/4);

		// Render tile debug
		if (debug){
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//------------------------------------- Render what's in Editors hand -------------------------------------//
			if (place_type == 0) {
				// Render Tile debug
				tl.RenderDebug(gRenderer, tile, mouseX, mouseY, mx, my, camx, camy, &rTiles[0], tb.tilesWidth);
			}else if (place_type == 1) {

				// Render tile in hand
				/*SDL_Color color;
				int tcWidth;
				int tcHeight;
				if (tc.type == 0) {
					color = {0, 0, 255};
					tcWidth = 16;
					tcHeight = 13;
				}else if (tc.type == 1) {
					color = {255, 0, 255};
					tcWidth = 16;
					tcHeight = 16;
				}else if (tc.type == 2) {
					color = {255, 255, 0};
					tcWidth = 16;
					tcHeight = 16;
				}
				for (int j = 0; j < tc.multiW; j++) {
					for (int h = 0; h < tc.multiH; h++) {
						//SDL_Rect tempr = {newMx, newMy, 32*multiW, 32*multiH};
						SDL_Rect tempr = {mouseX+j*tc.tilew, mouseY+h*tc.tileh, tcWidth, tcHeight};
						SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}
				}*/
			}else if (place_type == 2) {
				// Render Item in Hand
				// Render mouse coordinates snapped to grid
				obj.gItem.setAlpha(110);
				obj.gItem.render(gRenderer, mouseX, mouseY, 16, 16, &obj.rClips[obj.id]);
				SDL_Rect tempRect = {mouseX, mouseY, 16, 16};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}else if (place_type == 3) {
				int numberOfFramesPerRow = 0 + 2 * 2;
				int incrementToNextRowAmount = 9;
				int monsterId = (mon.type * incrementToNextRowAmount) + numberOfFramesPerRow;
				for (int j = 0; j < mon.multiW; j++) {
					for (int h = 0; h < mon.multiH; h++) {
						mon.gMonster.setAlpha(110);
						mon.gMonster.render(gRenderer, mouseX+j*16, mouseY+h*16, 16, 16, &mon.clip[monsterId]);
					}
				}

			}
			//------------------------------------- Render what's in Editors hand -------------------------------------//
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		// Render Tile Bar
		tb.Render(gRenderer, tilebar, tl.id);
	}
}

void PlayGame::RenderUI(SDL_Renderer *gRenderer) {

	/*tempRect = {doorX-camx, doorY-camy, 16, 16};
	SDL_SetRenderDrawColor(gRenderer, 86, 41, 22, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);*/

	// Player UI
	/* Render Player Health */
	/*for (int i=0; i<player.maxHealth; i++) {
		SDL_Rect tempRect = {20 + i * 5, 50, 3, 32};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.health; i++) {
		SDL_Rect tempRect = {20 + i * 5, 50, (500*player.health)/player.maxHealth, 32};
		SDL_SetRenderDrawColor(gRenderer, 15, 160, 15, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}*/
	/* Render health background */
	/*SDL_Rect tempRect = {5, 5, (40*player.maxHealth)/player.maxHealth, 4};
	SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);*/

	/* Render decaying health */
	/*if (player.healthDecay > player.health)
		player.healthDecay -= 0.5;
	tempRect = {5, 5, (40*player.healthDecay)/player.maxHealth, 4};
	SDL_SetRenderDrawColor(gRenderer, 80, 5, 5, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);*/




	//for (int i=0; i<player.keys; i++) {
		/*SDL_Rect tempRect = {5 + i * 2, 18, 1, 4};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);*/
	//}
	/*for (int i=0; i<player.grenades; i++) {
		SDL_Rect tempRect = {5 + i * 2, 18, 1, 4};
		SDL_SetRenderDrawColor(gRenderer, 175, 5, 175, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.grenadesMax; i++) {
		if (player.grenades == i) {
			SDL_Rect tempRect = {5 + i * 2, 18, int((1*player.grenadesCD)/300), 4};
			SDL_SetRenderDrawColor(gRenderer, 200, 20, 200, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);
		}
	}8/

	/* Render minimap */
	/*int minimapX = screenWidth - 48 - 14;
	int minimapY = screenHeight - 32 - 8;
	tempRect = {minimapX, minimapY , 48, 32};
	SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);*/

	/* Render Player on minimap */
	/*int tempX = (48*player.x2)/tl.levelWidth;
	int tempY = (32*player.y2)/tl.levelHeight;
	tempRect = {minimapX + tempX, minimapY + tempY , 1, 1};
	SDL_SetRenderDrawColor(gRenderer, 0, 100, 255, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);*/




	// Editor debug menu
	if (debug) {
		// Particle debug text info
		for (int i = 0; i < vf.max; i++) {
			if (vfx[i].isAlive) {
				std::stringstream tempss;
				tempss << int(vfx[i].duration/60);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
				gText.setAlpha(255);
				gText.setColor(255,255,255);
				int w = gText.getWidth()/4;
				int h = gText.getHeight()/4;
				gText.render(gRenderer, vfx[i].x+vfx[i].w/2-w/2-camx,
										vfx[i].y+vfx[i].h/2-h/2-camy,
										w, h);
			}
		}
	}
}

void PlayGame::RenderGUI(SDL_Renderer *gRenderer) {

	/*for (unsigned int i=0; i<player.spell.size(); i++) {
		// Render Mana background
		SDL_Rect tempRect = {75, 5 + i * 6, (40*player.spell[i].maxDuration)/player.spell[i].maxDuration, 4};
		SDL_SetRenderDrawColor(gRenderer, 35, 35, 35, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);

		// Render Spell Name
		std::stringstream tempss;
		tempss << player.spell[i].displayName;
		gText.setAlpha(255);
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), player.spell[i].color, gFont);
		int newWidth = gText.getWidth()/4;
		int newHeight = gText.getHeight()/4;
		gText.render(gRenderer, tempRect.x + tempRect.w + 4, tempRect.y, newWidth, newHeight);

		tempRect = {75, 5 + i * 6, (40*player.spell[i].currentDuration)/player.spell[i].maxDuration, 4};
		SDL_SetRenderDrawColor(gRenderer, player.spell[i].color.r, player.spell[i].color.g, player.spell[i].color.b, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);

	}*/

	if (player1.healthDecay > player1.health)
			player1.healthDecay -= 0.5;

	// Render Player Health bar
	/*renderStatusBar(gRenderer, player1.getCenterX()-camx, player1.y-camy, 16, 2,
					13,
					player1.health, player1.healthDecay, player1.maxHealth,
					{0, 80, 0}, {173, 45, 45},
					{90, 162, 105}, {255, 255, 255});

	// Render Player mana bar
	renderStatusBar(gRenderer, player1.getCenterX()-camx, player1.y-camy, 16, 2,
					11,
					player1.mana, 0, player1.maxMana,
					{0, 0, 80}, {0, 0, 0},
					{91, 108, 171}, {255, 255, 255});*/


	// Render Player amount of Keys
	SDL_Rect tempRect = {0,0,16,16};
	obj.gItem.render(gRenderer, 5, 18, 8, 8, &tempRect);
	std::stringstream tempss;
	tempss <<  "x " << player1.keys;
	tempss << " (" << player1.collectedKeys;
	tempss << "/" << tl.requiredKeys << ")";
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	int newWidth = gText.getWidth()/4;
	int newHeight = gText.getHeight()/4;
	gText.render(gRenderer, 5 + 8/2 + 3 + 2, 18 + 8/2 - newHeight/2,
			newWidth, newHeight);

	// Render Player amount of Coins
	tempRect = {0,16,16,16};
	obj.gItem.render(gRenderer, 5, 26, 8, 8, &tempRect);
	tempss.str(std::string());
	tempss <<  "x " << player1.coins;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	newWidth = gText.getWidth()/4;
	newHeight = gText.getHeight()/4;
	gText.render(gRenderer, 5 + 8/2 + 3 + 2, 26 + 8/2 - newHeight/2,
			newWidth, newHeight);

	// Current level
	tempss.str(std::string());
	tempss <<  "Level: " << playerStateLevel;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	newWidth = gText.getWidth()/4;
	newHeight = gText.getHeight()/4;
	gText.render(gRenderer, screenWidth - newWidth - 2, 2, newWidth, newHeight);


	// Render Player Spell cooldowns
	/*tempss.str(std::string());
	tempss <<  "Fireball: " << floor(player.spell[0].cooldownTimer / 60) << "s";
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	newWidth = gText.getWidth()/4;
	newHeight = gText.getHeight()/4;
	gText.render(gRenderer, 5, screenHeight - newHeight - 8, newWidth, newHeight);*/

	// Monster health bars
	mon.RenderGUI(gRenderer, monster, camx,camy, player1.getTargetID());

	// Render any text spawned
	for (int i = 0; i < 100; i++) {
		if (text[i].alive) {
			std::stringstream tempss;
			tempss << text[i].textfield.c_str();
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
			gText.setAlpha(text[i].alpha);
			gText.setColor(text[i].color.r, text[i].color.g, text[i].color.b);
			gText.render(gRenderer, text[i].x-camx, text[i].y-camy, gText.getWidth()/4, gText.getHeight()/4);

			/*SDL_Rect tempRect = {text[i].x - camx,text[i].y - camy,
							   text[i].w,  text[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
			SDL_RenderFillRect(gRenderer, &tempRect);*/
		}
	}

	// Render spell icons
	for (int i=0; i<4; i++) {
		float w = i*(16 + 3);
		float h = 16;
		float x = screenWidth/2 - 2*16;
		float y = screenHeight - h - 2;
		// Render icon
		gSpellIcons.setAlpha(215);
		gSpellIcons.render(gRenderer, x+w, y, 16, 16, &rSpellIcons[i]);
		// Render spell selection around correct spell
		if (i == player1.spellIndex+1) {
			gRect.setAlpha(215);
			gRect.render(gRenderer, x+w, y, 16, 16);
		}
	}

	// Render XBOX buttons
	for (int i=0; i<4; i++) {
		gXboxButtons.setAlpha(215);
		gXboxButtons.render(gRenderer, 5+i*(8 + 3), 5, 8, 8, &rXboxButtons[i]);
	}

	// Render cursor
	//gMouse.render(gRenderer, player1.bmx-5-camx, player1.bmy-5-camy, 10, 10, &rMouse[3]);

	float distance = sqrt( ((mx+camx - player1.x-player1.w/2) * (mx+camx - player1.x-player1.w/2)) +
						   ((my+camy - player1.y-player1.h/2) * (my+camy - player1.y-player1.h/2)) );

	//float angle = atan2(bmy - bmy2,bmx - bmx2);
	//float angle = angle * (180 / 3.1416);
	//if (angle < 0) { angle = 360 - (-angle); }
	float radians = (3.1415926536/180)*(player1.angle);
	float Cos = floor(cos(radians)*100+0.05)/100;
	float Sin = floor(sin(radians)*100+0.05)/100;
	//std::stringstream tempss2;
	//tempss2 << "angle: "  << player1.angle;
	//gText.loadFromRenderedText(gRenderer, tempss2.str().c_str(), {255, 255, 255, 255}, gFont);
	//gText.render(gRenderer, player1.x+player1.w/2 + Cos * 10, player1.y+player1.h/2 + Sin * 10, gText.getWidth()/4, gText.getHeight()/4);


	// Render left-side of status bar (the one with the circle
	int posW = 30;
	gGUI.render(gRenderer, screenWidth - 21 - 5 - posW, 5, 21, 16, &rGUI[0] );
	// render the individual background bars of the status bars
	for (int i=0; i<10; i++) {
		float x = screenWidth - 5 - posW;
		if (i == 0) {
			gGUI.render(gRenderer, x + i * 2, 5 + 1, 2, 14, &rGUI[1] );
		}else{
			gGUI.render(gRenderer, x + i * 2, 5 + 1, 2, 14, &rGUI[2] );
		}
	}
	// render the individual health bars based on Player's health
	for (int i=0; i<( (10*player1.health)/player1.maxHealth ); i++) {
		float x = screenWidth - 5 - posW;
		if (i == 0) {
			gGUI.render(gRenderer, x + i * 2, 5 + 1, 2, 4, &rHealthBar[0] );
		}else{
			gGUI.render(gRenderer, x + i * 2, 5 + 1, 2, 4, &rHealthBar[1] );
		}
	}
	// render the individual mana bars based on Player's mana bar
	for (int i=0; i<( (10*player1.mana)/player1.maxMana ); i++) {
		float x = screenWidth - 5 - posW;
		if (i == 0) {
			gGUI.render(gRenderer, x + i * 2, 10 + 1, 2, 4, &rManaBar[0] );
		}else{
			gGUI.render(gRenderer, x + i * 2, 10 + 1, 2, 4, &rManaBar[1] );
		}
	}
	// render the individual mana bars based on Player's Green Bar
	for (int i=0; i<( (10*500)/500 ); i++) {
		float x = screenWidth - 5 - posW;
		if (i == 0) {
			gGUI.render(gRenderer, x + i * 2, 15 + 1, 2, 4, &rGreenGar[0] );
		}else{
			gGUI.render(gRenderer, x + i * 2, 15 + 1, 2, 4, &rGreenGar[1] );
		}
	}

	// render the right-side of the health bar
	float x = screenWidth - 5 - posW;
	gGUI.render(gRenderer, x + 10 * 2, 5, 8, 16, &rGUI[3] );

	// Render reticle
	if (player1.controls == 0) {
		gMouse.render(gRenderer, mx-5, my-5, 10, 10, &rMouse[3]);
	}else{
		gMouse.render(gRenderer, player1.x+player1.w/2 + Cos * 32-5-camx, player1.y+player1.h/2 + Sin * 32-5-camy, 10, 10, &rMouse[3]);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Global Functions ////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void PlayGame::checkCollisionParticleMonster(Particle &part, Particle particles[]) {
	for (int i = 0; i < part.max; i++) {
			if (particles[i].alive && particles[i].damage > 0 && particles[i].onScreen) {
				for (int j = 0; j < mon.max; j++) {
					if (monster[j].alive && monster[j].onScreen) {
						if (particles[i].tag != monster[j].tag){
							if (checkCollision(particles[i].x, particles[i].y, particles[i].w, particles[i].h, monster[j].x, monster[j].y, monster[j].w, monster[j].h) ) {
								Mix_PlayChannel(3, sSpellExplode, 0);
								// reduce monster health
								monster[j].health -= particles[i].damage;
								// apply knockback effect
								knockbackEffect(particles[i].x, particles[i].y, particles[i].w, particles[i].h,
												monster[j].x, monster[j].y, monster[j].w, monster[j].h,
												monster[j].velX, monster[j].velY, randDouble(0.1, 0.3));
								// spawn blood particle effect
								for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
									int rands = rand() % 4 + 2;
									float newX = monster[j].x+monster[j].w/2;
									float newY = monster[j].y+monster[j].h/2;
									part.spawnParticleAngle(particles, "none", 4,
														newX-rands/2,
														newY-rands/2,
													   rands, rands,
													   h, randDouble(0.9, 3.1),
													   0.0,
													   {144, 10, 10, 255}, 1,
													   1, 1,
													   rand() % 100 + 150, rand() % 2 + 2,
													   rand() % 50 + 90, 0,
													   true, 0.11,
													   true, 0.11);
								}

								// remove particle
								part.Remove(particles, i);
							}
						}
					// particle collision with monster using a CIRCLE
					/*float bmx = monster[j].x + monster[j].w/2;
					float bmy = monster[j].y + monster[j].h/2;
					float bmx2 = particles[i].x + particles[i].w/2;
					float bmy2 = particles[i].y + particles[i].h/2;
					float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
										  (bmy - bmy2) * (bmy - bmy2));
					if (distance < monster[j].w/2 + particles[i].w/2) {
						Mix_PlayChannel(4, sSkeletonHurt, 0);
						//part.SpawnExplosion(particles, particles[i].x2, particles[i].y2, particles[i].color);
						// remove particle
						part.Remove(particles, i);
					}*/
				}

				if (monster[j].alive) {
					// Adjust Monster health
					if (monster[j].health <=0) {
						monster[j].health = 0;
					}
					if (monster[j].healthDecay >= monster[j].health) {
						monster[j].healthDecay -= 0.25;
					}
				}
			}
		}
	}
}

void PlayGame::checkCollisionTileMonster() {
	/*for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {
			for (int j = 0; j < tl.max; j++) {
				if (tile[j].alive && tile[j].collide){
					if (checkCollision(monster[i].x, monster[i].y, monster[i].w, monster[i].h, tile[j].x, tile[j].y, tile[j].w, tile[j].h)) {
						monster[i].collision = true;
					}else{
						monster[i].collision = false;
					}
				}
			}
		}
	}*/


	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {

			bool MonsterIsColliding = false;

			// Update collision with Tiles
			// Player Velocity X Axis


			// Move only in x-axis
			if (monster[i].targetDistanceX >= monster[i].targetDistanceY) {
				monster[i].x += monster[i].vX;
				monster[i].x += monster[i].velX;
				SDL_Rect rectA;
				if (monster[i].type == 12) {
					rectA.x = monster[i].x;
					rectA.y = monster[i].y;
					rectA.w = 48;
					rectA.h = 16;
				}
				if (monster[i].type < 12) {
					rectA.x = monster[i].x+4;
					rectA.y = monster[i].y+10;
					rectA.w = 9;
					rectA.h = 5;
				}
				bool moveBack;
				moveBack = false;
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collide) {
							SDL_Rect rectB;
							rectB.x = tile[j].x;
							rectB.y = tile[j].y;
							rectB.w = tile[j].w;
							rectB.h = tile[j].h;
							if  ( checkCollisionRect( rectA, rectB )) {
								// Continue handling collision
								moveBack = true;
							}
						}
					}
				}
				if (moveBack){
					monster[i].x -= monster[i].vX;
					monster[i].x -= monster[i].velX;
					MonsterIsColliding = true;
				}
			}

			// Move only in y-axis
			else{
				// Player Velocity Y Axis
				monster[i].y += monster[i].vY;
				monster[i].y += monster[i].velY;
				SDL_Rect rectA;
				if (monster[i].type == 12) {
					rectA.x = monster[i].x;
					rectA.y = monster[i].y;
					rectA.w = 48;
					rectA.h = 16;
				}
				if (monster[i].type < 12) {
					rectA.x = monster[i].x+4;
					rectA.y = monster[i].y+10;
					rectA.w = 9;
					rectA.h = 5;
				}
				if (monster[i].type == 12) {
					rectA.x = monster[i].x;
					rectA.y = monster[i].y;
					rectA.w = 48;
					rectA.h = 16;
				}
				if (monster[i].type < 12) {
					rectA.x = monster[i].x+4;
					rectA.y = monster[i].y+10;
					rectA.w = 9;
					rectA.h = 5;
				}
				bool moveBack;
				moveBack = false;
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collide) {
							SDL_Rect rectB;
							rectB.x = tile[j].x;
							rectB.y = tile[j].y;
							rectB.w = tile[j].w;
							rectB.h = tile[j].h;
							if  ( checkCollisionRect( rectA, rectB )) {
								// Continue handling collision
								moveBack = true;
							}
						}
					}
				}
				if (moveBack){
					monster[i].y -= monster[i].vY;
					monster[i].y -= monster[i].velY;
					MonsterIsColliding = true;
				}
			}

			//-------------------------------------------------------------------------------------------//
			//-------------------------------------------------------------------------------------------//
			//-------------------------------------------------------------------------------------------//

			// This is used to determine if we should find the shortest route to the Player
			if (MonsterIsColliding) {
				monster[i].collision = true;
			}else{
				monster[i].collision = false;
			}

			// Decrease knockback velocity each frame
			monster[i].velX = monster[i].velX - monster[i].velX * 0.08;
			monster[i].velY = monster[i].velY - monster[i].velY * 0.08;
		}
	}
}

void PlayGame::checkCollisionParticlePlayer(Particle &part, Particle particles[]) {
	// Player
	if (player1.isAlive){
		// Particle
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {

				// Check only if Particle is NOT a Star Particle
				//if (particles[i].type != 2){

				// Check only if Particle is NOT a Star Particle or a Grenade Particle
				if (particles[i].damage > 0){
					// Check only if Particle is NOT the Enemy's Particle
					if (particles[i].tag != player1.tag){

						// particle collision with player using a CIRCLE
						float bmx = player1.x + player1.w/2;
						float bmy = player1.y + player1.h/2;
						float bmx2 = particles[i].x + particles[i].w/2;
						float bmy2 = particles[i].y + particles[i].h/2;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						// collision occurred
						if (distance < player1.w/4 + particles[i].w/2) {

							// spawn blood particle effect
							for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
								int rands = rand() % 4 + 2;
								float newX = player1.getCenterX();
								float newY = player1.getCenterY();
								part.spawnParticleAngle(particles, "none", 4,
													newX-rands/2,
													newY-rands/2,
												   rands, rands,
												   h, randDouble(0.9, 3.1),
												   0.0,
												   {144, 10, 10, 255}, 1,
												   1, 1,
												   rand() % 100 + 150, rand() % 2 + 2,
												   rand() % 50 + 90, 0,
												   true, 0.11,
												   true, 0.11);
							}
							// play sound effect
							Mix_PlayChannel(-1, sPlayerHurt, 0);
							// reduce player health
							player1.health -= particles[i].damage;
							// remove particle
							part.Remove(particles, i);
							// Spawn damage effect on Player
							std::stringstream tempss;
							tempss << particles[i].damage;
							int randw = rand() % int(player1.w);
							tex.spawn(text, player1.x+randw-1, player1.getCenterY(),
									  0.0, -randDouble(0.1, 0.3), 255, tempss.str().c_str(), {255,255,0});

						}

						// particle collision with player using a BOX
						/*if (particles[i].x + particles[i].w > player1.x && particles[i].x < player1.x + player1.w
								&& particles[i].y + particles[i].h > player1.y
								&& particles[i].y < player1.y + player1.h) {
							particles[i].time = 0;
							// remove particle
							part.Remove(particles,i);

							player1.health -= particles[i].damage;
						}*/
					}
				}
			//	}
			}
		}	// end Particle
	}	// end Player
}

void PlayGame::checkCollisionGrenadePlayer(Particle &part, Particle particles[]) {
		/*for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				// Grenade particle
				if (particles[i].type == 3) {

					// Particle spin
					particles[i].angle += particles[i].angleSpe * particles[i].angleDir;

					// Particle death
					particles[i].time += particles[i].deathTimerSpeed;

					// Grenade has a smoke Particle effect coming out of it
					particles[i].timeri++;
					// Making this '30' frames will enable the Player to
					// watch 3 visual 'ticks' (particle sparks coming from the grenade
					 // that will explode momentarily after the 3 tick)
					//
					if (particles[i].timeri > 30) {
						particles[i].timeri = 0;
						for (double h=0.0; h< 360.0; h+=30){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							part.spawnParticleAngle(particles, "none", 4,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   h, randDouble(2.1, 9.1),
											   0.0,
											   {255, 144, 10, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.11,
											   true, randDouble(0.01, 0.6));
						}
					}

					// Particle death
					if (particles[i].time > particles[i].deathTimer) {
						for (int j=0; j<mon.max; j++) {
							if (monster[j].alive) {

								// monster target
								float bmx = particles[i].x+particles[i].w/2;
								float bmy = particles[i].y+particles[i].h/2;
								float bmx2 = monster[j].x+monster[j].w/2;
								float bmy2 = monster[j].y+monster[j].h/2;

								// monster distance from target
								float distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
								if (distance <= 0.5) {
									distance = 0.5;
								}
								// If within distance of explosion
								if (distance < 512){
								//if (distance < 104){
									// apply knock-back
									knockbackEffect(particles[i].x, particles[i].y, particles[i].w, particles[i].h,
													monster[j].x, monster[j].y, monster[j].w, monster[j].h, monster[j].vX, monster[j].vY, 15);
									// reduce health
									monster[j].health -= particles[i].damage;

									// spawn damage text
									std::stringstream tempss;
									tempss << particles[i].damage;
									int randw = rand() % int(monster[j].w);
									tex.spawn(text, monster[j].x+randw, monster[j].y+monster[j].h/2,
											  0.0, -3.7, 255, tempss.str().c_str(), {255,255, 0});
								}
							}
						}

						// play sound effect
						//Mix_PlayChannel(-1, sSpellExplode, 0);

						// spawn explosion particle effect
						for (double h=0.0; h< 360.0; h+=rand() % 10 + 2){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							part.spawnParticleAngle(particles, "none", 4,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   h, randDouble(2.1, 9.1),
											   0.0,
											   {255, 144, 10, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.11,
											   true, randDouble(0.005, 0.6));
						}
						for (double h=0.0; h< 360.0; h+=rand() % 10 + 2){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							part.spawnParticleAngle(particles, "none", 4,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   h, randDouble(2.1, 9.1),
											   0.0,
											   {255, 255, 0, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.11,
											   true, randDouble(0.0001, 0.001));
						}
						// remove particle
						part.Remove(particles, i);
					}
				}
			}
		}*/
}

void PlayGame::checkCollisionItemPlayer() {
	for (int i = 0; i < obj.max; i++) {
		if (item[i].alive) {
			// Collision with Keys
			if (item[i].id == 0) {
				if (checkCollision(player1.x, player1.y, player1.w, player1.h, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player keys
					player1.keys++;
					// also keep track of how many keys we have picked up iin this Stage Level
					player1.collectedKeys++;
					// ply sound effect
					Mix_PlayChannel(-1, sKeyPickup, 0);
					// remove item
					item[i].alive = false;
					obj.count--;
				}
			}
			// Collision with Small Yellow coin
			if (item[i].id == 7) {
				if (checkCollision(player1.x, player1.y, player1.w, player1.h, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player1.coins+=5;
					// ply sound effect
					playSFX(sounds, "Coin");
					// remove item
					item[i].alive = false;
					obj.count--;
					// spawn text on player
					std::stringstream tempss;
					tempss << "+5";
					float randVy = randDouble(0.1, 0.3);
					tex.spawn(text, player1.getX()+(rand() % 16)-5, player1.getCenterY(), 0.0, -randVy, 255, tempss.str().c_str(), {224, 211, 25}, randDouble(5, 7));
				}
			}
			// Collision with Large Yellow coin
			if (item[i].id == 8) {
				if (checkCollision(player1.x, player1.y, player1.w, player1.h, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player1.coins += 10;
					playSFX(sounds, "Coin");
					// remove item
					item[i].alive = false;
					obj.count--;
					// spawn text on player
					std::stringstream tempss;
					tempss << "+10";
					float randVy = randDouble(0.1, 0.3);
					tex.spawn(text, player1.getX()+(rand() % 16)-5, player1.getCenterY(), 0.0, -randVy, 255, tempss.str().c_str(), {224, 211, 25}, randDouble(5, 7));
				}
			}
			// Collision with Large Diamond
			if (item[i].id == 10) {
				if (checkCollision(player1.x, player1.y, player1.w, player1.h, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player1.coins += 50;
					// ply sound effect
					playSFX(sounds, "Coin");
					// remove item
					item[i].alive = false;
					obj.count--;
					// spawn text on player
					std::stringstream tempss;
					tempss << "+50";
					float randVy = randDouble(0.1, 0.3);
					tex.spawn(text, player1.getX()+(rand() % 16)-5, player1.getCenterY(), 0.0, -randVy, 255, tempss.str().c_str(), {224, 211, 25}, randDouble(5, 7));
				}
			}
			// Collision with Mana Pot
			if (item[i].id == 25) {
				if (checkCollision(player1.x, player1.y, player1.w, player1.h, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// remove item
					item[i].alive = false;
					obj.count--;
					// increase player coins
					player1.addMana(75);
					// ply sound effect
					Mix_PlayChannel(-1, sDrinkMana, 0);
					// spawn text on player
					std::stringstream tempss;
					tempss << "+75";
					float randVy = randDouble(0.1, 0.4);
					tex.spawn(text, player1.getX()+(rand() % 16)-5, player1.getCenterY(), 0.0, -randVy, 255, tempss.str().c_str(), {25, 251, 255}, randDouble(6, 9));
				}
			}
			// Collision with Health Pot
			if (item[i].id == 26) {
				if (checkCollision(player1.x, player1.y, player1.w, player1.h, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// remove item
					item[i].alive = false;
					obj.count--;
					// increase player coins
					player1.addHealth(20);
					// ply sound effect
					Mix_PlayChannel(-1, sDrinkHealth, 0);
					// spawn text on player
					std::stringstream tempss;
					tempss << "+20";
					float randVy = randDouble(0.1, 0.4);
					tex.spawn(text, player1.getX()+(rand() % 16)-5, player1.getCenterY(), 0.0, -randVy, 255, tempss.str().c_str(), {70, 255, 70}, randDouble(6, 9));
				}
			}
		}
	}
}

void PlayGame::checkCollisionTilePlayer() {
	// Y axis
	if (!player1.casting) {
		if (player1.moveUp) {
			// If Player holds control, they will not move
			if (!player1.ctrl) {
				if (player1.vY > -0.8) {
					player1.vY -= 0.10;
				}
				player1.moving = true;
			}
			// If player holds shift, they will stay in current direction
			if (!player1.shift) {
				//angle = 270.0;
			//	player1.facing = 1;
			}
		}
		else if (player1.moveDown) {
			if (!player1.ctrl) {
				if (player1.vY < 0.8) {
					player1.vY += 0.10;
				}
				player1.moving = true;
			}
			if (!player1.shift) {
				//angle = 90.0;
			//	player1.facing = 0;
			}
		}

		// X axis
		if (player1.moveLeft) {
			if (!player1.ctrl) {
				if (player1.vX > -0.8) {
					player1.vX -= 0.10;
				}
				player1.moving = true;
			}
			if (!player1.shift) {
				//angle = 180.0;
			//	player1.facing = 3;
			}
		}
		else if (player1.moveRight) {
			if (!player1.ctrl) {
				if (player1.vX < 0.8) {
					player1.vX += 0.10;
				}
				player1.moving = true;
			}
			if (!player1.shift) {
				//angle = 0.0;
			//	player1.facing = 2;
			}
		}
		// Set direction to shoot where Player's mouse is, unless we are casting
		if (player1.controls == 0) {
			//player1.bmx = mx+camx;
			//player1.bmy = my+camy;
			player1.bmx = player1.getCenterMouseX();
			player1.bmy = player1.getCenterMouseY();
		}else if (player1.controls == 1) {
			player1.bmx = player1.getCenterCursorX();
			player1.bmy = player1.getCenterCursorY();
		}
	}else{

	}

	// Get angle towards target

	// Keyboard controls
	if (player1.controls == 0) {
		player1.angle = atan2(player1.bmy - player1.y-player1.h/2, player1.bmx - player1.x-player1.w/2);
		player1.angle = atan2((my+camy) - player1.y-player1.h/2, (mx+camx) - player1.x-player1.w/2);
		player1.angle = player1.angle * (180/M_PI);
		if (player1.angle < 0) { player1.angle = 360 - (-player1.angle); }
	}
	// Gamepad controls
	else if (player1.controls == 1) {
		player1.angle = player1.RAngle;
	}


	// Do facing direction based on angle
	//if (!player1.shift) {
		if (player1.angle >= 315 || player1.angle < 45) {
			player1.facing = 2;
		}else if (player1.angle >= 45 && player1.angle < 135) {
			player1.facing = 0;
		}else if (player1.angle >= 135 && player1.angle < 225) {
			player1.facing = 3;
		}else if (player1.angle >= 225 && player1.angle < 315) {
			player1.facing = 1;
		}
	//}









	// Movement deceleration
	if (player1.moveUp && player1.moveLeft) {
		if (!shift) {
			//angle = 225.0;
		}
	}
	if (player1.moveUp && player1.moveRight) {
		if (!shift) {
			//angle = 315.0;
		}
	}
	if (player1.moveDown && player1.moveLeft) {
		if (!shift) {
			//angle = 135.0;
		}
	}
	if (player1.moveDown && player1.moveRight) {
		if (!player1.shift) {
			//angle = 45.0;
		}
	}


	// Update collision with Tiles
	//tl.checkCollision(tile, x, y, w, h, y, vY);

	// Update collision with Tiles
	// Player Velocity X Axis
	player1.x += player1.vX;
	SDL_Rect rectA;
	rectA.x = player1.x+4;
	rectA.y = player1.y+8;
	rectA.w = player1.w-8;
	rectA.h = player1.h/2;
	bool moveBack;
	moveBack = false;
	for (int i = 0; i < tl.max; i++) {
		if (tile[i].alive){
			if (tile[i].collide) {
				SDL_Rect rectB;
				rectB.x = tile[i].x;
				rectB.y = tile[i].y;
				rectB.w = tile[i].w;
				rectB.h = tile[i].h;
				if  ( checkCollisionRect( rectA, rectB )) {
					// If Player has more than 0 keys, then unlock door, otherwise continue collision check
					if (player1.keys > 0 && player1.useKey) {
						player1.useKey = false;
						if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
							tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
							player1.keys--;
							Mix_PlayChannel(-1, sKeyPickup, 0);
							tile[i].alive = false;
							tl.tileCount--;
						}
					}
					// Continue handling collision
					moveBack = true;
				}
			}
		}
	}
	if (moveBack){
		player1.x -= player1.vX;
	}

	// Player Velocity Y Axis
	player1.y += player1.vY;;
	rectA.x = player1.x+4;
	rectA.y = player1.y+8;
	rectA.w = player1.w-8;
	rectA.h = player1.h/2;
	moveBack = false;
	for (int i = 0; i < tl.max; i++) {
		if (tile[i].alive){
			if (tile[i].collide) {
				SDL_Rect rectB;
				rectB.x = tile[i].x;
				rectB.y = tile[i].y;
				rectB.w = tile[i].w;
				rectB.h = tile[i].h;
				if  ( checkCollisionRect( rectA, rectB )) {
					// If Player has more than 0 keys, then unlock door, otherwise continue collision check
					if (player1.keys > 0 && player1.useKey) {
						player1.useKey = false;
						if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
							tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
							player1.keys--;
							Mix_PlayChannel(-1, sKeyPickup, 0);
							tile[i].alive = false;
							tl.tileCount--;
						}
					}
					// Continue handling collision
					moveBack = true;
				}
			}
		}
	}
	if (moveBack){
		player1.y -= player1.vY;
	}

	// Is Player moving?
	if (!player1.moveLeft && !player1.moveRight) {
		player1.vX = player1.vX - player1.vX * 0.16;
	}
	if (!player1.moveUp && !player1.moveDown) {
		player1.vY = player1.vY - player1.vY * 0.16;
	}
	if (!player1.moveUp && !player1.moveDown && !player1.moveLeft && !player1.moveRight) {
		player1.moving = false;
	}
}

void PlayGame::checkCollisionTilecPlayer() {

}
void PlayGame::checkCollisionMonsterPlayer() {
	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {

			// Monster colliding with Player
				float bmx = player1.getCenterX();
				float bmy = player1.getCenterY();
				float bmx2 = monster[i].x2;
				float bmy2 = monster[i].y2;
				float angle = atan2(bmy - bmy2,bmx - bmx2);
				angle = angle * (180 / 3.1416);
				if (angle < 0) {
					angle = 360 - (-angle);
				}
				float radians = (3.1415926536/180)*(angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;
				float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
									  (bmy - bmy2) * (bmy - bmy2));
				if (distance <= 1) {
					distance = 1;
				}
				if (distance < monster[i].h/2 + 16/2) {
					//monster[i].vX -= 1 * Cos;
					//monster[i].vY -= 1 * Sin;
					player1.vX += 1 * Cos;
					player1.vY += 1 * Sin;
				}
		}
	}
}

void PlayGame::checkCollisionParticleTile(Particle &part, Particle particles[]) {
	for (int j = 0; j < tl.max; j++) {
		if (tile[j].alive && tile[j].collide && tile[j].screen) {
			for (int i = 0; i < part.max; i++) {
					if (particles[i].alive && particles[i].damage > 0 && particles[i].onScreen) {
					// particle collision with monster using a CIRCLE
					float bmx = tile[j].x + tile[j].w/2;
					float bmy = tile[j].y + tile[j].h/2;
					float bmx2 = particles[i].x + particles[i].w/2;
					float bmy2 = particles[i].y + particles[i].h/2;
					float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
										  (bmy - bmy2) * (bmy - bmy2));
					if (distance < tile[j].w/2 + particles[i].w/2) {
						// particle still has bounces left, bounce it
						/*if (particles[i].bounces > 0) {
							particles[i].bounces--;
							particles[i].vX = particles[i].vX *-1;
							particles[i].vY = particles[i].vY *-1;
							particles[i].angle = atan2(particles[i].vY, particles[i].vX);
							particles[i].angle = particles[i].angle * (180 / 3.1416);
							if (particles[i].angle < 0) { particles[i].angle = 360 - (-particles[i].angle); }
						}
						// particle ran out of bounces, destroy
						else{
						}*/
						Mix_PlayChannel(3, sSpellExplode, 0);
						part.SpawnExplosion(particles, particles[i].x2, particles[i].y2, particles[i].color);
						// remove particle
						part.Remove(particles, i);
						if (tile[j].destructible) {
							// Tile break by 1
							tile[j].hits++;
						}
					}
					/*if (checkCollision(particles[i].x, particles[i].y, particles[i].w, particles[i].h, tile[j].x, tile[j].y, 16, 16) ) {
						Mix_PlayChannel(3, sSpellExplode, 0);
						////part.SpawnExplosion(particles, particles[i].x2, particles[i].y2, particles[i].color);
						// remove particle
						part.Remove(particles, i);
					}*/
				}
			}
		}
	}
}

void PlayGame::checkEnemyPlayerVision(SDL_Renderer *gRenderer) {
	for (int i=0; i<mon.max; i++) {
		if (monster[i].alive) {
			Point p1, q1;
			p1.x = monster[i].x2;
			p1.y = monster[i].y2;
			q1.x = player1.getCenterX();
			q1.y = player1.getCenterY();

			bool hasVision = true;

			for (int j=0; j<tl.max; j++) {
				if (tile[j].alive && tile[j].collide) {
					// a: top left
					// b: top right
					// c: bottom left
					// d: bottom right
					Point a, b, c, d;
					// top left
					a.x = tile[j].x;
					a.y = tile[j].y;
					// top right
					b.x = tile[j].x+tile[j].w;
					b.y = tile[j].y;
					// bottom left
					c.x = tile[j].x;
					c.y = tile[j].y+tile[j].h;
					// bottom right
					d.x = tile[j].x+tile[j].w;
					d.y = tile[j].y+tile[j].h;

					// top side line
					/*SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
					SDL_RenderDrawLine(gRenderer, a.x-camx, a.y-camy, b.x-camx, b.y-camy);

					// right side line
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawLine(gRenderer, b.x-camx, b.y-camy, d.x-camx, d.y-camy);

					// bottom side line
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderDrawLine(gRenderer, d.x-camx, d.y-camy, c.x-camx, c.y-camy);

					// left side line
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
					SDL_RenderDrawLine(gRenderer, a.x-camx, a.y-camy, c.x-camx, c.y-camy);*/

					if (doIntersect(p1, q1, a, b)) {
						hasVision = false;
					}else if (doIntersect(p1, q1, b, d)) {
						hasVision = false;
					}else if (doIntersect(p1, q1, d, c)) {
						hasVision = false;
					}else if (doIntersect(p1, q1, c, a)) {
						hasVision = false;
					}
				}
			}
			monster[i].hasVision = hasVision;
		}
	}
}

void PlayGame::spawnAsteroidsNow2()
{
	// Increase number of Enemies spawned after "1000 * player.increment/2"
	/*if (e_dummy.mileScore > 1000 * (player.increment/2)){
		e_dummy.mileScore = 0;
		e_dummy.SPAWN_ENEMY = true;
		player.lives++;
	}*/

	// Spawn new wave of Enemies
	/*if (e_dummy.SPAWN_ENEMY){
		int randi, randw, randh;
			randi = rand() % 4 + 1;
			switch (randi){
				case 1:{
					//TODO - make Enemy X coordinate spawn more random
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
					}
					break;
				}
				case 2:{
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
					}
					break;
				}
				case 3:{
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
					}
					break;
				}
				case 4:{
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
					}
					break;
				}
			}

		// Increase enemy count
		if (e_dummy.ENEMY_SPAWN < 5){
			e_dummy.ENEMY_SPAWN++;
		}

		// Stop spawning Enemies
		e_dummy.SPAWN_ENEMY 	= false;
	}*/

	// Check if there is 0 Asteroids
	/*if (mon.count <= 0){
		if (player.increment < 50) {
			player.increment += rand() % 3 + 3;
		}
		player.wave++;

		for (int i=0; i<40+player.increment; i++){
			int randx 		= rand() % tl.levelWidth;
			int randy 		= rand() % tl.levelHeight;
			//s_spawn.spawn(spawner, 0+randx, 0+randy, randw, randh);
			//mon.spawn(monster, map.x+randx, map.y+randy,
			//		  80, 80, 128, 128,
			//		  0.0, randDouble(3.6, 4.1), 0, 100,
			//		  11, 57, 17);
		}

		// Give Player a 180 ms shield
		player.shield			= true;
		player.shieldT			= 180;
	}*/

}

void PlayGame::UpdatePlayer(Particle &part, Particle particles[]) {
	// Player died, reset and reload Level
	if (player1.health <= 0) {
		ResetLevel(part, particles);
	}
}

void PlayGame::UpdateMonsterPlayer(SDL_Renderer *gRenderer) {
	// Make vector of Tiles
	// With these Tiles, we will get the closest one to the Player
	std::vector<Tile*> vTiles[100];
	std::vector<Monster*> vMonsters;

	// Get correct Tiles to store in our vector of Tiles
	for (int j = 0; j < tl.max; j++) {
		if (tile[j].alive && !tile[j].collide) {
			// Get distance of Player from Tile
			float bmx = tile[j].x+tile[j].w/2;
			float bmy = tile[j].y+tile[j].h/2;
			float bmx2 = player1.getCenterX();
			float bmy2 = player1.getCenterY();
			tile[j].distanceP = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));

			// Monsters
			for (int i = 0; i < mon.max; i++) {
				if (monster[i].alive) {
					// If Monster is not Boss type
					if (monster[i].type != 12) {
						float bmx = tile[j].x+tile[j].w/2;
						float bmy = tile[j].y+tile[j].h/2;
						float bmx2 = monster[i].x+monster[i].w/2;
						float bmy2 = monster[i].y+monster[i].h/2;
						int distanceM= sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
						// If tile is less than or equal to 16 pixels.
						if (distanceM <= 20) {
							// Store Tiles in Pointer
							vTiles[i].push_back( &tile[j] );
						}
					}
				}	// end check for Particles
			}
		}
	}	// end check for Monsters

	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive && monster[i].collision) {
			//if (monster[i].alive && monster[i].collision && monster[i].hasVision) {
			//if (monster[i].alive && monster[i].alert && monster[i].distance > 48 && monster[i].distance < 100 && monster[i].collision) {
			/*std::stringstream tempss;
			tempss << "vTiles.size(): " << vTiles[i].size();
			gText.setAlpha(255);
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,0,0}, gFont);
			int newWidth = gText.getWidth()/4;
			int newHeight = gText.getHeight()/4;
			gText.render(gRenderer, monster[i].x-camx, monster[i].y+i*12-camy,
					newWidth, newHeight);*/

			// Each monster is given Tiles,
			// these Tiles are less than 16 pixels away from the Monster
			// Now we are going to find which of those Tiles are closest to the Player
			// After that we are going to move the Monster towards that Tile
			bool renderTileInfo = false;
			Tile** closestTile;
			closestTile = &vTiles[i][0];
			// Get Tile nearest to Player
			for (unsigned int j = 0; j < vTiles[i].size(); j++) {
				// Render Tiles that are less than 16 pixels away from Monster
				/*std::stringstream tempss;
				tempss << "p: " << vTiles[i][j]->distanceP;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				int newWidth = gText.getWidth()/4;
				int newHeight = gText.getHeight()/4;
				gText.render(gRenderer,
						vTiles[i][j]->x-camx,
						vTiles[i][j]->y-camy,
						newWidth, newHeight);*/
				// Get Tile that is closest to Player from list of Tiles we grabbed
				if (vTiles[i][j]->distanceP < closestTile[0]->distanceP) {
					closestTile[0] = vTiles[i][j];
					renderTileInfo = true;
				}
			}
			// Render nearest Tile
			for (unsigned int j = 0; j < vTiles[i].size(); j++) {
				//if (renderTileInfo) {
					// Move towards nearest Tile
					// Get Cosine and Sin of nearest Tile to Monster
					float bmx = closestTile[0]->x+closestTile[0]->w/2;
					float bmy = closestTile[0]->y+closestTile[0]->h/2;
					float bmx2 = monster[i].x+monster[i].w/2;
					float bmy2 = monster[i].y+monster[i].h/2;
					float angle = atan2(bmy - bmy2,bmx - bmx2);
					angle = angle * (180 / 3.1416);
					if (angle < 0) { angle = 360 - (-angle); }
					float radians = (3.1415926536/180)*(angle);
					float Cos = floor(cos(radians)*10+0.5)/10;
					float Sin = floor(sin(radians)*10+0.5)/10;
					float targetDistanceX = sqrt((bmx - bmx2) * (bmx - bmx2));
					float targetDistanceY = sqrt((bmy - bmy2) * (bmy - bmy2));
					if (targetDistanceX < 0.1) {
						targetDistanceX = 0.1;
					}
					if (targetDistanceY < 0.1) {
						targetDistanceY = 0.1;
					}
					monster[i].vX = 0.05 * (bmx - bmx2) / targetDistanceX;
					monster[i].vY = 0.05 * (bmy - bmy2) / targetDistanceY;

					// Move Monster x-axis{
					if (monster[i].distance > 16 && monster[i].distance < 100) {
						monster[i].x += monster[i].vX;
					}
					// if colliding, move Monster back in x-axis
					SDL_Rect rectA;
					rectA.x = monster[i].x+4;
					rectA.y = monster[i].y+10;
					rectA.w = 9;
					rectA.h = 5;
					bool moveBack;
					moveBack = false;
					for (int j = 0; j < tl.max; j++) {
						if (tile[j].alive){
							if (tile[j].collide) {
								SDL_Rect rectB;
								rectB.x = tile[j].x;
								rectB.y = tile[j].y;
								rectB.w = tile[j].w;
								rectB.h = tile[j].h;
								if  ( checkCollisionRect( rectA, rectB )) {
									// Continue handling collision
									moveBack = true;
								}
							}
						}
					}
					if (moveBack){
						monster[i].x -= monster[i].vX;
						monster[i].x += monster[i].velX;
					}
					// Move Monster x-axis
					if (monster[i].distance > 16 && monster[i].distance < 100) {
						monster[i].y += monster[i].vY;
					}
					// if colliding, move Monster back in x-axis
					rectA.x = monster[i].x+4;
					rectA.y = monster[i].y+10;
					rectA.w = 9;
					rectA.h = 5;
					moveBack = false;
					for (int j = 0; j < tl.max; j++) {
						if (tile[j].alive){
							if (tile[j].collide) {
								SDL_Rect rectB;
								rectB.x = tile[j].x;
								rectB.y = tile[j].y;
								rectB.w = tile[j].w;
								rectB.h = tile[j].h;
								if  ( checkCollisionRect( rectA, rectB )) {
									// Continue handling collision
									moveBack = true;
								}
							}
						}
					}
					if (moveBack){
						monster[i].y -= monster[i].vY;
						monster[i].y += monster[i].velY;
					}


					// Render nearest Tile
					//tempss.str(std::string());
					/*std::stringstream tempss;
					tempss << "p: " << closestTile[0]->distanceP;
					gText.setAlpha(255);
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,0,0}, gFont);
					int newWidth = gText.getWidth()/4;
					int newHeight = gText.getHeight()/4;
					gText.render(gRenderer,
							closestTile[0]->x-camx,
							closestTile[0]->y-camy,
							newWidth, newHeight);*/
				//}
			}
		}
	}
}

void PlayGame::ClampObjectsToLevelSize(Particle &part, Particle particles[]) {

	// Collision with door
	if (player1.collectedKeys >= tl.requiredKeys && player1.useKey) {
		if (checkCollision(player1.x, player1.y, player1.w, player1.h, doorX, doorY, 16, 16)) {
			// Increase Stage level
			playerStateLevel++;
			// Change to next Stage Level
			LoadLevel(playerStateLevel, part, particles);
		}
	}
	// Player
	if (player1.x+player1.w < 0) {
		player1.x = 0;
	}
	if (player1.x > 0+tl.levelWidth) {
		player1.x =0+tl.levelWidth-player1.w;
	}
	if (player1.y+player1.h < 0) {
		player1.y = 0;
	}
	if (player1.y > 0+tl.levelHeight) {
		player1.y = 0+tl.levelHeight-player1.h;
	}
	// Monster
	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {
			if( monster[i].x < 0 ){
				monster[i].x = 0;
			}
			if( monster[i].y < 0 ){
				monster[i].y = 0;
			}
			if( monster[i].x+monster[i].w > tl.levelWidth ){
				monster[i].x = tl.levelWidth-monster[i].w;
			}
			if( monster[i].y+monster[i].h > tl.levelHeight ){
				monster[i].y = tl.levelHeight-monster[i].h ;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MULTIPLE PURPOSE /////////////////////////////////////////////

void PlayGame::knockbackEffect(float targetX, float targetY, int targetW, int targetH,
							   float itemX, float itemY, int itemW, int itemH,
							   float &itemVX,float &itemVY, float force) {
	// monster target
	float bmx2 = targetX+targetW/2;
	float bmy2 = targetY+targetH/2;
	float bmx  = itemX+itemW/2;
	float bmy  = itemY+itemH/2;

	// monster distance from target
	float distance = sqrt( (bmx - bmx2) * (bmx - bmx2)    +   (bmy - bmy2) * (bmy - bmy2) );
	if (distance <= 0.5) {
		distance = 0.5;
	}
	// positive means going towards bmx, and bmy
	//float constVX = (bmx - bmx2) / distance;
	//float constVY = (bmy - bmy2) / distance;

	float angle = atan2(bmy - bmy2,bmx - bmx2);
	angle = angle * (180 / 3.1416);

	if (angle < 0) {
		angle = 360 - (-angle);
	}

	// positive means going towards bmx, and bmy
	itemVX += force * (cos( (3.14159265/180)*(angle) ));
	itemVY += force * (sin( (3.14159265/180)*(angle) ));
}

double PlayGame::checkDistance(double x2, double y2, double targetX, double targetY) {
	double distance;
	double bmx = targetX;
	double bmy = targetY;
	double bmx2 = x2;
	double bmy2 = y2;
	distance = sqrt( (bmx-bmx2)*(bmx-bmx2) + (bmy-bmy2)*(bmy-bmy2) );
	return distance;
}

////////////////////////////////////////////// MULTIPLE PURPOSE /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------ Save Functions ------------------------------------------------------//


void PlayGame::loadSpawnPoint(int level){
	// open config file
	std::stringstream filename;
	filename << "resource/data/maps/level" << level;
	filename << "/spawn.mp";
	std::fstream fileSpawn( filename.str().c_str() );
	if (fileSpawn.is_open()) {
		fileSpawn >>  spawnX >> spawnY >>  doorX >> doorY;
		player1.x		= spawnX;
		player1.y		= spawnY-6;
	}
	// Error opening spawn.mp file
	else{
		// Creating default spawn.mp
		std::stringstream filename;
		filename << "data/maps/level" << level;
		filename << "/spawn.mp";
		std::ofstream newSpawnFile( filename.str().c_str() );
		// If successfully created spawn.mp file
		if (newSpawnFile.is_open()) {
			newSpawnFile << "32 32 48 32";
			spawnX			= 32;
			spawnY			= 32;
			doorX			= 48;
			doorY			= 32;
			player1.x		= spawnX;
			player1.y		= spawnY-6;
		}
		newSpawnFile.close();
	}
	fileSpawn.close();
}

void PlayGame::ClearLevel(Particle &part, Particle particles[]) {
	tl.RemoveAll(tile);
	//tc.RemoveAll(tilec);
	obj.RemoveAll(item);
	part.RemoveAll(particles);
	mon.RemoveAll(monster);
}

void PlayGame::LoadLevel(int level, Particle &part, Particle particles[]) {
	// Reset Player movements and direction
	player1.facing = 0;
	player1.keys = 0;
	player1.collectedKeys = 0;
	player1.mana = player1.maxMana;
	player1.health = player1.maxHealth;
	player1.healthDecay = player1.maxHealth;
	player1.useKey = false;
	player1.moveLeft = false;
	player1.moveRight = false;
	player1.moveUp = false;
	player1.moveDown = false;
	// Remove Particles
	part.RemoveAll(particles);
	// Load Tiles
	tl.LoadData(tile, level);
	// Load Collision Tiles
	//tc.loadTiles(tilec, level);
	// Load Items (Items) Tiles
	obj.LoadData(item, level);
	// Load Monsters
	mon.LoadData(monster, level);
	// Load level spawn point
	loadSpawnPoint(level);
}

void PlayGame::ResetLevel(Particle &part, Particle particles[]) {
	player1.facing = 0;
	player1.keys = 0;
	player1.collectedKeys = 0;
	player1.mana = player1.maxMana;
	player1.health = player1.maxHealth;
	player1.healthDecay = player1.maxHealth;
	player1.moveLeft = false;
	player1.moveRight = false;
	player1.moveUp = false;
	player1.moveDown = false;
	LoadLevel(playerStateLevel, part, particles);
}
//------------------------------------------------------ Save Functions ------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------- Editor Controls --------------------------------------------//


// Mouse Pressed
PlayGame::Result PlayGame::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
			player1.setTarget(NULL, NULL, NULL);
			for (int i = 0; i < mon.max; i++) {
				if (monster[i].alive && monster[i].onScreen){
					if (checkCollision(mx+camx, my+camy, 1, 1, monster[i].x, monster[i].y, monster[i].w, monster[i].h )) {
						player1.setTarget(&monster[i].x2, &monster[i].y2, i);
					}
				}
			}
			//player1.setTarget( mon.getID(monster, mx+camx, my+camy) );
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = true;
			//vf.Spawn(vfx, mouseX+camx, mouseY+camy,
			//		16, 16,
			//		60*3, 30,
			//		1.0);
		}
		if (event.button.button == SDL_BUTTON_MIDDLE) {

		}
	}
	return Nothing;
}

// Mouse Released
PlayGame::Result PlayGame::mouseReleased(SDL_Event event){
	PlayGame::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = false;
		}
		if (event.button.button == SDL_BUTTON_MIDDLE) {

		}
	}
	return result;
}



// Editor controls
void PlayGame::editorOnKeyDown( SDL_Keycode sym, Particle &part, Particle particles[] )
{
	switch (sym) {
	case SDLK_UP:				// camera up
		camUp = true;
		break;
	case SDLK_DOWN:				// camera down
		camDown = true;
		break;
	case SDLK_LEFT:				// camera left
		camLeft = true;
		break;
	case SDLK_RIGHT:			// camera right
		camRight = true;
		break;
	case SDLK_y:				// camera lock
		camlock = (!camlock);
		break;
	case SDLK_b:				// Change Tile collision properties
		if (shift) {
			tl.ChangeCollision(tile, 0);
		}else{
			tl.ChangeCollision(tile, 1);
		}
		break;
	case SDLK_v:				// Change Tile destructible properties
		if (shift) {
			tl.ChangeDestructable(tile, 0);
		}else{
			tl.ChangeDestructable(tile, 1);
		}
		break;
	case SDLK_x:				// Save spawn point
		spawnX = mouseX+camx;
		spawnY = mouseY+camy;
		break;
	case SDLK_u:				// Spawn Tile as a collision or not
		tl.collide = (!tl.collide);
		break;
	case SDLK_j:				// Spawn Tile as a destructible or not
		tl.destructible = (!tl.destructible);
		break;
	case SDLK_z:				// Save door spawn point
		doorX = mouseX+camx;
		doorY = mouseY+camy;
		break;
	case SDLK_w:
		if (shift) {
			tb.Move(tilebar, "up");
		}
		break;
	case SDLK_s:
		if (shift) {
			tb.Move(tilebar, "down");
		}
		break;
	case SDLK_a:
		if (shift) {
			tb.Move(tilebar, "left");
		}
		break;
	case SDLK_d:
		if (shift) {
			tb.Move(tilebar, "right");
		}
		break;
	case SDLK_q:								// Change place type (i.e. Tiles or Collision Tiles)
		place_type++;
		break;
	case SDLK_TAB:								// Toggle hide other layers
		tl.hideOtherLayers = (!tl.hideOtherLayers);
		break;
	case SDLK_k:								// Change clamp size
		if (shift){
			if (clampSize > 2) {
				clampSize -=2;
			}
		}else{
			clampSize+=2;
		}
		break;
	case SDLK_l:								// Change place_type Layer
		if (shift) {
			if (place_type==0) {
				tl.layer--;
			}else if (place_type==1) {
				//tc.layer--;
			}
		}else{
			if (place_type==0) {
				tl.layer++;
			}else if (place_type==1) {
				//tc.layer++;
			}
		}
		break;
	case SDLK_i:								// Change place_type Type
		if (shift) {
			if (place_type==0) {
				tl.id--;
			}else if (place_type==1) {
				//tc.type--;
			}else if (place_type==2) {
				obj.id--;
			}else if (place_type==3) {
				mon.type--;
			}
		}else{
			if (place_type==0) {
				tl.id++;
			}else if (place_type==1) {
			//	tc.type++;
			}else if (place_type==2) {
				obj.id++;
			}else if (place_type==3) {
				mon.type++;
			}
		}
		break;
	case SDLK_PERIOD:							// Tile, change Layer
		if (shift)
			tl.ChangeLayer(tile, -1);
		else
			tl.ChangeLayer(tile, 1);
		break;
	case SDLK_c:								// Tile, copy Tile
		if (editor) {
			if (place_type == 0 ) {
				tl.Copy(tile);
			}else if (place_type == 1) {
			//	tc.copy(tilec);
			}
		}
		break;
	case SDLK_SPACE:							// Tile, remove all Tiles (and Collision Tiles)
		if (shift && editor) {
			ClearLevel(part, particles);

			/*if (place_type == 0 ) {
			//	tl.removeAllTiles(tile);
			}else if (place_type == 1) {
			//	tc.removeAll(tilec);
			}*/
		}
		break;
	case SDLK_LEFTBRACKET:						// Tile, subtract block in x-axis
		if (place_type == 0) {
			// If shift, then change tile size
			if (shift) {
				if (tl.tilew > 1) {
					tl.tilew -= 1;
				}
			}
			else{
				// If not shift, then change tile amount placed
				if (tl.multiW > 1) {
					tl.multiW -= 1;
				}
			}
		}else if (place_type == 1) {
			if (shift) {
				//if (tc.tilew > 1) {
				//	tc.tilew -= 1;
				//}
			}
			else{
				//if (tc.multiW > 1) {
				//	tc.multiW -= 1;
				//}
			}
		}else if (place_type == 3) {
			if (mon.multiW > 1) {
				mon.multiW -= 1;
			}
		}
		break;
	case SDLK_RIGHTBRACKET:						// Tile, add block in x-axis or change tile size
		if (place_type == 0) {
			if (shift) {
				tl.tilew += 1;
			}
			else{
				tl.multiW += 1;
			}
		}else if (place_type == 1) {
			if (shift) {
				//tc.tilew += 1;
			}
			else{
				//tc.multiW += 1;
			}
		}else if (place_type == 3) {
			mon.multiW += 1;
		}
		break;
	case SDLK_MINUS:							// Tile, subtract block in y-axis or change tile size
		if (place_type == 0) {
			if (shift) {
				if (tl.tileh > 1) {
					tl.tileh -= 1;
				}
			}
			else{
				if (tl.multiH > 1) {
					tl.multiH -= 1;
				}
			}
		}else if (place_type == 1) {
			if (shift) {
				//if (tc.tileh > 1) {
				//	tc.tileh -= 1;
				//}
			}
			else{
				//if (tc.multiH > 1) {
				//	tc.multiH -= 1;
				//}
			}
		}else if (place_type == 3) {
			if (mon.multiH > 1) {
				mon.multiH -= 1;
			}
		}
		break;
	case SDLK_EQUALS:							// Tile, add block in y-axis
		if (place_type == 0) {
			if (shift) {
				tl.tileh += 1;
			}
			else{
				tl.multiH += 1;
			}
		}else if (place_type == 1) {
			if (shift) {
				//tc.tileh += 1;
			}
			else{
				//tc.multiH += 1;
			}
		}else if (place_type == 3) {
			mon.multiH += 1;
		}
		break;
	case SDLK_9: {								// Load Room
			/*tl.loadTiles(tile);
			tc.loadTiles(tilec);
			//loadcTiles(ctile);

			std::stringstream tempss;
			tempss << "Loading Tiles";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/

			break;
		}
	case SDLK_0: {								// Save Room
			/*tl.saveTiles(tile);
			tc.saveTiles(tilec);
			//savecTiles(ctile);

			std::stringstream tempss;
			tempss << "Saving Tiles";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/
			break;
		}
	}
}
void PlayGame::editorOnKeyUp( SDL_Keycode sym ) {
	switch (sym) {
	case SDLK_UP:
		camUp = false;
		break;
	case SDLK_DOWN:
		camDown = false;
		break;
	case SDLK_LEFT:
		camLeft = false;
		break;
	case SDLK_RIGHT:
		camRight = false;
		break;
	}
}


//-------------------------------------------- Editor Controls --------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////







