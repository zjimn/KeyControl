#ifndef PROCESS_LAUNCHER_H
#define PROCESS_LAUNCHER_H
#include <string>

class ProcessLauncher {
public:
    ProcessLauncher(const std::string &executable, const std::string &arguments = "");

    bool Launch();

private:
    std::string executable_;
    std::string arguments_;
};
#endif
