#pragma once

#include "raylib.h"
#include "raymath.h"

class CTransform
{
public:
    Vector2 pos = {0.0, 0.0};
    float angle = 0;

    CTransform(const Vector2 &p, float a)
        : pos(p), angle(a) {}
};

class CVelocity
{
public:
    Vector2 velocity = {0.0, 0.0};

    CVelocity(const Vector2 &v) : velocity(v) {}
};

class CShape
{
public:
    float x;
    float y;
    float width;
    float height;
    Color color{255, 0, 0, 0};
    bool collisionEnabled = true;

    // Constructor initialization list initializes 'rect' with parameters 'x', 'y', 'width', and 'height'
    CShape(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
    {
    }
};

class CInput
// separate the concepts of mechanical input and game actions
// actions shouldn't know about mechanical input
{
public:
    bool move;
    bool attack;

    CInput() {}
};

class CSelectable
{
public:
    bool isSelected = true;
};

class CMovePoint
{
public:
    Vector2 pos = {0.0, 0.0};

    CMovePoint(const Vector2 &p)
        : pos(p) {}
};

class CSteering
{

public:
    int lookAhead;
    int numRays;
    float steerForce = 0.1f;
    Vector2 chosenDir = Vector2Zero();

    // context vectors
    std::vector<Vector2> rayDirections;
    std::vector<float> interest;
    std::vector<float> danger;

    // Constructor to initialize the vectors with the desired size
    CSteering(int numRays, int lookAhead, float steerForce)
        : numRays(numRays),
          rayDirections(numRays),
          interest(numRays),
          danger(numRays),
          lookAhead(lookAhead),
          steerForce(steerForce)
    {
    }
};

class CTile
{
public:
    int topLeftX;
    int topLeftY;
    std::pair<int, int> coord;
    bool isNavigable = true;

    CTile(int topLeftX, int topLeftY)
        : topLeftX(topLeftX), topLeftY(topLeftY) {}
};