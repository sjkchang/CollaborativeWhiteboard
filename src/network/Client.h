#ifndef CLIENT_H
#define CLIENT_H

#include "../model/EditManager.h"
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

class Client : public QTcpSocket, public Observer {
  Q_OBJECT
public:
  Client(EditManager *editManager, QObject *parent = nullptr);

  void update(Event *data) override;

public slots:
  void connectToServer(const QHostAddress &address, quint16 port);
  void disconnectFromServer() { this->disconnectFromHost(); }

  void sendMessage(const QString &type, const QString &data);
  void sendImage(const QImage &image);
  void sendImageDiff(const ImageDiff &diff);

private slots:
  void readyRead();

signals:
  void connected();
  void disconnected();

protected:
  void processMessage();

private:
  EditManager *editManager;
  QByteArray buffer;
};

#endif // CLIENT_H