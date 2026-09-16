#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// A standard C function that we will expose to the Lua environment
static int c_print_warning(lua_State *L) {
    // Read the first argument from the Lua stack as a string
    const char *msg = luaL_checkstring(L, 1);
    printf("[C ENGINE LOG] %s\n", msg);
    
    // Return the number of values we are pushing back to Lua as a result (0 here)
    return 0;
}

int main() {
    // Initialize the Lua state
    lua_State *L = luaL_newstate();
    
    // Load standard Lua libraries (math, string, table, etc.)
    luaL_openlibs(L); 

    // Register our C function so the Lua script can call it
    lua_register(L, "print_warning", c_print_warning);

    // Load and execute the Lua script
    if (luaL_dofile(L, "config.lua") != LUA_OK) {
        fprintf(stderr, "Failed to load script: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // --- PATTERN A: Reading Configuration Data ---
    printf("--- Engine Setup ---\n");
    
    // Push the global variables from Lua onto the C stack
    lua_getglobal(L, "window_title");
    const char *title = lua_tostring(L, -1);
    
    lua_getglobal(L, "window_width");
    int width = lua_tointeger(L, -1);
    
    lua_getglobal(L, "window_height");
    int height = lua_tointeger(L, -1);
    
    printf("Initializing window: '%s' (%dx%d)\n\n", title, width, height);
    
    // Clear the stack (we pushed 3 global variables)
    lua_pop(L, 3); 

    // --- PATTERN B: Executing Lua Logic from C ---
    printf("--- Executing Script Logic ---\n");
    
    // Push the Lua function we want to call onto the stack
    lua_getglobal(L, "calculate_frame_delay");
    
    // Push arguments for the function (target_fps, active_entities)
    lua_pushinteger(L, 60);
    lua_pushinteger(L, 1500);
    
    // Call the function: 2 arguments, 1 expected return value, 0 error handler
    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error calling function: %s\n", lua_tostring(L, -1));
    } else {
        // Retrieve the return value from the top of the stack
        double delay = lua_tonumber(L, -1);
        printf("Calculated frame delay: %.2f ms\n", delay);
        
        // Clean up the return value from the stack
        lua_pop(L, 1); 
    }

    // Clean up and close Lua
    lua_close(L);
    return 0;
}