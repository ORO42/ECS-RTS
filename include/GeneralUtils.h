#pragma once

#include <variant>
#include <optional>

#include "raylib.h"

struct Velocity2D
{
    float x;
    float y;
};

// Struct representing a line segment
struct Line
{
    int startX;
    int startY;
    int endX;
    int endY;
};

// Struct representing a point
struct Point
{
    float x;
    float y;
};

float distanceBetween(const Vector2 p1, const Vector2 p2);

Velocity2D calculateDirection(const Vector2 from, const Vector2 to);

Vector2 getRectVCenter(const Vector2 rectPos, const Vector2 size);

bool isLineIntersectingRec(const Vector2 &start, const Vector2 &end, const Rectangle &rect);

std::vector<Line> getRectangleLines(int posX, int posY, int width, int height);

// Function to calculate the cross product of two vectors
float crossProduct(Point a, Point b, Point c);

// Function to check if two line segments intersect
bool doLinesIntersect(Point p1, Point q1, Point p2, Point q2);

// Function to translate x, y pos to gameboard grid coord
std::pair<int, int> getPositionOnGameboard(int x, int y);