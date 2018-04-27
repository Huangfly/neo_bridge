//
// Created by huang on 18-4-25.
//
#include <neo_bridge/CConfig.h>
#include <cstring>

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

using namespace Neo_Config;
static ConfigParamer* configParamer = NULL;

CConfig::CConfig() {
    paramer = new ConfigParamer();
}

CConfig::~CConfig() {
    delete paramer;
}


bool LoadString(lua_State *L,const char* str, std::string &out_)
{
    lua_getglobal(L, str);
    if (lua_isstring(L, -1))
    {
        out_ = lua_tostring(L, -1);
        printf("    :%s = %s\n",str,out_.c_str());

        return true;
    }
    printf("no found %s\n",str);
    return false;
}

bool CConfig::LoadFille(const char *filename) {
    lua_State* L = luaL_newstate();

    luaL_openlibs(L); //新版本库添加的方法
    if(luaL_loadfile(L,filename) || lua_pcall(L,0,0,0)){
        luaL_error(L,"loadfile error! %s \n",lua_tostring(L,-1));
        return false;
    }



    std::string str;// = lua_tostring(L,-2);
    if(!LoadString(L,"map_frame_id",paramer->mapFrameId_))return false;
    if(!LoadString(L,"map_topic",paramer->mapTopic_))return false;

    if(!LoadString(L,"scan_frame_id",paramer->scanFrameId_))return false;
    if(!LoadString(L,"scan_topic",paramer->scanTopic_))return false;

    if(!LoadString(L,"robot_frame_id",paramer->robotFrameId_))return false;
    if(!LoadString(L,"cmdvel_topic",paramer->cmdvelTopic_))return false;

    if(!LoadString(L,"mapping_shell_dir",paramer->mappingDir_))return false;
    if(!LoadString(L,"mapping_kill_shell_dir",paramer->mappingKillDir_))return false;

    if(!LoadString(L,"navigation_shell_dir",paramer->navigationDir_))return false;
    if(!LoadString(L,"navigation_kill_shell_dir",paramer->navigationKillDir_))return false;
    //printf("map: %s\n",str.c_str());

    LoadString(L,"map_save_dir",paramer->mapSaveDir_);
    LoadString(L,"path_topic",paramer->pathTopic_);

    if(configParamer == NULL){
        configParamer = new ConfigParamer();
    }
    memcpy(configParamer, this->paramer, sizeof(ConfigParamer));

    return true;
}

ConfigParamer *::Neo_Config::GetConfigParamer() {
    return configParamer;
}
