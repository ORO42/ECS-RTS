#include <stdio.h>
#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>

#include "include/Game.h"
#include "include/GeneralUtils.h"
#include "include/raymath.h"

Game::Game()
{
    init();
};

void Game::init()
{
    // set default window parameters
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "My first Raylib");
    SetTargetFPS(60);
    SetWindowPosition(0, 0);
    spawnUnit();
    spawnRectangularGrid(32, 32);
}

void Game::run()
{
    // todo add pause functionality here

    while (!WindowShouldClose())
    {
        // update the entity manager
        m_entities.update();
        sMovement();
        // sCollision();
        sSteering();

        sUserInput();
        sRender();

        // increment current frame
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
    // TODO
}

void Game::spawnUnit()
{
    auto entity = m_entities.addEntity("unit");

    entity->cTransform = std::make_shared<CTransform>((Vector2){500.0f, 500.0f}, 0.0f);

    entity->cShape = std::make_shared<CShape>(entity->cTransform->pos.x, entity->cTransform->pos.y, 32.0f, 32.0f);
    entity->cShape->color = {255, 0, 0, 255};

    entity->cSelectable = std::make_shared<CSelectable>();

    entity->cSteering = std::make_shared<CSteering>(8, 60, 0.1f);
}

void Game::sMovement()
{
    // all entity movement in this function
    for (auto &e : m_entities.getEntities("unit"))
    {
        if (e->isActive())
        {
            if (e->cMovePoint)
            {
                // move the entity towards the move point
                // TODO create steering component and handle here if entity has it

                // draw center point
                DrawCircleV(getRectVCenter(e->cTransform->pos, {e->cShape->width, e->cShape->height}), 25, {0, 0, 255, 25});

                // Calculate direction vector from rectangle's position to target
                Velocity2D direction = calculateDirection(e->cTransform->pos, e->cMovePoint->pos);

                // Scale direction vector by maximum speed
                // the float value is maxSpeed TODO add as component
                Velocity2D desiredVelocity = {direction.x * 50.0f, direction.y * 50.0f};

                if (e->cSteering)
                {
                    // Add drift component
                    float driftFactor = 50.0f;
                    // Velocity2D drift = {1.0f, 0.0f}; // e.g. drift to the right
                    // desiredVelocity.x += driftFactor * drift.x;
                    // desiredVelocity.y += driftFactor * drift.y;
                    desiredVelocity.x += driftFactor * e->cSteering->chosenDir.x;
                    desiredVelocity.y += driftFactor * e->cSteering->chosenDir.y;
                }

                // if (CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec)) {

                // }

                // Update rectangle's position based on velocity and elapsed time
                float frameTime = GetFrameTime();
                if (distanceBetween({e->cTransform->pos.x, e->cTransform->pos.y}, e->cMovePoint->pos) >= 32.0f)
                {
                    e->cTransform->pos.x += desiredVelocity.x * frameTime;
                    e->cTransform->pos.y += desiredVelocity.y * frameTime;
                }
                else
                {
                    // remove move point
                    e->cMovePoint = nullptr;
                }
            }
        }
    }
}

// void Game::sCollision()
// {
//     for (auto bullet : m_entities.getEntities("bullet"))
//     {
//         for (auto enemy : m_entities.getEntities("enemy"))
//         {
//         }
//     }
// }

void Game::sSteering()
{
    // only run setup once (this may be inefficient as static duplicates state in memory n times)
    // other option would be to move setup code into component constructor
    static bool initialized = false;
    if (!initialized)
    {
        // Code to run only once
        initialized = true;

        // for i in num_rays:
        // var angle = i * 2 * PI / num_rays
        // ray_directions[i] = Vector2.RIGHT.rotated(angle)

        for (auto &e : m_entities.getEntities())
        {
            if (e->cSteering)
            {
                for (int i = 0; i < e->cSteering->numRays; ++i)
                {
                    Vector2 tempRay = Vector2One();
                    float angle = static_cast<float>(i) * 2.0f * PI / e->cSteering->numRays;
                    e->cSteering->rayDirections.push_back(Vector2Rotate(tempRay, angle));
                }
            }
        }
    }

    // TODO is this susceptible to iterator invalidation?
    for (auto &e : m_entities.getEntities())
    {
        if (e->cTransform)
        {
            //         int thisId = e->id();
            //         float thisPosX = e->cTransform->pos.x;
            //         float thisPosY = e->cTransform->pos.y;

            if (e->cSteering && e->cMovePoint)
            {
                for (const auto &direction : e->cSteering->rayDirections)
                {
                    //                 // Calculate the end point of the line
                    Vector2 endPoint = {e->cTransform->pos.x + direction.x * e->cSteering->lookAhead,
                                        e->cTransform->pos.y + direction.y * e->cSteering->lookAhead};

                    // Draw rays (for demonstration)
                    DrawLine(e->cTransform->pos.x, e->cTransform->pos.y, endPoint.x, endPoint.y, BLACK);

                    //                 // handle ray intersection
                    //                 for (int i = 0; i < e->cSteering->numRays; ++i)
                    //                 {
                    //                     // Calculate the end point of the line
                    //                     Vector2 endPoint = {e->cTransform->pos.x + direction.x * e->cSteering->lookAhead,
                    //                                         e->cTransform->pos.y + direction.y * e->cSteering->lookAhead};
                    //                     for (auto &e : m_entities.getEntities())
                    //                     {
                    //                         // ignore self collision
                    //                         if (e->id() != thisId)
                    //                         {
                    //                             std::vector<Line> eRectLines = getRectangleLines(e->cTransform->pos.x, e->cTransform->pos.y, e->cShape->width, e->cShape->height);
                    //                             for (const auto &line : eRectLines)
                    //                             {
                    //                                 // check against each ray's start and end and each rect's start and end
                    //                                 // p1 is one endpoint of the first line segment.
                    //                                 // q1 is the other endpoint of the first line segment.
                    //                                 bool isIntersection = doLinesIntersect({thisPosX, thisPosY}, {endPoint.x, endPoint.y}, {static_cast<float>(line.startX), static_cast<float>(line.startY)}, {static_cast<float>(line.endX), static_cast<float>(line.endY)});
                    //                                 if (isIntersection)
                    //                                 {
                    //                                     std::cout << "intersection" << std::flush;
                    //                                 }
                    //                                 else
                    //                                 {
                    //                                 }
                    //                             }
                    //                         }
                    //                     }
                    //                 }
                }
            }
        }
    }
}

void Game::sSpawnTile(int tlx, int tly)
{
    auto entity = m_entities.addEntity("tile");
    entity->cTile = std::make_shared<CTile>(tlx, tly);
    entity->cTile->topLeftX = tlx;
    entity->cTile->topLeftY = tly;
    entity->cTile->coord = getPositionOnGameboard(tlx, tly);
}

void Game::spawnRectangularGrid(int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int tlx = x * m_tileSize;
            int tly = y * m_tileSize;
            sSpawnTile(tlx, tly);
        }
    }
}

void Game::sSetSelectionStart()
{
    m_isSelecting = true;
    m_selectionStart = GetMousePosition();
}

void Game::sSelection(std::string mode)
{
    Vector2 mousePos = GetMousePosition();
    // Calculate the top-left corner and the width/height of the selection rectangle
    float x = std::min(m_selectionStart.x, mousePos.x);
    float y = std::min(m_selectionStart.y, mousePos.y);
    float width = std::abs(mousePos.x - m_selectionStart.x);
    float height = std::abs(mousePos.y - m_selectionStart.y);

    Rectangle selectionRect = {x, y, width, height};
    DrawRectangleLinesEx(selectionRect, 1.0f, {0, 0, 0, 255});
    DrawCircleV({m_selectionStart.x, m_selectionStart.y}, 3.0f, {255, 0, 0, 255});
    DrawCircleV({mousePos.x, mousePos.y}, 3.0f, {255, 0, 0, 255});

    // Iterate through entities with a Selectable and apply logic based on AABB collision
    for (auto &unit : m_entities.getEntities("unit"))
    {
        if (unit->isActive() && unit->cSelectable)
        {
            // Get the bounding box of the current game object
            Rectangle unitRect = {
                unit->cTransform->pos.x,
                unit->cTransform->pos.y,
                unit->cShape->width,
                unit->cShape->height};

            // Check if the bounding boxes intersect
            if (CheckCollisionRecs(selectionRect, unitRect))
            {
                if (mode == "select")
                {
                    unit->cSelectable->isSelected = true;
                    unit->cShape->color = (Color){0, 255, 0, 255};
                }
            }
            else
            {
                unit->cSelectable->isSelected = false;
                unit->cShape->color = (Color){255, 0, 0, 255};
            }
        }
    }
}

int Game::sGUI()
{
    CloseWindow();
    return 0;
}

void Game::sRender()
{
    BeginDrawing();

    // draw all tiles
    for (auto &e : m_entities.getEntities("tile"))
    {
        DrawRectangleLines(e->cTile->topLeftX, e->cTile->topLeftY, m_tileSize, m_tileSize, BLACK);
    }

    for (auto &e : m_entities.getEntities())
    {
        if (e->cTransform && e->cShape)
        {
            DrawRectangle(e->cTransform->pos.x, e->cTransform->pos.y, e->cShape->width, e->cShape->height, e->cShape->color);
            if (e->cMovePoint)
            {
                DrawLineEx(e->cTransform->pos, e->cMovePoint->pos, 1.0f, {0, 0, 0, 255});
            }
        }
    }

    DrawFPS(10, 10);

    ClearBackground(RAYWHITE);
    EndDrawing();
}

void Game::sUserInput()
{
    // catch user input, do not set players movement here

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        sSetSelectionStart();

        // iterate through entities with Selectable and set to false
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        sSelection("select");
    }

    if (m_isSelecting && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        m_isSelecting = false;
    }

    if (IsKeyPressed(KEY_M))
    {
        // Iterate through entities with a Selectable and apply logic based on AABB collision
        for (auto &unit : m_entities.getEntities("unit"))
        {
            if (unit->isActive() && unit->cSelectable->isSelected == true)
            {
                unit->cMovePoint = std::make_shared<CMovePoint>((Vector2)GetMousePosition());
            }
        }
    }
}