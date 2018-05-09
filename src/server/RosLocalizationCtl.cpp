//
// Created by huang on 18-4-24.
//

#include "RosLocalizationCtl.h"

RosLocalizationCtl::RosLocalizationCtl():CommandExecutor("") {
    this->isRun = false;
    this->cmd_ = "rosservice call /StartLocalization";
}

RosLocalizationCtl::~RosLocalizationCtl() {

}

bool RosLocalizationCtl::Done() {
    printf("Start Localization\n");
    int ret = system(this->cmd_.c_str());
    printf("status %d\n",ret);
    return true;
}

bool RosLocalizationCtl::isRunning() {
    return isRun;
}

bool RosLocalizationCtl::Kill() {
    return true;
}
