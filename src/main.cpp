#include <raylib.h>
#include <iostream>
#include "Player/Player.h"
#include "Enemies/Enemies.h"
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
float shootCooldown = 0.25f; // Time between shots in seconds
float shootTimer = 0.f;       // Timer to track shooting

//Enemy Variables
EnemyManager enemyManager; // making an instance of the enemy manager

// Score Variables
float score = 0.0f;           // current score
int finalScore = 0;           // score from the last finished run
int highScore = 0;            // best score across all runs
float survivalPointsRate = 10.0f; // points per second survived
int enemyKillPoints = 25;    // points per enemy killed


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

}



void CheckCollision(Player& defaultPlayer)
{
    for (int i = 0; i < enemyManager.enemies.size(); i++)
    {
        float dx = defaultPlayer.PlayerPositionX - enemyManager.enemies[i].x;
        float dy = defaultPlayer.PlayerPositionY - enemyManager.enemies[i].y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < defaultPlayer.PlayerRadius + 50) // enemy radius
        {
            defaultPlayer.PlayerHealth -= 20;           // decrease health
            enemyManager.enemies.erase(enemyManager.enemies.begin() + i); // remove enemy
            i--;                                       // adjust index
        }
    }
}

//Screen Modes

void TitleScreen(int& ScreenMode)
{
    ClearBackground(RED);

    for (int i = 0; i < 100; i++)
    {
        DrawText(TextFormat("High Score: %i", highScore), 150 - 2 * i, 200 - 2 * i, 40, BLACK);
        DrawText("Galactic Transit", 150 - 2 * i, 100 - 2 * i, 80, BLACK);
    }

    DrawText(TextFormat("High Score: %i", highScore), 150, 200, 40, WHITE);
    DrawText("Galactic Transit", 150, 100, 80, WHITE);

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
            score = 0.0f; // reset score when starting a new game
			enemyManager.Clear(); // clear any enemies from previous run
            bullets.clear();
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
    enemyManager.Update(GetFrameTime());
    enemyManager.Render();

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
        for (int j = 0; j < enemyManager.enemies.size(); j++)
        {
            float dx = bullets[i].x - enemyManager.enemies[j].x;
            float dy = bullets[i].y - enemyManager.enemies[j].y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < 50 + 5)
            {
                bullets[i].active = false;
                enemyManager.enemies[j].hp -= 1;
                if (enemyManager.enemies[j].hp <= 0)
                {
                    score += enemyManager.enemies[j].scoreReward;
                    enemyManager.enemies.erase(enemyManager.enemies.begin() + j);
                    j--;
                }
            }
        }
    }

    // Score from survival time
    score += survivalPointsRate * GetFrameTime();
    DrawText(TextFormat("Score: %i", (int)score), 1000, 50, 40, WHITE);


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
        finalScore = (int)score; // save final score
        if (finalScore > highScore) highScore = finalScore; // update high score
        ScreenMode = 2;   // go to DeathScreen
    }
}

void DeathScreen(Player& defaultPlayer, int& ScreenMode)
{
    ClearBackground(RED);
    for (int i = 0; i < 180; i++)
    {
        DrawText("DESTROYED", 100 - 2 * i, 100 - 2 * i, 180, BLACK);

        DrawText(TextFormat("Final Score: %i", finalScore), 100 - i, 300 - i, 80, BLACK);

        DrawText(TextFormat("High Score: %i", highScore), 100 - i, 400 - i, 80, BLACK);
    }
    DrawText("DESTROYED", 100, 100, 180, WHITE);

    DrawText(TextFormat("Final Score: %i", finalScore), 100, 300, 80, WHITE);

    DrawText(TextFormat("High Score: %i", highScore), 100, 400, 80, WHITE);

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
            score = 0.0f;                          // reset current score
            defaultPlayer.PlayerHealth = 100;     // reset health
            defaultPlayer.SpawnPlayer(100, 400);  // reset position
			enemyManager.Clear();                 // clear enemies from previous run
            bullets.clear();                      // clear bullets from previous run
            ScreenMode = 1;                       // go back to game
            cout << "Restart Game!!";
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

    InitWindow(windowWidth, windowHeight, "Galactic Transit");

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



