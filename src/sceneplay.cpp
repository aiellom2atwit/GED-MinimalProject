#include "sceneplay.hpp"
#include "scenemenu.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include "physics.hpp"

/**
 * Constructor for the Play Scene
 * 
 * @param gameEngine raw pointer to the game engine class
 * @param levelPath Path to level defintion file, relative to exe
 */
ScenePlay::ScenePlay(GameEngine* gameEngine, std::string levelPath):Scene(gameEngine){
    this->levelPath=levelPath;
    init(this->levelPath);
}

/**
 * Functions that initializes the scene:
 * 
 * 1. Loads Level information
 * 2. Spawns Players  
 * 3. Registers input action  
 * 4. Sets up camera  
 * 
 * @param levelPath Path to level defintion file, relative to exe
 */
void ScenePlay::init(const std::string& levelPath){
    //loadLevel(levelPath);
    spawnPlayer();

    //Add actions to JUMP, SHOOT, LEFT, and RIGHT
    registerAction(KEY_LEFT_SHIFT, "SHOOT");
    registerAction(KEY_A, "LEFT");
    registerAction(KEY_D, "RIGHT");
    registerAction(KEY_W, "UP");
    registerAction(KEY_S, "DOWN");


    registerAction(KEY_B,"BB");
    registerAction(KEY_G,"GRID");
    registerAction(KEY_T,"TEX");
    registerAction(KEY_ESCAPE,"QUIT");
    registerAction(KEY_R,"RELOAD");

    mainCamera=Camera2D({gameEngine->getWidth()/2.0f*GetWindowScaleDPI().x,gameEngine->getHeight()/2.0f*GetWindowScaleDPI().y},{gameEngine->getWidth()/2.0f,gameEngine->getHeight()/2.0f},0,GetWindowScaleDPI().x);

}

/**
 * Loads level information from level definition file.
 * 
 * Once loaded, the correct entity type is created and setup.
 * 
 * @param levelPath Path to level defintion file, relative to exe
 */
void ScenePlay::loadLevel(const std::string& levelPath){
    std::ifstream file(levelPath);
    std::string str;
    std::string type; 
    int x, y;
    //TODO: Add the reading of decorations (DEC) from the level definition file
    //Refer to the assignment PDF for the formatting of decorations
    while (file.good()) {
        file>>str;
        if(str=="TILE"){
            file>>type>>x>>y;
            auto e=entityManager.addEntity(str, type);
            e->addComponent<CAnimation>(gameEngine->getAssets().getAnimation(type),true);
            float scaledX=gameEngine->getAssets().getAnimation(type).getScaledSize().x;
            float scaledY=gameEngine->getAssets().getAnimation(type).getScaledSize().y;
            float tileSizeX=gameEngine->getTileSizeX();
            float tileSizeY=gameEngine->getTileSizeY();
            float bboxSizeX=tileSizeX;
            float bboxSizeY=tileSizeY;
            if(scaledX>tileSizeX || scaledY>tileSizeY){
                bboxSizeX=scaledX;
                bboxSizeY=scaledY;
            }
            e->addComponent<CBoundingBox>(Vec2(bboxSizeX,bboxSizeY));
            Vec2 pos = gridToMidPixel(x,y,e);
            e->addComponent<CTransform>(Vec2(pos.x,pos.y), Vec2(0.0f,0.0f), 0.0f);
            e->getComponent<CTransform>().prevPosition.x=pos.x;
            e->getComponent<CTransform>().prevPosition.y=pos.y;
        }
        if(str=="PLAYER"){
            file>>playerConfig.GX>>playerConfig.GY>>playerConfig.CW>>playerConfig.CH>>playerConfig.S>>playerConfig.G>>playerConfig.JS;
        }
    }
}

/**
 * Animation System
 * 
 * For the player: Sets the state and animation depending on user input.
 * For other entities: Updates the animation via the CAnimation component.
 *
 */
void ScenePlay::sAnimation(){
    CInput& input = player->getComponent<CInput>();
    CState& state = player->getComponent<CState>();

    //TODO: Based on state and user input, change the animation of the player
    // Handle direction
    if(input.right) state.direction = 1;
    if(input.left)  state.direction = -1;

    // Determine which animation to play based on priority
    // (air state takes priority over ground state)
    std::string newState;

    // Only swap animation if state actually changed
    if(newState != state.state){
        state.state = newState;
        player->addComponent<CAnimation>(gameEngine->getAssets().getAnimation(newState), true);
    }

    //Update all the animation frames
    for(auto& e : entityManager.getEntities()){
        if(e->hasComponent<CAnimation>()){
            e->getComponent<CAnimation>().animation.update();
        }
    }

}


/**
 * Movement System
 * 
 * Moves all DYNAMIC entities
 * 
 */
void ScenePlay::sMovement(){
    CInput& input = player->getComponent<CInput>();
    CState& state = player->getComponent<CState>();
    CTransform& transform = player->getComponent<CTransform>();


    for(auto& e : entityManager.getEntities("DYNAMIC")){
        if(!e->hasComponent<CGravity>()) continue;

        
        //TODO: Implement the movement for DYNAMIC entities
        //1.If the entity is the player, use the input bools
        //  to determine direction and x velocity.
        if (e == player) {
            transform.velocity.x = 0;
            if(input.left)  transform.velocity.x = -playerConfig.S / 60.0f;
            if(input.right) transform.velocity.x =  playerConfig.S / 60.0f;



            
        }
        
        
        transform.prevPosition = transform.position;
        transform.position += transform.velocity;

        //3. This is also a good place to update the lifespan component
    }

    //Camera movement, based on player transform
    if(transform.position.x>=gameEngine->getWidth()/2){
        mainCamera.target.x=transform.position.x;
    }
}

/**
 * Collision System
 * 
 * Checks for any collisions between DYNAMIC objects and any other objects
 * 
 */
void ScenePlay::sCollision(){
    //Check if player fell below screen
    if(player->getComponent<CTransform>().position.y > gameEngine->getHeight()){
        reload = true;
        return;
    }

    for(auto& de : entityManager.getEntities("DYNAMIC")){
        int topYCollisions=0;
        for(auto& e : entityManager.getEntities()){
            if(de==e) continue;
            if(e->getTag()=="DEC") continue; //decorations

            //TODO: Implement the rest of the collision logic
            //note how the outer loop is over only the DYNAMIC
            //objects, this reduces the number of inner loop iterations
            //I have included the loops, a skip if de==e 
            //and a skip if the other entity is a decoration
            Vec2 overlap = Physics::getOverlap(de, e);
            Vec2 prevOverlap = Physics::getPreviousOverlap(de, e);

            if (overlap.x > 0 && overlap.y > 0) {
                CTransform& deTransform = de->getComponent<CTransform>();

                //prevOverlap tells you which axis the collision came from
                if (prevOverlap.x > 0) {
                    //came from top or bottom - resolve Y
                    if (deTransform.position.y < e->getComponent<CTransform>().position.y) {
                        deTransform.position.y -= overlap.y; //push up
                        deTransform.velocity.y = 0;
                    } else {
                        deTransform.position.y += overlap.y; //push down
                    }
                }
                if (prevOverlap.y > 0) {
                    //came from left or right - resolve X
                    if (deTransform.position.x < e->getComponent<CTransform>().position.x) {
                        deTransform.position.x -= overlap.x; //push left
                        deTransform.velocity.x = 0;
                    } else {
                        deTransform.position.x += overlap.x; //push right
                    }
                }
            }
        }
    }
}

/**
 * Render System
 * 
 * Renders the textures, using the camera system
 * 
 */
void ScenePlay::sRender(){
    BeginDrawing();
    BeginMode2D(mainCamera);
    ClearBackground(BLUE);

    //********** Raylib Drawing Content **********
        if(renderTextures)
            renderTex();
        if(renderBoundingBox)
            renderBB();
        if(renderGridLines)
            renderGrid();

        EndMode2D();
        //********** ImGUI Content *********
        //draw the gui

    EndDrawing();
}

/**
 * Do Action System
 * 
 * Updates the player input and game state based on PRESS or RELEASE actions
 * 
 * @param action action sent from the gameEngine that contains type and name
 */
void ScenePlay::sDoAction(const Action& action){
    CInput& input = player->getComponent<CInput>();

    //TODO: Add in the results of the additional actions
    //that you register in the init() function: LEFT, RIGHT, JUMP, SHOOT
    //also add in what happens when you RELEASE the button


    if((action.getType()=="PRESS")){
        if(action.getName()=="SHOOT") {
            input.shoot = true;
        }
        if(action.getName()=="LEFT") {
            input.left = true;
        }
        if(action.getName()=="RIGHT") {
            input.right = true;
        }
        if(action.getName()=="UP") {
            input.up = true;
        }
        if(action.getName()=="DOWN") {
            input.down = true;
        }
        if(action.getName()=="BB"){
            renderBoundingBox=!renderBoundingBox;
        }
        if(action.getName()=="GRID"){
            renderGridLines=!renderGridLines;
        }
        if(action.getName()=="TEX"){
            renderTextures=!renderTextures;
        }
        if(action.getName()=="QUIT"){
            gameEngine->changeScene("MENU",std::make_shared<SceneMenu>(gameEngine));
        }
        if(action.getName()=="RELOAD"){
            reload=true;
        }
    }
    if((action.getType()=="RELEASE")){
        if(action.getName()=="LEFT")  input.left = false;
        if(action.getName()=="RIGHT") input.right = false;
        if(action.getName()=="UP") input.up = false;
        if(action.getName()=="DOWN") input.down = false;
        if(action.getName()=="RELOAD") reload = false;
    }

}

/**
 * Spawns Player
 * 
 * Example of spawning a simple player
 * 
 */
void ScenePlay::spawnPlayer(){
    //TODO: Sample player spawning, you may need/want
    //to update this once you have some more mechanics finished
    
    player=entityManager.addEntity("DYNAMIC", "PLAYER");
    player->addComponent<CState>("STAND");
    player->addComponent<CInput>();
    player->addComponent<CGravity>();
    player->addComponent<CAnimation>(gameEngine->getAssets().getAnimation("MEGASTAND"),true);
    int scaledHeight=player->getComponent<CAnimation>().animation.getScaledSize().y;
    int scaledWidth=player->getComponent<CAnimation>().animation.getScaledSize().y;
    player->addComponent<CBoundingBox>(Vec2(playerConfig.CW, playerConfig.CH));
    Vec2 pos = gridToMidPixel(playerConfig.GX,playerConfig.GY,player);
    player->addComponent<CTransform>(Vec2(pos.x,pos.y), Vec2(0.0f,0.0f), 0.0f);
}

/**
 * Spawns a bullet at the player's location
 */
void ScenePlay::spawnBullet(){
    //TODO: Spawn bullet using the players location

}

/**
 * Reloads this play scene
 */
void ScenePlay::reloadScene(){
    gameEngine->changeScene("PLAY",std::make_shared<ScenePlay>(gameEngine, levelPath));
}


/**
 * Updates all systems and entityManager
 */
void ScenePlay::update(){
    entityManager.update();

    sMovement();
    sAnimation();
    sCollision();
    sRender();

    //waiting until the end of the frame to actually reload scene
    if(reload==true){
        reloadScene();
    }
    
}