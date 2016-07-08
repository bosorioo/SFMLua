#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

namespace Image
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Save(lua_State* L);
    int Resize(lua_State* L);
    int LoadFromFile(lua_State* L);
    int LoadFromString(lua_State* L);
    int Call(lua_State* L);
    int GetSize(lua_State* L);
    int GetPixel(lua_State* L);
    int GetPixelData(lua_State* L);
    int SetPixel(lua_State* L);
    int PixelIndexing(lua_State* L);
    int Grayscale(lua_State* L);
    int GetPixelString(lua_State* L);
    int SetPixelString(lua_State* L);
    int AlphaMask(lua_State* L);
    int PixelTransform(lua_State* L);
    int VerticalFlip(lua_State* L);
    int HorizontalFlip(lua_State* L);
    int CopyPixels(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",        Create},
        {"loadFromFile",  LoadFromFile},
        {"loadFromString",LoadFromString},
        {"saveTo",        Save},
        {"resize",        Resize},
        {"getSize",       GetSize},
        {"getPixel",      GetPixel},
        {"getPixelData",  GetPixelData},
        {"setPixel",      SetPixel},
        {"toGrayscale",   Grayscale},
        {"getPixels",     GetPixelString},
        {"setPixels",     SetPixelString},
        {"alphaMask",     AlphaMask},
        {"flipHorizontally", HorizontalFlip},
        {"flipVertically", VerticalFlip},
        {"copyPixels",    CopyPixels},
        {"pixelTransform",PixelTransform},
    {0, 0}};

     static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
        {"__call",        PixelIndexing},
    {0, 0}};
}

#endif // IMAGE_H_INCLUDED
