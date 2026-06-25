#include "scene.hpp"
#include <iostream>

Scene::Scene(GameEngine* gameEngine) : gameEngine(gameEngine){}
Scene::Scene()=default;

/**
 * Sends action to the Do Action system in the currently running scene
 * 
 * @param action Action that gets sent from gameEngine user input
 */
void Scene::doAction(const Action& action){
    sDoAction(action);
}

/**
 * Sends action to the Do Action system in the currently running scene
 * 
 * @param type int that represents the type of action, e.g. raylib key code
 * @param name string that contains the name of the action
 */
void Scene::registerAction(int type, std::string name){
    actionMap[type]=name;
}

/**
 * Returns if the scene has ended
 * 
 * @return true/false if the scene has ended
 */
bool Scene::hasEnded() const{
    return ended;
}

/**
 * Gets the current frame of the game
 * 
 * @return int that contains the current frame of the game
 */
int Scene::currentFrame() const{
    return currFrame;
}

/**
 * Draws the bounding boxes for each entity
 */
void Scene::renderBB(){
    for(auto& e : entityManager.getEntities()){
        if(!e->hasComponent<CBoundingBox>()) continue;
        auto& box=e->getComponent<CBoundingBox>();
        auto& transform=e->getComponent<CTransform>();
        Vec2 size=box.size;
        float x=transform.position.x-size.x/2;
        float y=transform.position.y-size.y/2;
        int lineThickenss=2;
        DrawRectangleLinesEx({x,y,size.x,size.y}, lineThickenss, GREEN);
    }
}

/**
 * Draws the textures for each entity in the game. Textures rendered last will be on top.
 */
void Scene::renderTex(){
    renderType("DEC");
    renderType("TILE");
    renderType("DYNAMIC");
}


/**
 * Draws the textures for each entity based on input type
 * 
 * @param type string that contains the type of object to draw
 */
void Scene::renderType(const std::string& type){
    for(auto& e : entityManager.getEntities(type)){
        auto& transform=e->getComponent<CTransform>();
        auto& anim = e->getComponent<CAnimation>();
        auto& state = e->getComponent<CState>();
        const Texture2D& tex = anim.animation.getTexture();
        
        float scaledWidth=anim.animation.getScaledSize().x;
        float scaledHeight=anim.animation.getScaledSize().y;

        float x=transform.position.x;
        float y=transform.position.y;

        Rectangle src = anim.animation.getFrameRect();
        src.width*=state.direction;
        Rectangle dest = {x,y,scaledWidth,scaledHeight};
        Vector2 origin = {scaledWidth/2,scaledHeight/2};

        DrawTexturePro(tex,src,dest,origin,transform.angle,WHITE);
    }
}

/**
 * Draws the game grid overlay, including (x,y) positions
 */
void Scene::renderGrid(){
    int gridSizeX=gameEngine->getTileSizeX();
    int gridSizeY=gameEngine->getTileSizeY();
    int width=gameEngine->getWidth();
    int height=gameEngine->getHeight();
    int tilesX=gameEngine->getTilesX();
    int tilesY=gameEngine->getTilesY();
    for(int x=gridSizeX;x<tilesX*gridSizeX;x+=gridSizeX){
        DrawLine(x,0,x,height,WHITE);
    }
    for(int y=gridSizeY;y<height;y+=gridSizeX){
        DrawLine(0,height-y,tilesX*gridSizeX,height-y,WHITE);
    }
    for(int i=0;i<tilesX*gridSizeX;i+=gridSizeX){
        for(int j=0;j<height;j+=gridSizeY){
            int textSize=12;
            std::string label="("+std::to_string(i/gridSizeX)+","+std::to_string(j/gridSizeY)+")";
            DrawText(label.c_str(),i+2,height-j-gridSizeY+2,textSize,WHITE);
        }
    }
}

/**
 * Gets the registered action map
 * 
 * @return map of registered action: keycode->action name 
 */
std::map<int, std::string>& Scene::getActionMap(){
    return actionMap;
}


/**
 * Gets the position in world space based on game grid location
 * 
 * @param gridX X coordinate on the game grid
 * @param gridY X coordinate on the game grid
 * @param entity Entity that should be placed on the grid
 * @return Vec2 that contains the center pixel in world space corresponding to the input grid coords
 */
Vec2 Scene::gridToMidPixel(int gridX, int gridY, std::shared_ptr<Entity> entity){
    //Takes a grid position (x,y) and an entity
    //Returns a Vec2 where the center of the entity should be positioned such that it fits in the grid location
    int windowHeight = gameEngine->getHeight();
    
    //Use the entities animation size and the grid size (tileSizeX and tileSizeY from gameEngine)
    Animation& anim = entity->getComponent<CAnimation>().animation;
    Vec2 animSize = anim.getScaledSize();
    int tileSizeX = gameEngine->getTileSizeX();
    int tileSizeY = gameEngine->getTileSizeY();

    float cellLeft = gridX * tileSizeX;
    float cellBottom = windowHeight - (gridY * tileSizeY) - tileSizeY;

    //The bottom left of the entity should align with the bottom left of the cell
    float centerX = cellLeft + tileSizeX / 2.0f;
    float centerY = cellBottom + tileSizeY / 2.0f;


    printf("INFO: PLACED ENTITY: %s \tat x=%.1f, \ty=%.1f\n", entity->getID().c_str(), centerX, centerY);
    return Vec2(centerX, centerY);
}