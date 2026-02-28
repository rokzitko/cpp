// main.cpp

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <iostream>
#include <string>

static void die_if_lua_error(lua_State* L, int status) {
    if (status != LUA_OK) {
        const char* msg = lua_tostring(L, -1);
        std::cerr << "Lua error: " << (msg ? msg : "(unknown)") << "\n";
        lua_pop(L, 1); // remove error message
        std::exit(1);
    }
}

// Example: a C function callable from Lua: add(2,3) -> 5
static int l_add(lua_State* L) {
    // Lua stack: args at 1..n
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    lua_pushnumber(L, a + b); // return value
    return 1; // number of return values
}

int main() {
    lua_State* L = luaL_newstate();        // create interpreter
    if (!L) return 1;

    luaL_openlibs(L);                      // standard libs

    // Register C function as global "add"
    lua_pushcfunction(L, l_add);
    lua_setglobal(L, "add");

    // Run a Lua snippet
    die_if_lua_error(L, luaL_dostring(L, R"(
        print("Hello from Lua")
        print("2+3=", add(2,3))
        function f(x) return x*x end
    )"));

    // Call Lua function f(7) from C++
    lua_getglobal(L, "f");                 // push function
    lua_pushnumber(L, 7);                  // push arg
    die_if_lua_error(L, lua_pcall(L, 1, 1, 0)); // call with 1 arg, 1 return

    double out = luaL_checknumber(L, -1);
    lua_pop(L, 1);                         // pop return value
    std::cout << "f(7) = " << out << "\n";

    lua_close(L);                          // cleanup
    return 0;
}
