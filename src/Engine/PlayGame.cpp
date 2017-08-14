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

void PlayGame::Init() {
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
	for (int h = 0; h < tb.tilesHeight; h++) {
		for (int w = 0; w < tb.tilesWidth; w++) {
			rTiles[i].x = 0 + w * 16;
			rTiles[i].y = 0 + h * 16;
			rTiles[i].w = 16;
			rTiles[i].h = 16;
			i++;
		}
	}
	for (int h = 0; h < 4; h++) {
		for (int w = 0; w < 4; w++) {
			rTiles[i].x = 0 + w * 32;
			rTiles[i].y = 128 + h * 32;
			rTiles[i].w = 32;
			rTiles[i].h = 32;
			i++;
		}
	}
	rDoor[0] = {0, 0, 16, 16};
	rDoor[1] = {16, 0, 16, 16};
	rDoor[2] = {32, 0, 16, 16};
	// Initialize
	// Items
	obj.Init(item);
    // Zombies
	mon.Init(monster);
	// Particles
	part.init(particles);
	// Spawners
	spaw.init(spawner);
	// Player
	player.Init();
	player.SetName("Carl");
	// Text
	tex.init(text);
	// Tiles
	tl.Init(tile);
	// Collision Tiles
	tc.init(tilec);
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
void PlayGame::Load(LWindow &gWindow, SDL_Renderer *gRenderer) {

	// Load Video settings from file
	loadVideoCFG();

	// Load Audio settings from file
	loadAudioCFG();

	// Load audio files
	LoadAudioFiles();

	// Apply audio configurations
	applyOldAudioCFG();

 	// load textures
	gDoor.loadFromFile(gRenderer, 	"resource/gfx/door.png");
	gBG.loadFromFile(gRenderer, 		"resource/gfx/test.png");
	gCircle.loadFromFile(gRenderer, 	"resource/gfx/circle.png");

	// load fonts
	gFont 	= TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 18);
	gFont13 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 13);
	gFont26 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 26);

	// load particle textures
	gParticles.loadFromFile(gRenderer, "resource/gfx/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);

	// load particle textures
	gLightBG.loadFromFile(gRenderer, "resource/gfx/light_bg.png");
	gLight.loadFromFile(gRenderer, "resource/gfx/light.png");

	//Load texture target
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	// load media for other classes
	part.load(gRenderer);
	player.Load(gRenderer);
	spaw.load(gRenderer);
	tb.Load(gRenderer);
	tl.Load(gRenderer);
	obj.Load(gRenderer);
	mon.Load(gRenderer);

	// Apply video configurations
	//applyVideoCFG(gWindow);
}

void PlayGame::Free() {
	// free textures
	gParticles.free();
	gText.free();
	gBG.free();
	gCircle.free();

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
	player.Free();
	spaw.free();
	tl.Free();
	obj.Free();
	mon.Free();
}

void PlayGame::Show(LWindow &gWindow, SDL_Renderer *gRenderer, PlayGame::Result &result, int levelToLoad) {
	// Initialize
	Init();

	//fri.Spawn(frigate, 0, 2, map.x+tl.levelWidth - 620 - 180, map.y + tl.levelHeight/2 - 280/2, 180.0, 0.0, "Team1");

	// Load resources
	Load(gWindow, gRenderer);

	LoadLevel(levelToLoad);
	playerStateLevel = levelToLoad;

    // Play Music, looped
	//Mix_FadeInMusic(sAmbientMusic, -1, 0);

	// While loop
	while (!quit) {

		// Start FPS cap
		fps.start();

		// Always show cursor
		SDL_ShowCursor(true);

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Close program
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {

					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_q:
						if (!editor) {
							ResetLevel();
						}
						break;
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
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
						if (editor) {
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
							std::stringstream CollisionTileSaveData;
							CollisionTileSaveData << tc.saveTiles(tilec);

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
									  CollisionTileSaveData.str().c_str(),
									  ItemSaveData.str().c_str(),
									  MonsterSaveData.str().c_str());
							// Editor visual message
							tempss.str(std::string());
							tempss << "Finished saving.";
							tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());
						}
						break;
					case SDLK_ESCAPE:
						start(gWindow, gRenderer, player);
						break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyDown(event.key.keysym.sym);
					}
					/* Player Controls */
					else{
						// Player key down
						player.OnKeyDown(player, event.key.keysym.sym);
					}	// end editor check
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					// Local controls
					switch (event.key.keysym.sym) {
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
						player.OnKeyUp(player, event.key.keysym.sym);
					} // end editor
				}
				//////////////////////////////////////

				/////////////////////////////////////

				// Update Player click state
				if (!editor) {
					// Player Mouse Click state
					player.mouseClickState(player, event);

					// Update Xbox 360 controls
					player.updateJoystick(player, event);
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
		Update(gWindow, gRenderer);

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
			Render(gRenderer, gWindow);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );
		//------------------------------------ Draw on Scene here ------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render lights
			RenderLights(gRenderer);

			/// Render Scene
			// If in editor mode, turn off lights (which is actually just setting our Scene Texture from "ADD to "BLEND" blend mode)
			if (editor) {
				gTargetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
			}else{
				gTargetTexture.setBlendMode(SDL_BLENDMODE_MOD);
			}
			gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight, NULL, 0.0);

			/// Render Debug
			RenderDebug(gRenderer);

			/// Render Text
			RenderText(gRenderer, gWindow);

			/// Render UI
			RenderUI(gRenderer);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}
	// Free everything
	Free();
}

// Update everything
void PlayGame::Update(LWindow &gWindow, SDL_Renderer *gRenderer) {
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
	if (tc.type < 0) {
		tc.type = 0;
	}
	if (tc.type > 2) {
		tc.type = 0;
	}
	if (tc.layer > 2) {
		tc.layer = 0;
	}
	if (obj.id > 56) {
		obj.id = 0;
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
	if (shift) {
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
	tc.update(tilec, mouseX+camx, mouseY+camy, mx+camx, my+camy, camx, camy, screenWidth, screenHeight);

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
						tc.spawn(tilec, mouseX, mouseY, camx, camy);
					}else if (place_type == 2) {
						obj.Spawn(item, mouseX+camx, mouseY+camy, 16, 16);
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
						tc.remove(tilec, 1);
					}else if (place_type == 2) {
						obj.Remove(item, 1);
					}
				}
			}else{
				tb.Select(tilebar, tl.id);
			}
		}
	}

	// Update Particles
	part.Update(particles, 0, 0, tl.levelWidth, tl.levelHeight, sSpellExplode);
	part.updateStarParticles(particles, 0, 0, tl.levelWidth, tl.levelHeight);
	part.updateBulletParticles(particles, 0, 0, tl.levelWidth, tl.levelHeight);

	// Update Spawner: spawns an Asteroid
	spaw.update(spawner, player.x+player.w/2, player.y+player.h/2, mx, my, camx, camy);

	// Check collision between Tile & Player
	checkCollisionTilePlayer();

	// Check collision between Tilec & Player
	checkCollisionTilecPlayer();

	// Update Player
	player.Update(particles, part,
			  	  tc, tilec,
			  	  tl, tile,
				  mx, my, camx, camy,
				  gWindow, gRenderer,
				  gText, gFont26, {255,255,255},
				  sAtariBoom, sLazer, sGrenade,
				  sGrenadePickup, sPistolReload);

	// If Player is alive
	if (player.alive && !editor) {

		// update zombies
		mon.Update(monster, particles, part, player, sLazer, camx, camy);
	}

	// Collision, particle & monster
	checkCollisionParticleMonster();

	// Collision, tile & monster
	checkCollisionTileZombie();

	// Update Enemy Particle & Player collision check
	checkCollisionParticlePlayer();

	// Collision: Grenade Particle & Monster
	checkCollisionGrenadePlayer();

	// Check collision between Items & Player
	checkCollisionItemPlayer();

	// Collision: Particle & Collision Tiles
	checkCollisionParticleTile();

	// Update Asteroids: Wave re-spawn
	spawnAsteroidsNow2();

	// Update Player manually
	UpdatePlayer();

	// Handle collision of items and Level Size
	ClampObjectsToLevelSize();

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
			float bmx, bmy;
			bmx  = player.x+player.w/2-screenWidth/2;
			bmy  = player.y+player.h/2-screenHeight/2;
			float distance = sqrt((bmx - camx) * (bmx - camx)+
								  (bmy - camy) * (bmy - camy));

			// Camera target
			float vX, vY;
			if (distance >= 1){
				vX 	= 2 * (10*distance/256) * (bmx - camx) / distance;
				vY 	= 2 * (10*distance/256) * (bmy - camy) / distance;
				camx += vX;
				camy += vY;
			}

			// Camera bounds
			if( camx < 0 ){
				camx = 0;
			}
			if( camy < 0 ){
				camy = 0;
			}
			if( camx+screenWidth > tl.levelWidth ){
				camx = tl.levelWidth-screenWidth;
			}
			if( camy+screenHeight > tl.levelHeight ){
				camy = tl.levelHeight-screenHeight ;
			}
		}
	}
}

void PlayGame::RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow) {
	//gBG.render(gRenderer, map.x-camx, map.y-camy, tl.levelWidth, tl.levelHeight);
}

void PlayGame::Render(SDL_Renderer *gRenderer, LWindow &gWindow) {

	// Render particles
	part.renderStarParticle(particles, camx, camy, 1, gRenderer);

	// Render Colonies that spawn Asteroids
	spaw.render(spawner, camx, camy, gRenderer);

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
		if (player.collectedKeys == tl.requiredKeys) {
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 16, 16, &rDoor[1]);
		}else{
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 16, 16, &rDoor[0]);
		}
	}

	// Render Items
	obj.Render(gRenderer, item, camx, camy);

	// Render monsters
	mon.RenderBehind(gRenderer, monster, camx, camy, player.y, player.h);

	// Render our player
	player.Render(mx, my, camx, camy, gWindow,
				gRenderer,
				gFont13, gFont26,
				{255,255,255}, part.count, gText);

	// Render monsters
	mon.RenderInFront(gRenderer, monster, camx, camy, player.y, player.h);

	// Render particles
	part.renderBulletParticle(particles, camx, camy, 1, gRenderer);

	// Render tile, appliances
	tl.Render(gRenderer, tile, 2, camx, camy);
	tl.Render(gRenderer, tile, 3, camx, camy);
	tl.Render(gRenderer, tile, 4, camx, camy);
	tl.Render(gRenderer, tile, 5, camx, camy);
	tl.Render(gRenderer, tile, 6, camx, camy);
}

void PlayGame::RenderLights(SDL_Renderer *gRenderer) {

	// Handle dim effect
	if (dimSize > 20) {
		dimDir = dimDir * -1;
	}
	else if (dimSize < 1) {
		dimDir = dimDir * -1;
	}

	dimSize += 0.5 * (dimDir * 0.5);

	// Render default lighting background (sets the darkness level
	gLightBG.setAlpha(50);
	gLightBG.setColor(255,255,255);
	gLightBG.setBlendMode(SDL_BLENDMODE_ADD);
	gLightBG.render( gRenderer, 0, 0, screenWidth, screenHeight);

	// Render Player lighting
	/*dimTimer += 15;
	if (dimTimer > 60) {
		dimTimer = 0;
		dimSize = 32;
	}*/

	// Player Spawn Point light
	gLight.setAlpha(255);
	gLight.setColor(199,21,133);
	gLight.setBlendMode(SDL_BLENDMODE_ADD);
	gLight.render( gRenderer, spawnX+16/2-32/2-dimSize/2-camx, spawnY+16/2-32/2-dimSize/2-camy, 32 + dimSize, 32 + dimSize);

	// Player Door Point light
	gLight.setAlpha(255);
	gLight.setColor(255,255,255);
	gLight.setBlendMode(SDL_BLENDMODE_ADD);
	gLight.render( gRenderer, doorX+16/2-32/2-dimSize/2-camx , doorY+16/2-32/2-dimSize/2-camy, 32 + dimSize, 32 + dimSize);


	// Player default light
	/*gLight.setAlpha(255);
	gLight.setColor(255,255,255);
	gLight.setBlendMode(SDL_BLENDMODE_ADD);
	gLight.render( gRenderer, player.x2-64-dimSize/2-camx, player.y2-64-dimSize/2-camy, 128 + dimSize, 128 + dimSize);*/

	// Render Monster lights
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

	// Render Particle lights
	for (int i = 0; i < part.max; i++) {
		if (particles[i].alive) {
			if (particles[i].damage > 0) {
				gLight.setAlpha(180);
				gLight.setColor(particles[i].color.r, particles[i].color.g, particles[i].color.b);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				gLight.render( gRenderer, particles[i].x2-108/2-camx, particles[i].y2-108/2-camy, 108, 108);
			}else{
				gLight.setAlpha(particles[i].alpha);
				gLight.setColor(particles[i].color.r, particles[i].color.g, particles[i].color.b);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				SDL_Rect rParticles = {0,0,8,8};
				gLight.render( gRenderer, particles[i].x2-(particles[i].w/2)*7-camx, particles[i].y2-(particles[i].h/2)*7-camy, particles[i].w*7, particles[i].h*7, NULL, particles[i].angle);

			}
		}
	}

	for (int i = 0; i < obj.max; i++) {
		if (item[i].alive) {
			if (item[i].id == 0) {
				gLight.setAlpha(255);
				gLight.setColor(189, 203, 197);
				//gLight.setColor(255, 255, 255);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			else if (item[i].id == 7 || item[i].id == 8) {
				gLight.setAlpha(255);
				//gLight.setColor(251, 236, 68);
				gLight.setColor(255, 255, 0);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			else if (item[i].id == 10) {
				gLight.setAlpha(255);
				gLight.setColor(9, 159, 220);
				//gLight.setColor(255, 255, 255);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
			// Mana pot
			else if (item[i].id == 25) {
				gLight.setAlpha(255);
				//gLight.setColor(9, 159, 220);
				gLight.setColor(108, 80, 225);
				gLight.setBlendMode(SDL_BLENDMODE_ADD);
				int newWidth = item[i].w*3 + dimSize;
				int newHeight = item[i].h*3 + dimSize;
				gLight.render( gRenderer, item[i].x2-newWidth/2-camx,
										  item[i].y2-newHeight/2-camy,
										  newWidth, newHeight);

			}
		}
	}
}

void PlayGame::RenderDebug(SDL_Renderer *gRenderer)
{

	/*tempRect = {mx-602/2, my-280/2, 620, 280};
	SDL_SetRenderDrawColor(gRenderer, 0, 210, 0, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);*/

	// If debugging, show debug info
	if (debug) {
		/* Render Player debug */
		// Render circle
		gCircle.setColor(255,255,255);
		gCircle.setAlpha(180);
		gCircle.render(gRenderer, player.x-camx,player.y-camy, player.w, player.h);

		// Original box, untouched
		SDL_Rect tempRect = {player.x-camx, player.y-camy, player.w, player.h};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
		int playerX = player.x - 3;
		int playerY = player.y - 9;
		int playerW = 16;
		int playerH = 16;
		tempRect = {playerX-camx, playerY-camy, playerW, playerH};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
		// Box, centered
		/*tempRect = {player.x2-camx, player.y2-camy, player.radius, player.radius};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);*/

		/* Render Particle debug */
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				if (particles[i].damage > 0) {
					// Original box, untouched
					SDL_Rect tempRect = {particles[i].x-camx, particles[i].y-camy, particles[i].w, particles[i].h};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);
				}


				// Render circle
				/*gCircle.setColor(255,255,255);
				gCircle.setAlpha(180);
				gCircle.render(gRenderer, particles[i].x-camx,
						particles[i].y-camy,8, 8);*/
			}
		}

		/* Render Monster debug */
		for (int i = 0; i < mon.max; i++) {
			if (monster[i].alive) {
				// Original box, untouched
				SDL_Rect tempRect = {monster[i].x-camx, monster[i].y-camy, monster[i].w, monster[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
				// Box, centered
				tempRect = {monster[i].x2-camx, monster[i].y2-camy, monster[i].w, monster[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Render circle
				gCircle.setColor(255,255,255);
				gCircle.setAlpha(180);
				gCircle.render(gRenderer, monster[i].x2-camx,
						monster[i].y2-camy,
						monster[i].w, monster[i].h);

				// Render angle Text
				std::stringstream tempss;
				tempss << monster[i].frame*monster[i].facing;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				int w = gText.getWidth()/4;
				int h = gText.getHeight()/4;
				gText.render(gRenderer, monster[i].x - camx, monster[i].y-h - camy, w, h);

			}
		}
		tc.render(gRenderer, tilec, camx, camy);

		// Render Map Box
		tempRect = {0-camx, 0-camy, tl.levelWidth, tl.levelHeight};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

	}	// end debug
}

// Render text
void PlayGame::RenderText(SDL_Renderer *gRenderer, LWindow &gWindow) {

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

	/* Render Particle debug */
	/*
	for (int i = 0; i < part.max; i++) {
		if (particles[i].alive) {
			if (particles[i].damage > 0) {
				// Render angle Text
				std::stringstream tempss;
				tempss << particles[i].tag;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				gText.render(gRenderer, particles[i].x - camx/particles[i].layer, particles[i].y-gText.getHeight() - camy/particles[i].layer, gText.getWidth(), gText.getHeight());

			}
		}
	}
	*/

	// Render any text spawned
	for (int i = 0; i < 100; i++) {
		if (text[i].alive) {
			std::stringstream tempss;
			tempss << text[i].textfield.c_str();
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
			gText.setAlpha(text[i].alpha);
			gText.setColor(text[i].color.r, text[i].color.g, text[i].color.b);
			gText.render(gRenderer, text[i].x, text[i].y, gText.getWidth()/4, gText.getHeight()/4);

			/*SDL_Rect tempRect = {text[i].x - camx,text[i].y - camy,
							   text[i].w,  text[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
			SDL_RenderFillRect(gRenderer, &tempRect);*/
		}
	}

	// Editor debug menu
	if (editor) {

		// Render collision tile debug
		for (int i = 0; i < tc.max; i++) {
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
		}

		// Render hand debug info
		std::stringstream tempss;
		tempss << "place_type: "  << place_type   << ", id: " 		 << tl.id
				   << ", layer: "     << tl.layer     << ", layerC: " 	 << tc.layer
				   << ", editor: " 	 << editor
			   << ", tl.multiW: " << tl.multiW    << ", tl.multiH: " << tl.multiH
			   << ", tc.multiW: " << tc.multiW    << ", tc.multiH: " << tl.multiH
			   << ", tl.tilew: "  << tl.tilew     << ", tl.tileh: "  << tl.tileh
			   << ", tc.tilew: "  << tc.tilew     << ", tc.tileh: "  << tc.tileh
			   << ", tl.Count: "  << tl.tileCount << ", tc.count: "  << tc.count
			   << ", obj.count: " << obj.count << ", part.count: " << part.count;
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont, 200);
		gText.setAlpha(255);
		gText.render(gRenderer, 0+screenWidth-gText.getWidth()/4, 0, gText.getWidth()/4, gText.getHeight()/4);

		// Render tile debug
		if (debug){
			if (place_type == 0) {
				// Render Tile debug
				tl.RenderDebug(gRenderer, tile, mouseX, mouseY, mx, my, camx, camy, &rTiles[0], tb.tilesWidth);
			}else if (place_type == 1) {

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
				}
			}else if (place_type == 2) {
				// Render Item in Hand
				// Render mouse coordinates snapped to grid
				obj.gItem.setAlpha(110);
				obj.gItem.render(gRenderer, mouseX, mouseY, 16, 16, &obj.rClips[obj.id]);
				SDL_Rect tempRect = {mouseX, mouseY, 16, 16};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}else if (place_type == 3) {
				// Render Item in Hand
				// Render mouse coordinates snapped to grid
				mon.gMonster.setAlpha(110);
				mon.gMonster.render(gRenderer, mouseX, mouseY, 16, 16, &mon.clip[0]);
				SDL_Rect tempRect = {mouseX, mouseY, 16, 16};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}
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

	for (unsigned int i=0; i<player.spell.size(); i++) {
		// Render Mana background
		SDL_Rect  tempRect = {75, 12 + i * 6, (40*player.spell[i].baseCooldown)/player.spell[i].baseCooldown, 4};
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

		tempRect = {75, 12 + i * 6, (40*player.spell[i].cooldownTimer)/player.spell[i].baseCooldown, 4};
		SDL_SetRenderDrawColor(gRenderer, player.spell[i].color.r, player.spell[i].color.g, player.spell[i].color.b, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);

	}

	if (player.healthDecay > player.health)
			player.healthDecay -= 0.5;
	// Render Mana background
	SDL_Rect  tempRect = {5, 12, (40*player.maxHealth)/player.maxHealth, 4};
	SDL_SetRenderDrawColor(gRenderer, 0, 80, 0, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	// Render health
	tempRect = {5, 5, (40*player.healthDecay)/player.maxHealth, 4};
	SDL_SetRenderDrawColor(gRenderer, 173, 98, 140, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);
	tempRect = {5, 5, (40*player.health)/player.maxHealth, 4};
	SDL_SetRenderDrawColor(gRenderer, 90, 162, 105, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	// Render Mana background
	tempRect = {5, 12, (40*player.maxMana)/player.maxMana, 4};
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 80, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	// Render Mana
	tempRect = {5, 12, (40*player.mana)/player.maxMana, 4};
	SDL_SetRenderDrawColor(gRenderer, 91, 108, 171, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	// Render Player amount of Keys
	tempRect = {0,0,16,16};
	obj.gItem.render(gRenderer, 5, 18, 8, 8, &tempRect);
	std::stringstream tempss;
	tempss <<  "x " << player.keys;
	tempss << " (" << player.collectedKeys;
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
	tempss <<  "x " << player.coins;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	newWidth = gText.getWidth()/4;
	newHeight = gText.getHeight()/4;
	gText.render(gRenderer, 5 + 8/2 + 3 + 2, 26 + 8/2 - newHeight/2,
			newWidth, newHeight);


	tempss.str(std::string());
	tempss <<  "Level: " << player.spell[0].cooldownTimer;
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Global Functions ////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void PlayGame::checkCollisionParticleMonster() {
	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {

			// monster circle collision check with other zombies
			for (int j = 0; j < part.max; j++) {
				if (particles[j].alive) {
					if (particles[j].damage > 0){
						if (particles[j].tag != monster[i].tag){
							float bmx = particles[j].x+particles[j].w/2;
							float bmy = particles[j].y+particles[j].h/2;
							float bmx2 = monster[i].x+monster[i].w/2;
							float bmy2 = monster[i].y+monster[i].h/2;
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

							if (distance < monster[i].w/2 + 2) {
								// reduce monster health
								monster[i].health -= particles[j].damage;
								// apply knockback effect
								knockbackEffect(particles[j].x, particles[j].y, particles[j].w, particles[j].h,
												monster[i].x, monster[i].y, monster[i].w, monster[i].h,
												monster[i].velX, monster[i].velY, randDouble(0.8, 1.2));

								// spawn blood particle effect
								for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
									int rands = rand() % 4 + 2;
									float newX = monster[i].x+monster[i].w/2;
									float newY = monster[i].y+monster[i].h/2;
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
								particles[j].alive = false;
								part.count--;
							}

							/*float bmx = particles[j].x+particles[j].w/2;
							float bmy = particles[j].y+particles[j].h/2;
							float bmx2 = monster[i].x+monster[i].w/2;
							float bmy2 = monster[i].y+monster[i].h/2;
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
							if (distance <= 0.1) {
								distance = 0.1;
							}
							if (distance < monster[i].w/2 + 4) {
								monster[i].x -= 1 * Cos;
								monster[i].y -= 1 * Sin;
							}*/
						}
					}
				}
			}
		}
	}
}

void PlayGame::checkCollisionTileZombie() {
	for (int i = 0; i < mon.max; i++) {
		if (monster[i].alive) {

			// monster circle collision check with other zombies
			for (int j = 0; j < tl.max; j++) {
				if (tile[j].alive && tile[j].collide) {
					float bmx = tile[j].x+tile[j].w/2;
					float bmy = tile[j].y+tile[j].h/2;
					float bmx2 = monster[i].x+monster[i].w/2;
					float bmy2 = monster[i].y+monster[i].h/2;
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
					if (distance <= 0.1) {
						distance = 0.1;
					}
					if (distance < monster[i].w/2 + tile[j].w/2) {
						monster[i].x -= 1 * Cos;
						monster[i].y -= 1 * Sin;
					}
				}
			}
		}
	}
}

void PlayGame::checkCollisionParticlePlayer() {
	int playerX = player.x - 3;
	int playerY = player.y - 9;
	int playerW = 16;
	int playerH = 16;
	// Player
	if (player.alive && !player.shield){
		// Particle
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {

				// Check only if Particle is NOT a Star Particle
				//if (particles[i].type != 2){

				// Check only if Particle is NOT a Star Particle or a Grenade Particle
				if (particles[i].damage > 0){
					// Check only if Particle is NOT the Enemy's Particle
					if (particles[i].tag != player.tag){

						// particle collision with player using a CIRCLE
						float bmx = playerX + playerW/2;
						float bmy = playerY + playerH/2;
						float bmx2 = particles[i].x + particles[i].w/2;
						float bmy2 = particles[i].y + particles[i].h/2;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						// collision occurred
						if (distance < playerW/2 + particles[i].w/2) {
							// spawn blood particle effect
							// spawn blood particle effect
							for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
								int rands = rand() % 4 + 2;
								float newX = playerX+playerW/2;
								float newY = playerY+playerH/2;
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
							// reduce player health
							//player.health -= particles[i].damage;
							// remove particle
							particles[i].alive = false;
							part.count--;
						}

						// particle collision with player using a BOX
						/*if (particles[i].x + particles[i].w > player.x && particles[i].x < player.x + player.w
								&& particles[i].y + particles[i].h > player.y
								&& particles[i].y < player.y + player.h) {
							particles[i].time = 0;
							particles[i].alive = false;
							part.count--;

							player.health -= particles[i].damage;
						}*/
					}
				}
			//	}
			}
		}	// end Particle
	}	// end Player
}

void PlayGame::checkCollisionGrenadePlayer() {
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				// Grenade particle
				if (particles[i].type == 3) {

					// Particle spin
					particles[i].angle += particles[i].angleSpe * particles[i].angleDir;

					// Particle death
					particles[i].time += particles[i].deathTimerSpeed;

					// Grenade has a smoke Particle effect coming out of it
					particles[i].timeri++;
					/* Making this '30' frames will enable the Player to
					 * watch 3 visual 'ticks' (particle sparks coming from the grenade
					 * that will explode momentarily after the 3 tick)
					 */
					if (particles[i].timeri > 30) {
						particles[i].timeri = 0;
						for (double h=0.0; h< 360.0; h+=30){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							part.spawnParticleAngle(particles, "none", 2,
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
						Mix_PlayChannel(-1, sSpellExplode, 0);

						// spawn explosion particle effect
						for (double h=0.0; h< 360.0; h+=rand() % 10 + 2){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							/*int r = rand() % 255;
							int g = rand() % 255;
							int b = rand() % 255;
							SDL_Color color = {r, g, b};*/
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
						particles[i].alive = false;
						part.count--;
					}
				}
			}
		}
}

void PlayGame::checkCollisionItemPlayer() {
	int playerX = player.x - 3;
	int playerY = player.y - 9;
	int playerW = 16;
	int playerH = 16;
	for (int i = 0; i < obj.max; i++) {
		if (item[i].alive) {
			// Collision with Keys
			if (item[i].id == 0) {
				if (checkCollision(playerX, playerY, playerW, playerH, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player keys
					player.keys++;
					// also keep track of how many keys we have picked up iin this Stage Level
					player.collectedKeys++;
					// ply sound effect
					Mix_PlayChannel(-1, sKeyPickup, 0);
					// remove item
					item[i].alive = false;
					obj.count--;
				}
			}
			// Collision with Small Yellow coin
			if (item[i].id == 7) {
				if (checkCollision(playerX, playerY, playerW, playerH, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player.coins++;
					// ply sound effect
					Mix_PlayChannel(-1, sKeyPickup, 0);
					// remove item
					item[i].alive = false;
					obj.count--;
				}
			}
			// Collision with Large Yellow coin
			if (item[i].id == 8) {
				if (checkCollision(playerX, playerY, playerW, playerH, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player.coins += 10;
					// ply sound effect
					Mix_PlayChannel(-1, sKeyPickup, 0);
					// remove item
					item[i].alive = false;
					obj.count--;
				}
			}
			// Collision with Large Diamond
			if (item[i].id == 10) {
				if (checkCollision(playerX, playerY, playerW, playerH, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player.coins += 100;
					// ply sound effect
					Mix_PlayChannel(-1, sKeyPickup, 0);
					// remove item
					item[i].alive = false;
					obj.count--;
				}
			}
			// Collision with Mana Pot
			if (item[i].id == 25) {
				if (checkCollision(playerX, playerY, playerW, playerH, item[i].x, item[i].y, item[i].w, item[i].h-8) ) {
					// increase player coins
					player.mana += 100;
					// ply sound effect
					Mix_PlayChannel(-1, sKeyPickup, 0);
					// remove item
					item[i].alive = false;
					obj.count--;
				}
			}
		}
	}
}

void PlayGame::checkCollisionTilePlayer() {
	int playerX = player.x - 3;
	int playerY = player.y - 9;
	int playerW = 16;
	int playerH = 16;
	// Move Player
	if (!player.moveDelay) {
		// Y axis
		if (player.moveUp) {
			// If Player holds control, they will not move
			if (!player.ctrl) {
				player.vY = -1;
				player.moving = true;
			}
			// If player holds shift, they will stay in current direction
			if (!player.shift) {

			}
		}
		else if (player.moveDown) {
			if (!player.ctrl) {
				player.vY = 1;
				player.moving = true;
			}
		}else {
			player.vY = 0;
		}
		// X axis
		if (player.moveLeft) {
			if (!player.ctrl) {
				player.vX = -1;
				player.moving = true;
			}
		}
		else if (player.moveRight) {
			if (!player.ctrl) {
				player.vX = 1;
				player.moving = true;
			}
		}else{
			player.vX = 0;
		}

		if (player.moveUp) {
			if (!player.shift) {
				player.angle = 270.0;
				player.facing = 1;
			}
		}
		if (player.moveDown) {
			if (!player.shift) {
				player.angle = 90.0;
				player.facing = 0;
			}
		}
		if (player.moveLeft) {
			if (!player.shift) {
				player.angle = 180.0;
				player.facing = 3;
			}
		}
		if (player.moveRight) {
			if (!player.shift) {
				player.angle = 0.0;
				player.facing = 2;
			}
		}
		if (player.moveUp && player.moveLeft) {
			if (!player.shift) {
				player.angle = 225.0;
			}
		}
		if (player.moveUp && player.moveRight) {
			if (!player.shift) {
				player.angle = 315.0;
			}
		}
		if (player.moveDown && player.moveLeft) {
			if (!player.shift) {
				player.angle = 135.0;
			}
		}
		if (player.moveDown && player.moveRight) {
			if (!player.shift) {
				player.angle = 45.0;
			}
		}


		// Update collision with Tiles
		//tl.checkCollision(tile, x, y, w, h, y, vY);

		// Update collision with Tiles
		// Player Velocity X Axis
		player.x += player.vX;
		SDL_Rect rectA;
		rectA.x = player.x;
		rectA.y = player.y;
		rectA.w = player.w;
		rectA.h = player.h;
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
					if  ( tl.checkCollisionRect( rectA, rectB )) {
						// If Player has more than 0 keys, then unlock door, otherwise continue collision check
						if (player.keys > 0) {
							if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
							    tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
								player.keys--;
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
			player.x -= player.vX;
		}

		// Player Velocity Y Axis
		player.y += player.vY;
		rectA.x = player.x;
		rectA.y = player.y;
		rectA.w = player.w;
		rectA.h = player.h;
		moveBack = false;
		for (int i = 0; i < tl.max; i++) {
			if (tile[i].alive){
				if (tile[i].collide) {
					SDL_Rect rectB;
					rectB.x = tile[i].x;
					rectB.y = tile[i].y;
					rectB.w = tile[i].w;
					rectB.h = tile[i].h;
					if  ( tl.checkCollisionRect( rectA, rectB )) {
						// If Player has more than 0 keys, then unlock door, otherwise continue collision check
						if (player.keys > 0) {
							if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
							    tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
								player.keys--;
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
			player.y -= player.vY;
		}
	}	// end check if Player is not delayed of movement
}

void PlayGame::checkCollisionTilecPlayer() {
}

void PlayGame::checkCollisionParticleTile() {
	for (int i = 0; i < part.max; i++) {
		if (particles[i].alive && particles[i].damage > 0) {
			for (int j = 0; j < tl.max; j++) {
				if (tile[j].alive && tile[j].collide) {
					// particle collision with monster using a CIRCLE
					/*float bmx = tile[j].x + tile[j].w/2;
					float bmy = tile[j].y + tile[j].h/2;
					float bmx2 = particles[i].x + particles[i].w/2;
					float bmy2 = particles[i].y + particles[i].h/2;
					float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
										  (bmy - bmy2) * (bmy - bmy2));
					if (distance < 32) {
						particles[i].alive = false;
						part.count--;
					}*/
					if (checkCollision(particles[i].x, particles[i].y, particles[i].w, particles[i].h, tile[j].x, tile[j].y, 16, 16) ) {
						Mix_PlayChannel(-1, sSpellExplode, 0);
						part.SpawnExplosion(particles, particles[i].x2, particles[i].y2, particles[i].color);
						particles[i].alive = false;
						part.count--;
					}
				}
			}
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

void PlayGame::UpdatePlayer() {
	// Player died, reset and reload Level
	if (player.health <= 0) {
		ResetLevel();
	}
}
void PlayGame::ClampObjectsToLevelSize() {

	// Collision with door
	if (player.collectedKeys == tl.requiredKeys) {
		if (checkCollision(player.x, player.y, player.w, player.h, doorX, doorY, 16, 16)) {
			// Increase Stage level
			playerStateLevel++;
			// Change to next Stage Level
			LoadLevel(playerStateLevel);
		}
	}
	// Player
	if (player.x+player.w < 0) {
		player.x = 0;
	}
	if (player.x > 0+tl.levelWidth) {
		player.x =0+tl.levelWidth-player.w;
	}
	if (player.y+player.h < 0) {
		player.y = 0;
	}
	if (player.y > 0+tl.levelHeight) {
		player.y = 0+tl.levelHeight-player.h;
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
		player.x		= spawnX + player.w/2;
		player.y		= spawnY + player.h/2;
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
			player.x		= spawnX + player.w/2;
			player.y		= spawnY + player.h/2;
		}
		newSpawnFile.close();
	}
	fileSpawn.close();
}

void PlayGame::ClearLevel() {
	tl.RemoveAll(tile);
	tc.RemoveAll(tilec);
	obj.RemoveAll(item);
	part.RemoveAll(particles);
	mon.RemoveAll(monster);
}

void PlayGame::LoadLevel(int level) {
	// Reset Player movements and direction
	player.facing = 0;
	player.keys = 0;
	player.collectedKeys = 0;
	player.mana = player.maxMana;
	player.health = 225;
	player.moveLeft = false;
	player.moveRight = false;
	player.moveUp = false;
	player.moveDown = false;
	// Remove Particles
	part.RemoveAll(particles);
	// Load Tiles
	tl.LoadData(tile, level);
	// Load Collision Tiles
	tc.loadTiles(tilec, level);
	// Load Items (Items) Tiles
	obj.LoadData(item, level);
	// Load Monsters
	mon.LoadData(monster, level);
	// Load level spawn point
	loadSpawnPoint(level);
}

void PlayGame::ResetLevel() {
	player.facing = 0;
	player.keys = 0;
	player.collectedKeys = 0;
	player.mana = player.maxMana;
	player.health = 225;
	player.moveLeft = false;
	player.moveRight = false;
	player.moveUp = false;
	player.moveDown = false;
	LoadLevel(playerStateLevel);
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
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = true;
			if (place_type == 3) {
				mon.Spawn(monster, mouseX+camx, mouseY+camy,
								  16, 16, 16, 16,
								  0.0, 1.0,
								  0, tl.layer, 100,
								  0, 0, 0);
			}
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
void PlayGame::editorOnKeyDown( SDL_Keycode sym )
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
	case SDLK_x:				// Save spawn point
		spawnX = mouseX+camx;
		spawnY = mouseY+camy;
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
				tc.layer--;
			}
		}else{
			if (place_type==0) {
				tl.layer++;
			}else if (place_type==1) {
				tc.layer++;
			}
		}
		break;
	case SDLK_i:								// Change place_type Type
		if (shift) {
			if (place_type==0) {
				tl.id--;
			}else if (place_type==1) {
				tc.type--;
			}else if (place_type==2) {
				obj.id--;
			}
		}else{
			if (place_type==0) {
				tl.id++;
			}else if (place_type==1) {
				tc.type++;
			}else if (place_type==2) {
				obj.id++;
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
				tc.copy(tilec);
			}
		}
		break;
	case SDLK_SPACE:							// Tile, remove all Tiles (and Collision Tiles)
		if (shift && editor) {
			ClearLevel();

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
				if (tc.tilew > 1) {
					tc.tilew -= 1;
				}
			}
			else{
				if (tc.multiW > 1) {
					tc.multiW -= 1;
				}
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
				tc.tilew += 1;
			}
			else{
				tc.multiW += 1;
			}
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
				if (tc.tileh > 1) {
					tc.tileh -= 1;
				}
			}
			else{
				if (tc.multiH > 1) {
					tc.multiH -= 1;
				}
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
				tc.tileh += 1;
			}
			else{
				tc.multiH += 1;
			}
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







