#include "barlib.h"
#include "sol/sol.hpp"
#include <iostream>


void echo(std::string value) {
    std::cout << value << std::endl;
}


BARLIB_API void dobar () {
    sol::state lua{};
    lua["echo"] = &echo;

    sol::environment env = sol::environment(lua, sol::create, lua.globals());
    lua.safe_script_file("scripts/script.lua", env);
    sol::protected_function hook = env["Hook"];
    hook();
}
