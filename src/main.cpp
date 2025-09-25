#include <raylib.h>
#include <iostream>
#include "Player/PLayer.h"
#include <vector>

using namespace std;

vector<float> enemiesXPos = { 1200 };
vector<float> enemiesYPos = { 500 };

float spawnTime = 0.5; //How much time it takes for a new enemy to spawn
float enemyTimer = 0;
float enemySpeed = 400;

void RenderBackground()
{
	
}

void RenderPlayer()
{
	
}

void RenderEnemy()
{
	for (int i = 0; i < enemiesXPos.size(); i++) // Render the Enemies, then move them left
	{
		DrawCircle(enemiesXPos[i], enemiesYPos[i], 50, RED);
		enemiesXPos[i] -= enemySpeed * GetFrameTime();
	}

	if (enemyTimer > spawnTime) // if 1 second has passed spawn a new enemy
	{
		enemyTimer = 0;
		enemiesXPos.push_back(1200);
		enemiesYPos.push_back(rand() % 901 + 50); //randomly spawned enemy on y row
	}
	enemyTimer += GetFrameTime();
}

void CheckCollision()
{
	for (int i = 0; i < enemiesXPos.size(); i++)
	{

	}
}


//Screen Modes

void TitleScreen()
{
	ClearBackground(RED);
	for (int i = 0; i < 100; i++)
	{
		DrawText("High Score: ", 150 - 2 * i, 200 - 2 * i, 40, BLACK);
		DrawText("Untitled Shooter Game", 150 - 2*i, 100 - 2*i, 80,BLACK);
	}
	DrawText("High Score: ", 150, 200, 40, WHITE);
	DrawText("Untitled Shooter Game", 150, 100, 80, WHITE);

	//Make Start Button

	Vector2 ButtonPosition = {300,400};
	Vector2 ButtonSize = {620,200};

	Rectangle startButton = {ButtonPosition.x,ButtonPosition.y,ButtonSize.x,ButtonSize.y};
	DrawRectangle(ButtonPosition.x,ButtonPosition.y,ButtonSize.x,ButtonSize.y,WHITE);
	Vector2 mousePoint = GetMousePosition();

	DrawText("Start", ButtonPosition.x + 2,ButtonPosition.y, 220, BLACK);

	if (CheckCollisionPointRec(mousePoint, startButton))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			cout << "Start Game!!";
			//screenMode = 1;
		}
	}

	DrawText("CREDITS: Caleb Chilson, Magnus Helvig, Maksim Popovski, & Marcus Jakobsen", 50, 900, 30, WHITE);
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
	ClearBackground(RED);
	for (int i = 0; i < 180; i++)
	{
		DrawText("DESTROYED", 100 - 2*i, 100 - 2*i, 180, BLACK);

		DrawText("Final Score: ", 100 - i, 300 - i, 80, BLACK);

		DrawText("High Score: ", 100 - i, 400 - i, 80, BLACK);
	}
	DrawText("DESTROYED", 100, 100, 180, WHITE);

	DrawText("Final Score: ", 100, 300, 80, WHITE);

	DrawText("High Score: ", 100, 400, 80, WHITE);

	//Make Start Button

	Vector2 ButtonPosition = { 300,600 };
	Vector2 ButtonSize = { 620,110 };

	Rectangle startButton = { ButtonPosition.x,ButtonPosition.y,ButtonSize.x,ButtonSize.y };
	DrawRectangle(ButtonPosition.x, ButtonPosition.y, ButtonSize.x, ButtonSize.y, WHITE);
	Vector2 mousePoint = GetMousePosition();

	DrawText("RESTART", ButtonPosition.x + 2, ButtonPosition.y, 124, BLACK);

	if (CheckCollisionPointRec(mousePoint, startButton))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			cout << "Restart Game!!";
			//screenMode = 1;
		}
	}

	DrawText("CREDITS: Caleb Chilson, Magnus Helvig, Maksim Popovski, & Marcus Jakobsen", 50,900,30,WHITE);
}







int main()
{
	srand(time(0)); // Get new random numbers

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