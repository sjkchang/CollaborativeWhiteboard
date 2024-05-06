#ifndef CANVAS_EVENT_MANAGER_H
#define CANVAS_EVENT_MANAGER_H

#include "ImageDiff.h"
#include <QHash>
#include <QList>
#include <functional>
#include <type_traits>
#include <typeindex>

enum EventType { NEW_IMAGE, NEW_EDIT, REMOTE_EDIT, SAVE, OPEN };

class Event {
public:
  Event(EventType type) : type(type) {}
  virtual ~Event() {}

  EventType type;
};

class NewImageEvent : public Event {
public:
  NewImageEvent(QImage image) : image(image), Event(EventType::NEW_IMAGE) {}
  QImage image;
};

class NewEditEvent : public Event {
public:
  NewEditEvent(ImageDiff edit) : edit(edit), Event(EventType::NEW_EDIT) {}
  ImageDiff edit;
};

class RemoteEditEvent : public Event {
public:
  RemoteEditEvent(ImageDiff edit) : edit(edit), Event(EventType::REMOTE_EDIT) {}
  ImageDiff edit;
};

class SaveEvent : public Event {
public:
  SaveEvent(QString fileName) : fileName(fileName), Event(EventType::SAVE) {}
  QString fileName;
};

class OpenEvent : public Event {
public:
  OpenEvent(QString fileName) : fileName(fileName), Event(EventType::OPEN) {}
  QString fileName;
};

#endif // CANVAS_EVENT_MANAGER_H