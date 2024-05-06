#include "Host.h"
#include <QBuffer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

Host::Host(EditManager *editManager, QObject *parent)
    : QTcpServer(parent), editManager(editManager) {}

void Host::startServer(int port) {
  if (!listen(QHostAddress::Any, port)) {
    qDebug() << "Server could not start!";
  } else {
    qDebug() << "Server started on port " << port;
  }
}

void Host::stopServer() {
  for (QTcpSocket *client : clients.keys()) {
    client->disconnectFromHost();
    clients.remove(client);
  }
  close();
  qDebug() << "Server stopped";
}

void Host::incomingConnection(qintptr socketDescriptor) {
  QTcpSocket *clientSocket = new QTcpSocket(this);
  clientSocket->setSocketDescriptor(socketDescriptor);
  clientSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,
                                1 * 1024 * 1024);

  connect(clientSocket, &QTcpSocket::readyRead, this, &Host::readyRead);
  connect(clientSocket, &QTcpSocket::disconnected, this, &Host::disconnected);

  clients.insert(clientSocket, QByteArray());

  qDebug() << "Client connected, total clients:" << clients.count();

  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);
  editManager->getImage().save(&buffer, "PNG");

  QJsonObject message;
  message["type"] = "image";
  message["data"] = QString::fromLatin1(byteArray.toBase64());
  QJsonDocument doc(message);
  if (clientSocket->state() == QAbstractSocket::ConnectedState) {
    clientSocket->write(doc.toJson());
    clientSocket->write(
        "\n"); // Use the newline character as the end-of-object delimiter
    clientSocket->flush();
  }
}

void Host::disconnected() {
  QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
  if (!client) {
    return;
  }

  // Remove client from the map
  clients.remove(client);
  client->deleteLater();

  qDebug() << "Client disconnected, total clients:" << clients.count();
}

void Host::readyRead() {
  qDebug() << "Host::readyRead";
  QTcpSocket *sendingClient = qobject_cast<QTcpSocket *>(sender());
  if (!sendingClient) {
    return;
  }

  QByteArray rawData = sendingClient->readAll();
  clients[sendingClient].append(rawData);
  if (!rawData.isEmpty() && rawData.endsWith('\n')) {
    processMessage(sendingClient);
  }
}

void Host::processMessage(QTcpSocket *sendingClient) {
  qDebug() << "Host::processMessage";
  QByteArray &rawData = clients[sendingClient];

  for (QTcpSocket *client : clients.keys()) {
    if (client != sendingClient) {
      client->write(rawData);
      client->write("\n");
      client->flush();
    }
  }

  rawData.clear();
}