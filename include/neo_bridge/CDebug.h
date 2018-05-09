//
// Created by huang on 18-5-3.
//

#ifndef NEO_BRIDGE_CDEBUG_H
#define NEO_BRIDGE_CDEBUG_H

#define INFO_LEVEL 3
#define DEBUG_LEVEL 2
#define ERROR_LEVEL 1


namespace Neo_Log{
    bool checkDebuglevel(int level);
    void InitDebug();
}

#define DebugBase(level,...)  { if(Neo_Log::checkDebuglevel(level)) {printf("%s: ",__FUNCTION__);printf(__VA_ARGS__);} }
#define DebugBaseN(level,...)  { if(Neo_Log::checkDebuglevel(level)) {printf(__VA_ARGS__);} }

#define NeoInfo(...)    DebugBase(INFO_LEVEL,__VA_ARGS__)
#define NeoDebug(...)    DebugBase(DEBUG_LEVEL,__VA_ARGS__)
#define NeoDebugN(...)  DebugBaseN(DEBUG_LEVEL,__VA_ARGS__)
#define NeoERROR(...)    DebugBase(ERROR_LEVEL,__VA_ARGS__)

#endif //NEO_BRIDGE_CDEBUG_H
