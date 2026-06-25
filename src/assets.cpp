#include "assets.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include "animation.hpp"


Assets::Assets()=default;

Assets::~Assets(){
    //unload fonts
    for(auto& [name,font] : fontMap){
        UnloadFont(font);
    }
    //unload Texture2Ds
    for(auto& [name,tex] : textureMap){
        UnloadTexture(tex);
    }

    for(auto& [name,music] : musicMap) {
        UnloadMusicStream(music);
    }
}

/**
 * Loads the game textures, fonts, and animation definitions from the asset definition file
 * 
 * @param path string that contains the path to the asset definition file
 */
void Assets::load(const std::string path){
    std::ifstream file(path);
    std::string str;

    while (file.good()) {
        file >> str;
        std::string name, path, tex;
        int frameCount, speed, scaleFactor;
        switch (toascii(str[0])) {
            case AssetTypes::FONT: 
                file >> name >> path;
                addFont(name,path);
                break;
            case AssetTypes::TEXTURE: 
                file >> name >> path;
                addTexture(name,path);
                break;
            case AssetTypes::ANIMATION:
                file>>name>>tex>>frameCount>>speed>>scaleFactor;
                addAnimation(name,tex,frameCount,speed, scaleFactor);
                break;
            case AssetTypes::SONG:
                file >> name >> path;
                addMusic(name, path);
                break;
        }   

    }
    file.close();
}

void Assets::addMusic(const std::string& name, const std::string& path){
    InitAudioDevice(); // safe to call multiple times
    Music music = LoadMusicStream(path.c_str());
    musicMap[name] = music;
}

const Music& Assets::getMusic(const std::string& name) const{
    return musicMap.at(name);
}

/**
 * Gets the raylib texture based based on name
 * 
 * @param name Texture name
 */
const Texture2D& Assets::getTexture(const std::string& name) const{
    return textureMap.at(name);
}

/**
 * Gets the raylib font asset based on name
 * 
 * @param name Font name
 */
const Font& Assets::getFont(const std::string& name) const{
    return fontMap.at(name);
}

/**
 * Gets the animation asset based on name
 * 
 * @param name Animation name
 */
const Animation& Assets::getAnimation(const std::string& name) const{
    return animationMap.at(name);
}

/**
 * Loads the font using raylib library and adds to the font map data structure
 * 
 * @param name Font name
 * @param path Font path relative to the exe
 */
void Assets::addFont(const std::string& name, const std::string& path){
    Font font=LoadFont(path.c_str());
    fontMap[name]=font;
}

/**
 * Loads the texutre using raylib library and adds to the texture map data structure
 * 
 * @param name Texture name
 * @param path Texture path relative to the exe
 */
void Assets::addTexture(const std::string& name, const std::string& path){
    Texture2D tex=LoadTexture(path.c_str());
    SetTextureFilter(tex,TEXTURE_FILTER_POINT);
    textureMap[name]=tex;
}

/**
 * Creates the animation object from the input information
 * 
 * @param name Animation name
 * @param textureName Texture name for use with this animation
 * @param frameCount Number of frame in this animation
 * @param speed How fast to change each frame when running
 * @param scaleFactor int that determins the up-scale of the texture when animating
 */
void Assets::addAnimation(const std::string& name, const std::string& textureName, int frameCount, int speed, int scaleFactor){
    Animation anim = Animation(name, textureMap[textureName], {(float)scaleFactor,(float)scaleFactor}, frameCount, speed);
    animationMap[name]=anim;
}
