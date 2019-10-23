#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <string>
#include <map>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "SFMLua/SFMLuaClass.h"

namespace SFMLuaThread
{
    void Register(lua_State* L);

    typedef std::map<lua_Number, LuaData> IMap;
    typedef std::map<std::string, LuaData> SMap;
    typedef std::deque<LuaData> TChannel;

    struct LuaThread
    {
        LuaThread() {L = NULL; running = cancelled = finished = false;}
        ~LuaThread();

        void addError(const char* str);
        void addError(const std::string& str);
        void destroy();

        lua_State *L;

        unsigned id;
        std::string error;
        std::thread thread;

        std::mutex mutex;
        std::condition_variable cond_var;

        IMap num_table;
        SMap str_table;
        TChannel channel_out, channel_in;

        bool running, cancelled, finished;
    };

    void setInitialCode(const std::string& code);
    bool isAnyRunning();
    void waitThreads();
}

#endif // THREAD_H_INCLUDED
