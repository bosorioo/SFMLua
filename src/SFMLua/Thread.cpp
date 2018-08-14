#include "SFMLua/LuaScript.h"
#include "SFMLua/LuaAux.h"
#include "SFMLua/SFMLuaClass.h"
#include "SFMLua/Classes.h"
#include "SFMLua/Thread.h"

extern "C" {
    #include "LuaGL/LuaGL.h"
}

#include "internal-lua/sfmlua.lua.h"
#include "internal-lua/oop.lua.h"

#include <condition_variable>
#include <mutex>
#include <vector>

std::string luaInitString;
std::vector<SFMLuaThread::LuaThread*> threads;

int runningThreads = 0;
sf::ThreadLocalPtr<SFMLuaThread::LuaThread> ThreadPtr;

SFMLuaThread::LuaThread::~LuaThread()
{
    destroy();
}

void SFMLuaThread::LuaThread::addError(const char* str)
{
    if (error.empty())
        error = str;
    else
    {
        error += '\n';
        error.append(str);
    }
}

void SFMLuaThread::LuaThread::addError(const std::string& str)
{
    if (error.empty())
        error = str;
    else
    {
        error += '\n';
        error.append(str);
    }
}

void SFMLuaThread::LuaThread::destroy()
{
    cancelled = true;
    cond_var.notify_all();

    if (thread.joinable())
        thread.join();

    if (L)
    {
        lua_close(L);
        L = nullptr;
    }

    if (threads.size() > id && threads[id] == this)
        threads[id] = nullptr;
}

void SFMLuaThread::setInitialCode(const std::string& code)
{
    luaInitString = code;
}

bool SFMLuaThread::isAnyRunning()
{
    return runningThreads > 0;
}

void SFMLuaThread::waitThreads()
{
    for (unsigned i = 0; i < threads.size(); i++)
    {
        if (threads[i] && threads[i]->running)
        {
            threads[i]->destroy();
        }
    }
}

void LuaThreadFunc(SFMLuaThread::LuaThread* lt)
{
    ThreadPtr = lt;

    if (lua_pcall(lt->L, 0, 0, 0) != LUA_OK)
        lt->addError(lua_tostring(lt->L, -1));

    runningThreads--;
    lt->running = false;
    lt->finished = true;
}

int LuaThread_Get(lua_State* L)
{
    LuaUserData userdata;
    userdata.metaname = THREAD_TABLE;

    if (lua_isnumber(L, 1))
    {
        unsigned index = lua_tointeger(L, 1);

        if (index >= threads.size())
            return 0;

        if (threads[index] == NULL)
            return 0;

        userdata.aliases = static_cast<int16_t*>(LuaUserData::from_userdata_block(threads[index]));
        LuaAux::aliasUserData(L, &userdata);
        return 1;
    }
    else if (ThreadPtr != NULL)
    {
        userdata.aliases = static_cast<int16_t*>(LuaUserData::from_userdata_block(ThreadPtr));
        LuaAux::aliasUserData(L, &userdata);
        return 1;
    }

    return 0;
}

int LuaThread_GetId(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (lt)
    {
        lua_pushnumber(L, lt->id);
        return 1;
    }
    else if (ThreadPtr != NULL)
    {
        lua_pushnumber(L, ThreadPtr->id);
        return 1;
    }

    return 0;
}

int LuaThread_Create(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::newUserData<SFMLuaThread::LuaThread>(L, true, THREAD_TABLE);

    if (!lt)
        return 0;

    lt->L = luaL_newstate();

    if (!lt->L)
        return 0;

    luaL_openlibs(lt->L);

    if (luaL_dostring(lt->L, luaInitString.c_str()))
    {
        lt->addError(lua_tostring(lt->L, -1));
        lua_pop(lt->L, 1);
    }

    if (lua_gettop(L) == 1 || lua_toboolean(L, 2))
    {
        LuaScript::RegisterFunctions(lt->L);

        if (luaL_loadbuffer(lt->L, lua_script_sfmlua, sizeof(lua_script_sfmlua), "sfmlua (internal script") ||
            lua_pcall(lt->L, 0, LUA_MULTRET, 0))
        {
            lt->addError("sfmlua.lua initialization file failed:");
            lt->addError(lua_tostring(lt->L, -1));
            lua_pop(lt->L, 1);
        }

        if (luaL_loadbuffer(lt->L, lua_script_oop, sizeof(lua_script_oop), "oop (internal script)") ||
            lua_pcall(lt->L, 0, LUA_MULTRET, 0))
        {
            lt->addError("internal lua source 'oop.lua' initialization failed:");
            lt->addError(lua_tostring(lt->L, -1));
            lua_pop(lt->L, 1);
        }

    }
    else
    {
        SFMLuaThread::Register(lt->L);
    }

    if (lua_type(L, 1) == LUA_TSTRING)
    {
        if (luaL_loadbuffer(lt->L, lua_tostring(L, 1), lua_rawlen(L, 1), "Thread Code"))
        {
            lua_pushboolean(L, false);
            lua_pushstring(L, lua_tostring(lt->L, -1));
            lua_close(lt->L);
            return 2;
        }
    }
    else if (lua_type(L, 1) == LUA_TFUNCTION)
    {
        std::string function_buffer;

        lua_pushvalue(L, 1);

        if (lua_dump(L, LuaAux::dumpFunction, &function_buffer, 1))
        {
            lua_close(lt->L);
            return 0;
        }

        lua_pop(L, 1);

        if (luaL_loadbuffer(lt->L, function_buffer.c_str(), function_buffer.size(), "Thread Function"))
        {
            lua_pushboolean(L, false);
            lua_pushstring(L, lua_tostring(lt->L, -1));
            lua_close(lt->L);
            return 2;
        }
    }
    else
    {
        lua_close(lt->L);
        return 0;
    }

    unsigned threadId;

    for (threadId = 0; threadId < threads.size(); threadId++)
        if (threads[threadId] == NULL)
            break;

    lt->id = threadId;

    if (threadId >= threads.size())
    {
        lt->id = threads.size();
        threads.push_back(lt);
    }
    else
        threads[threadId] = lt;

    lua_pushnumber(L, lt->id);
    return 2;
}

int LuaThread_Launch(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    if (lt->thread.joinable())
    {
        lt->cancelled = true;
        lt->thread.join();
        lt->finished = false;
        lt->cancelled = false;
        lt->channel_in.clear();
        lt->channel_out.clear();
        lt->num_table.clear();
        lt->str_table.clear();
    }

    lt->running = true;
    lt->thread = std::thread(std::bind(LuaThreadFunc, lt));

    runningThreads++;
    lua_pushnumber(L, lt->id);
    return 2;
}

int LuaThread_DoLuaString(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);
    size_t code_len;
    const char* code = lua_tolstring(L, 2, &code_len);

    if (!lt || lt->running || code_len == 0)
        return 0;

    bool fail = luaL_loadstring(lt->L, code) || lua_pcall(lt->L, 0, 0, 0);

    if (fail)
        lt->addError(lua_tostring(lt->L, -1));

    lua_pushboolean(L, !fail);
    return 1;
}

int LuaThread_Wait(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt || !lt->running || !lt->thread.joinable())
        return 0;

    lt->thread.join();
    lt->running = false;

    return 1;
}

int LuaThread_Cancel(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    std::unique_lock<std::mutex> lock(lt->mutex);
    lt->cancelled = true;
    lt->cond_var.notify_all();
    return 0;
}

int LuaThread_IsCancelled(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    lua_pushboolean(L, lt->cancelled);
    return 1;
}

int LuaThread_IsRunning(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    lua_pushboolean(L, lt->running);
    return 1;
}

int LuaThread_IsFinished(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    lua_pushboolean(L, lt->finished);
    return 1;
}

int LuaThread_GetError(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    if (!lt->error.empty())
        lua_pushstring(L, lt->error.c_str());
    else
        lua_pushboolean(L, false);

    return 1;
}

int LuaThread_Call(lua_State* L)
{
    lua_remove(L, 1);
    return LuaThread_Create(L);
}

int LuaThread_Kill(lua_State* L)
{
    SFMLuaClassDestroy(SFMLuaThread::LuaThread);
    return 0;
}

int LuaThread_Map(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt)
        return 0;

    int8_t type = lua_type(L, 2);
    bool set = lua_gettop(L) >= 3;

    std::unique_lock<std::mutex> lock(lt->mutex);

    if (set)
    {
        if (type == LUA_TNUMBER)
            lt->num_table[lua_tonumber(L, 2)].set(L, 3);
        else if (type == LUA_TSTRING)
            lt->str_table[lua_tostring(L, 2)].set(L, 3);
    }
    else
    {
        if (type == LUA_TNUMBER)
            lt->num_table[lua_tonumber(L, 2)].get(L);
        else if (type == LUA_TSTRING)
            lt->str_table[lua_tostring(L, 2)].get(L);
        else
            lua_pushnil(L);

        return 1;
    }

    return 0;
}

int LuaThread_Send(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);
    int32_t args = lua_gettop(L);

    if (!lt || args <= 1 || lt->cancelled)
        return 0;

    SFMLuaThread::TChannel* channel;

    if (ThreadPtr == lt)
        channel = &lt->channel_out;
    else
        channel = &lt->channel_in;

    std::unique_lock<std::mutex> lock(lt->mutex);

    LuaData* data;

    for (int32_t index = 2; index <= args; index++)
    {
        channel->push_back(LuaData());
        data = &channel->back();
        data->set(L, index);
    }

    lt->cond_var.notify_all();
    return 0;
}

int LuaThread_Receive(lua_State* L)
{
    SFMLuaThread::LuaThread* lt = LuaAux::getUserData<SFMLuaThread::LuaThread>(L, 1, THREAD_TABLE);

    if (!lt || lt->cancelled)
        return 0;

    SFMLuaThread::TChannel* channel;
    int32_t requests = luaL_optinteger(L, 2, 1), pops = 0;
    bool blocking = lua_toboolean(L, 3);
    LuaData* data;

    if (requests <= 0)
        return 0;

    if (ThreadPtr == lt)
        channel = &lt->channel_in;
    else
        channel = &lt->channel_out;

    std::unique_lock<std::mutex> lock(lt->mutex);

    while (pops < requests && !lt->cancelled)
    {
        if (blocking)
        {
            while (channel->empty() && !lt->cancelled)
                lt->cond_var.wait(lock);

            if (lt->cancelled)
                return pops;
        }
        else if (channel->empty())
            break;

        data = &channel->front();
        data->get(L);
        channel->pop_front();

        pops++;
    }

    return pops;
}

enum SFMLuaLibraries {
    SFMLuaLibraries_Basic = 1 << 0,
    SFMLuaLibraries_Window = 1 << 1,
    SFMLuaLibraries_Image = 1 << 2,
    SFMLuaLibraries_Text = 1 << 3,
    SFMLuaLibraries_Font = 1 << 4,
    SFMLuaLibraries_RenderTexture = 1 << 5,
    SFMLuaLibraries_VertexArray = 1 << 6,
    SFMLuaLibraries_Sprite = 1 << 7,
    SFMLuaLibraries_Texture = 1 << 8,
    SFMLuaLibraries_ConvexShape = 1 << 9,
    SFMLuaLibraries_RegularShape = 1 << 10,
    SFMLuaLibraries_View = 1 << 11,
    SFMLuaLibraries_Shader = 1 << 12,
    SFMLuaLibraries_OpenGL = 1 << 13,
    SFMLuaLibraries_Network = 1 << 14,
    SFMLuaLibraries_GIF = 1 << 15,
    SFMLuaLibraries_Graphics = 1 << 16
};

int32_t LuaThread_LoadLibrary(lua_State* L)
{
    int32_t flags = lua_tonumber(L, 1);

    if (flags & SFMLuaLibraries_Window)
        Window::Register(L);

    if (flags & SFMLuaLibraries_Image)
        Image::Register(L);

    if (flags & SFMLuaLibraries_Text)
        Text::Register(L);

    if (flags & SFMLuaLibraries_Font)
        Font::Register(L);

    if (flags & SFMLuaLibraries_RenderTexture)
        RenderTexture::Register(L);

    if (flags & SFMLuaLibraries_VertexArray)
        VertexArray::Register(L);

    if (flags & SFMLuaLibraries_Sprite)
        Sprite::Register(L);

    if (flags & SFMLuaLibraries_Texture)
        Texture::Register(L);

    if (flags & SFMLuaLibraries_ConvexShape)
        ConvexShape::Register(L);

    if (flags & SFMLuaLibraries_RegularShape)
        RegularShape::Register(L);

    if (flags & SFMLuaLibraries_View)
        View::Register(L);

    if (flags & SFMLuaLibraries_Shader)
        Shader::Register(L);

    if (flags & SFMLuaLibraries_OpenGL)
        lua_opengl(L);

    if (flags & SFMLuaLibraries_Network)
        Network::Register(L);

    if (flags & SFMLuaLibraries_GIF)
        Gif::Register(L);

    if (flags & SFMLuaLibraries_Basic)
        LuaScript::loadBasicFunctions(L);

    if (flags & SFMLuaLibraries_Graphics)
        Graphics::Register(L);

    return 0;
}

static const luaL_Reg Methods [] = {
    {"get", LuaThread_Get},
    {"getId", LuaThread_GetId},
    {"doLuaString", LuaThread_DoLuaString},
    {"create", LuaThread_Create},
    {"launch", LuaThread_Launch},
    {"wait", LuaThread_Wait},
    {"cancel", LuaThread_Cancel},
    {"isCancelled", LuaThread_IsCancelled},
    {"isRunning", LuaThread_IsRunning},
    {"isFinished", LuaThread_IsFinished},
    {"getError", LuaThread_GetError},
    {"map", LuaThread_Map},
    {"send", LuaThread_Send},
    {"receive", LuaThread_Receive},
    {"load", LuaThread_LoadLibrary},
{0, 0}};

static const luaL_Reg Meta[] = {
    {"__gc",    LuaThread_Kill},
    {"__eq", LuaAux::standardUserData__eq},
{0, 0}};

void SFMLuaThread::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, THREAD_TABLE, LuaThread_Call, true);
}
