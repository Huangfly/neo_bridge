//
// Created by huang on 18-2-28.
//

#include <neo_bridge/CCommandExecutor.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int killProcess(pid_t pid)
{
    char popbuffer[20] = {0};
    if(pid < 500)
        return false;
    sprintf(popbuffer,"kill -s SIGINT %d",pid);
    return system(popbuffer);
}

int killProcessRosLaunch(char *cmd_name)
{

    char popbuffer[120] = {0};
    sprintf(popbuffer,"kill -2 $(ps -au | grep \"%s\"|grep \"/usr/bin/python\"|awk '{print $2}')",cmd_name);
    return system(popbuffer);
}

int CmdProcessOpen(const char *cmdstring, const char *outfilename)
{
    int     i, pfd;
    pid_t   pid;
    char new_cmd[256] = {0};

    if(cmdstring == NULL)return -1;

    if( outfilename == NULL )
    {
        return -1;
    }
    signal(SIGCHLD, SIG_IGN);
    sprintf(new_cmd,"%s > %s",cmdstring,outfilename);
    printf("%s\n",new_cmd);
    if ( (pid = fork()) < 0)
        return -1;   /* errno set by fork() */
    else if (pid == 0) {                            /* child */
        execl("/bin/sh", "sh", "-c", new_cmd, (char *) 0);
        _exit(127);
    }



    return pid;
}

CommandExecutor::CommandExecutor(const std::string &cmd) {
    this->cmd_ = cmd;
}

CommandExecutor::~CommandExecutor() {

}
