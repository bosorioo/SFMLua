#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"
#include "SFMLua/LuaScript.h"

#include <string>
#include <list>
#include <cmath>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

std::list<Window::Window*> g_Windows;

void Window::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, WINDOW_TABLE, Call);
}

void Window::CloseAll()
{
    for (auto it = g_Windows.begin(); it != g_Windows.end(); it++)
        if ((*it)->isOpen())
            (*it)->close();

    g_Windows.clear();
    Graphics::SetTarget(static_cast<sf::RenderTarget*>(nullptr));
}

int Window::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Window::Create(lua_State* L)
{
    if(lua_isuserdata(L, 1))
    {
        Window* window = LuaAux::newUserData<Window>(L, false, WINDOW_TABLE);

        if (!window)
            return 0;

        Window* target = LuaAux::getUserData<Window>(L, 2);

        if (target)
            new (window) sf::RenderWindow(target->getSystemHandle());
        else
            new (window) sf::RenderWindow(sf::WindowHandle(lua_touserdata(L, 1)));

        g_Windows.push_back(window);
        Graphics::SetTarget(window);

        return 1;
    }

    sf::VideoMode mode(800, 600);
    sf::String caption = "Untitled";
    sf::Uint32 style = sf::Style::Default;
    sf::ContextSettings settings;

    settings.stencilBits = 8;

    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "height"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            mode.height = lua_tointeger(L, 0);

        lua_getfield(L, 1, "width"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            mode.width = lua_tointeger(L, 0);

        lua_getfield(L, 1, "antialiasing"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            settings.antialiasingLevel = lua_tointeger(L, 0);

        lua_getfield(L, 1, "depthbits"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            settings.depthBits = lua_tointeger(L, 0);

        lua_getfield(L, 1, "stencilbits"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            settings.stencilBits = lua_tointeger(L, 0);

        lua_getfield(L, 1, "major"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            settings.majorVersion = lua_tointeger(L, 0);

        lua_getfield(L, 1, "minor"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            settings.minorVersion = lua_tointeger(L, 0);

        lua_getfield(L, 1, "style"); lua_pop(L, 1);
        if (lua_isnumber(L, 0))
            style = lua_tointeger(L, 0);

        if (style & sf::Style::Fullscreen)
        {
            style = sf::Style::Fullscreen;
            bool found = false;
            std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
            for (std::size_t i = 0; i < modes.size(); ++i)
            {
                mode = modes[i];
                if (mode.isValid())
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                mode = sf::VideoMode::getDesktopMode();
        }
        else if (style & sf::Style::None)
            style = sf::Style::None;

        lua_getfield(L, 1, "title"); lua_pop(L, 1);
        if (!lua_isnil(L, 0))
            caption = sf::String(lua_tostring(L, 0));
    }

    Window* window = LuaAux::newUserData<Window>(L, false, WINDOW_TABLE);

    if (!window)
        return 0;

    new (window) sf::RenderWindow(mode, caption, style, settings);
    g_Windows.push_back(window);
    Graphics::SetTarget(window);

    return 1;
}

int Window::Capture(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    sf::Image* image = LuaAux::newUserData<sf::Image>(L, true, IMAGE_TABLE);

    if (!image)
        return 0;

    image->create(window->getSize().x, window->getSize().y, window->capture().getPixelsPtr());
    return 1;
}

int32_t Lua_pollEventIteratorFunction(lua_State* L)
{
    int upval = lua_upvalueindex(1);
    int index = lua_tonumber(L, upval);
    lua_rawgeti(L, 1, index);
    index++;
    lua_pushnumber(L, index);
    lua_replace(L, upval);
    return 1;
}

int Window::PollEvents(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    bool isBlocking = LuaAux::getBool(L, 2, false);
    sf::Event event;

    lua_newtable(L);

    int index = 0;

    #ifdef _WIN32
    if (isBlocking)
    {
        WaitMessage();
    }
    #endif // _WIN32

    while (isBlocking ? window->waitEvent(event) : window->pollEvent(event))
    {
        isBlocking = false;
        index++;
        lua_pushinteger(L, index);
        lua_newtable(L);
        lua_pushinteger(L, event.type);
        lua_setfield(L, -2, "type");
        if (event.type == sf::Event::KeyPressed ||
            event.type == sf::Event::KeyReleased)
        {
            lua_pushboolean(L, event.key.control);
            lua_setfield(L, -2, "control");
            lua_pushboolean(L, event.key.system);
            lua_setfield(L, -2, "system");
            lua_pushboolean(L, event.key.alt);
            lua_setfield(L, -2, "alt");
            lua_pushboolean(L, event.key.shift);
            lua_setfield(L, -2, "shift");
            lua_pushinteger(L, event.key.code);
            lua_setfield(L, -2, "keycode");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, event.type == sf::Event::KeyPressed ? "KeyPressed" : "KeyReleased");
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            lua_pushnumber(L, event.mouseMove.x);
            lua_setfield(L, -2, "x");
            lua_pushnumber(L, event.mouseMove.y);
            lua_setfield(L, -2, "y");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "MouseMoved");
        }
        else if (event.type == sf::Event::MouseButtonPressed ||
                 event.type == sf::Event::MouseButtonReleased)
        {
            lua_pushnumber(L, event.mouseButton.x);
            lua_setfield(L, -2, "x");
            lua_pushnumber(L, event.mouseButton.y);
            lua_setfield(L, -2, "y");
            lua_pushnumber(L, event.mouseButton.button);
            lua_setfield(L, -2, "button");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, event.type == sf::Event::MouseButtonPressed ? "MousePressed" : "MouseReleased");
        }
        else if (event.type == sf::Event::Resized)
        {
            lua_pushnumber(L, event.size.width);
            lua_setfield(L, -2, "width");
            lua_pushnumber(L, event.size.height);
            lua_setfield(L, -2, "height");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "Resized");
        }
        else if (event.type == sf::Event::MouseWheelMoved)
        {
            lua_pushnumber(L, event.mouseWheel.delta);
            lua_setfield(L, -2, "delta");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "MouseWheel");
        }
        else if (event.type == sf::Event::TextEntered)
        {
            lua_pushnumber(L, event.text.unicode);
            lua_setfield(L, -2, "unicode");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "TextEntered");
        }
        else if (event.type == sf::Event::JoystickConnected ||
                 event.type == sf::Event::JoystickDisconnected)
        {
            lua_pushnumber(L, event.joystickButton.joystickId);
            lua_setfield(L, -2, "id");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, event.type == sf::Event::JoystickConnected ? "JoystickConnected" : "JoystickDisconnected");
        }
        else if (event.type == sf::Event::JoystickButtonPressed ||
                 event.type == sf::Event::JoystickButtonReleased)
        {
            lua_pushnumber(L, event.joystickButton.joystickId);
            lua_setfield(L, -2, "id");
            lua_pushnumber(L, event.joystickButton.button);
            lua_setfield(L, -2, "button");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, event.type == sf::Event::JoystickButtonPressed ? "JoystickPressed" : "JoystickReleased");
        }
        else if (event.type == sf::Event::JoystickMoved)
        {
            lua_pushnumber(L, event.joystickMove.joystickId);
            lua_setfield(L, -2, "id");
            lua_pushnumber(L, event.joystickMove.axis);
            lua_setfield(L, -2, "axis");
            lua_pushnumber(L, event.joystickMove.position);
            lua_setfield(L, -2, "value");
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "JoystickMoved");

        }
        else if (event.type == sf::Event::TouchBegan ||
                 event.type == sf::Event::TouchEnded ||
                 event.type == sf::Event::TouchMoved)
        {
            lua_pushnumber(L, event.touch.finger);
            lua_setfield(L, -2, "finger");
            lua_pushnumber(L, event.touch.x);
            lua_setfield(L, -2, "x");
            lua_pushnumber(L, event.touch.y);
            lua_setfield(L, -2, "y");
        }
        else if (event.type == sf::Event::Closed)
        {
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "Closed");
        }
        else if (event.type == sf::Event::GainedFocus)
        {
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "FocusOn");
        }
        else if (event.type == sf::Event::LostFocus)
        {
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "FocusOff");
        }
        else if (event.type == sf::Event::MouseLeft)
        {
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "MouseLeft");
        }
        else if (event.type == sf::Event::MouseEntered)
        {
            lua_pushboolean(L, true);
            lua_setfield(L, -2, "MouseEntered");
        }

        lua_settable(L, -3);
    }

    lua_newtable(L); // create new metatable to make the table iterable in loops
    lua_pushstring(L, "__call"); // key
    lua_pushnumber(L, 1); // value (the upvalue of the closure, meaning the index of event in the table)
    lua_pushcclosure(L, Lua_pollEventIteratorFunction, 1);
    lua_rawset(L, -3); // metatable.__call = cclosure
    lua_setmetatable(L, -2); // setmetatable(event_table, metatable)

    return 1;
}

int Window::Refresh(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
            break;
        }
    }

    return 0;
}

int Window::SetIcon(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 2, IMAGE_TABLE);

    if (!window || !image)
        return 0;

    sf::Vector2u dim = image->getSize();
    window->setIcon(dim.x, dim.y, image->getPixelsPtr());
    return 0;
}

int Window::Display(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->display();
    return 0;
}

int Window::Clear(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    if (lua_gettop(L) > 1)
        window->clear(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    else
        window->clear();
    return 0;
}

int Window::IsOpen(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    lua_pushboolean(L, window->isOpen());
    return 1;
}

int Window::SetFocus(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->requestFocus();
    return 0;
}

int Window::GetFocus(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    lua_pushboolean(L, window->hasFocus());
    return 1;
}

int Window::SetPosition(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    window->setPosition(sf::Vector2i(lua_tointeger(L, 2), lua_tointeger(L, 3)));
    return 0;
}

int Window::SetSize(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    window->setSize(sf::Vector2u(lua_tointeger(L, 2), lua_tointeger(L, 3)));
    return 0;
}

int Window::GetPosition(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    sf::Vector2i pos = window->getPosition();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Window::GetHandle(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    lua_pushlightuserdata(L, window->getSystemHandle());
    return 1;
}

int Window::GetSize(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    sf::Vector2u pos = window->getSize();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Window::MapPixelPosition(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);
    sf::View* view = LuaAux::getUserData<sf::View>(L, 2, VIEW_TABLE);

    if (!window)
        return 0;

    sf::Vector2f pos;

    if (view)
        pos = window->mapPixelToCoords(sf::Vector2i(lua_tonumber(L, 2), lua_tonumber(L, 3)), *view);
    else
        pos = window->mapPixelToCoords(sf::Vector2i(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Window::MapCoordPosition(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);
    sf::View* view = LuaAux::getUserData<sf::View>(L, 2, VIEW_TABLE);

    if (!window)
        return 0;

    sf::Vector2i pos;

    if (view)
        pos = window->mapCoordsToPixel(sf::Vector2f(lua_tonumber(L, 2), lua_tonumber(L, 3)), *view);
    else
        pos = window->mapCoordsToPixel(sf::Vector2f(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Window::SetTitle(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window || !lua_isstring(L, 2))
        return 0;

    window->setTitle(sf::String(lua_tostring(L, 2)));
    return 0;
}

int Window::SetVisible(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->setVisible(LuaAux::getBool(L, 2, true));
    return 0;
}

int Window::SetVsync(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->setVerticalSyncEnabled(LuaAux::getBool(L, 2, true));
    return 0;
}

int Window::SaveGLStates(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->pushGLStates();
    return 0;
}

int Window::RestoreGLStates(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->popGLStates();
    return 0;
}

int Window::ResetGLStates(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->resetGLStates();
    return 0;
}

int Window::SetMouseVisible(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->setMouseCursorVisible(LuaAux::getBool(L, 2, true));
    return 0;
}

int Window::SetKeyRepeat(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->setKeyRepeatEnabled(LuaAux::getBool(L, 2, true));
    return 0;
}

int Window::SetActive(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->setActive(LuaAux::getBool(L, 2, true));
    return 0;
}

int Window::Points(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    Graphics::SetTarget(window);
    lua_remove(L, 1);
    return Graphics::Points(L);
}

int Window::Print(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    Graphics::SetTarget(window);
    lua_remove(L, 1);
    return Graphics::Print(L);
}

int Window::SetView(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);
    sf::View* view = LuaAux::getUserData<sf::View>(L, 2, VIEW_TABLE);

    if (!window || !view)
        return 0;

    window->setView(*view);
    return 0;
}

int Window::GetView(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    sf::View* view = LuaAux::newUserData<sf::View>(L, true, VIEW_TABLE);

    if (view)
        *view = window->getView();

    return 1;
}

int Window::TranslateView(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    sf::View v = window->getView();
    v.move(luaL_optnumber(L, 2, 0), luaL_optnumber(L, 3, 0));
    window->setView(v);
    return 0;
}

int Window::ResetView(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    window->setView(window->getDefaultView());
    return 0;
}


int Window::Close(lua_State* L)
{
    Window* window = LuaAux::getUserData<Window>(L, 1, WINDOW_TABLE);

    if (!window)
        return 0;

    auto it = std::find(g_Windows.begin(), g_Windows.end(), window);

    if (it != g_Windows.end())
        g_Windows.erase(it);

    if (Graphics::GetTarget() == window)
        Graphics::SetTarget(static_cast<sf::RenderTarget*>(nullptr));

    window->close();
    return 0;
}

int Window::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(Window);
    return 0;
}

Window::Window::~Window()
{
    auto it = std::find(g_Windows.begin(), g_Windows.end(), this);

    if (it != g_Windows.end())
        g_Windows.erase(it);

    if (Graphics::GetTarget() == this)
        Graphics::SetTarget(static_cast<sf::RenderTarget*>(nullptr));
}
