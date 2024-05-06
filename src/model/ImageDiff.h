#ifndef IMAGE_DIFF_H
#define IMAGE_DIFF_H

#include <QColor>
#include <QDateTime>
#include <QHash>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPoint>
#include <QRgb>
#include <QStack>

struct PixelDelta {
public:
  QPoint position;
  int deltaRed;
  int deltaGreen;
  int deltaBlue;
  int deltaAlpha;
  PixelDelta(const QPoint &pos, int dR, int dG, int dB, int dA)
      : position(pos), deltaRed(dR), deltaGreen(dG), deltaBlue(dB),
        deltaAlpha(dA) {}
};

class ImageDiff {
public:
  ImageDiff() {}
  ImageDiff(QList<PixelDelta> deltas) : deltas(deltas) {
    timestamp = QDateTime::currentDateTime();
  }
  ImageDiff(const QImage &oldImage, const QImage &newImage);
  QList<PixelDelta> deltas;
  QDateTime timestamp;

  static void apply(QImage &image, ImageDiff &diff);
  static void unapply(QImage &image, ImageDiff &diff);

  QString toJson() const;
  void fromJson(const QString &jsonString);
  ImageDiff getReversedDiff() const;
};

#endif // IMAGE_DIFF_H