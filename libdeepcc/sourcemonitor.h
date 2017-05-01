/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                    Source file change monitor                      ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-04-27               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_SOURCEMONITOR_H
#define DEEPC_SOURCEMONITOR_H

#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>

#include "cbtree.h"


namespace deepC
{

//
// The source monitor watches the given set of source files for changes.
// If they change it determines what the changes are and notifies the
// lexer that changes have occurred.
//

class SourceMonitor
{
private:
    // The source files to monitor.
    std::vector<std::string>   filenames_;
    int                        notifyFd_;
    std::map<int, std::string> notifyDescs_;
    bool                       isMonitoring_;

    // A thread to do the monitoring.
    std::thread                monitorThread_;

    // Last error message.
    std::string                errorMessage_;

private:
    // Run monitoring thread.
    void runMonitor();

    // Start/stop monitoring.
    bool startMonitoring();
    void stopMonitoring();

public:
    // Constructor.
    SourceMonitor(const std::vector<std::string> &filenames);
    ~SourceMonitor();

    // Start monitoring a given set of files.
    bool monitorFiles(const std::vector<std::string> &filenames);
    bool isMonitoring() const { return isMonitoring_; }

    // Returns the last error message.
    std::string getErrorMessage() const { return errorMessage_; }
};

}  // namespace deepC

#endif // DEEPC_SOURCEMONITOR_H
