#include <raylib.h>
#include <iostream>
#include "Player/Player.h"
#include <vector>

using namespace std;
//Bullet Variables
struct Bullet
{
    float x, y; // Position of the bullet
    float speed = 800.f; // Speed of the bullet
    bool active = true; // For deleting bullets
};
vector<Bullet> bullets; // Store all bullets in a vector
float shootCooldown = 0.15f; // Time between shots in seconds
float shootTimer = 0.f;       // Timer to track shooting

//Enemy Variables
vector<float> enemiesXPos = { 1200 };
vector<float> enemiesYPos = { 500 };
vector<int> enemiesHP = { 3 };

float spawnTime = 0.5f;   // How much time it takes for a new enemy to spawn
float enemyTimer = 0;
float enemySpeed = 400;

//Background Variables
struct Star {
    float x, y, speed;
};
static vector<Star> stars;

//Init stars for background
void InitBackground()
{
    stars.clear();
    for (int i = 0; i < 100; i++) {
        Star s;
        s.x = GetRandomValue(0, 1280);
        s.y = GetRandomValue(0, 1024);
        s.speed = GetRandomValue(50, 200); //different speeds for parallax effect
        stars.push_back(s);
    }
}

//Render stars and move them left
void RenderBackground()
{
    ClearBackground(BLACK);

    for (auto& s : stars) {
        DrawPixel(s.x, s.y, WHITE);
        s.x -= s.speed * GetFrameTime();

        //respawn star when off screen
        if (s.x < 0) {
            s.x = 1280;
            s.y = GetRandomValue(0, 1024);
            s.speed = GetRandomValue(50, 200);

        }
    }
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
        enemiesXPos.push_back(1280 + 50);
        enemiesYPos.push_back(GetRandomValue(20 + 50, 1024 - 20 - 50)); // randomly spawned enemy on y axis
        enemiesHP.push_back(GetRandomValue(1, 5)); // new enemy HP
    }
    enemyTimer += GetFrameTime();
}

void CheckCollision(Player& defaultPlayer)
{
    for (int i = 0; i < enemiesXPos.size(); i++)
    {
        float dx = defaultPlayer.PlayerPositionX - enemiesXPos[i];
        float dy = defaultPlayer.PlayerPositionY - enemiesYPos[i];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < defaultPlayer.PlayerRadius + 50) // 50 = enemy radius
        {
            defaultPlayer.PlayerHealth -= 10; // decrease health
            enemiesXPos.erase(enemiesXPos.begin() + i); // remove enemy
            enemiesYPos.erase(enemiesYPos.begin() + i);
            enemiesHP.erase(enemiesHP.begin() + i); // remove corresponding HP
            i--; // adjust index after erase
        }
    }
}

//Screen Modes

void TitleScreen(int& ScreenMode)
{
    ClearBackground(RED);

    for (int i = 0; i < 100; i++)
    {
        DrawText("High Score: ", 150 - 2 * i, 200 - 2 * i, 40, BLACK);
        DrawText("Untitled Shooter Game", 150 - 2 * i, 100 - 2 * i, 80, BLACK);
    }


    DrawText("High Score: ", 150, 200, 40, WHITE);
    DrawText("Untitled Shooter Game", 150, 100, 80, WHITE);

    // Make Start Button
    Vector2 ButtonPosition = { 300, 400 };
    Vector2 ButtonSize = { 620, 200 };

    Rectangle startButton = { ButtonPosition.x, ButtonPosition.y, ButtonSize.x, ButtonSize.y };
    DrawRectangle(ButtonPosition.x, ButtonPosition.y, ButtonSize.x, ButtonSize.y, WHITE);
    DrawText("Start", ButtonPosition.x + 2, ButtonPosition.y, 220, BLACK);

    Vector2 mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, startButton))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // one click only
        {
            ScreenMode = 1; // start the game
        }
    }

    DrawText("CREDITS: Caleb Chilson, Magnus Helvig, Maksim Popovski, & Marcus Jakobsen", 50, 900, 30, WHITE);
}

void GameScreen(Player& defaultPlayer, int& ScreenMode)
{
    RenderBackground();   // Background effect
    defaultPlayer.RenderPlayer();
    defaultPlayer.ControllPlayer();

    shootTimer += GetFrameTime(); // Update shoot timer

    // Shoot bullet
    if (IsKeyDown(KEY_SPACE) && shootTimer >= shootCooldown)
    {
        Bullet newBullet;
        newBullet.x = defaultPlayer.PlayerPositionX + defaultPlayer.PlayerRadius;
        newBullet.y = defaultPlayer.PlayerPositionY - 2;
        newBullet.speed = 800.f;
        bullets.push_back(newBullet);

        shootTimer = 0.f; // Timer reset
    }

    RenderEnemy();
    CheckCollision(defaultPlayer);

    // Update and draw bullets
    for (int i = 0; i < bullets.size(); i++)
    {
        bullets[i].x += bullets[i].speed * GetFrameTime(); // move bullet
        DrawRectangle(bullets[i].x, bullets[i].y - 2, 15, 4, WHITE); // draw bullet

        // remove if off-screen
        if (bullets[i].x > 1280) bullets[i].active = false;
    }

    // Remove inactive bullets
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
        [](Bullet& b) { return !b.active; }), bullets.end());

    // Check bullet collisions with enemies
    for (int i = 0; i < bullets.size(); i++)
    {
        for (int j = 0; j < enemiesXPos.size(); j++)
        {
            float dx = bullets[i].x - enemiesXPos[j];
            float dy = bullets[i].y - enemiesYPos[j];
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < 50 + 5) // enemy radius + bullet radius
            {
                bullets[i].active = false;
                enemiesHP[j] -= 1; // decrease HP
                if (enemiesHP[j] <= 0) // remove enemy if HP <= 0
                {
                    enemiesXPos.erase(enemiesXPos.begin() + j);
                    enemiesYPos.erase(enemiesYPos.begin() + j);
                    enemiesHP.erase(enemiesHP.begin() + j);
                    j--; // adjust index after erase
                }
            }
        }
    }

    // HP-bar animation
    float lerpSpeed = 8.0f * GetFrameTime(); // animatiom speed for HP bar
    defaultPlayer.PlayerHealthVisual += (defaultPlayer.PlayerHealth - defaultPlayer.PlayerHealthVisual) * lerpSpeed;
    // Smooth red HP (damage overlay)
    float damageLerpSpeed = 3.0f * GetFrameTime();
    defaultPlayer.PlayerHealthDamage += (defaultPlayer.PlayerHealthVisual - defaultPlayer.PlayerHealthDamage) * damageLerpSpeed;


    //HP bar in top-left corner
    float hpBarWidth = 350;        // horizontal width
    float hpBarHeight = 25;        // vertical height
    float xPos = 50; // left margin
    float yPos = 1024 - 25 - hpBarHeight; // top margin

    // Draw background
    DrawRectangle(xPos, yPos, hpBarWidth, hpBarHeight, DARKGRAY);
    // Draw red damage overlay (slightly lagging behind white)
    DrawRectangle(xPos, yPos, hpBarWidth * (defaultPlayer.PlayerHealthDamage / 100.0f), hpBarHeight, RED);
    // Draw white current HP on top
    DrawRectangle(xPos, yPos, hpBarWidth * (defaultPlayer.PlayerHealthVisual / 100.0f), hpBarHeight, WHITE);
    // Draw border
    DrawRectangleLines(xPos, yPos, hpBarWidth, hpBarHeight, LIGHTGRAY);


    // Check if player died
    if (defaultPlayer.PlayerHealth <= 0)
    {
        ScreenMode = 2;   // go to DeathScreen
    }
}

void DeathScreen(Player& defaultPlayer, int& ScreenMode)
{
    ClearBackground(RED);
    for (int i = 0; i < 180; i++)
    {
        DrawText("DESTROYED", 100 - 2 * i, 100 - 2 * i, 180, BLACK);

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
            defaultPlayer.PlayerHealth = 100;          // reset health
            defaultPlayer.SpawnPlayer(100, 400);       // reset position
            enemiesXPos.clear();                        // clear enemies
            enemiesYPos.clear();
            ScreenMode = 1;                             // go back to game
            cout << "Restart Game!!";
            //screenMode = 1;
        }
    }

    DrawText("CREDITS: Caleb Chilson, Magnus Helvig, Maksim Popovski, & Marcus Jakobsen", 50, 900, 30, WHITE);
}

int main()
{
    srand(time(0)); // Get new random numbers

    //Variables
    int ScreenMode = 0;

    float windowWidth = 1280;
    float windowHeight = 1024;

    InitWindow(windowWidth, windowHeight, "Untitled Shooter Game");

    SetTargetFPS(60);

    //Init background
    InitBackground();

    Player defaultPlayer;

    defaultPlayer.SpawnPlayer(100, 400);

    while (!WindowShouldClose())
    {
        //Initialize
        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(100, 100);

        //Gameplay
        switch (ScreenMode)
        {
        case 0:
            TitleScreen(ScreenMode);
            break;
        case 1:
            GameScreen(defaultPlayer, ScreenMode);
            break;
        case 2:
            DeathScreen(defaultPlayer, ScreenMode);
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



