#include "Enemies.h"
#include <cstdlib>

// Constructor
Enemy::Enemy(float startX, float startY, EnemyType t)
    : x(startX), y(startY), type(t), initialY(startY)
{
    switch (type)
    {
    case EnemyType::SlowStraight: speed = 400.f; hp = 3; scoreReward = 15; break; // original speed
    case EnemyType::FastStraight: speed = 500.f; hp = 2; scoreReward = 30; break; // faster
    case EnemyType::Wave:        speed = 400.f; hp = 4; scoreReward = 60;
        waveAmplitude = 150.f; // bigger amplitude
        waveFrequency = 2.f;
        break;
    }
}

// Update enemy position
void Enemy::Update(float deltaTime)
{
    x -= speed * deltaTime; // Move left

    // Wave movement for Wave type
    if (type == EnemyType::Wave)
    {
        y = initialY + waveAmplitude * sinf(x / 1280 * waveFrequency * 2 * 3.14159f);
    }
}

// Render enemy
void Enemy::Render()
{
    DrawCircle(x, y, 50, RED);
}

// Update all enemies and spawn new ones
void EnemyManager::Update(float deltaTime)
{
    enemyTimer += deltaTime; // total elapsed time

    // Update existing enemies
    for (int i = 0; i < enemies.size(); i++)
    {
        enemies[i].Update(deltaTime);

        // Remove enemies that went off screen
        if (enemies[i].x < -50)
        {
            enemies.erase(enemies.begin() + i);
            i--;
        }
    }

    // Spawn enemies according to elapsed time using intervals
    static float spawnTimerSlow = 0.f;
    static float spawnTimerFast = 0.f;
    static float spawnTimerWave = 0.f;

    spawnTimerSlow += deltaTime;
    spawnTimerFast += deltaTime;
    spawnTimerWave += deltaTime;

    // SlowStraight: spawn every 0.5s
    if (spawnTimerSlow >= 0.5f)
    {
        spawnTimerSlow = 0.f;
        float yPos = 50 + rand() % (1024 - 100);
        enemies.push_back(Enemy(1280 + 50, yPos, EnemyType::SlowStraight));
    }

    // FastStraight: spawn every 0.7s after 30s
    if (enemyTimer >= 30.f && spawnTimerFast >= 0.7f)
    {
        spawnTimerFast = 0.f;
        float yPos = 50 + rand() % (1024 - 100);
        enemies.push_back(Enemy(1280 + 50, yPos, EnemyType::FastStraight));
    }

    // Wave: spawn every 1.0s after 50s
    if (enemyTimer >= 50.f && spawnTimerWave >= 1.0f)
    {
        spawnTimerWave = 0.f;
        float yPos = 50 + rand() % (1024 - 100);
        enemies.push_back(Enemy(1280 + 50, yPos, EnemyType::Wave));
    }
}

// Render all enemies
void EnemyManager::Render()
{
    for (auto& e : enemies)
        e.Render();
}

// Clear all enemies
void EnemyManager::Clear()
{
    enemies.clear();
    enemyTimer = 0.f;
}
