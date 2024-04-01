#include <iostream>
#include "gencpp.hpp"
#include "genlua.hpp"

enum class Lang {Cpp, Lua, Hpp};

void generate(Lang lang, const std::filesystem::path& brofile)
{
    if(!std::filesystem::exists(brofile))
    {
        std::cout << "bro file not found";
        return;
    }

    try
    {
        switch(lang)
        {
            case Lang::Cpp: std::cout << gencpp(brofile, false); break;
            case Lang::Lua: std::cout << genlua(brofile); break;
            case Lang::Hpp: std::cout << gencpp(brofile, true); break;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception thrown: " << e.what();
    }
}

int main(int argc, char** argv)
{
    if(argc <= 2)
    {
        std::cout << "Need atleast 2 args";
        return -1;
    }

    std::string_view lang = argv[1];
    std::filesystem::path file = argv[2];

    if(lang == "cpp")
        generate(Lang::Cpp, file);
    else if(lang == "lua")
        generate(Lang::Lua, file);
    else if(lang == "hpp")
        generate(Lang::Hpp, file);
    else
        std::cout << "Only languages 'cpp' and 'lua' are allowed";
}

