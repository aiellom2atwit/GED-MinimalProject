#pragma once

#include <string>
#include <raylib.h>
#include "vec2.hpp"
#include "animation.hpp"

/**
 * Base class for all Component objects
 */
class Component{
    public:
    bool has=false; /* Boolean if the object has this component */
};

/**
 * Transform Component
 * 
 * Defines the position, velocty, scale, and angle of an entity
 * 
 * All entities should have a transform
 */
class CTransform : public Component{
    public:
        Vec2 position;     /* Current position (x,y) of the entity */
        Vec2 prevPosition; /* Previous frame position (x,y) of the entity */
        Vec2 velocity;     /* Current velocity (vx,vy) of the entity */
        Vec2 scale;        /* Current scale (sx,xy) of the entity */
        float angle;       /* Current rotation angle of the player (deg) */

        CTransform();
        CTransform(const Vec2& position, const Vec2& velocity, float angle);
};

/**
 * Lifespan Component
 * 
 * Added to entities that have a limited lifespan, e.g. bullets
 */
class CLifespan : public Component{
    public:
        int remaining=0; /* Remaining frames that the entity is alive */
        int total=0;     /* Total number of frame the entity can live */
        CLifespan(int total);
};

/**
 * Input Component
 * 
 * Contains booleans for the possible inputs that the player can perform
 */
class CInput : public Component{
    public:
        bool jump=false;  /* Is the player pressing the jump button */
        bool left=false;  /* Is the player pressing the left button */
        bool right=false; /* Is the player pressing the right button */
        bool shoot=false; /* Is the player pressing the shoot button */
        bool up=false;
        bool down=false;

        CInput();
};

/**
 * Bounding Box Component
 * 
 * Defines the hight and width of the bounding box used in the physics system
 */
class CBoundingBox : public Component{
    public:
        Vec2 size = {0.0f, 0.0f}; /* Width and height of the entity bounding box */
        CBoundingBox();
        CBoundingBox(const Vec2& s);
};

/**
 * Gravity Component
 * 
 * Defines the gravity factor to apply to velocity each frame
 */
class CGravity : public Component{
    public:
        float factor=50.0f; /* Gravity factor to add to velocity each frame */
        CGravity();
        CGravity(float factor);
};

/**
 * Animation Component
 * 
 * Defines the animation properties
 */
class CAnimation : public Component {
    public:
        Animation animation; /*Actual animation object for this component */
        bool repeat;         /*true/false if this animation repeats after the last animation frame */

        CAnimation() = default;
        CAnimation(const Animation &animation, bool r) : animation(animation), repeat(r) {}
};

/**
 * State Component
 * 
 * Defines the state of the object, including if it is on the ground and the direction, left/right, that it is facing
 */
class CState : public Component {
    public:
        std::string state;     /*Current state name */
        bool isGrounded=false; /*Is the entity touching the top of another entity */
        int direction=1;       /*left (-1) or right (1) facing of the entity */
        int jumpFrames = 0;
        int framesUngrounded = 0;

        CState();
        CState(std::string state);
};

