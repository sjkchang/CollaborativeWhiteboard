/*
#include "CanvasEventManager.h"

void CanvasEventManager::subscribe(EventType type, Observer *observer) {
  observers[type].push_back(observer);
}

void CanvasEventManager::unsubscribe(EventType type, Observer *observer) {
  observers[type].remove(observer);
}

void CanvasEventManager::notify(Event *data) {

  qDebug() << "CanvasEventManager::notify";
  qDebug() << "event type: " << data->type;
  for (auto &obs : observers[data->type]) {
    // Check if the observer is not null
    if (obs) {
      obs->update(data);
    } else {
      qDebug() << "Observer is null";
    }
  }
}*/