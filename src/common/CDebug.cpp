//
// Created by huang on 18-5-3.
//
#include <neo_bridge/CDebug.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <neo_bridge/CConfig.h>

static char mode;

namespace Neo_Log{

    void InitDebug(){
        mode = 0;
        Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();
        if( config_ptr == NULL ){
            printf("error: no found config file.\n");
            return;
        }

        mode = config_ptr->DebugMode;
    }

    bool checkDebuglevel(int level){
        if(level <= mode){
            return true;
        }
        return false;
    }

}
