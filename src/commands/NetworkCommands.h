#include "../network/Client.h"
#include "../network/Host.h"
#include "ICommand.h"
#include <QFileDialog>
#include <QImage>
#include <QString>
#include <string>

class StartHostCommand : public ICommand {
public:
  StartHostCommand(Host *host, Client *client, int port = 3000)
      : host(host), client(client), port(port) {}
  void execute() override {
    host->startServer(port);
    client->connectToServer(QHostAddress::LocalHost, port);
  }

private:
  Host *host;
  Client *client;
  int port;
};

class StopHostCommand : public ICommand {
public:
  StopHostCommand(Host *host, Client *client) : host(host), client(client) {}
  void execute() override {
    host->stopServer();
    client->disconnectFromServer();
  }

private:
  Host *host;
  Client *client;
};

class ConnectToHostCommand : public ICommand {
public:
  ConnectToHostCommand(Client *client, const QHostAddress &address, int port)
      : client(client), address(address), port(port) {}
  void execute() override { client->connectToServer(address, port); }

private:
  Client *client;
  QHostAddress address;
  int port;
};

class DisconnectFromHostCommand : public ICommand {
public:
  DisconnectFromHostCommand(Client *client) : client(client) {}
  void execute() override { client->disconnectFromServer(); }

private:
  Client *client;
};