#ifndef RENDERTEXTURE_H_INCLUDED
#define RENDERTEXTURE_H_INCLUDED

namespace RenderTexture
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int Display(lua_State* L);
    int Draw(lua_State* L);
    int Clear(lua_State* L);
    int GetImage(lua_State* L);
    int SetSmooth(lua_State* L);
    int SetActive(lua_State* L);
    int Destroy(lua_State* L);
    int GetSize(lua_State* L);
    int MapPixelPosition(lua_State* L);
    int MapCoordPosition(lua_State* L);
    int SetView(lua_State* L);
    int GetView(lua_State* L);
    int GetTexture(lua_State* L);
    int GetSprite(lua_State* L);
    int ResetView(lua_State* L);
    int TranslateView(lua_State* L);
    int SaveGLStates(lua_State* L);
    int RestoreGLStates(lua_State* L);
    int ResetGLStates(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",      Create},
        {"display",     Display},
        {"draw",        Draw},
        {"clear",       Clear},
        {"getImage",    GetImage},
        {"setSmooth",   SetSmooth},
        {"setActive",   SetActive},
        {"getSize",     GetSize},
        {"setView",         SetView},
        {"getView",         GetView},
        {"getTexture",      GetTexture},
        {"getSprite",       GetSprite},
        {"resetView",       ResetView},
        {"translateView",   TranslateView},
        {"saveGLStates",    SaveGLStates},
        {"restoreGLStates", RestoreGLStates},
        {"resetGLStates",   ResetGLStates},
        {"mapPixelPosition",MapPixelPosition},
        {"mapCoordPosition",MapCoordPosition},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // RENDERTEXTURE_H_INCLUDED
