#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "Lua/lua.hpp"

namespace Graphics
{
    void Register(lua_State* L);
    sf::RenderTarget* GetTarget();
    void SetTarget(sf::RenderTarget* target);

    int BezierCurve(lua_State* L);
    int Rectangle(lua_State* L);
    int Circle(lua_State* L);
    int Arc(lua_State* L);

    int SetFont(lua_State* L);
    int Print(lua_State* L);

    int SetColor(lua_State* L);
    int GetColor(lua_State* L);

    int SetOutlineColor(lua_State* L);
    int GetOutlineColor(lua_State* L);

    int SetOutlineWidth(lua_State* L);
    int GetOutlineWidth(lua_State* L);

    int SetTarget(lua_State* L);
    int Ellipse(lua_State* L);
    int Line(lua_State* L);
    int Point(lua_State* L);
    int Points(lua_State* L);

    int GetDesktopSize(lua_State* L);
}

#endif // GRAPHICS_H
