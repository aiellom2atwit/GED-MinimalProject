#pragma once
#include <string>
#include "entity.hpp"
#include "scene.hpp"
/**
 * Contains the player data from the level definiton file. Filled when a scene loads a level.
 */
struct PlayerConfig{float GX, GY, CW, CH, S, G, JS;};

/**
 * Scene that contains all logic and information for playing a loaded level
 */
class ScenePlay : public Scene{
    private:
        PlayerConfig playerConfig;      /* Information about the properties of the player for this level */
        std::string levelPath;          /* Relative path to the level definition file from exe */
        std::shared_ptr<Entity> player; /* Shared pointer reference to the player entity */
        bool reload=false;              /* If the Scene should be reloaded at the end of the current frame */

        void init(const std::string& levelPath);
        void sAnimation();
        void sMovement();
        void sCollision();
        void sRender();
        void sDoAction(const Action& action);
        void spawnPlayer();
        void spawnBullet();
        void loadLevel(const std::string& levelPath);
        void reloadScene();
    public:
        ScenePlay(GameEngine* gameEngine,std::string levelPath);
        ScenePlay()=default;
        void update();
};