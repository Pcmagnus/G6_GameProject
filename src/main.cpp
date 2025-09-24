#include <raylib.h>
#include <iostream>
#include "Player/PLayer.h"

using namespace std;

void RenderBackground()
{
	
}

void RenderPlayer()
{
	
}

void RenderEnemy()
{
	
}

void CheckCollision()
{
	
}

//Screen Modes

void TitleScreen()
{
	
}

void GameScreen()
{
	RenderBackground();
	RenderPlayer();
	RenderEnemy();
	CheckCollision();
}

void DeathScreen()
{
	
}


int main()
{
	//Variables
	int ScreenMode = 1;

	float windowWidth = 1280;
	float windowHeight = 1024;

	InitWindow(windowWidth, windowHeight, "Untitled Shooter Game");

	SetTargetFPS(60);

	Player defaultPlayer;

	defaultPlayer.SpawnPlayer(100, 400);

	while (!WindowShouldClose())
	{
		//Initialize
		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(100, 100);

		defaultPlayer.RenderPlayer();
		defaultPlayer.ControllPlayer();

		//Gameplay
		switch (ScreenMode)
		{
		case 0:
			TitleScreen();
			break;
		case 1:
			GameScreen();
			break;
		case 2:
			DeathScreen();
			break;
		default:
			cout << "Error, went past case 3 in Screens";
			break;
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}