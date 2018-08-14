#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "SFML/System.hpp"
#include "internal-lua/sfmlua.lua.h"
#include "internal-lua/oop.lua.h"
#include "internal-lua/traceback.lua.h"
#include "sfmlua/LuaScript.h"
#include "sfmlua/Classes.h"
#include "sfmlua/Utils.h"

#if defined(_WIN32)
    #if !defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0502
        #undef _WIN32_WINNT
        #define _WIN32_WINNT 0x0502
    #endif
    #include "windows.h"
#endif

extern "C" {
    #include "LuaGL/LuaGL.h"
}

sf::Mutex consoleMutex;
std::string LuaScript::logFile;//("sfmlua.log.txt");
std::ofstream log_file;
bool LuaScript::consoleEnabled = false;
bool LuaScript::exitPauseEnabled = false;
bool LuaScript::errorPauseEnabled = false;
bool LuaScript::logConsole = true;

lua_State* L = nullptr;

struct closeSignal {
    bool close = false;
};

closeSignal* cs = new closeSignal;

void LuaStop(lua_State *L, lua_Debug *ar)
{
    lua_pushnil(L);
    lua_error(L);
}

void signal_handler(int param)
{
    if (cs->close)
        return;

    Network::CloseSockets();
    lua_sethook(L, LuaStop, LUA_MASKCOUNT, 1);

    #ifdef _WIN32
    DWORD inputsWritten;
    INPUT_RECORD buffer;
    KEY_EVENT_RECORD keyEvent;
    buffer.EventType = KEY_EVENT;
    keyEvent.bKeyDown = 1;
    keyEvent.wRepeatCount = 1;
    keyEvent.wVirtualKeyCode = VK_RETURN;
    keyEvent.wVirtualScanCode = MapVirtualKey(VK_RETURN, 0);;
    keyEvent.uChar.AsciiChar = '\r';
    keyEvent.dwControlKeyState = 0;
    std::memcpy(&buffer.Event, &keyEvent, sizeof(keyEvent));
    WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &buffer, 1, &inputsWritten);
    #endif // _WIN32

    while (cs->close == false)
        sf::sleep(sf::milliseconds(15));
}

void LuaScript::clearLogFile()
{
    if (!LuaScript::logFile.empty())
    {
        if (log_file.is_open())
            log_file.close();

        log_file.open(LuaScript::logFile.c_str(), std::ios::out);
    }
}

void LuaScript::notifyLogFile(std::string notification)
{
    if (log_file.is_open())
    {
        log_file << notification << "\n";
        log_file.flush();
    }
}

void LuaScript::cout(std::string error)
{
    if (log_file.is_open())
        notifyLogFile(error);

    if (LuaScript::logConsole)
        std::cout << error << "\n";
}

void LuaScript::cout(lua_State* L)
{
    LuaScript::cout(LuaAux::getString(L, -1));
}

bool LuaScript::setConsole(bool enabled, std::string consoleTitle)
{
    #if defined(_WIN32)
    if (enabled && !consoleEnabled)
    {
        if (AllocConsole() == 0)
        {
            std::cerr << "Console allocation failed.\n";
            LuaScript::consoleEnabled = false;
            return false;
        }

        const int MAX_CONSOLE_LINES = 5000;
        CONSOLE_SCREEN_BUFFER_INFO console_info;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
        console_info.dwSize.Y = MAX_CONSOLE_LINES;
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), console_info.dwSize);
        SetConsoleTitle(TEXT(consoleTitle.c_str()));

        FILE* fp;

        fp = freopen("CONOUT$", "w", stdout);
        if (!fp) std::cout << "Redirection of stdout for console creation failed.\n";

        fp = freopen("CONIN$", "r", stdin);
        if (!fp) std::cout << "Redirection of stdin for console creation failed.\n";

        fp = freopen("CONOUT$", "w", stderr);
        if (!fp) std::cout << "Redirection of stderr for console creation failed.\n";
    }
    else if (!enabled && consoleEnabled)
    {
        HWND consoleWindow = GetConsoleWindow();

        bool freed = FreeConsole() != 0;

        if (!freed)
            std::cerr << "Console free failed (" << GetLastError() << ").\n";

        bool hidden = ShowWindow(consoleWindow, SW_HIDE);

        if (!hidden)
            std::cerr << "Console hiding failed (" << GetLastError() << ").\n";
    }

    #endif

    LuaScript::consoleEnabled = enabled;

    return true;
}

void LuaScript::setExitPause(bool enabled)
{
    LuaScript::exitPauseEnabled = enabled;
}

void LuaScript::setErrorPause(bool enabled)
{
    LuaScript::errorPauseEnabled = enabled;
}

void LuaScript::hideConsole()
{
    #ifdef _WIN32

    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);

    #endif // _WIN32
}

void LuaScript::showConsole()
{
    #ifdef _WIN32

    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_SHOW);

    #endif // _WIN32
}

int LuaScript::sfml_run(int argc, char* args[])
{
    L = luaL_newstate();

    if (!L)
        return 2;

    luaL_openlibs(L);
    LuaScript::RegisterFunctions(L);

    std::string entryFile = LuaScript::sfml_init(L, argc, args);

    if (luaL_loadbuffer(L, lua_script_sfmlua, sizeof(lua_script_sfmlua), "sfmlua (internal script") ||
        lua_pcall(L, 0, LUA_MULTRET, 0))
    {
        LuaScript::cout("sfmlua.lua initialization file failed.");
        LuaScript::cout(L);
        //LuaScript::sfml_close(L, false);
        //return 3;
    }

    if (luaL_loadbuffer(L, lua_script_oop, sizeof(lua_script_oop), "oop (internal script)") ||
        lua_pcall(L, 0, LUA_MULTRET, 0))
    {
        LuaScript::cout("internal lua source 'oop.lua' initialization failed.");
        LuaScript::cout(L);
        //LuaScript::sfml_close(L, false);
        //return 3;
    }

    for (int i = 0; i < argc; i++)
    {
        if (memcmp(args[i], "-lua", 4) == 0)
        {
            if (luaL_dostring(L, &args[i][4]))
            {
                std::string error("Command line script in argument #");
                std::stringstream ss;
                ss << i;
                error += ss.str() + " couldn't run.";
                LuaScript::cout(error);
                LuaScript::cout(L);
            }
        }
    }

    if (entryFile.empty())
    {
        LuaScript::cout("main.lua or entry point not found.");
        LuaScript::sfml_close(L, false);
        return 4;
    }

    lua_pushcfunction(L, sfml_traceback);
    int index = lua_gettop(L);
    bool error = false;

    if (luaL_loadfile(L, entryFile.c_str()))
    {
        error = true;

        std::vector<std::string> errSplit = Utils::splitString(lua_tostring(L, -1), ':');

        if (errSplit.size() >= 3)
        {

            std::string str = "Main file loading fail:\n\t";

            if (errSplit[0].size() == 1) // Path started with something like C:/
            {
                errSplit[1].insert(0, errSplit[0] + ':');
                errSplit.erase(errSplit.begin());
            }

            str += errSplit[2];
            LuaScript::cout(str);
            str = "\t "; str += Utils::getFileName(errSplit[0]);
            str += " on line "; str += errSplit[1];
            LuaScript::cout(str);
        }
        else
        {
            LuaScript::cout("Syntax error:");
            LuaScript::cout(lua_tostring(L, -1));
        }
    }
    else if (lua_pcall(L, 0, LUA_MULTRET, index))
    {
        error = true;
    }


    return LuaScript::sfml_close(L, error);
}

std::string LuaScript::sfml_init(lua_State* L, int argc, char* args[])
{
    std::string entryFile = LuaAux::getEntryFile(argc, args);
    std::string globalStrings[3];

    LuaAux::newGlobal(L, "WorkDir", (globalStrings[0] = Utils::getDirectory(entryFile)));
    LuaAux::newGlobal(L, "ExecDir", (globalStrings[1] = Utils::getDirectory(*args)));
    LuaAux::newGlobal(L, "ExeName", (globalStrings[2] = Utils::getFileName(*args)));

    logFile = globalStrings[1] + "sfml log.txt";
    log_file.open(logFile.c_str(), std::ios::out);
    notifyLogFile("Lua script entry file: " + entryFile);

    std::string argumentsNotification("\nArguments:");

    lua_newtable(L);
    for (int i = 0; i < argc; i++)
    {
        lua_pushstring(L, args[i]);
        lua_rawseti(L, -2, i + 1);
        argumentsNotification += "\n\t";
        argumentsNotification += args[i];
    }

    lua_setglobal(L, "Arguments");
    notifyLogFile(argumentsNotification + "\n");
    argumentsNotification.clear();

    SFMLuaThread::setInitialCode(std::string(
        "WorkDir = '" + Utils::getDoubleBackslash(globalStrings[0]) + "'\n" +
        "ExecDir = '" + Utils::getDoubleBackslash(globalStrings[1]) + "'\n" +
        "ExeName = '" + Utils::getDoubleBackslash(globalStrings[2]) + "'\n"
    ));

    #if defined(_WIN32)
    LuaScript::consoleEnabled = GetConsoleWindow() != NULL;
    #endif // defined

    return entryFile;
}

int LuaScript::sfml_close(lua_State* L, bool error)
{
    Window::CloseAll();

    if (LuaScript::consoleEnabled && (LuaScript::exitPauseEnabled || (error && LuaScript::errorPauseEnabled)))
        LuaScript::showConsole();

    if (SFMLuaThread::isAnyRunning())
    {
        LuaScript::cout("Waiting for threads to finish...");
        SFMLuaThread::waitThreads();
    }

    LuaScript::cout("Program finished.");

    if (L)
        lua_close(L);

    cs->close = true;

    log_file.close();

    if (LuaScript::consoleEnabled)
    {
        if (LuaScript::exitPauseEnabled || (LuaScript::errorPauseEnabled && error))
        {
            LuaScript::cout("Press enter to exit...");
            std::cin.get();
        }

        LuaScript::setConsole(false);
    }

    return error ? 1 : 0;
}

int LuaScript::sfml_traceback(lua_State* L)
{
    std::string err = LuaAux::getString(L, -1);
    bool failed = false;

    if (luaL_dostring(L, lua_script_traceback))
        failed = true;

    if (!failed)
    {
        lua_getglobal(L, "onTrace");
        luaL_traceback(L, L, NULL, 0);
        lua_pushstring(L, err.c_str());

        if (lua_pcall(L, 2, LUA_MULTRET, 0))
            failed = true;
    }

    if (failed)
    {
        luaL_traceback(L, L, NULL, 0);

        LuaScript::cout("Unformatted traceback:\n");
        LuaScript::cout(lua_tostring(L, -1));

        LuaScript::cout("\nLua error:");
        LuaScript::cout(err);
    }

    return 0;
}

int LuaScript::LuaFunction_getCurrentLine(lua_State* L)
{
    lua_pushnumber(L, LuaAux::getCurrentLine(L));
    return 1;
}

int LuaScript::LuaFunction_enableConsole(lua_State* L)
{
    lua_pushboolean(L, LuaScript::setConsole(true, LuaAux::getString(L, 1)));
    return 1;
}

int LuaScript::LuaFunction_disableConsole(lua_State* L)
{
    LuaScript::setConsole(false);
    return 0;
}

int LuaScript::LuaFunction_hideConsole(lua_State* L)
{
    LuaScript::hideConsole();
    return 0;
}

int LuaScript::LuaFunction_showConsole(lua_State* L)
{
    LuaScript::showConsole();
    return 0;
}

int LuaScript::LuaFunction_getConsoleInput(lua_State *L)
{
    if (!LuaScript::consoleEnabled)
        return 0;

    //sf::Lock lock(consoleMutex);

    int maxLength = luaL_optnumber(L, 1, -1);
    std::string input;
    char c;

    do {
        std::cin.get(c);
        if (c == '\n')
            break;
        else if (maxLength)
        {
            maxLength--;
            input += c;
        }
    } while (1);

    lua_pushstring(L, input.c_str());
    return 1;
}

int LuaScript::LuaFunction_setExitPause(lua_State* L)
{
    LuaScript::setExitPause(LuaAux::getBool(L, 1, true));
    return 0;
}

int LuaScript::LuaFunction_setErrorPause(lua_State* L)
{
    LuaScript::setErrorPause(LuaAux::getBool(L, 1, true));
    return 0;
}

int LuaScript::LuaFunction_setNotification(lua_State* L)
{
    LuaScript::logConsole = LuaAux::getBool(L, 1, false);
    LuaScript::logFile = LuaAux::getString(L, 2);
    return 0;
}

int LuaScript::LuaFunction_notify(lua_State* L)
{
    LuaScript::cout(LuaAux::getString(L, 1));
    return 0;
}

int LuaScript::LuaFunction_improvedPrint(lua_State* L)
{
    static bool beginOfLine = true;
    // sf::Lock lock(consoleMutex);

    if (lua_gettop(L) == 0)
    {
        if (!beginOfLine)
        {
            std::cout << "\n";
            beginOfLine = true;
        }

        return 0;
    }

    int nargs = lua_gettop(L);
    int type = LUA_TBOOLEAN;
    int i;
    bool called_tostring;

    for (i = 1; i <= nargs; i++)
    {
        called_tostring = false;
        type = lua_type(L, i);

        if ((type == LUA_TUSERDATA || type == LUA_TTABLE) && lua_getmetatable(L, i))
        {
            lua_pushstring(L, "__tostring");
            lua_rawget(L, -2);

            if (lua_isfunction(L, -1))
            {
                called_tostring = true;
                lua_pushvalue(L, i);
                lua_call(L, 1, 1);
                lua_replace(L, i);
            }
            else if (lua_isstring(L, -1))
            {
                called_tostring = true;
                lua_replace(L, i);
            }

            lua_pop(L, 1);
            type = lua_type(L, i);
        }

        switch (type)
        {
            case LUA_TSTRING: {std::cout << lua_tostring(L, i); break;}
            case LUA_TBOOLEAN: {std::cout << (lua_toboolean(L, i) ? "true" : "false"); break;}
            case LUA_TNUMBER: {
                lua_Number ln = lua_tonumber(L, i);
                uint64_t ui = static_cast<uint64_t>(ln);
                 int64_t ii = static_cast< int64_t>(ln);
                if ((lua_Number)ui == ln)
                {
                    std::cout << ui;
                }
                else if ((lua_Number)ii == ln)
                {
                    std::cout << ii;
                }
                else
                {
                    char buffer[32];
                    int bytes = snprintf(buffer, sizeof(buffer), "%.8f", ln);

                    while (bytes-- > 0)
                    {
                        if (buffer[bytes] == '0' &&
                            buffer[bytes - 1] != ',' &&
                            buffer[bytes - 1] != '.')
                        {
                            buffer[bytes] = '\0';
                        }
                        else
                        {
                            break;
                        }
                    }

                    std::cout << buffer;
                }
                break;
            }
            case LUA_TNIL: {std::cout << "nil"; break;}
            case LUA_TUSERDATA:
            {
                std::cout << lua_typename(L, type) << ": ";
                std::cout << static_cast<const LuaUserData*>(lua_topointer(L, i))->block;
                break;
            }
            default: {std::cout << lua_typename(L, type) << ": " << lua_topointer(L, i); break;}
        }

        if (i != nargs)
            std::cout << "\t";
    }

    if (type != LUA_TSTRING || called_tostring)
    // variable type holds the lua_type for the last printed argument
    // If it is a string, user must give linefeed by himself, otherwise,
    // print endl
    {
        beginOfLine = true;
        std::cout << "\n";
    }
    else
    {
        beginOfLine = *(lua_tostring(L, i - 1) + lua_rawlen(L, i - 1) - 1) == '\n';
    }

    return 0;
}

int LuaScript::LuaFunction_isKeyPressed(lua_State* L)
{
    lua_pushboolean(L, sf::Keyboard::isKeyPressed((sf::Keyboard::Key)lua_tointeger(L, 1)));
    return 1;
}

int LuaScript::LuaFunction_isMousePressed(lua_State* L)
{
    lua_pushboolean(L, sf::Mouse::isButtonPressed((sf::Mouse::Button)lua_tointeger(L, 1)));
    return 1;
}

int LuaScript::LuaFunction_getMousePosition(lua_State* L)
{
    sf::Vector2i pos;
    Window::Window* window = LuaAux::getUserData<Window::Window>(L, 1, WINDOW_TABLE);

    if (window)
        pos = sf::Mouse::getPosition(*window);
    else
        pos = sf::Mouse::getPosition();

    lua_pushinteger(L, pos.x);
    lua_pushinteger(L, pos.y);
    return 2;
}

int LuaScript::LuaFunction_setMousePosition(lua_State* L)
{

    if (sf::RenderWindow* window = LuaAux::getUserData<sf::RenderWindow>(L, 1, WINDOW_TABLE))
        sf::Mouse::setPosition(sf::Vector2i(luaL_optnumber(L, 2, 0), luaL_optnumber(L, 3, 0)), *window);
    else
        sf::Mouse::setPosition(sf::Vector2i(luaL_optnumber(L, 1, 0), luaL_optnumber(L, 2, 0)));

    return 0;
}

int LuaScript::LuaFunction_systemSleep(lua_State* L)
{
    sf::sleep(sf::milliseconds(luaL_optnumber(L, 1, 10)));
    return 0;
}

int LuaScript::LuaFunction_isJoystickOn(lua_State* L)
{
    lua_pushboolean(L, sf::Joystick::isConnected(lua_tointeger(L, 1)));
    return 1;
}

int LuaScript::LuaFunction_updateJoystick(lua_State* L)
{
    sf::Joystick::update();
    return 0;
}

int LuaScript::LuaFunction_Reference(lua_State* L)
{
    LuaUserData* ref_ptr = nullptr;

    if (lua_isnumber(L, 1))
    {
        ref_ptr = reinterpret_cast<LuaUserData*>(lua_tointeger(L, 1));
    }
    else
    {
        switch (lua_type(L, 1))
        {
            case LUA_TUSERDATA:
            case LUA_TLIGHTUSERDATA:

                ref_ptr = static_cast<LuaUserData*>(lua_touserdata(L, 1));
                break;

            default:
                break;
        }
    }

    if (!ref_ptr || !LuaAux::referUserData(L, ref_ptr))
        return 0;

    return 1;
}

int LuaScript::LuaFunction_Alias(lua_State* L)
{
    LuaUserData* ref_ptr = nullptr;

    if (lua_isnumber(L, 1))
    {
        ref_ptr = reinterpret_cast<LuaUserData*>(lua_tointeger(L, 1));
    }
    else
    {
        switch (lua_type(L, 1))
        {
            case LUA_TUSERDATA:
            case LUA_TLIGHTUSERDATA:

                ref_ptr = static_cast<LuaUserData*>(lua_touserdata(L, 1));
                break;

            default:
                break;
        }
    }

    if (!ref_ptr || !LuaAux::aliasUserData(L, ref_ptr))
        return 0;

    return 1;
}

void LuaScript::loadBasicFunctions(lua_State* L)
{
    // number = debug.getline()
    LuaAux::newGlobal(L, "debug", "getline", LuaFunction_getCurrentLine);

    // Console.open(; string: title)
    LuaAux::newGlobal(L, "Console", "open", LuaFunction_enableConsole);
    // Console.close()
    LuaAux::newGlobal(L, "Console", "close", LuaFunction_disableConsole);
    // Console.hide()
    LuaAux::newGlobal(L, "Console", "hide", LuaFunction_hideConsole);
    // Console.show()
    LuaAux::newGlobal(L, "Console", "show", LuaFunction_showConsole);
    // string = Console.getInput(; number: maxLength)
    LuaAux::newGlobal(L, "Console", "getInput", LuaFunction_getConsoleInput);
    // Console.setPauseOnExit(; boolean: enabled)
    LuaAux::newGlobal(L, "Console", "setPauseOnExit", LuaFunction_setExitPause);
    // Console.setPauseOnExit(; boolean: enabled)
    LuaAux::newGlobal(L, "Console", "setPauseOnError", LuaFunction_setErrorPause);
    // Console.setNotification(string: logFile, boolean: logConsole)
    LuaAux::newGlobal(L, "Console", "setNotification", LuaFunction_setNotification);
    // Console.notify(string: msg)
    LuaAux::newGlobal(L, "Console", "notify", LuaFunction_notify);

     // System.sleep(number: miliseconds = 10)
    LuaAux::newGlobal(L, "System", "sleep", LuaFunction_systemSleep);

    lua_getglobal(L, "print");
    lua_setglobal(L, "_print");

    // print(...)
    LuaAux::newGlobal(L, "print", LuaFunction_improvedPrint);

    LuaAux::newGlobal(L, "Reference", LuaFunction_Reference);
    LuaAux::newGlobal(L, "Alias", LuaFunction_Alias);
}

void LuaScript::RegisterFunctions (lua_State* L)
{

    LuaScript::loadBasicFunctions(L);
    // Joystick.update()
    LuaAux::newGlobal(L, "Joystick", "update", LuaFunction_updateJoystick);
    // boolean = Joystick.isConnected(number: joystickId)
    LuaAux::newGlobal(L, "Joystick", "isConnected", LuaFunction_isJoystickOn);

    // boolean = Mouse.isButtonDown(number: buttonId)
    LuaAux::newGlobal(L, "Mouse", "isButtonDown", LuaFunction_isMousePressed);
    // number: x, number: y = Mouse.getPosition(; userdata: window)
    LuaAux::newGlobal(L, "Mouse", "getPosition", LuaFunction_getMousePosition);
    // Mouse.setPosition([userdata: window,] number: x, number: y)
    LuaAux::newGlobal(L, "Mouse", "setPosition", LuaFunction_setMousePosition);

    // boolean = Keys.isKeyDown(number: keyId)
    LuaAux::newGlobal(L, "Keys", "isKeyDown", LuaFunction_isKeyPressed);

    LuaAux::newGlobal (L, "Keys", "Unknown", sf::Keyboard::Unknown);
    LuaAux::newGlobal (L, "Keys", "A", sf::Keyboard::A);
    LuaAux::newGlobal (L, "Keys", "B", sf::Keyboard::B);
    LuaAux::newGlobal (L, "Keys", "C", sf::Keyboard::C);
    LuaAux::newGlobal (L, "Keys", "D", sf::Keyboard::D);
    LuaAux::newGlobal (L, "Keys", "E", sf::Keyboard::E);
    LuaAux::newGlobal(L, "Keys", "F", sf::Keyboard::F);
    LuaAux::newGlobal(L, "Keys", "G", sf::Keyboard::G);
    LuaAux::newGlobal(L, "Keys", "H", sf::Keyboard::H);
    LuaAux::newGlobal(L, "Keys", "I", sf::Keyboard::I);
    LuaAux::newGlobal(L, "Keys", "J", sf::Keyboard::J);
    LuaAux::newGlobal(L, "Keys", "K", sf::Keyboard::K);
    LuaAux::newGlobal(L, "Keys", "L", sf::Keyboard::L);
    LuaAux::newGlobal(L, "Keys", "M", sf::Keyboard::M);
    LuaAux::newGlobal(L, "Keys", "N", sf::Keyboard::N);
    LuaAux::newGlobal(L, "Keys", "O", sf::Keyboard::O);
    LuaAux::newGlobal(L, "Keys", "P", sf::Keyboard::P);
    LuaAux::newGlobal(L, "Keys", "Q", sf::Keyboard::Q);
    LuaAux::newGlobal(L, "Keys", "R", sf::Keyboard::R);
    LuaAux::newGlobal(L, "Keys", "S", sf::Keyboard::S);
    LuaAux::newGlobal(L, "Keys", "T", sf::Keyboard::T);
    LuaAux::newGlobal(L, "Keys", "U", sf::Keyboard::U);
    LuaAux::newGlobal(L, "Keys", "V", sf::Keyboard::V);
    LuaAux::newGlobal(L, "Keys", "W", sf::Keyboard::W);
    LuaAux::newGlobal(L, "Keys", "X", sf::Keyboard::X);
    LuaAux::newGlobal(L, "Keys", "Y", sf::Keyboard::Y);
    LuaAux::newGlobal(L, "Keys", "Z", sf::Keyboard::Z);
    LuaAux::newGlobal(L, "Keys", "Num0", sf::Keyboard::Num0);
    LuaAux::newGlobal(L, "Keys", "Num1", sf::Keyboard::Num1);
    LuaAux::newGlobal(L, "Keys", "Num2", sf::Keyboard::Num2);
    LuaAux::newGlobal(L, "Keys", "Num3", sf::Keyboard::Num3);
    LuaAux::newGlobal(L, "Keys", "Num4", sf::Keyboard::Num4);
    LuaAux::newGlobal(L, "Keys", "Num5", sf::Keyboard::Num5);
    LuaAux::newGlobal(L, "Keys", "Num6", sf::Keyboard::Num6);
    LuaAux::newGlobal(L, "Keys", "Num7", sf::Keyboard::Num7);
    LuaAux::newGlobal(L, "Keys", "Num8", sf::Keyboard::Num8);
    LuaAux::newGlobal(L, "Keys", "Num9", sf::Keyboard::Num9);
    LuaAux::newGlobal(L, "Keys", "Escape", sf::Keyboard::Escape);
    LuaAux::newGlobal(L, "Keys", "LControl", sf::Keyboard::LControl);
    LuaAux::newGlobal(L, "Keys", "LShift", sf::Keyboard::LShift);
    LuaAux::newGlobal(L, "Keys", "LAlt", sf::Keyboard::LAlt);
    LuaAux::newGlobal(L, "Keys", "LSystem", sf::Keyboard::LSystem);
    LuaAux::newGlobal(L, "Keys", "RControl", sf::Keyboard::RControl);
    LuaAux::newGlobal(L, "Keys", "RShift", sf::Keyboard::RShift);
    LuaAux::newGlobal(L, "Keys", "RAlt", sf::Keyboard::RAlt);
    LuaAux::newGlobal(L, "Keys", "RSystem", sf::Keyboard::RSystem);
    LuaAux::newGlobal(L, "Keys", "Menu", sf::Keyboard::Menu);
    LuaAux::newGlobal(L, "Keys", "LBracket", sf::Keyboard::LBracket);
    LuaAux::newGlobal(L, "Keys", "RBracket", sf::Keyboard::RBracket);
    LuaAux::newGlobal(L, "Keys", "SemiColon", sf::Keyboard::SemiColon);
    LuaAux::newGlobal(L, "Keys", "Comma", sf::Keyboard::Comma);
    LuaAux::newGlobal(L, "Keys", "Period", sf::Keyboard::Period);
    LuaAux::newGlobal(L, "Keys", "Quote", sf::Keyboard::Quote);
    LuaAux::newGlobal(L, "Keys", "Slash", sf::Keyboard::Slash);
    LuaAux::newGlobal(L, "Keys", "BackSlash", sf::Keyboard::BackSlash);
    LuaAux::newGlobal(L, "Keys", "Tilde", sf::Keyboard::Tilde);
    LuaAux::newGlobal(L, "Keys", "Equal", sf::Keyboard::Equal);
    LuaAux::newGlobal(L, "Keys", "Dash", sf::Keyboard::Dash);
    LuaAux::newGlobal(L, "Keys", "Space", sf::Keyboard::Space);
    LuaAux::newGlobal(L, "Keys", "Return", sf::Keyboard::Return);
    LuaAux::newGlobal(L, "Keys", "BackSpace", sf::Keyboard::BackSpace);
    LuaAux::newGlobal(L, "Keys", "Tab", sf::Keyboard::Tab);
    LuaAux::newGlobal(L, "Keys", "PageUp", sf::Keyboard::PageUp);
    LuaAux::newGlobal(L, "Keys", "PageDown", sf::Keyboard::PageDown);
    LuaAux::newGlobal(L, "Keys", "End", sf::Keyboard::End);
    LuaAux::newGlobal(L, "Keys", "Home", sf::Keyboard::Home);
    LuaAux::newGlobal(L, "Keys", "Insert", sf::Keyboard::Insert);
    LuaAux::newGlobal(L, "Keys", "Delete", sf::Keyboard::Delete);
    LuaAux::newGlobal(L, "Keys", "Add", sf::Keyboard::Add);
    LuaAux::newGlobal(L, "Keys", "Subtract", sf::Keyboard::Subtract);
    LuaAux::newGlobal(L, "Keys", "Multiply", sf::Keyboard::Multiply);
    LuaAux::newGlobal(L, "Keys", "Divide", sf::Keyboard::Divide);
    LuaAux::newGlobal(L, "Keys", "Left", sf::Keyboard::Left);
    LuaAux::newGlobal(L, "Keys", "Right", sf::Keyboard::Right);
    LuaAux::newGlobal(L, "Keys", "Up", sf::Keyboard::Up);
    LuaAux::newGlobal(L, "Keys", "Down", sf::Keyboard::Down);
    LuaAux::newGlobal(L, "Keys", "Numpad0", sf::Keyboard::Numpad0);
    LuaAux::newGlobal(L, "Keys", "Numpad1", sf::Keyboard::Numpad1);
    LuaAux::newGlobal(L, "Keys", "Numpad2", sf::Keyboard::Numpad2);
    LuaAux::newGlobal(L, "Keys", "Numpad3", sf::Keyboard::Numpad3);
    LuaAux::newGlobal(L, "Keys", "Numpad4", sf::Keyboard::Numpad4);
    LuaAux::newGlobal(L, "Keys", "Numpad5", sf::Keyboard::Numpad5);
    LuaAux::newGlobal(L, "Keys", "Numpad6", sf::Keyboard::Numpad6);
    LuaAux::newGlobal(L, "Keys", "Numpad7", sf::Keyboard::Numpad7);
    LuaAux::newGlobal(L, "Keys", "Numpad8", sf::Keyboard::Numpad8);
    LuaAux::newGlobal(L, "Keys", "Numpad9", sf::Keyboard::Numpad9);
    LuaAux::newGlobal(L, "Keys", "F1", sf::Keyboard::F1);
    LuaAux::newGlobal(L, "Keys", "F2", sf::Keyboard::F2);
    LuaAux::newGlobal(L, "Keys", "F3", sf::Keyboard::F3);
    LuaAux::newGlobal(L, "Keys", "F4", sf::Keyboard::F4);
    LuaAux::newGlobal(L, "Keys", "F5", sf::Keyboard::F5);
    LuaAux::newGlobal(L, "Keys", "F6", sf::Keyboard::F6);
    LuaAux::newGlobal(L, "Keys", "F7", sf::Keyboard::F7);
    LuaAux::newGlobal(L, "Keys", "F8", sf::Keyboard::F8);
    LuaAux::newGlobal(L, "Keys", "F9", sf::Keyboard::F9);
    LuaAux::newGlobal(L, "Keys", "F10", sf::Keyboard::F10);
    LuaAux::newGlobal(L, "Keys", "F11", sf::Keyboard::F11);
    LuaAux::newGlobal(L, "Keys", "F12", sf::Keyboard::F12);
    LuaAux::newGlobal(L, "Keys", "F13", sf::Keyboard::F13);
    LuaAux::newGlobal(L, "Keys", "F14", sf::Keyboard::F14);
    LuaAux::newGlobal(L, "Keys", "F15", sf::Keyboard::F15);
    LuaAux::newGlobal(L, "Keys", "Pause", sf::Keyboard::Pause);
    LuaAux::newGlobal(L, "Keys", "KeyCount", sf::Keyboard::KeyCount);

    LuaAux::newGlobal(L, "Events", "Closed", sf::Event::Closed);
    LuaAux::newGlobal(L, "Events", "KeyPressed", sf::Event::KeyPressed);
    LuaAux::newGlobal(L, "Events", "KeyReleased", sf::Event::KeyReleased);
    LuaAux::newGlobal(L, "Events", "Resized", sf::Event::Resized);
    LuaAux::newGlobal(L, "Events", "MousePressed", sf::Event::MouseButtonPressed);
    LuaAux::newGlobal(L, "Events", "MouseReleased", sf::Event::MouseButtonReleased);
    LuaAux::newGlobal(L, "Events", "MouseWheel", sf::Event::MouseWheelMoved);
    LuaAux::newGlobal(L, "Events", "MouseMoved", sf::Event::MouseMoved);
    LuaAux::newGlobal(L, "Events", "FocusOn", sf::Event::GainedFocus);
    LuaAux::newGlobal(L, "Events", "FocusOff", sf::Event::LostFocus);
    LuaAux::newGlobal(L, "Events", "MouseEntered", sf::Event::MouseEntered);
    LuaAux::newGlobal(L, "Events", "MouseLeft", sf::Event::MouseLeft);
    LuaAux::newGlobal(L, "Events", "TextEntered", sf::Event::TextEntered);
    LuaAux::newGlobal(L, "Events", "JoystickOn", sf::Event::JoystickConnected);
    LuaAux::newGlobal(L, "Events", "JoystickOff", sf::Event::JoystickDisconnected);
    LuaAux::newGlobal(L, "Events", "JoystickDown", sf::Event::JoystickButtonPressed);
    LuaAux::newGlobal(L, "Events", "JoystickUp", sf::Event::JoystickButtonReleased);
    LuaAux::newGlobal(L, "Events", "JoystickMoved", sf::Event::JoystickMoved);
    LuaAux::newGlobal(L, "Events", "TouchBegan", sf::Event::TouchBegan);
    LuaAux::newGlobal(L, "Events", "TouchEnded", sf::Event::TouchEnded);
    LuaAux::newGlobal(L, "Events", "TouchMoved", sf::Event::TouchMoved);

    LuaAux::newGlobal(L, "Mouse", "Left", sf::Mouse::Left);
    LuaAux::newGlobal(L, "Mouse", "Right", sf::Mouse::Right);
    LuaAux::newGlobal(L, "Mouse", "Middle", sf::Mouse::Middle);
    LuaAux::newGlobal(L, "Mouse", "X1", sf::Mouse::XButton1);
    LuaAux::newGlobal(L, "Mouse", "X2", sf::Mouse::XButton2);

    LuaAux::newGlobal(L, "Styles", "Bold", sf::Text::Bold);
    LuaAux::newGlobal(L, "Styles", "Italic", sf::Text::Italic);
    LuaAux::newGlobal(L, "Styles", "Underlined", sf::Text::Underlined);

    LuaAux::newGlobal(L, "Styles", "Titlebar", sf::Style::Titlebar);
    LuaAux::newGlobal(L, "Styles", "Close", sf::Style::Close);
    LuaAux::newGlobal(L, "Styles", "Fullscreen", sf::Style::Fullscreen);
    LuaAux::newGlobal(L, "Styles", "None", sf::Style::None);
    LuaAux::newGlobal(L, "Styles", "Resize", sf::Style::Resize);

    LuaAux::newGlobal(L, "Vertex", "Lines", sf::Lines);
    LuaAux::newGlobal(L, "Vertex", "LinesStrip", sf::LinesStrip);
    LuaAux::newGlobal(L, "Vertex", "Points", sf::Points);
    LuaAux::newGlobal(L, "Vertex", "Quads", sf::Quads);
    LuaAux::newGlobal(L, "Vertex", "Triangles", sf::Triangles);
    LuaAux::newGlobal(L, "Vertex", "TrianglesFan", sf::TrianglesFan);
    LuaAux::newGlobal(L, "Vertex", "TrianglesStrip", sf::TrianglesStrip);


    Window::Register(L);
    Image::Register(L);
    Text::Register(L);
    Font::Register(L);
    RenderTexture::Register(L);
    VertexArray::Register(L);
    Sprite::Register(L);
    Texture::Register(L);
    ConvexShape::Register(L);
    RegularShape::Register(L);
    View::Register(L);
    Shader::Register(L);
    SFMLuaThread::Register(L);
    lua_opengl(L);
    Network::Register(L);
    Gif::Register(L);
    Graphics::Register(L);
}
