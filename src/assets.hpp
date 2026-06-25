#pragma once
#include <string>
#include <raylib.h>
#include <map>
#include "animation.hpp"

/**
 * Contains the ascii code for the first letter of the asset type
 */
enum AssetTypes {SONG = 'S', TEXTURE=84, ANIMATION=65, FONT=70};


/**
 * 
 * Contains maps (string->Asset Type) for Font, Texture, and Animation objects. Maps are filled when a scene loads.
 * 
 * Primarily used to retrieve an asset by string name.
 * 
 */
class Assets{
    private:
        std::map<std::string, Texture2D> textureMap;   /* Map from string texture name to raylib Texture2D struct */
        std::map<std::string, Font> fontMap;           /* Map from string font name to raylib Font struct */
        std::map<std::string, Animation> animationMap; /* Map from string animation name to Animation object */
        std::map<std::string, Music> musicMap;
    public:
        Assets();
        ~Assets();
        void load(const std::string path);
        const Texture2D& getTexture(const std::string& name) const;
        const Font& getFont(const std::string& name) const;
        const Animation& getAnimation(const std::string& name) const;

        void addFont(const std::string& name, const std::string& path);
        void addTexture(const std::string& name, const std::string& path);
        void addAnimation(const std::string& name, const std::string& texture, int frameCount, int speed, int scaleFactor);
        void addMusic(const std::string& name, const std::string& path);
        const Music& getMusic(const std::string& name) const;
};