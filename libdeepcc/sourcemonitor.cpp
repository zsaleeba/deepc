/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                      Source file collection                        ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-03-26               ***
 ***                                                                    ***
 **************************************************************************/

#include <vector>
#include <string>
#include <errno.h>
#include <poll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

#include "sourcemonitor.h"


namespace deepC
{

//
// The source monitor watches the given set of source files for changes.
// If they change it determines what the changes are and notifies the
// lexer that changes have occurred.
//

// Constructor.
SourceMonitor::SourceMonitor(const std::vector<std::string> &filenames) : notifyFd_(-1), isMonitoring_(false)
{
    filenames_ = filenames;
    monitorThread_ = std::thread([this](){ runMonitor(); });
    startMonitoring();
}


SourceMonitor::~SourceMonitor()
{
    stopMonitoring();
}


//
// NAME:        monitorFiles
// ACTION:      Gives a new set of files to monitor and starts
//              monitoring them.
// PARAMETERS:  filenames - the set of files to monitor.
//

bool SourceMonitor::monitorFiles(const std::vector<std::string> &filenames)
{
    stopMonitoring();
    filenames_ = filenames;
    return startMonitoring();
}


//
// NAME:        startMonitoring
// ACTION:      Start monitoring each of the files for changes.
//

bool SourceMonitor::startMonitoring()
{
    notifyFd_ = inotify_init();
    if (notifyFd_ < 0)
    {
        errorMessage_ = std::string("can't monitor files, inotify_init(): ") + strerror(errno);
    }

    for (auto &fname: filenames_)
    {
        int watch = inotify_add_watch(notifyFd_, fname.c_str(), IN_CLOSE_WRITE);
        if (watch < 0)
        {
            errorMessage_ = std::string("can't monitor files, inotify_add_watch(") + fname + "): " + strerror(errno);
            stopMonitoring();
            return false;
        }

        notifyDescs_[watch] = fname.c_str();
    }

    isMonitoring_ = true;
    return true;
}


//
// NAME:        stopMonitoring
// ACTION:      Stop watching the filesystem for changes.
//

void SourceMonitor::stopMonitoring()
{
    if (notifyFd_ < 0)
        return;

    // Stop monitoring each of the files.
    for (auto const &noteDesc: notifyDescs_)
    {
        inotify_rm_watch(notifyFd_, noteDesc.first);
    }

    // Close the notify fd.
    close(notifyFd_);
    notifyFd_ = -1;
    notifyDescs_.clear();
    isMonitoring_ = false;
}


//
// NAME:        runMonitor
// ACTION:      Watches a set of files or directories for changes and
//              notifies watchers when something happens.
//

void SourceMonitor::runMonitor()
{
    struct inotify_event event;

    while (true)
    {
        int rc = read(notifyFd_, &event, sizeof(event));
        if (rc != -1)
        {
            // Read an event.
            if (event.mask & IN_CLOSE_NOWRITE)
            {
                auto foundDesc = notifyDescs_.find(event.wd);
                if (foundDesc != notifyDescs_.end())
                {
                    std::string modFilename = (*foundDesc).second;
                    // XXX - notify watchers.

                }
            }
        }
        else
        {
            // Read error.
            if (errno != EAGAIN)
            {
                // Abort with error.
                errorMessage_ = std::string("can't monitor files, read(): ") + strerror(errno);
                // XXX - how to report this error?
                return;
            }
        }
    }
}


}  // namespace deepC
