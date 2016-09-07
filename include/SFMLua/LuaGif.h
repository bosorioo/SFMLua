#ifndef LUAGIF_H_INCLUDED
#define LUAGIF_H_INCLUDED

#include "Giflib/gif_lib.h"
#include <string>
#include <SFML/Graphics.hpp>

namespace LuaGif
{
    void Register(lua_State* L);

    struct GifFrame
    {
        int delay, index, disposal;
        sf::Texture texture;
        sf::Sprite sprite;
        GifFrame* nextFrame;
    };

    struct GifImage
    {
        ~GifImage();

        sf::RenderTexture canvas;
        sf::Sprite sprite;
        sf::Color backgroundColor;
        int frameCount;
        GifFrame* frames;
    };

    void setGifError(std::string& out, int error);
    bool loadGif(GifImage& gif, const char* filePath, std::string& error_out);
    void unloadGif(GifImage& gif);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int LoadFromFile(lua_State* L);
    int GetSprite(lua_State* L);
    int GetCanvas(lua_State* L);
    int GetFrameCount(lua_State* L);
    int SetFrameIndex(lua_State* L);
    int StepFrame(lua_State* L);
    int Draw(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",          Create},
        {"loadFromFile",    LoadFromFile},
        {"getSprite",       GetSprite},
        {"getCanvas",       GetCanvas},
        {"getFrameCount",   GetFrameCount},
        {"setFrameIndex",   SetFrameIndex},
        {"stepFrame",       StepFrame},
        {"draw",            Draw},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // RENDERTEXTURE_H_INCLUDED
