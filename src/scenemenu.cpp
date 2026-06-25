#include "scenemenu.hpp"
#include "sceneplay.hpp"
#include <iostream>

SceneMenu::SceneMenu(GameEngine* gameEngine) : Scene(gameEngine){
    init();
}

void SceneMenu::init(){
    title = "Trust No Man";
    subtitle = "A game inspired by Faith: The Unholy Trinity";
    menuStrings = {"New Game", "Continue Game", "Settings"};
    menuText = "W/S: Navigate      Space: Select      ESC: Quit";
    levelPaths.push_back("./assets/levels/level.txt");

    registerAction(KEY_S,      "DOWN");
    registerAction(KEY_W,      "UP");
    registerAction(KEY_SPACE,  "PLAY");
    registerAction(KEY_ESCAPE, "QUIT");

    // Start title music
    Music& music = const_cast<Music&>(gameEngine->getAssets().getMusic("TitleTheme"));
    SetMusicVolume(music, 0.5f);
    PlayMusicStream(music);

    screenTarget = LoadRenderTexture(gameEngine->getWidth(), gameEngine->getHeight());
    crtShader = LoadShader(0, "./assets/shaders/crt.frag");
    timeLoc = GetShaderLocation(crtShader, "time");
    resolutionLoc = GetShaderLocation(crtShader, "resolution");
    float res[2] = { (float)gameEngine->getWidth(), (float)gameEngine->getHeight() };
    SetShaderValue(crtShader, resolutionLoc, res, SHADER_UNIFORM_VEC2);
}

void SceneMenu::sRender(){
    // Must be called every frame to stream music
    Music& music = const_cast<Music&>(gameEngine->getAssets().getMusic("TitleTheme"));
    UpdateMusicStream(music);

    // Advance background animation frame
    bgFrameTimer++;
    if(bgFrameTimer >= bgFrameSpeed){
        bgFrameTimer = 0;
        bgFrame = (bgFrame + 1) % bgFrameCount;
    }

    BeginTextureMode(screenTarget);
    ClearBackground(BLACK);

    // Draw animated background — each frame is 256px wide in the sheet
    const Texture2D& bgTex = gameEngine->getAssets().getTexture("TitleHouseBG");
    int frameWidth = bgTex.width / bgFrameCount;  // 256
    Rectangle src  = { (float)(bgFrame * frameWidth), 0, (float)frameWidth, (float)bgTex.height };
    Rectangle dest = { 500, 50, 750, 700 };  // x, y, width, height — tweak these
    DrawTexturePro(bgTex, src, dest, {0,0}, 0.0f, WHITE);

    // Then draw menu text on top
    const Font& font = gameEngine->getAssets().getFont("dosvga");

    DrawTextEx(font, title.c_str(), Vector2(50, 50), 64, 1, WHITE);
    DrawTextEx(font, subtitle.c_str(), Vector2(50, 120), 32, 1, DARKGRAY);

    if(inSettings){
        // Settings screen
        DrawTextEx(font, "SETTINGS", Vector2(50, 150+70), 48, 1, GRAY);
        DrawTextEx(font, "Nothing here yet!", Vector2(50, 220+70), 32, 1, GRAY);
        DrawTextEx(font, "Press ESC to go back", Vector2(50, 700+70), 24, 1, DARKGRAY);
    } else {
        // Main menu
        for(int i = 0; i < (int)menuStrings.size(); i++){
            Color textColor = (i == selectedMenuItem) ? RED : GRAY;
            DrawTextEx(font, menuStrings[i].c_str(), Vector2(50, 75*(i+3)), 48, 1, textColor);
        }
        DrawTextEx(font, menuText.c_str(), Vector2(50, 750), 24, 1, GRAY);
    }

    EndTextureMode();

    // 2. Draw texture to screen with CRT shader
    BeginDrawing();
        ClearBackground(BLACK);
        shaderTime += GetFrameTime();
        SetShaderValue(crtShader, timeLoc, &shaderTime, SHADER_UNIFORM_FLOAT);
        BeginShaderMode(crtShader);
            // Note: RenderTexture is flipped in Y
            DrawTexturePro(
                screenTarget.texture,
                { 0, 0, (float)screenTarget.texture.width, -(float)screenTarget.texture.height },
                { 0, 0, (float)gameEngine->getWidth(), (float)gameEngine->getHeight() },
                { 0, 0 }, 0.0f, WHITE
            );
        EndShaderMode();
    EndDrawing();
}

void SceneMenu::sDoAction(const Action& action){
    if(action.getType() == "PRESS"){

        if(inSettings){
            // Only ESC works in settings for now
            if(action.getName() == "QUIT") inSettings = false;
            return;
        }

        if(action.getName() == "UP"){
            selectedMenuItem--;
            if(selectedMenuItem < 0) selectedMenuItem = menuStrings.size()-1;
        }
        if(action.getName() == "DOWN"){
            selectedMenuItem++;
            if(selectedMenuItem > (int)menuStrings.size()-1) selectedMenuItem = 0;
        }
        if(action.getName() == "PLAY"){
            if(menuStrings[selectedMenuItem] == "New Game"){
                // Stop music before switching scene
                Music& music = const_cast<Music&>(gameEngine->getAssets().getMusic("TitleTheme"));
                StopMusicStream(music);
                gameEngine->changeScene("PLAY", std::make_shared<ScenePlay>(gameEngine, levelPaths[0]));
            }
            if(menuStrings[selectedMenuItem] == "Continue Game"){
                // For now same as New Game — hook up save later
                Music& music = const_cast<Music&>(gameEngine->getAssets().getMusic("TitleTheme"));
                StopMusicStream(music);
                gameEngine->changeScene("PLAY", std::make_shared<ScenePlay>(gameEngine, levelPaths[0]));
            }
            if(menuStrings[selectedMenuItem] == "Settings"){
                inSettings = true;
            }
        }
        if(action.getName() == "QUIT"){
            gameEngine->quit();
        }
    }
}

void SceneMenu::update(){
    sRender();
}

SceneMenu::~SceneMenu(){
    UnloadRenderTexture(screenTarget);
    UnloadShader(crtShader);
}