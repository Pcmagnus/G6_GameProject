#pragma once
#include <raylib.h>
#include <string>

class Player
{
public:

    // Constructors
    Player();
    Player(Color inPlayerColor, int inPlayerWidth, int inPlayerHeight, int inPlayerRadius);



    // Functions
    void SpawnPlayer(int inXPosition, int inYPosition);
    void RenderPlayer();
    virtual void ControllPlayer();

    float PlayerPositionX{ 100.f };
    float PlayerPositionY{ 100.f };
    float PlayerSpeed{ 500.f };

    Color PlayerColor{ BLUE };

    int PlayerWidth{ 40 };
    int PlayerHeight{ 40 };
    int PlayerRadius{ 40 };
};