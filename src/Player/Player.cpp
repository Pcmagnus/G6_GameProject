#include "Player.h"

Player::Player()
{
}

Player::Player(Color inPlayerColor, int inPlayerWidth, int inPlayerHeight, int inPlayerRadius)
{
    PlayerColor = inPlayerColor;
    PlayerWidth = inPlayerWidth;
    PlayerHeight = inPlayerHeight;
    PlayerRadius = inPlayerRadius;
}


void Player::SpawnPlayer(int inXPosition, int inYPosition)
{
    PlayerPositionX = inXPosition;
    PlayerPositionY = inYPosition;
}


void Player::RenderPlayer()
{
    DrawCircle(PlayerPositionX, PlayerPositionY, PlayerRadius, PlayerColor);
}

void Player::ControllPlayer()
{
    if (IsKeyDown(KEY_W))
    {
        PlayerPositionY -= PlayerSpeed * GetFrameTime() * 0.7;
    }
    if (IsKeyDown(KEY_S))
    {
        PlayerPositionY += PlayerSpeed * GetFrameTime() * 1.5;
    }
    if (IsKeyDown(KEY_A))
    {
        PlayerPositionX -= PlayerSpeed * GetFrameTime() * 0.7;
    }
    if (IsKeyDown(KEY_D))
    {
        PlayerPositionX += PlayerSpeed * GetFrameTime() * 1.3;
    }


    //Boundaries
    if (PlayerPositionX < PlayerRadius) {
        PlayerPositionX = PlayerRadius; // Clamp to the left edge
    }
    else if (PlayerPositionX + PlayerWidth > 1280) {
        PlayerPositionX = 1280 - PlayerWidth; // Clamp to the right edge
    }

    // Vertical boundary checks
    if (PlayerPositionY < PlayerRadius) {
        PlayerPositionY = PlayerRadius; // Clamp to the top edge
    }
    else if (PlayerPositionY + PlayerHeight > 1024) {
        PlayerPositionY = 1024 - PlayerHeight; // Clamp to the bottom edge
    }

}


