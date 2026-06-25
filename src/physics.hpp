#pragma once
#include "vec2.hpp"
#include "entity.hpp"
#include <memory>

/**
 * Contains only static functions for computing bounding box overlap between two entities
 */
class Physics{
    public:
        static Vec2 getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
        static Vec2 getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};