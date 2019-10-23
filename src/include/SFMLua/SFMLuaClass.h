#ifndef SFMLUACLASS_H
#define SFMLUACLASS_H

#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <limits>
#include "Lua/lua.hpp"
#include <typeinfo>

#define SFMLuaClassDestroy(Class) \
    static_cast<LuaUserData*>( lua_touserdata(L, 1) ) -> gc <Class> ();

class LuaData
{
public:

    LuaData();
    LuaData(lua_State* L, int index);
    ~LuaData();

    void set(lua_State* L, int index);
    void get(lua_State* L) const;
    const int type() const { return m_type; }
    void clear();

private:

    int m_type;

    union {
        void* m_ptr;
        std::string* m_str;
        std::map<LuaData*, LuaData*>* m_tab;
        lua_Number m_num;
    };
};

struct LuaUserData
{
    static void* from_userdata_block(void* ud_block);

    template <class Class>
    void alloc(size_t len, bool default_ctor) {
        new (this) LuaUserData;
        aliases = reinterpret_cast<int16_t*>(new int8_t [len + sizeof(int16_t) * 2]);
        references = aliases + 1;
        block = references + 1;
        is_ref = true;
        *aliases = *references = 1;

        if (default_ctor)
            new (reinterpret_cast<Class*>(block)) Class;
    }

    void refer(void* data_ptr);
    void alias(void* data_ptr);

    template <class C>
    void gc() {

        if (is_ref && *references > 0)
        {
            (*references)--;

            if (*references == 0)
                reinterpret_cast<C*>(block)->~C();
        }

        (*aliases)--;

        if (*aliases == 0)
            delete [] aliases;

        this->~LuaUserData();
    }

    int16_t *block, *aliases, *references;
    std::string metaname;
    bool is_ref;
};

#endif // SFMLUACLASS_H
