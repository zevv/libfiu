
/*
 * Simple Lua binding for fiu.
 *
 * - faildata is not supported, at this time I cannot think of a way to make
 *   this work with the Lua garbage collector. We can ref lua values so they
 *   can be found back (and unreffed) by fiu_faildata(), but when faildata() is
 *   never called, the data will stay referenced for ever and never be
 *   collected.
 *
 * - external callback is not yet supported, because I do not need it at this
 *   time. There's also the problem of passing the Lua state to the callback
 *   function, as there is no callback pointer available. We could abuse the
 *   faildata pointer for this, just as in the Python binding.
 */

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define FIU_ENABLE 1

#include <fiu.h>
#include <fiu-control.h>


static int fail(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);

	int r = fiu_fail(name);

	lua_pushboolean(L, r != 0);
	lua_pushnumber(L, r);
	return 2;
}


static int enable(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);
	int failnum = lua_tonumber(L, 2);
	int flags = lua_tonumber(L, 3);

	lua_pushvalue(L, 3);
	long ref = luaL_ref(L, LUA_REGISTRYINDEX);
	int r = fiu_enable(name, failnum, (void *)ref, flags);

	lua_pushboolean(L, r == 0);
	return 1;
}


static int enable_random(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);
	int failnum = lua_tonumber(L, 2);
	int flags = lua_tonumber(L, 3);
	float probability = lua_tonumber(L, 4);
	
	int r = fiu_enable_random(name, failnum, NULL, flags, probability);

	lua_pushboolean(L, r == 0);
	return 1;
}


static int enable_stack_by_name(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);
	int failnum = lua_tonumber(L, 2);
	int flags = lua_tonumber(L, 3);
	const char *func_name = luaL_checkstring(L, 4);
	int func_pos_in_stack = luaL_checknumber(L, 5);

	int r = fiu_enable_stack_by_name(name, failnum, NULL, flags, func_name, func_pos_in_stack);

	lua_pushboolean(L, r == 0);
	return 1;
}


static int disable(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);

	int r = fiu_disable(name);

	lua_pushboolean(L, r == 0);
	return 1;
}


static int rc_fifo(lua_State *L)
{
	const char *basename = luaL_checkstring(L, 1);

	int r = fiu_rc_fifo(basename);
	
	lua_pushboolean(L, r == 0);
	return 1;
}


static struct luaL_Reg fiu_table[] = {
	{ "fail", fail },
//	{ "failinfo", failinfo },
	{ "enable", enable },
	{ "enable_random", enable_random },
//	{ "enable_external", enable_external },
	{ "enable_stack_by_name", enable_stack_by_name },
	{ "disable", disable },
	{ "rc_fifo", rc_fifo },
	{ NULL }
};


int luaopen_fiu(lua_State *L)
{
        luaL_newlib(L, fiu_table);
	fiu_init(0);
        return 1;
}

