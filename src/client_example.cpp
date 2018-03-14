//
// Created by huang on 17-12-5.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  /* netdb is necessary for struct hostent */
#include <cerrno>
#include <fcntl.h>
#include <signal.h>


#define PORT 8888  /* server port */

#define MAXDATASIZE 600


typedef struct packet_head
{
    int size;// header len + data len + sum len( 1 byte )
    int funcId;
    int msg_code;//00 successed   FF fail
    int trans_id;
}P_HEAD;


typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
}MAP_PACKAGE_POP;

typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
    unsigned int size;
    unsigned int width;
    unsigned int hight;
    char data[512];
}MAP_PACKAGE_ACK;

static pid_t    *childpid = NULL;
/* ptr to array allocated at run-time */
static int      maxfd;  /* from our open_max(), {Prog openmax} */

#define SHELL   "/bin/sh"

int killProcess(pid_t pid)
{
    char popbuffer[20] = {0};
    if(pid < 500)
        return false;
    sprintf(popbuffer,"kill %d",pid);
    return system(popbuffer);
}
int killProcessRosLaunch(char *cmd_name)
{

    char popbuffer[120] = {0};
    sprintf(popbuffer,"kill -2 $(ps -au | grep \"%s\"|grep \"/usr/bin/python\"|awk '{print $2}')",cmd_name);
    printf("%s\n",popbuffer);
    return system(popbuffer);
}
FILE *
mypopen(const char *cmdstring, const char *type, pid_t &cmd_pid)
{
    int     i, pfd[2];
    pid_t   pid;
    FILE    *fp;

    /* only allow "r" or "w" */
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
        errno = EINVAL;     /* required by POSIX.2 */
        return(NULL);
    }

    if (childpid == NULL) {     /* first time through */
        /* allocate zeroed out array for child pids */
        maxfd = 10;
        if ( (childpid = (pid_t *)calloc(maxfd, sizeof(pid_t))) == NULL)
            return(NULL);
    }

    if (pipe(pfd) < 0)
        return(NULL);   /* errno set by pipe() */

    if ( (pid = fork()) < 0)
        return(NULL);   /* errno set by fork() */
    else if (pid == 0) {                            /* child */
        if (*type == 'r') {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO) {
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        } else {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO) {
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }
        /* close all descriptors in childpid[] */
        for (i = 0; i < maxfd; i++)
            if (childpid[ i ] > 0)
                close(i);

        execl(SHELL, "sh", "-c", cmdstring, (char *) 0);
        _exit(127);
    }

    cmd_pid = pid;
    /* parent */
    if (*type == 'r') {
        close(pfd[1]);
        if ( (fp = fdopen(pfd[0], type)) == NULL)
            return(NULL);
    } else {
        close(pfd[0]);
        if ( (fp = fdopen(pfd[1], type)) == NULL)
            return(NULL);
    }
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */
    return(fp);
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

    sprintf(new_cmd,"%s > %s",cmdstring,outfilename);
    printf("%s\n",new_cmd);
    if ( (pid = fork()) < 0)
        return -1;   /* errno set by fork() */
    else if (pid == 0) {                            /* child */
        execl(SHELL, "sh", "-c", new_cmd, (char *) 0);
        _exit(127);
    }



    return pid;
}

int main(int argc, char *argv[]) {
    std::string str;
    str = "";
    pid_t cmd_pid;
    char buffer[256] = {0};
    std::string cmd_name = "rostopic hz /rosout";
    signal(SIGCHLD, SIG_IGN);
    //FILE *fp = mypopen("top", "r", cmd_pid);
    cmd_pid = CmdProcessOpen(cmd_name.c_str(), "./lll.log");
    char ch;


/*
    while( (ch = fgetc(fp)) > 0 )
    {
        if(ch <= 0)break;

        str.push_back(ch);
    }*/
    //printf("aa-----------------\n");
    printf("--------------\n%d\n", cmd_pid);
    //killProcess(cmd_pid);
    //pclose(fp);
    //sleep(3);
    killProcessRosLaunch((char *) cmd_name.c_str());
    printf("dsadsa\n");
    while(1);
    /*popen("bash /home/huang/stage_ws/slam.sh","w");
    while(1)
    {
        char ch = getchar();
        printf("-----------Input %c-----\n",ch);
        if(ch == 'k'){
            system("rosnode kill /slam_gmapping");
            printf("end----------------------\n");
            break;
        }
    }*/

/*    int sockfd, num;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in server;

    if (argc != 2)
    {
        printf("Usage: %s <IP Address>\n",argv[0]);
        exit(1);
    }

    if((he=gethostbyname(argv[1]))==NULL)
    {
        printf("gethostbyname() error\n");
        exit(1);
    }

    if((sockfd=socket(AF_INET,SOCK_STREAM, 0))==-1)
    {
        printf("socket() error\n");
        exit(1);
    }
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server))==-1)
    {
        printf("connect() error\n");
        exit(1);
    }
    unsigned char str[50] = {0};
    unsigned char ack[600] = {0};
    P_HEAD *head = (P_HEAD*)(str+1);
    MAP_PACKAGE_POP *map_pop = (MAP_PACKAGE_POP*)(str+1+16);
    P_HEAD *ackhead = (P_HEAD*)(ack+1);
    MAP_PACKAGE_ACK *map_ack = (MAP_PACKAGE_ACK*)(ack+1+16);
    head->funcId = 2;
    head->size = 16+8;
    map_pop->package_num = 0;
    map_pop->package_sum = 0;
    str[0] = 0xAA;
    str[1+16+8+1]=0xAB;

    do{
        //sleep(1);
        //printf("send : \n id:%d  size:%d\n",head->funcId,head->size);
        printf("sum:%d num:%d\n",map_pop->package_sum,map_pop->package_num);
        for (int i = 0; i < 25; ++i) {
            printf("%02X ",str[i]);
        }
        printf("\n");
        if((num=send(sockfd,str,head->size+9,0))==-1){
            printf("send() error\n");
            exit(1);
        }

        //printf("wait ack...\n");
        int ret = 0;

            if((ret=recv(sockfd,ack,sizeof(MAP_PACKAGE_ACK)+19,0)) == -1) {
                printf("recv() error\n");
                exit(1);
            }
        if(ackhead->funcId == 2 )
        {
            printf("ack sum:%d num:%d\n",map_ack->package_sum,map_ack->package_num);
            map_pop->package_sum = map_ack->package_sum;
            map_pop->package_num = map_ack->package_num;
            num = 0;
            if (map_pop->package_sum <= map_pop->package_num)
            {
                break;
            }
        }
        else
        {
            printf("ack fail.\n");
            exit(1);
        }

    }while(map_pop->package_sum != 0 && map_pop->package_sum >= map_ack->package_num);


    //buf[num-1]='\0';
    printf("server message: %s\n",buf);
    close(sockfd);*/
    return 0;
}
