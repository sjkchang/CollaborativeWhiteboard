#include "ImageDiff.h"

ImageDiff::ImageDiff(const QImage &oldImage, const QImage &newImage) {
  timestamp = QDateTime::currentDateTime();
  for (int x = 0; x < oldImage.width(); x++) {
    for (int y = 0; y < oldImage.height(); y++) {
      QPoint point = QPoint(x, y);
      QColor currentColor = oldImage.pixelColor(point);
      QColor newColor = newImage.pixelColor(point);
      if (currentColor != newColor) {
        PixelDelta delta =
            PixelDelta(point, newColor.red() - currentColor.red(),
                       newColor.green() - currentColor.green(),
                       newColor.blue() - currentColor.blue(),
                       newColor.alpha() - currentColor.alpha());
        deltas.append(delta);
      }
    }
  }
}

void ImageDiff::unapply(QImage &image, ImageDiff &diff) {
  for (const PixelDelta &delta : diff.deltas) {
    QColor currentColor = image.pixelColor(delta.position);
    QColor newColor =
        QColor(qBound(0, currentColor.red() - delta.deltaRed, 255),
               qBound(0, currentColor.green() - delta.deltaGreen, 255),
               qBound(0, currentColor.blue() - delta.deltaBlue, 255),
               qBound(0, currentColor.alpha() - delta.deltaAlpha, 255));
    image.setPixelColor(delta.position, newColor);
  }
}

void ImageDiff::apply(QImage &image, ImageDiff &diff) {
  for (const PixelDelta &delta : diff.deltas) {
    QColor currentColor = image.pixelColor(delta.position);
    QColor newColor =
        QColor(qBound(0, currentColor.red() + delta.deltaRed, 255),
               qBound(0, currentColor.green() + delta.deltaGreen, 255),
               qBound(0, currentColor.blue() + delta.deltaBlue, 255),
               qBound(0, currentColor.alpha() + delta.deltaAlpha, 255));
    image.setPixelColor(delta.position, newColor);
  }
}

QString ImageDiff::toJson() const {
  QJsonObject root;
  QJsonArray deltaArray;

  for (const PixelDelta &delta : deltas) {
    QJsonObject deltaObj;
    deltaObj["position_x"] = delta.position.x();
    deltaObj["position_y"] = delta.position.y();
    deltaObj["deltaRed"] = delta.deltaRed;
    deltaObj["deltaGreen"] = delta.deltaGreen;
    deltaObj["deltaBlue"] = delta.deltaBlue;
    deltaObj["deltaAlpha"] = delta.deltaAlpha;
    deltaArray.append(deltaObj);
  }

  root["deltas"] = deltaArray;
  root["timestamp"] = timestamp.toString(Qt::ISODate);

  QJsonDocument doc(root);

  return doc.toJson(QJsonDocument::Compact);
}

void ImageDiff::fromJson(const QString &jsonString) {
  deltas.clear();

  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
  if (jsonDoc.isNull()) {
    qDebug() << "Failed to create JSON document.";
    return; // Return an empty ImageDiff or handle error accordingly
  }

  QJsonObject jsonObject = jsonDoc.object();
  QList<PixelDelta> parsedDeltas;
  QJsonArray deltaArray = jsonObject["deltas"].toArray();
  for (const QJsonValue &value : deltaArray) {
    QJsonObject deltaObj = value.toObject();
    QPoint position(deltaObj["position_x"].toInt(),
                    deltaObj["position_y"].toInt());
    int dR = deltaObj["deltaRed"].toInt();
    int dG = deltaObj["deltaGreen"].toInt();
    int dB = deltaObj["deltaBlue"].toInt();
    int dA = deltaObj["deltaAlpha"].toInt();
    parsedDeltas.append(PixelDelta(position, dR, dG, dB, dA));
  }
  qDebug() << "Pixel Delta Size" << parsedDeltas.size();

  deltas = parsedDeltas;
  timestamp =
      QDateTime::fromString(jsonObject["timestamp"].toString(), Qt::ISODate);
}

ImageDiff ImageDiff::getReversedDiff() const {
  QList<PixelDelta> reversedDeltas;
  for (const PixelDelta &delta : deltas) {
    reversedDeltas.append(PixelDelta(delta.position, -delta.deltaRed,
                                     -delta.deltaGreen, -delta.deltaBlue,
                                     -delta.deltaAlpha));
  }
  return ImageDiff(reversedDeltas);
}