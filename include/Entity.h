#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Components.h"

class Entity
{
    friend class EntityManager;

    bool m_active = true;
    size_t m_id = 0;
    std::string m_tag = "default";

    // constructor and destructor
    Entity(const size_t &id, const std::string &tag);

public:
    // component pointers
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CSelectable> cSelectable;
    std::shared_ptr<CMovePoint> cMovePoint;
    std::shared_ptr<CSteering> cSteering;
    std::shared_ptr<CTile> cTile;

    // private member access function
    bool isActive() const;
    const std::string &tag() const;
    const size_t &id() const;
    void destroy();
};