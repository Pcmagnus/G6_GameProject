#include <raylib.h>
#include <iostream>
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
	srand(time(0)); // Get new random numbers

	//Variables
	int ScreenMode = 1;

	float windowWidth = 1280;
	float windowHeight = 1024;

	InitWindow(windowWidth, windowHeight, "Untitled Shooter Game");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		//Initialize
		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(100, 100);
		
		RenderEnemy;



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