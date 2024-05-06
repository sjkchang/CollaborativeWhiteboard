#include "CanvasCommands.h"

void UndoCommand::execute() { editManager->undo(); }
void RedoCommand::execute() { editManager->redo(); }

void OpenCommand::execute() {
  QString fileName = QFileDialog::getOpenFileName(
      nullptr, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");
  if (fileName.isEmpty()) {
    return;
  }
  QImage image(fileName);
  editManager->open(image, fileName);
}

void SaveAsCommand::execute() {
  QString fileName = QFileDialog::getSaveFileName(
      nullptr, "Save Image", "", "Image Files (*.png *.jpg *.bmp)");
  if (fileName.isEmpty()) {
    return;
  }
  editManager->save(fileName);
}

void SaveCommand::execute() {
  if (saveManager->fileName.isEmpty()) {
    SaveAsCommand saveAsCommand(editManager);
    saveAsCommand.execute();
  } else {
    editManager->save(saveManager->fileName);
  }
}

void SetColorCommand::execute() {
  QColor newColor = QColorDialog::getColor(toolManager->getColor());
  if (newColor.isValid()) {
    toolManager->setColor(newColor);
  }
}

void SetBrushSizeCommand::execute() {
  bool ok;
  int size = QInputDialog::getInt(nullptr, "Brush Size", "Enter brush size:",
                                  toolManager->getBrushSize(), 1, 100, 1, &ok);
  if (ok) {
    toolManager->setBrushSize(size);
  }
}

void SetOpacityCommand::execute() {
  bool ok;
  int opacity = QInputDialog::getInt(nullptr, "Opacity", "Enter opacity:",
                                     toolManager->getOpacity(), 0, 255, 1, &ok);
  if (ok) {
    toolManager->setOpacity(opacity);
  }
}
