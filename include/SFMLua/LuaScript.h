#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include "SFMLua/LuaAux.h"

namespace LuaScript
{
    void RegisterFunctions(lua_State* L);
    void clearLogFile();
    void notifyLogFile(std::string notification);
    void cout(std::string error);
    void cout(lua_State* L);
    bool setConsole(bool enabled, std::string consoleTitle = "Console");
    void setExitPause(bool enabled);
    void setErrorPause(bool enabled);
    void hideConsole();

    extern std::string fontFile;
    extern std::string logFile;
    extern bool consoleEnabled;
    extern bool exitPauseEnabled;
    extern bool errorPauseEnabled;
    extern bool logConsole;

    int LuaFunction_getCurrentLine(lua_State* L);
    int LuaFunction_enableConsole(lua_State* L);
    int LuaFunction_disableConsole(lua_State* L);
    int LuaFunction_hideConsole(lua_State* L);
    int LuaFunction_getConsoleInput(lua_State* L);
    int LuaFunction_internal_open(lua_State* L);
    int LuaFunction_setExitPause(lua_State* L);
    int LuaFunction_setErrorPause(lua_State* L);
    int LuaFunction_setNotification(lua_State* L);
    int LuaFunction_notify(lua_State* L);
    int LuaFunction_improvedPrint(lua_State* L);
    int LuaFunction_isKeyPressed(lua_State* L);
    int LuaFunction_isMousePressed(lua_State* L);
    int LuaFunction_getMousePosition(lua_State* L);
    int LuaFunction_setMousePosition(lua_State* L);
    int LuaFunction_systemSleep(lua_State* L);
    int LuaFunction_isJoystickOn(lua_State* L);
    int LuaFunction_updateJoystick(lua_State* L);
    int LuaFunction_Reference(lua_State* L);
    int LuaFunction_Alias(lua_State* L);

    void loadBasicFunctions(lua_State* L);

    std::string sfml_init(lua_State* L, int argc, char* args[]);
    int sfml_run(int argc, char* args[]);
    int sfml_close(lua_State* L, bool error);
    int sfml_traceback(lua_State* L);
};

#endif // LUASCRIPT_H
