#pragma once
#include <vector>
#include <raylib.h>
#include <cmath>

// Enemy types enumeration
enum class EnemyType { SlowStraight, FastStraight, Wave };

class Enemy
{
public:
    float x, y;            // Current position
    float speed;            // Horizontal speed
    int hp;                 // Hit points
    int scoreReward;        // Points awarded on kill
    EnemyType type;         // Enemy type

    // Wave movement parameters
    float waveAmplitude = 50.f;
    float waveFrequency = 2.f;
    float initialY;         // Original Y position for wave calculation

    Enemy(float startX, float startY, EnemyType t);

    void Update(float deltaTime);  // Update enemy position
    void Render();                 // Draw enemy
};

// Enemy manager class to handle multiple enemies
class EnemyManager
{
public:
    std::vector<Enemy> enemies;  // All active enemies
    float enemyTimer = 0.f;      // Total elapsed time for spawn conditions

    void Update(float deltaTime); // Update all enemies and spawn new ones
    void Render();                // Render all enemies
    void Clear();                 // Remove all enemies
};
