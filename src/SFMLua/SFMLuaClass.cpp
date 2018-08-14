#include "SFMLua/Classes.h"
#include "SFMLua/SFMLuaClass.h"
#include "SFMLua/LuaAux.h"

LuaData::LuaData() : m_type(LUA_TNIL)
{

}

LuaData::LuaData(lua_State* L, int index)
{
    set(L, index);
}

LuaData::~LuaData()
{
    clear();
}

void LuaData::set(lua_State* L, int index)
{
    clear();
    index = lua_absindex(L, index);
    m_type = lua_type(L, index);

    switch (m_type)
    {
        case LUA_TNIL:
        case LUA_TNONE:

            break;

        case LUA_TTABLE:
        {
            m_tab = new std::map<LuaData*, LuaData*>;

            lua_pushnil(L);

            while (lua_next(L, index) != 0)
            {
                LuaData* key = new LuaData;
                LuaData* value = new LuaData;

                m_tab->insert(std::pair<LuaData*, LuaData*>(key, value));

                key->set(L, -2);
                value->set(L, -1);

                lua_pop(L, 1);
            }

            lua_pop(L, 1);

            break;
        }

        case LUA_TSTRING:
        {
            m_str = new std::string(lua_tostring(L, index), lua_rawlen(L, index));
            break;
        }

        case LUA_TNUMBER:
        {
            m_num = lua_tonumber(L, index);
            break;
        }

        case LUA_TBOOLEAN:
        {
            m_num = lua_toboolean(L, index);
            break;
        }

        case LUA_TFUNCTION:
        {
            m_str = new std::string;

            lua_pushvalue(L, index);

            if (lua_dump(L, LuaAux::dumpFunction, m_str, 1))
                clear();

            lua_pop(L, 1);
            break;
        }

        case LUA_TUSERDATA:
        {
            m_ptr = lua_touserdata(L, index);

            LuaUserData* userdata = static_cast<LuaUserData*>(m_ptr);

            *(userdata->references) += 1;
            *(userdata->aliases) += 1;

            break;
        }

        case LUA_TLIGHTUSERDATA:
        {
            m_ptr = lua_touserdata(L, index);
            break;
        }

        default:
            break;
    }
}

void LuaData::get(lua_State* L) const
{
    std::map<LuaData*, LuaData*>::iterator it, it_end;
    int index;

    switch (m_type)
    {
        case LUA_TNIL: case LUA_TNONE:
        {
            lua_pushnil(L);
            break;
        }

        case LUA_TTABLE:
        {
            lua_newtable(L);
            index = lua_absindex(L, -1);
            it_end = m_tab->end();

            for (it = m_tab->begin(); it != it_end; it++)
            {
                it->first->get(L);
                it->second->get(L);
                lua_rawset(L, index);
            }

            break;
        }

        case LUA_TSTRING:
        {
            lua_pushlstring(L, (const char*) m_str->data(), m_str->size());
            break;
        }

        case LUA_TNUMBER:
        {
            lua_pushnumber(L, m_num);
            break;
        }

        case LUA_TBOOLEAN:
        {
            lua_pushboolean(L, m_num == static_cast<lua_Number>(1.f) );
            break;
        }

        case LUA_TFUNCTION:
        {
            if (luaL_loadbuffer(L, (const char*) m_str->data(), m_str->size(), "") != LUA_OK)
            {
                lua_pop(L, 1);
                lua_pushnil(L);
            }

            break;
        }

        case LUA_TUSERDATA:
        {

            if (!LuaAux::referUserData(L, static_cast<LuaUserData*>(m_ptr)))
                lua_pushnil(L);

            break;
        }

        case LUA_TLIGHTUSERDATA:
        {
            lua_pushlightuserdata(L, m_ptr);
            break;
        }

        default:
            break;
    }
}

void LuaData::clear()
{
    std::map<LuaData*, LuaData*>::iterator it, it_end;

    switch (m_type)
    {
        case LUA_TTABLE:
        {
            it_end = m_tab->end();

            for (it = m_tab->begin(); it != it_end; it++)
            {
                delete it->first;
                delete it->second;
            }

            delete m_tab;
            break;
        }

        case LUA_TSTRING:
        case LUA_TFUNCTION:

            delete m_str;
            break;

        case LUA_TUSERDATA:
        {
            LuaUserData* userdata = static_cast<LuaUserData*>(m_ptr);

            if (userdata->metaname == CONVEXSHAPE_TABLE)
                userdata->gc<sf::ConvexShape>();
            else if (userdata->metaname == FONT_TABLE)
                userdata->gc<sf::Font>();
            else if (userdata->metaname == IMAGE_TABLE)
                userdata->gc<sf::Image>();
            else if (userdata->metaname == GIF_TABLE)
                userdata->gc<Gif::GifImage>();
            else if (userdata->metaname == TCPSOCKET_TABLE)
                userdata->gc<sf::TcpSocket>();
            else if (userdata->metaname == TCPLISTENER_TABLE)
                userdata->gc<sf::TcpListener>();
            else if (userdata->metaname == UDPSOCKET_TABLE)
                userdata->gc<sf::UdpSocket>();
            else if (userdata->metaname == SOCKETSELECTOR_TABLE)
                userdata->gc<sf::SocketSelector>();
            else if (userdata->metaname == REGULARSHAPE_TABLE)
                userdata->gc<sf::CircleShape>();
            else if (userdata->metaname == RENDERTEXTURE_TABLE)
                userdata->gc<sf::RenderTexture>();
            else if (userdata->metaname == WINDOW_TABLE)
                userdata->gc<Window::Window>();
            else if (userdata->metaname == SHADER_TABLE)
                userdata->gc<sf::Shader>();
            else if (userdata->metaname == SPRITE_TABLE)
                userdata->gc<sf::Sprite>();
            else if (userdata->metaname == TEXT_TABLE)
                userdata->gc<sf::Text>();
            else if (userdata->metaname == TEXTURE_TABLE)
                userdata->gc<sf::Texture>();
            else if (userdata->metaname == THREAD_TABLE)
                userdata->gc<SFMLuaThread::LuaThread>();
            else if (userdata->metaname == VERTEXARRAY_TABLE)
                userdata->gc<sf::VertexArray>();
            else if (userdata->metaname == VIEW_TABLE)
                userdata->gc<sf::View>();
            else
            {
                *(userdata->aliases) -= 1;
                *(userdata->references) -= 1;
            }

            break;
        }

        default:
            break;

    }

    m_type = LUA_TNIL;
}

void* LuaUserData::from_userdata_block(void* ud_block)
{
    return static_cast<void*>(static_cast<int16_t*>(ud_block) - 2);
}

void LuaUserData::refer(void* data_ptr)
{
    new (this) LuaUserData;

    this->aliases = static_cast<int16_t*>(data_ptr);
    this->references = this->aliases + 1;
    this->block = this->aliases + 2;

    is_ref = true;
    (*aliases)++;
    (*references)++;
}

void LuaUserData::alias(void* data_ptr)
{
    new (this) LuaUserData;

    this->aliases = static_cast<int16_t*>(data_ptr);
    this->references = this->aliases + 1;
    this->block = this->aliases + 2;

    is_ref = false;
    (*aliases)++;
}
