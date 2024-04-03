// generalUtils.cpp

#include <iostream>
#include <cmath>
#include <vector>

#include "include/GeneralUtils.h"

// Function to calculate the distance between two points
float distanceBetween(const Vector2 p1, const Vector2 p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Function to calculate the direction vector from one point to another
Velocity2D calculateDirection(const Vector2 from, const Vector2 to)
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return {dx / distance, dy / distance};
}

Vector2 getRectVCenter(const Vector2 rectPos, const Vector2 size)
{
    // Calculate the center point
    float centerX = rectPos.x + size.x / 2.0f;
    float centerY = rectPos.y + size.y / 2.0f;

    // Return the center point
    return {centerX, centerY};
}

bool isLineIntersectingRec(const Vector2 &start, const Vector2 &end, const Rectangle &rect)
{
    // Check if the line segment intersects any of the rectangle's sides

    float x1 = rect.x;
    float y1 = rect.y;
    float x2 = rect.x + rect.width;
    float y2 = rect.y;
    float x3 = rect.x + rect.width;
    float y3 = rect.y + rect.height;
    float x4 = rect.x;
    float y4 = rect.y + rect.height;

    // Calculate the direction vectors of the rectangle's sides
    float dx1 = x2 - x1;
    float dy1 = y2 - y1;
    float dx2 = x3 - x2;
    float dy2 = y3 - y2;
    float dx3 = x4 - x3;
    float dy3 = y4 - y3;
    float dx4 = x1 - x4;
    float dy4 = y1 - y4;

    // Calculate the normal vectors of the rectangle's sides
    float nx1 = -dy1;
    float ny1 = dx1;
    float nx2 = -dy2;
    float ny2 = dx2;
    float nx3 = -dy3;
    float ny3 = dx3;
    float nx4 = -dy4;
    float ny4 = dx4;

    // Calculate the vectors from start point to rectangle vertices
    float vx1 = x1 - start.x;
    float vy1 = y1 - start.y;
    float vx2 = x2 - start.x;
    float vy2 = y2 - start.y;
    float vx3 = x3 - start.x;
    float vy3 = y3 - start.y;
    float vx4 = x4 - start.x;
    float vy4 = y4 - start.y;

    // Check if the vectors are on the same side of the line
    bool side1 = (vx1 * nx1 + vy1 * ny1) * (end.x - start.x) + (vx1 * dx1 + vy1 * dy1) * (end.y - start.y) >= 0;
    bool side2 = (vx2 * nx2 + vy2 * ny2) * (end.x - start.x) + (vx2 * dx2 + vy2 * dy2) * (end.y - start.y) >= 0;
    bool side3 = (vx3 * nx3 + vy3 * ny3) * (end.x - start.x) + (vx3 * dx3 + vy3 * dy3) * (end.y - start.y) >= 0;
    bool side4 = (vx4 * nx4 + vy4 * ny4) * (end.x - start.x) + (vx4 * dx4 + vy4 * dy4) * (end.y - start.y) >= 0;

    // If all sides have the same orientation, the line does not intersect the rectangle
    if (side1 == side2 && side2 == side3 && side3 == side4)
    {
        return false;
    }

    // Otherwise, the line intersects the rectangle
    return true;
}

// Function to calculate the lines making up a rectangle
std::vector<Line> getRectangleLines(int posX, int posY, int width, int height)
{
    std::vector<Line> lines;

    // Top line
    lines.push_back({posX, posY, posX + width, posY});

    // Right line
    lines.push_back({posX + width, posY, posX + width, posY + height});

    // Bottom line
    lines.push_back({posX + width, posY + height, posX, posY + height});

    // Left line
    lines.push_back({posX, posY + height, posX, posY});

    return lines;
}

// Function to calculate the cross product of two vectors
float crossProduct(Point a, Point b, Point c)
{
    float x1 = b.x - a.x;
    float y1 = b.y - a.y;
    float x2 = c.x - a.x;
    float y2 = c.y - a.y;

    return x1 * y2 - x2 * y1;
}

// Function to check if two line segments intersect
bool doLinesIntersect(Point p1, Point q1, Point p2, Point q2)
{
    // Find the four orientations needed for general and special cases
    float o1 = crossProduct(p1, q1, p2);
    float o2 = crossProduct(p1, q1, q2);
    float o3 = crossProduct(p2, q2, p1);
    float o4 = crossProduct(p2, q2, q1);

    // General case
    if (o1 * o2 < 0 && o3 * o4 < 0)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && (p2.x >= std::min(p1.x, q1.x) && p2.x <= std::max(p1.x, q1.x)) &&
        (p2.y >= std::min(p1.y, q1.y) && p2.y <= std::max(p1.y, q1.y)))
        return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && (q2.x >= std::min(p1.x, q1.x) && q2.x <= std::max(p1.x, q1.x)) &&
        (q2.y >= std::min(p1.y, q1.y) && q2.y <= std::max(p1.y, q1.y)))
        return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && (p1.x >= std::min(p2.x, q2.x) && p1.x <= std::max(p2.x, q2.x)) &&
        (p1.y >= std::min(p2.y, q2.y) && p1.y <= std::max(p2.y, q2.y)))
        return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && (q1.x >= std::min(p2.x, q2.x) && q1.x <= std::max(p2.x, q2.x)) &&
        (q1.y >= std::min(p2.y, q2.y) && q1.y <= std::max(p2.y, q2.y)))
        return true;

    return false; // Doesn't fall in any of the above cases
}

// Function to translate x, y pos to gameboard grid coord
std::pair<int, int> getPositionOnGameboard(int x, int y)
{
    // Assuming each tile is 32x32
    int gridX = x / 32 + 1;
    int gridY = y / 32 + 1;
    return std::make_pair(gridX, gridY);
}