#ifndef EDIT_HISTORY_H
#define EDIT_HISTORY_H

#include "Canvas.h"
#include "Events.h"
#include "ImageDiff.h"
#include <QColor>
#include <QDateTime>
#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QPoint>
#include <QRgb>
#include <QStack>

class Observer {
public:
  virtual void update(Event *data) = 0;
};

class Canvas;

class EditManager {
public:
  EditManager(Canvas *canvas) : canvas(canvas) {}
  void makeEdit(ImageDiff diff, bool localChange = true);
  void newImage(QImage image, bool localChange = true);
  void undo();
  void redo();

  bool save(QString fileName);
  void open(QImage image, QString fileName);

  QImage getImage();

  void subscribe(EventType type, Observer *observer);
  void unsubscribe(EventType type, Observer *observer);
  void notify(Event *data);

private:
  Canvas *canvas;
  QStack<QList<PixelDelta>> undoStack;
  QStack<QList<PixelDelta>> redoStack;
  std::map<EventType, std::list<Observer *>> observers;
};

#endif