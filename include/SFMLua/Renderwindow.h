#ifndef RENDERWINDOW_H_INCLUDED
#define RENDERWINDOW_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Lua/lua.hpp"
#include "SFMLua/LuaAux.h"

namespace Window
{
    void Register(lua_State* L);

    class Window : public sf::RenderWindow
    {
        public:
            ~Window();
    };

    void CloseAll(void);

    int Create(lua_State* L);
    int GetHandle(lua_State* L);
    int Call(lua_State* L);
    int PollEvents(lua_State* L);
    int Display(lua_State* L);
    int Close(lua_State* L);
    int Clear(lua_State* L);
    int IsOpen(lua_State* L);
    int GetPosition(lua_State* L);
    int SetPosition(lua_State* L);
    int SetTitle(lua_State* L);
    int SetVisible(lua_State* L);
    int SetVsync(lua_State* L);
    int SetMouseVisible(lua_State* L);
    int SetKeyRepeat(lua_State* L);
    int SetActive(lua_State* L);
    int Print(lua_State* L);
    int Refresh(lua_State* L);
    int SetIcon(lua_State* L);
    int Points(lua_State* L);
    int SetView(lua_State* L);
    int GetView(lua_State* L);
    int TranslateView(lua_State* L);
    int ResetView(lua_State* L);
    int SaveGLStates(lua_State* L);
    int RestoreGLStates(lua_State* L);
    int ResetGLStates(lua_State* L);
    int SetFocus(lua_State* L);
    int GetFocus(lua_State* L);
    int SetSize(lua_State* L);
    int GetSize(lua_State* L);
    int Capture(lua_State* L);
    int MapPixelPosition(lua_State* L);
    int MapCoordPosition(lua_State* L);

    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",          Create},
        {"getHandle",       GetHandle},
        {"getEvents",       PollEvents},
        {"points",          Points},
        {"display",         Display},
        {"clear",           Clear},
        {"isOpen",          IsOpen},
        {"close",           Close},
        {"getSize",         GetSize},
        {"setSize",         SetSize},
        {"getPosition",     GetPosition},
        {"setPosition",     SetPosition},
        {"setTitle",        SetTitle},
        {"setVisible",      SetVisible},
        {"setVerticalSync", SetVsync},
        {"setMouseVisible", SetMouseVisible},
        {"setKeyRepeat",    SetKeyRepeat},
        {"setActive",       SetActive},
        {"print",           Print},
        {"refresh",         Refresh},
        {"setIcon",         SetIcon},
        {"setView",         SetView},
        {"getView",         GetView},
        {"translateView",   TranslateView},
        {"requestFocus",    SetFocus},
        {"hasFocus",        GetFocus},
        {"resetView",       ResetView},
        {"saveGLStates",    SaveGLStates},
        {"restoreGLStates", RestoreGLStates},
        {"resetGLStates",   ResetGLStates},
        {"capture",         Capture},
        {"mapPixelPosition",MapPixelPosition},
        {"mapCoordPosition",MapCoordPosition},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",            Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // RENDERWINDOW_H_INCLUDED
