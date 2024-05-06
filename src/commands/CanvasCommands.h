#ifndef CANVAS_COMMANDS_H
#define CANVAS_COMMANDS_H

#include "../model/Canvas.h"
#include "../model/EditManager.h"
#include "../model/SaveManager.h"
#include "../model/Tool.h"
#include "ICommand.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QImage>
#include <QString>
#include <qinputdialog.h>
#include <string>

class UndoCommand : public ICommand {
public:
  UndoCommand(EditManager *editManager) : editManager(editManager) {}
  void execute() override;

private:
  EditManager *editManager;
};

class RedoCommand : public ICommand {
public:
  RedoCommand(EditManager *editManager) : editManager(editManager) {}
  void execute() override;

private:
  EditManager *editManager;
};

class OpenCommand : public ICommand {
public:
  OpenCommand(EditManager *editManager) : editManager(editManager) {}
  void execute() override;

private:
  EditManager *editManager;
};

class SaveAsCommand : public ICommand {
public:
  SaveAsCommand(EditManager *editManager) : editManager(editManager) {}
  void execute() override;

private:
  EditManager *editManager;
};

class SaveCommand : public ICommand {
public:
  SaveCommand(EditManager *editManager, SaveManager *saveManager)
      : editManager(editManager), saveManager(saveManager) {}
  void execute() override;

private:
  EditManager *editManager;
  SaveManager *saveManager;
};

class SetToolCommand : public ICommand {
public:
  SetToolCommand(ToolManager *toolManager, QString toolName)
      : toolManager(toolManager), toolName(toolName) {}
  void execute() override { toolManager->setCanvasTool(toolName); }

private:
  ToolManager *toolManager;
  QString toolName;
};

class SetColorCommand : public ICommand {
public:
  SetColorCommand(ToolManager *toolManager) : toolManager(toolManager) {}
  void execute() override;

private:
  ToolManager *toolManager;
};

class SetBrushSizeCommand : public ICommand {
public:
  SetBrushSizeCommand(ToolManager *toolManager) : toolManager(toolManager) {}
  void execute() override;

private:
  ToolManager *toolManager;
};

class SetOpacityCommand : public ICommand {
public:
  SetOpacityCommand(ToolManager *toolManager) : toolManager(toolManager) {}
  void execute() override;

private:
  ToolManager *toolManager;
};

#endif // CANVAS_COMMANDS_H