#include "barlib.h"
#include "sol/sol.hpp"
#include <iostream>
#include <string>


void echo(std::string value) {
    std::cout << value << std::endl;
}

inline int at_panic_no_exceptions(lua_State* L) {
    (void)L;
    return -1;
}

BARLIB_API void dobar () {
    const std::string filename = "scripts/script.lua";
    sol::state lua{};
    lua["echo"] = &echo;

    sol::environment env = sol::environment(lua, sol::create, lua.globals());
    
    //lua.safe_script_file("scripts/script.lua", env);
    //sol::protected_function_result pfr = lua.do_file("scripts/script.lua", sol::load_mode::any);
    //sol::protected_function hook = env["Hook"];
    //hook();
    sol::load_status x = static_cast<sol::load_status>(luaL_loadfilex(lua.lua_state(), filename.c_str(), NULL));
    if (x != sol::load_status::ok) {
        std::cerr << "error loading" << std::endl;
        sol::protected_function_result result = sol::protected_function_result(
            lua.lua_state(),
            sol::absolute_index(lua.lua_state(), -1),
            0, 1, static_cast<sol::call_status>(x)
        );
    }
    sol::unsafe_function pf(lua.lua_state(), -1);
    env.set_on(pf);
    sol::unsafe_function_result result = pf();
}
