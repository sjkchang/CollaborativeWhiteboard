#ifndef HOST_H
#define HOST_H

#include "../model/EditManager.h"
#include <QImage>
#include <QTcpServer>
#include <QTcpSocket>
#include <string>

struct Connection {
  std::string uuid;
  QTcpSocket *socket;
};

class Host : public QTcpServer {
  Q_OBJECT
public:
  Host(EditManager *editManager, QObject *parent = nullptr);

  void startServer(int port);
  void stopServer();

protected:
  void incomingConnection(qintptr socketDescriptor) override;

private slots:
  void readyRead();
  void disconnected();

private:
  void processMessage(QTcpSocket *sendingClient);

  QMap<QTcpSocket *, QByteArray> clients;
  EditManager *editManager;
};

#endif // HOST_H