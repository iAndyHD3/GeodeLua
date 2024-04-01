#include "gencpp.hpp"
#include <broma.hpp>
#include <fmt/format.h>
#include <string>
#include <fstream>
#include <iostream>
#include <span>

using namespace broma;

std::optional<std::string_view> getFunctionNameFromField(const broma::Field& field)
{
    if(auto* ptr = std::get_if<FunctionBindField>(&field.inner))
    {
        return ptr->prototype.name;
    }
    if(auto* ptr = std::get_if<OutOfLineField>(&field.inner))
    {
        return ptr->prototype.name;
    }
    return {};
}

int foundCount(std::span<broma::Field> iterable, const broma::Field& tofind)
{
    auto tofind_name_opt = getFunctionNameFromField(tofind);
    if(!tofind_name_opt) return -1;

    const auto& tofind_name = *tofind_name_opt;
    int n = 0;
    for(const auto& field : iterable)
    {
        if(auto name = getFunctionNameFromField(field); *name == tofind_name)
        {
            n++;
        }
    }
    return n;
}


std::vector<std::string> getNonPublicSymbols()
{
    std::filesystem::path p = CURRENT_SOURCE_DIR;
    p /= "protected_private.txt";

    std::string line;
    std::ifstream file(p);
    if(!file) return {};
    std::vector<std::string> ret;
    ret.reserve(300);
    while(file)
    {
        std::getline(file, line);
        ret.push_back(line);
    }
    return ret;
}

bool isFunctionValid(const broma::Class& cl, const FunctionBindField* f)
{
    if(f->prototype.ret.name == "TodoReturn") return false;

    constexpr auto npos = std::string::npos;

    if(f->prototype.name.find('~') != npos) return false; //destructor
    if(cl.name.find(f->prototype.name) != npos) return false; //constructor


    bool links_windows = static_cast<int>(cl.attributes.links) & static_cast<int>(broma::Platform::Windows);
    if(!links_windows && f->binds.win == -1) return false;

    static std::vector<std::string> protected_private_funcs = getNonPublicSymbols();

    
    for(const auto& bad_func : protected_private_funcs)
    {
        auto func_name = fmt::format("::{}(", f->prototype.name);
        if(bad_func.find(cl.name) != npos && bad_func.find(func_name) != npos)
        {
            return false;
        }
    }

    //discard if there is a r-value reference argument (out parameters)
    //lua doesn't support those, and as a result sol doesn't aswell
    for(const auto& arg : f->prototype.args)
    {
        if(auto& arg_name = arg.first.name; arg_name.find('&') != npos && arg_name.find("const") == npos)
        {
            return false;
        }
    }
    return true;
}


std::string gencpp(const std::filesystem::path& brofile, bool hppmode)
{
    std::string topstring;


    auto addline = [&topstring](std::string_view content, int indent = 0, bool newline = true)
    {
        for(int i = 0; i < indent; i++)
            topstring += '\t';

        topstring += content;
        if(newline) topstring += '\n';
    };

    auto addnewline = [&topstring]() { topstring += '\n'; };

    addline("#include <Geode/Geode.hpp>");
    addline("#include \"sol.hpp\"");
    //addline("using namespace cocos2d;");
    addnewline();

    constexpr const char* FUNCTION_START =
    R"R(void register_{0}(sol::state_view lua))R";

    constexpr const char* CHAR_FUNCTION_END = "}";
    constexpr const char* CHAR_FUNCTION_START = "{";
    constexpr const char* CLASS_START =
    R"R(sol::table t = lua.create_named_table("{}");)R";

    constexpr const char* FUNCTION_BIND = 
    R"R(tt["{1}"] = sol::c_call<decltype(&{0}::{1}), &{0}::{1}>;)R";

    //sol::c_call<decltype(&{0}::{1}), &{0}::{1}>



    std::vector<std::string> class_names_included;

    broma::Root root = broma::parse_file(brofile.string());
    for(const auto& cl : root.classes)
    {
        std::vector<broma::Field> valid_fields;
        //add all names to vector
        for(const auto& field : cl.fields)
        {
            if(auto* ptr = std::get_if<FunctionBindField>(&field.inner); ptr)
            {
                valid_fields.emplace_back(field);
            }
            else if(auto ptr = std::get_if<OutOfLineField>(&field.inner))
            {
                valid_fields.emplace_back(field);
            }
        }

        std::vector<std::string> funcsToAdd;

        for(const auto& field : valid_fields)
        {
            if(auto ptr = std::get_if<FunctionBindField>(&field.inner))
            {
                if(isFunctionValid(cl, ptr) && foundCount(valid_fields, field) == 1)
                {
                    //remove cocos2d:: namespace
                    funcsToAdd.emplace_back(fmt::format(FUNCTION_BIND, cl.name, getFunctionNameFromField(field).value()), 1);
                }
            }
        }

        if(funcsToAdd.empty()) continue;
        
        if(auto namespac = cl.name.find("::"); namespac != std::string::npos)
        {
            if(hppmode)
            {
                class_names_included.push_back(cl.name);
                addline(fmt::format(FUNCTION_START, std::string(cl.name).replace(namespac, 2, "_")), 0, false);
                addline(";");
            }
            else
            {
                addline(fmt::format(FUNCTION_START, std::string(cl.name).replace(namespac, 2, "_")));
                addline(CHAR_FUNCTION_START);
                addline(fmt::format(CLASS_START, cl.name.substr(namespac + 2)), 1);
            }
        }
        else
        {
            if(hppmode)
            {
                class_names_included.push_back(cl.name);
                addline(fmt::format(FUNCTION_START, cl.name), 0, false);
                addline(";");
            }
            else
            {
                addline(fmt::format(FUNCTION_START, cl.name));
                addline(CHAR_FUNCTION_START);
                addline(fmt::format(CLASS_START, cl.name, cl.name), 1);
            }
        }


        if(hppmode) continue;

        for(const auto& lines : funcsToAdd)
        {
            addline(lines, 1);
        }
        
        //addline(");", 1);
        addline("}");
    }

    if(hppmode)
    {
        addline("");
        addline("");
        addline("");

        addline("void includeClass(std::string_view cl, sol::this_state lua)");
        addline("{");

        for(auto& cl_name : class_names_included)
        {
            std::string cl_name_cmp = cl_name;

            if(auto namesp = cl_name.find("::"); namesp != std::string::npos)
            {
                cl_name_cmp = cl_name.substr(namesp + 2);
                cl_name.replace(namesp, 2, "_");
            }

            addline(fmt::format("if(cl == \"{}\") return register_{}(lua);", cl_name_cmp, cl_name), 1);
            
        }
        addline("}");
    }

    return topstring;
}