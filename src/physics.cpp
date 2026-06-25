#include "physics.hpp"
#include <cmath>

/**
 * Compute overlap this frame
 * 
 * @param a  shared pointer to entity a
 * @param b  shared pointer to entity b
 * @return Vec2  Contains the x and y overlap of entities a and b
 */
Vec2 Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b){
    //TODO: Implement the overlap math between two entities as we discussed in class
    Vec2 posA = a->getComponent<CTransform>().position;
    Vec2 posB = b->getComponent<CTransform>().position;
    Vec2 bboxSizeA = a->getComponent<CBoundingBox>().size;
    Vec2 bboxSizeB = b->getComponent<CBoundingBox>().size;

    Vec2 delta = Vec2(std::abs(posA.x - posB.x), std::abs(posA.y - posB.y));
    Vec2 overlap = (bboxSizeA * 0.5f + bboxSizeB * 0.5f) - delta;

    return overlap;
}

/**
 * Compute overlap from previous frame
 * 
 * @param a  shared pointer to entity a
 * @param b  shared pointer to entity b
 * @return Vec2  Contains the x and y overlap of entities a and b
 */
Vec2 Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b){
    //TODO:: Same as getOverlap, but uses the previous positions of Entity a and b
    Vec2 posA = a->getComponent<CTransform>().prevPosition;
    Vec2 posB = b->getComponent<CTransform>().prevPosition;
    Vec2 bboxSizeA = a->getComponent<CBoundingBox>().size;
    Vec2 bboxSizeB = b->getComponent<CBoundingBox>().size;

    Vec2 delta = Vec2(std::abs(posA.x - posB.x), std::abs(posA.y - posB.y));
    Vec2 overlap = (bboxSizeA * 0.5f + bboxSizeB * 0.5f) - delta;

    return overlap;
}