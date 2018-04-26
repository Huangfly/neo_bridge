//
// Created by huang on 18-4-25.
//

#ifndef NEO_BRIDGE_CCONFIG_H
#define NEO_BRIDGE_CCONFIG_H

#include <iostream>

namespace Neo_Config {
    class ConfigParamer{
    public:
        ConfigParamer(){}
        std::string mapFrameId_;
        std::string robotFrameId_;
        std::string scanFrameId_;

        std::string mapTopic_;
        std::string scanTopic_;
        std::string cmdvelTopic_;

        std::string mappingDir_;
        std::string mappingKillDir_;

        std::string navigationDir_;
        std::string navigationKillDir_;
    };

    ConfigParamer* GetConfigParamer();

    class CConfig {
    public:
        CConfig();

        ~CConfig();

        ConfigParamer* paramer;

        bool LoadFille(const char *filename);
    };
}
#endif //NEO_BRIDGE_CCONFIG_H
