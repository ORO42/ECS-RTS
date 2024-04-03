#pragma once

#include "Entity.h"
#include "EntityManager.h"

class Game
{
    EntityManager m_entities; // vector of entities to maintain
    int m_currentFrame = 0;
    bool m_paused = false;
    bool m_running = true;
    bool m_isSelecting = false;
    Vector2 m_selectionStart = {0.0, 0.0};
    int m_tileSize = 32;

    void init();
    void setPaused(bool paused);

    // void sCollision();
    void sMovement();
    void spawnUnit();
    void sSetSelectionStart();
    void sSelection(std::string mode);
    void sSteering();

    void sSpawnTile(int tlx, int tly);
    void spawnRectangularGrid(int width, int height);

    void sUserInput();

    void sRender();
    int sGUI();

public:
    Game();
    void run();
};