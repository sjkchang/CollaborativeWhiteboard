#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../model/Canvas.h"
#include "../model/EditManager.h"
#include "../model/SaveManager.h"
#include "../model/Tool.h"
#include "../network/Client.h"
#include "../network/Host.h"
#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  void createActions();
  void createMenus();

  Canvas *canvas;
  EditManager *editManager;
  SaveManager *saveManager;
  ToolManager *toolManager;

  Host *host;
  Client *client;

  QMenu *fileMenu;
  QAction *openAct;
  QAction *saveAct;
  QAction *saveAsAct;
  QAction *exitAct;

  QMenu *editMenu;
  QAction *undoAct;
  QAction *redoAct;

  QMenu *toolsMenu;
  QMenu *toolSelectionMenu;
  QList<QAction *> toolActs;
  QAction *toolColorAct;
  QAction *toolSizeAct;
  QAction *toolOpacityAct;

  QMenu *connectionMenu;
  QAction *startHostAct;
  QAction *stopHostAct;
  QAction *connectAct;
  QAction *disconnectAct;
};
#endif // MAINWINDOW_H
