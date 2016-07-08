#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

extern sf::Shader* g_Shader;

void VertexArray::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, VERTEXARRAY_TABLE, Call);
}

int VertexArray::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int VertexArray::Create(lua_State* L)
{
    sf::PrimitiveType type = sf::Points;
    unsigned int p_count = 0;

    if (lua_isnumber(L, 1))
        p_count = lua_tonumber(L, 1);

    if (lua_isnumber(L, 2))
        type = (sf::PrimitiveType)lua_tonumber(L, 2);

    if (p_count < 0)
        p_count = 0;

    sf::VertexArray* varr = LuaAux::newUserData<sf::VertexArray>(L, false, VERTEXARRAY_TABLE);

    if (!varr)
        return 0;

    new (varr) sf::VertexArray(type, p_count);
    return 1;
}

int VertexArray::SetVertex(lua_State* L)
{
    sf::Color color(255, 255, 255);
    bool changeColor = false;
    sf::VertexArray* varr = LuaAux::getUserData<sf::VertexArray>(L, 1, VERTEXARRAY_TABLE);

    if (!varr || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
        return 0;

    sf::Vector2f position((float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4));

    if (lua_istable(L, 5))
    {
        color = LuaAux::getColor(L, 5, true);
        changeColor = true;
    }

    unsigned i = lua_tonumber(L, 2) - 1;

    if (i > varr->getVertexCount())
        return 0;

    if (changeColor)
        (*varr)[i].color = color;

    (*varr)[i].position = position;
    return 0;
}

int VertexArray::SetPrimitive(lua_State* L)
{
    sf::VertexArray* varr = LuaAux::getUserData<sf::VertexArray>(L, 1, VERTEXARRAY_TABLE);

    if (!varr || !lua_isnumber(L, 2))
        return 0;

    varr->setPrimitiveType((sf::PrimitiveType)lua_tonumber(L, 2));
    return 0;
}

int VertexArray::Translate(lua_State* L)
{
    sf::VertexArray* varr = LuaAux::getUserData<sf::VertexArray>(L, 1, VERTEXARRAY_TABLE);

    if (!varr || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    sf::Vector2f delta(lua_tonumber(L, 2), lua_tonumber(L, 3));

    for (int i = (int)varr->getVertexCount() - 1; i >= 0; i--)
        (*varr)[i].position += delta;

    return 0;
}

int VertexArray::Draw(lua_State* L)
{
    sf::VertexArray* varr = LuaAux::getUserData<sf::VertexArray>(L, 1, VERTEXARRAY_TABLE);

    if (!varr)
        return 0;

    sf::RenderWindow* wnd = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);
    sf::RenderTexture* canvas;

    if (wnd)
        wnd->draw(*varr, g_Shader);
    else if ((canvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE)))
        canvas->draw(*varr, g_Shader);

    return 0;
}

int VertexArray::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::VertexArray);
    return 0;
}
