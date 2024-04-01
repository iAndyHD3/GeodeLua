#include <Geode/binding/MenuLayer.hpp>

using namespace cocos2d;
using namespace geode;

#include <Geode/modify/MenuLayer.hpp>
#include "sol.hpp"
#include "codegen.hpp"

void sol_panic(sol::optional<std::string> maybe_msg)
{
	if(maybe_msg)
	{
		geode::log::error("A lua panic occurred {}", *maybe_msg);
	}
	else
	{
		geode::log::error("A lua panic ocurred but no error was found");
	}
}

sol::protected_function_result internal_error(lua_State*, sol::protected_function_result pfr)
{
	sol::error err = pfr;
	geode::log::error("{}", err.what());
	return pfr;
}


class $modify(MyMenuLayer, MenuLayer)
{
	void onMoreGames(CCObject*)
	{
		sol::state lua;

		lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string, sol::lib::package);
		lua.set_function("include", &includeClass);

		std::filesystem::path dir = CURRENT_SOURCE_DIR;
		dir /= "test.lua";
		lua.script_file(dir.string(), internal_error);
	}
};









/*
inline int ClassName_XXXX_f_g (lua_State* L) {
     // Get the class here
     ClassName* c = sol::stack::get<ClassName*>(c, 1);
     // add safety checks here
     // Use the generator to yank out arguments
     decltype(auto) result = (c->XXXX)( sol::stack::get<Arg1>(L, 2), sol::stack::get<Arg2>(L, 3), sol::stack::get<Arg3>(L, 4), ... );

     // push the result
     // this generally uses copy-semantics behavior: to get
     return stack::push(L, std::forward<decltype(result)>(result));
}

inline int fatalError(lua_State* L, const char* error) {
    lua_pushstring(L, error);
    lua_error(L);
	return -1;
}

inline int CCDirector_getRunningScene(lua_State* L)
{
	cocos2d::CCDirector* c = sol::stack::get<cocos2d::CCDirector*>(L, 1);
	if(!geode::cast::typeinfo_cast<cocos2d::CCDirector*>(c))
	{
		fatalError(L, "Wrong pointer type detected in ..");
		return -1;
	}

	decltype(auto) result = (c->getRunningScene)();
	return sol::stack::push(L, std::forward<decltype(result)>(result));
}

template<typename T>
T LuaHelper_getSelf(lua_State* L)
{
	auto self_opt = sol::stack::check_get<sol::userdata>(L, 1);

	if(!self_opt)
	{
		fatalError(L, "self argument is nil or the wrong type. Please make sure to call your instance-functions with obj:func( ... ) or obj.func(obj, ... )");
	}
	auto ptr = self_opt.value().as<T>();
	if(!ptr)
	{
		fatalError(L, "self argument is nil.");
	}

	if(auto self = geode::cast::typeinfo_cast<T>(ptr))
	{
		return self;
	}

	fatalError(L, "self is not of the correct type");
	return nullptr;
}

template<typename T>
T getArgN(lua_State* L, int index, const char* possible_error)
{
	auto arg = sol::stack::check_get<T>(L, index);
	if(!arg) { fatalError(L, possible_error); }
	return arg.value();
}


inline int CCNode_setPositionX(lua_State* L)
{
	auto self = LuaHelper_getSelf<CCNode*>(L);
	auto arg1 = getArgN<float>(L, 2, "Could not get 1st 'x' argument of CCNode.setPositionX");
	(self->setPositionX)(arg1);
	return 0;
}
*/
