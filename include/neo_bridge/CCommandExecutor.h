//
// Created by huang on 18-2-28.
//

#ifndef NEO_BRIDGE_CCOMMANDEXECUTOR_H
#define NEO_BRIDGE_CCOMMANDEXECUTOR_H

#include <string>

int killProcess(pid_t pid);
int CmdProcessOpen(const char *cmdstring, const char *outfilename);

class CommandExecutor {
public:
    CommandExecutor(const std::string& cmd);
    virtual ~CommandExecutor();
    virtual bool Done() = 0;
    virtual int ReturnValue() = 0;
    virtual bool Kill() = 0;
protected:
    std::string cmd_;
};

#endif //NEO_BRIDGE_CCOMMANDEXECUTOR_H
