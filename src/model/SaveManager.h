#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include "EditManager.h"

class SaveManager : public Observer {

public:
  SaveManager() = default;
  ~SaveManager() = default;

  bool modifiedSinceLastSave = true;

  void update(Event *data) override {
    modifiedSinceLastSave = true;
    if (data->type == EventType::SAVE) {
      SaveEvent *saveEvent = dynamic_cast<SaveEvent *>(data);
      fileName = saveEvent->fileName;
      modifiedSinceLastSave = false;
    } else if (data->type == EventType::OPEN) {
      OpenEvent *openEvent = dynamic_cast<OpenEvent *>(data);
      fileName = openEvent->fileName;
      modifiedSinceLastSave = false;
    }
  };

  QString fileName;
};

#endif // SAVE_MANAGER_H