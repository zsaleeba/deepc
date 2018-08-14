#ifndef LSPSERVER_H
#define LSPSERVER_H

#include "abstractlspserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>

using namespace jsonrpc;

class LspServer : public AbstractLspServer
{
public:
    LspServer(AbstractServerConnector &connector);

    virtual void notifyServer();
    virtual std::string sayHello(const std::string& name);
};

#endif // LSPSERVER_H
