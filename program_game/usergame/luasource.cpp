#include "luasource.hh"
#include "cqctool.hh"
#include "cqfoundation.hh"

static const char *const sLuaSources[] = {
    //use script generate_lua(.bat) to create GENERATED_LUA.
    #include "../GENERATED_LUA/GENERATED_LUA"
    nullptr, nullptr,
};

static void updateFile(const std::string &path, const char *name, const char *text) {
    FILE *file = fopen(path.c_str(), "wb");
    if (file != nullptr) {
        I("lua source: update '%s'", name);
        fwrite(text, 1, strlen(text), file);
        fclose(file);
    } else {
        I("lua source: failed to update '%s'", name);
    }
}

void LuaSource::update(const std::string &directory) {
    I("lua source: update");
    
    for (auto it = sLuaSources; it[0] && it[1]; it += 2) {
        const char *name = it[0];
        const char *text = it[1];
        
        std::string path = cqPathString::append(directory, name);
        updateFile(path, name, text);
    }
}
