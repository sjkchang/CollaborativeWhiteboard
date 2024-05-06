#include "EditManager.h"

QImage EditManager::getImage() { return canvas->image; }

void EditManager::makeEdit(ImageDiff diff, bool localChange) {
  // If the change is local, push the change to the undo stack. We don't need to
  // modfiy the actual image because the change has already been applied.
  if (localChange) {
    undoStack.push(diff.deltas);
    redoStack.clear();

    Event *editEvent = new NewEditEvent(diff);
    notify(editEvent);
  } else {
    Event *editEvent = new RemoteEditEvent(diff);
    notify(editEvent);
  }
  // If the change is not local, apply the change to the image. Do not push
  // the change to the undo stack because a user cannot undo a remote change.
  ImageDiff::apply(canvas->image, diff);
  canvas->update();
}

void EditManager::newImage(QImage image, bool localChange) {
  undoStack.clear();
  redoStack.clear();

  canvas->image = image;
  canvas->update();

  if (localChange == true) {
    Event *newImageEvent = new NewImageEvent(image);
    notify(newImageEvent);
  }
}

void EditManager::open(QImage image, QString fileName) {
  newImage(image, true);
  Event *openEvent = new OpenEvent(fileName);
  notify(openEvent);
}

void EditManager::undo() {
  if (!undoStack.isEmpty()) {

    ImageDiff edit = ImageDiff(undoStack.pop());
    ImageDiff::unapply(canvas->image, edit);
    redoStack.push(edit.deltas);
    canvas->update();

    Event *editEvent = new NewEditEvent(edit.getReversedDiff());
    notify(editEvent);
  }
}

void EditManager::redo() {
  if (!redoStack.isEmpty()) {
    ImageDiff edit = ImageDiff(redoStack.pop());
    ImageDiff::apply(canvas->image, edit);
    undoStack.push(edit.deltas);

    canvas->update();

    Event *editEvent = new NewEditEvent(edit);
    notify(editEvent);
  }
}

void EditManager::subscribe(EventType type, Observer *observer) {
  observers[type].push_back(observer);
}

void EditManager::unsubscribe(EventType type, Observer *observer) {
  observers[type].remove(observer);
}

void EditManager::notify(Event *data) {

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
}

bool EditManager::save(QString fileName) {
  QImage image = canvas->image;
  bool saved = image.save(fileName);

  Event *saveEvent = new SaveEvent(fileName);
  notify(saveEvent);

  return saved;
}
