#include "foolib.h"
#include "lua.hpp"
#include <iostream>

int echo(lua_State* L) {
    int n = lua_gettop(L); // num args
    for (int i = 1; i <= n; i++) {
        const char* arg = lua_tostring(L, i);
        std::cout << arg;
    }
    std::cout << std::endl;
    return 0; // num results
}

int newtable(lua_State* L) {
    lua_newtable(L);
    return lua_gettop(L);
}


FOOLIB_API void dofoo() {
    lua_State* L = luaL_newstate();
    lua_register(L, "echo", &echo);
    
    int ti = newtable(L); // fileenv
    int mi = newtable(L); // meta
    lua_pushstring(L, "__index"); // key
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS); // value
    lua_rawset(L, mi); // pops key, value
    lua_setmetatable(L, ti); // pops meta table
    int fileenv = luaL_ref(L, LUA_REGISTRYINDEX); // put fileenv into registry

    int loadresult = luaL_loadfile(L, "scripts/script.lua");
    if (loadresult != LUA_OK) {
        int i = lua_gettop(L);
        const char* err = lua_tostring(L, i);
        std::cerr << "error loading file: " << err << std::endl;
        lua_pop(L, 1);
        return;
    }

    int fi = lua_gettop(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, fileenv); // fileenv table
    const char* upvalue_name = lua_setupvalue(L, fi, 1); // pops upvalue

    if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
        int i = lua_gettop(L);
        const char* err = lua_tostring(L, i);
        std::cerr << "error executing file: " << err << std::endl;
        lua_pop(L, 1);
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, fileenv);
    lua_pushstring(L, "Hook");
    lua_rawget(L, -2);

    if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
        int i = lua_gettop(L);
        const char* err = lua_tostring(L, i);
        std::cerr << "error executing function: " << err << std::endl;
        lua_pop(L, 1);
    }
}
