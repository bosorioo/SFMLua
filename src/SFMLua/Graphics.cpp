#include "SFMLua/Graphics.h"
#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"
#include "SFMLua/Utils.h"
#include <SFML/OpenGL.hpp>

sf::Font g_Font;
sf::RenderTarget* g_Target = nullptr;
sf::Color g_drawingColor (255, 255, 255);
sf::Color g_outlineColor (255, 255, 255);
float g_outlineWidth = 0.f;

extern sf::Shader* g_Shader;

void Graphics::Register(lua_State* L)
{
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "setFont", SetFont);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "rectangle", Rectangle);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "arc", Arc);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "circle", Circle);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "ellipse", Ellipse);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "line", Line);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "point", Point);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "print", Print);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "points", Points);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "setColor", SetColor);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "setOutlineColor", SetOutlineColor);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "getColor", GetColor);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "getOutlineColor", GetOutlineColor);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "setOutlineWidth", SetOutlineWidth);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "getOutlineWidth", GetOutlineWidth);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "setTarget", SetTarget);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "getDesktopSize", GetDesktopSize);
    LuaAux::newGlobal(L, GRAPHICS_TABLE, "bezierCurve", BezierCurve);
}

void Graphics::SetTarget(sf::RenderTarget* target)
{
    g_Target = target;
}

sf::RenderTarget* Graphics::GetTarget()
{
    return g_Target;
}

int Graphics::SetFont(lua_State* L)
{
    sf::Font* font;

    if (lua_isstring(L, 1))
        lua_pushboolean(L, g_Font.loadFromFile(lua_tostring(L, 1)));
    else if ((font = LuaAux::getUserData<sf::Font>(L, 1, FONT_TABLE)))
    {
        g_Font = *font;
        lua_pushboolean(L, true);
    }
    else
    {
        g_Font.~Font();
        lua_pushnil(L);
    }

    return 1;
}

int Graphics::Print(lua_State* L)
{
    if (!g_Target)
        return 0;

    sf::Text txt("", g_Font, 12);
    txt.setColor(g_drawingColor);

    if (lua_isstring(L, 1))
        txt.setString(lua_tostring(L, 1));

    if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
        txt.move(lua_tonumber(L, 2), lua_tonumber(L, 3));

    if (lua_istable(L, 4))
    {
        lua_getfield(L, 4, "color"); lua_pop(L, 1);
        if (lua_istable(L, 0))
            txt.setColor(LuaAux::getColorFromTable(L, 0));

        lua_getfield(L, 4, "style"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            txt.setStyle(lua_tonumber(L, 0));

        lua_getfield(L, 4, "size"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            txt.setCharacterSize(lua_tonumber(L, 0));
    }

    g_Target->draw(txt);
    return 0;
}

int Graphics::SetColor(lua_State* L)
{
    g_drawingColor = LuaAux::getColor(L, 1, lua_gettop(L) == 1);
    return 0;
}

int Graphics::SetOutlineColor(lua_State* L)
{
    g_outlineColor = LuaAux::getColor(L, 1, lua_gettop(L) == 1);
    return 0;
}

int Graphics::GetColor(lua_State* L)
{
    lua_pushnumber(L, g_drawingColor.r);
    lua_pushnumber(L, g_drawingColor.g);
    lua_pushnumber(L, g_drawingColor.b);
    lua_pushnumber(L, g_drawingColor.a);
    return 4;
}

int Graphics::GetOutlineColor(lua_State* L)
{
    lua_pushnumber(L, g_outlineColor.r);
    lua_pushnumber(L, g_outlineColor.g);
    lua_pushnumber(L, g_outlineColor.b);
    lua_pushnumber(L, g_outlineColor.a);
    return 4;
}

int Graphics::SetOutlineWidth(lua_State* L)
{
    g_outlineWidth = lua_tonumber(L, 1);
    return 0;
}

int Graphics::GetOutlineWidth(lua_State* L)
{
    lua_pushnumber(L, g_outlineWidth);
    return 1;
}

int Graphics::Rectangle(lua_State* L)
{
    if (!g_Target)
        return 0;

    static sf::RectangleShape rect;
    bool fill = lua_toboolean(L, 1);

    if (!lua_gettop(L))
    {
        g_Target->draw(rect, g_Shader);
        return 0;
    }

    if (fill)
    {
        rect.setFillColor(g_drawingColor);
        rect.setOutlineColor(g_outlineColor);
        rect.setOutlineThickness(g_outlineWidth);
    }
    else
    {
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(g_drawingColor);
        rect.setOutlineThickness(-1.f);
    }

    if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
    {
        rect.setPosition(lua_tonumber(L, 2), lua_tonumber(L, 3));
    }

    if (lua_isnumber(L, 4) && lua_isnumber(L, 5))
        rect.setSize(sf::Vector2f(lua_tonumber(L, 4), lua_tonumber(L, 5)));

    g_Target->draw(rect, g_Shader);
    return 0;
}

int Graphics::Arc(lua_State* L)
{
    if (!g_Target)
        return 0;

    bool filled(lua_toboolean(L, 1));
    sf::Vector2f position(lua_tonumber(L, 2), lua_tonumber(L, 3));
    float radius(lua_tonumber(L, 4)), ang1(lua_tonumber(L, 5)), ang2(lua_tonumber(L, 6));
    unsigned segments(lua_tointeger(L, 7) + 1);
    float angvar = (ang2 - ang1) / segments;

    if (segments < 2)
        return 0;

    segments += filled + 1;

    sf::Vertex verts[segments];
    sf::Vertex* ptr = verts;

    if (filled)
        ptr++->position = position;

    for (unsigned i = filled; i < segments; i++)
    {
        if (i != filled)
            ang1 += angvar;

        ptr->color = g_drawingColor;
        ptr->position = sf::Vector2f(std::cos(ang1) * radius + position.x, std::sin(ang1) * radius + position.y);
        ptr++;
    }

    g_Target->draw(verts, segments, filled ? sf::TrianglesFan : sf::LinesStrip, g_Shader);
    return 0;
}

int Graphics::Circle(lua_State* L)
{
    if (!g_Target)
        return 0;

    static sf::CircleShape circle;

    if (!lua_gettop(L))
    {
        g_Target->draw(circle, g_Shader);
        return 0;
    }

    if (lua_toboolean(L, 1))
    {
        circle.setFillColor(g_drawingColor);
        circle.setOutlineThickness(0.);
    }
    else if (lua_isboolean(L, 1))
    {
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(g_drawingColor);
        circle.setOutlineThickness(g_outlineWidth);
    }

    if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
        circle.setPosition(lua_tonumber(L, 2), lua_tonumber(L, 3));

    if (lua_isnumber(L, 4))
    {
        circle.move(-lua_tonumber(L, 4), -lua_tonumber(L, 4));
        circle.setRadius(lua_tonumber(L, 4));
    }

    if (lua_isnumber(L, 5))
        circle.setPointCount((unsigned)lua_tointeger(L, 5));

    g_Target->draw(circle, g_Shader);
    return 0;
}

int Graphics::SetTarget(lua_State* L)
{
    Window::Window* wnd = LuaAux::getUserData<Window::Window>(L, 1, WINDOW_TABLE);
    sf::RenderTexture* rtx;

    if (wnd)
    {
        g_Target = wnd;
        wnd->setActive();
    }
    else if ((rtx = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE)))
    {
        g_Target = rtx;
        rtx->setActive();
    }

    return 0;
}

int Graphics::GetDesktopSize(lua_State* L)
{
    lua_pushinteger(L, sf::VideoMode::getDesktopMode().width);
    lua_pushinteger(L, sf::VideoMode::getDesktopMode().height);
    return 2;
}

int Graphics::Ellipse(lua_State* L)
{
    if (!g_Target)
        return 0;

    static sf::VertexArray ellipse;

    if (!lua_gettop(L))
    {
        g_Target->draw(ellipse, g_Shader);
        return 0;
    }

    bool filled(lua_toboolean(L, 1));
    sf::Vector2f position(lua_tonumber(L, 2), lua_tonumber(L, 3));
    float a(lua_tonumber(L, 4)), b(lua_tonumber(L, 5));
    unsigned segments(lua_tointeger(L, 6));
    float from = 0., to = 2 * 3.141592;

    if (lua_isnumber(L, 7) && lua_isnumber(L, 8))
    {
        from = lua_tonumber(L, 7);
        to = lua_tonumber(L, 8);
    }

    ellipse.resize(segments + 1 + filled);

    float delta = (to - from) / segments;
    unsigned i = 0;

    sf::Vertex& vert = ellipse[0];

    ellipse.setPrimitiveType(filled ? sf::TrianglesFan : sf::LinesStrip);

    if (filled)
    {
        vert.color = g_drawingColor;
        vert.position = position;
        i++;
    }

    vert = ellipse[i];
    vert.color = g_drawingColor;
    vert.position.x = position.x + a * std::cos(from);
    vert.position.y = position.y - b * std::sin(from);

    float t = from + delta;

    for (i++; i < segments + filled; i++, t = t + delta)
    {
        vert = ellipse[i];
        vert.color = g_drawingColor;
        vert.position.x = position.x + a * std::cos(t);
        vert.position.y = position.y - b * std::sin(t);
    }

    ellipse[segments + filled].color = g_drawingColor;
    ellipse[segments + filled].position.x = position.x + a * std::cos(to);
    ellipse[segments + filled].position.y = position.y - b * std::sin(to);

    g_Target->draw(ellipse, g_Shader);
    return 0;
}

int Graphics::Line(lua_State* L)
{
    if (!g_Target)
        return 0;

    sf::Vertex verts[2];
    verts[0].color = verts[1].color = g_drawingColor;

    while (lua_gettop(L) >= 4)
    {
        verts[0].position = sf::Vector2f(lua_tonumber(L, 1), lua_tonumber(L, 2));
        verts[1].position = sf::Vector2f(lua_tonumber(L, 3), lua_tonumber(L, 4));
        LuaAux::popBottom(L, 4);
        g_Target->draw(verts, 2, sf::Lines, g_Shader);
    }
    return 0;
}

int Graphics::Point(lua_State* L)
{
    if (!g_Target)
        return 0;

    int32_t points = lua_gettop(L) / 2, index = 0;

    sf::Vertex vertices[points];

    while (index < points)
    {
        vertices[index].color = g_drawingColor;
        vertices[index].position = sf::Vector2f(lua_tonumber(L, 2 * index + 1), lua_tonumber(L, 2 * index + 2));
    }

    g_Target->draw(vertices, points, sf::Points, g_Shader);

    return 0;
}

int Graphics::Points(lua_State* L)
{
    if (!g_Target || !lua_istable(L, 1))
        return 0;

    sf::PrimitiveType type = sf::LinesStrip;
    sf::Color color = g_drawingColor;
    unsigned length = lua_rawlen(L, 1);

    if (length % 2)
        length--;

    if (lua_isnumber(L, 2))
        type = (sf::PrimitiveType)lua_tointeger(L, 2);
    else if (lua_isnumber(L, 3))
        type = (sf::PrimitiveType)lua_tointeger(L, 3);

    sf::Vertex verts[length / 2];
    sf::Vertex* ptr = verts;
    int colorTableIndex = 0;

    if (lua_istable(L, 2) && lua_rawlen(L, 2) * 2 >= length)
        colorTableIndex = 2;
    else if (lua_istable(L, 3) && lua_rawlen(L, 3) * 2 >= length)
        colorTableIndex = 3;

    if (colorTableIndex)
    {
        for (unsigned i = 0; i < length;)
        {
            lua_rawgeti(L, 1, ++i); lua_pop(L, 1);
            ptr->position.x = lua_tonumber(L, 0);

            lua_rawgeti(L, 1, ++i); lua_pop(L, 1);
            ptr->position.y = lua_tonumber(L, 0);

            lua_rawgeti(L, colorTableIndex, i / 2); lua_pop(L, 1);
            ptr->color = sf::Color(lua_tointeger(L, 0));
            ptr++;
        }
    }
    else
    {
        for (unsigned i = 0; i < length;)
        {
            lua_rawgeti(L, 1, ++i); lua_pop(L, 1);
            ptr->position.x = lua_tonumber(L, 0);

            lua_rawgeti(L, 1, ++i); lua_pop(L, 1);
            ptr->position.y = lua_tonumber(L, 0);

            ptr->color = color;
            ptr++;
        }

    }
    g_Target->draw(verts, length / 2, type, g_Shader);
    return 0;
}

int Graphics::BezierCurve(lua_State *L)
{
    int count = lua_gettop(L), i;
    float step = 0.01;

    if (count & 1)
    {
        step = lua_tonumber(L, 1);
        lua_remove(L, 1);
        count = (count - 1)/2;
    }

    Utils::BezierPoint points[count];

    for (i = 0; i < count; i++)
    {
        points[i].x = lua_tonumber(L, i * 2 + 1);
        points[i].y = lua_tonumber(L, i * 2 + 2);
    }

    lua_newtable(L);
    Utils::BezierPoint temp;

    int n = 1;
    for (float t = 0; t <= 1; t += step)
    {
        temp = Utils::BezierCurvePoint(points, t, count);
        lua_pushnumber(L, temp.x);
        lua_rawseti(L, -2, n++);
        lua_pushnumber(L, temp.y);
        lua_rawseti(L, -2, n++);
    }
    return 1;
}
