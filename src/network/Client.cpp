#include "Client.h"

Client::Client(EditManager *editManager, QObject *parent)
    : QTcpSocket(parent), editManager(editManager) {
  setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,
                  1 * 1024 * 1024);

  qDebug() << "Client created";
  connect(this, &QTcpSocket::readyRead, this, &Client::readyRead);
  connect(this, &QTcpSocket::connected, this,
          [this]() { qDebug() << "Connected to server"; });
  connect(this, &QTcpSocket::disconnected, this, [this]() {
    disconnectFromServer();
    qDebug() << "Disconnected from server";
  });
  connect(this,
          QOverload<QAbstractSocket::SocketError>::of(
              &QAbstractSocket::errorOccurred),
          [=](QAbstractSocket::SocketError socketError) {
            qDebug() << "Error occurred:" << socketError;
          });
}

void Client::connectToServer(const QHostAddress &address, quint16 port) {
  this->connectToHost(address, port);

  if (this->waitForConnected()) {
    emit connected();
  } else {
    emit disconnected();
  }
}
void Client::sendMessage(const QString &type, const QString &data) {
  QJsonObject message;
  message["type"] = type;
  message["data"] = data;
  QJsonDocument doc(message);
  if (state() == QAbstractSocket::ConnectedState) {
    this->write(doc.toJson());
    this->write(
        "\n"); // Use the newline character as the end-of-object delimiter
    this->flush();
  }
}

void Client::sendImage(const QImage &image) {
  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);
  image.save(&buffer, "PNG");

  QJsonObject message;
  message["type"] = "image";
  message["data"] = QString::fromLatin1(byteArray.toBase64());
  QJsonDocument doc(message);
  if (state() == QAbstractSocket::ConnectedState) {
    this->write(doc.toJson());
    this->write(
        "\n"); // Use the newline character as the end-of-object delimiter
    this->flush();
  }
}

void Client::sendImageDiff(const ImageDiff &diff) {
  QJsonObject message;
  message["type"] = "image-diff";
  message["data"] = diff.toJson();
  QJsonDocument doc(message);
  // qDebug() << "Sending diff" << doc.toJson();
  if (state() == QAbstractSocket::ConnectedState) {
    qDebug() << "Sending diff" << doc.toJson().size();

    this->write(doc.toJson());
    this->write(
        "\n"); // Use the newline character as the end-of-object delimiter
    this->flush();
  }
}

void Client::processMessage() {
  QJsonDocument doc = QJsonDocument::fromJson(buffer);
  QJsonObject message = doc.object();

  if (message["type"].toString() == "image") {
    QByteArray byteArray =
        QByteArray::fromBase64(message["data"].toString().toLatin1());
    QImage image;
    image.loadFromData(byteArray);
    editManager->newImage(image, false);
  } else if (message["type"].toString() == "image-diff") {
    QString data = message["data"].toString();
    ImageDiff diff;
    diff.fromJson(data);
    editManager->makeEdit(diff, false);
  }

  buffer.clear();
}

void Client::readyRead() {
  QByteArray rawData = readAll();
  buffer.append(rawData);
  if (!rawData.isEmpty() && rawData.endsWith('\n')) {
    processMessage();
  }
}

void Client::update(Event *data) {
  switch (data->type) {
  case EventType::NEW_IMAGE:
    sendImage(dynamic_cast<NewImageEvent *>(data)->image);
    break;
  case EventType::NEW_EDIT:
    sendImageDiff(dynamic_cast<NewEditEvent *>(data)->edit);
    break;
  default:
    break;
  }
}