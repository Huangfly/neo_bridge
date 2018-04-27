//
// Created by huang on 18-4-24.
//

#include "RosLocationCtl.h"

RosLocationCtl::RosLocationCtl():CommandExecutor("") {
    this->isRun = false;
    this->cmd_ = "rosservice call /StartLocalization";
}

RosLocationCtl::~RosLocationCtl() {

}

bool RosLocationCtl::Done() {
    printf("Start Location\n");
    int ret = system(this->cmd_.c_str());
    printf("status %d\n",ret);
    return true;
}

int RosLocationCtl::ReturnValue() {
    return 0;
}

bool RosLocationCtl::Kill() {
    return true;
}
