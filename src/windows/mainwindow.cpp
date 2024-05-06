#include "mainwindow.h"
#include "../commands/CanvasCommands.h"
#include "../commands/NetworkCommands.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // constructor implementation
  canvas = new Canvas(this);
  editManager = canvas->getEditManager();
  toolManager = canvas->getToolManager();
  toolManager->setCanvasTool("Pencil");

  this->setCentralWidget(canvas);
  setWindowTitle(tr("Canvas"));
  resize(1024, 716);

  createActions();
  createMenus();

  host = new Host(editManager);
  client = new Client(editManager);

  saveManager = new SaveManager();
  editManager->subscribe(EventType::NEW_IMAGE, saveManager);
  editManager->subscribe(EventType::NEW_EDIT, saveManager);
  editManager->subscribe(EventType::REMOTE_EDIT, saveManager);
  editManager->subscribe(EventType::SAVE, saveManager);
  editManager->subscribe(EventType::OPEN, saveManager);
}

void MainWindow::closeEvent(QCloseEvent *event) { event->accept(); }

void MainWindow::createActions() {
  /*
   * File Actions
   */

  // Create the action tied to the menu
  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  connect(openAct, &QAction::triggered,
          [this]() { OpenCommand(editManager).execute(); });

  // Get a list of the supported file formats
  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  connect(saveAct, &QAction::triggered,
          [this]() { SaveCommand(editManager, saveManager).execute(); });

  // Get a list of the supported file formats
  saveAsAct = new QAction(tr("Save As..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  connect(saveAsAct, &QAction::triggered,
          [this]() { SaveAsCommand(editManager).execute(); });

  // Create exit action and tie to MainWindow::close()
  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  /*
   * Edit Actions
   */
  undoAct = new QAction(tr("Undo..."), this);
  undoAct->setShortcut(QKeySequence::Undo);
  connect(undoAct, &QAction::triggered,
          [this]() { UndoCommand(editManager).execute(); });

  redoAct = new QAction(tr("Redo..."), this);
  redoAct->setShortcut(QKeySequence::Redo);
  connect(redoAct, &QAction::triggered,
          [this]() { RedoCommand(editManager).execute(); });

  /*
   * Tool Actions
   */
  // Get a list of the available tools
  foreach (QString toolName, toolManager->getToolNames()) {
    // Create an action for each tool
    QAction *action = new QAction(toolName, this);
    connect(action, &QAction::triggered, [this, toolName]() {
      SetToolCommand(toolManager, toolName).execute();
    });

    toolActs.append(action);
  }

  // Create pen color action and tie to MainWindow::penColor()
  toolColorAct = new QAction(tr("Pen &Color..."), this);
  connect(toolColorAct, &QAction::triggered,
          [this]() { SetColorCommand(toolManager).execute(); });

  // Create pen width action and tie to MainWindow::penWidth()
  toolSizeAct = new QAction(tr("Pen &Width..."), this);
  connect(toolSizeAct, &QAction::triggered,
          [this]() { SetBrushSizeCommand(toolManager).execute(); });

  // Create pen opacity action and tie to MainWindow::penOpacity()
  toolOpacityAct = new QAction(tr("Pen &Opacity..."), this);
  connect(toolOpacityAct, &QAction::triggered,
          [this]() { SetOpacityCommand(toolManager).execute(); });

  /*
   * Connection Actions
   */
  // Create connect action and tie to MainWindow::connectToHost()
  connectAct = new QAction(tr("Connect..."), this);
  connect(connectAct, &QAction::triggered, [this]() {
    ConnectToHostCommand(client, QHostAddress::LocalHost, 3000).execute();

    editManager->subscribe(EventType::NEW_IMAGE, client);
    editManager->subscribe(EventType::NEW_EDIT, client);

    stopHostAct->setEnabled(false);
    startHostAct->setEnabled(false);
    connectAct->setEnabled(false);
    disconnectAct->setEnabled(true);
  });

  // Create disconnect action and tie to MainWindow::disconnectFromHost()
  disconnectAct = new QAction(tr("Disconnect..."), this);
  connect(disconnectAct, &QAction::triggered, [this]() {
    DisconnectFromHostCommand(client).execute();
    disconnectAct->setEnabled(false);
    connectAct->setEnabled(true);
    startHostAct->setEnabled(true);
    stopHostAct->setEnabled(false);
  });
  disconnectAct->setEnabled(false);

  // Create start host action and tie to MainWindow::startHost()
  startHostAct = new QAction(tr("Start Hosting..."), this);
  connect(startHostAct, &QAction::triggered, [this]() {
    StartHostCommand(host, client, 3000).execute();

    editManager->subscribe(EventType::NEW_IMAGE, client);
    editManager->subscribe(EventType::NEW_EDIT, client);

    stopHostAct->setEnabled(true);
    startHostAct->setEnabled(false);
    connectAct->setEnabled(false);
  });

  // Create stop host action and tie to MainWindow::stopHost()
  stopHostAct = new QAction(tr("Stop Hosting..."), this);
  connect(stopHostAct, &QAction::triggered, [this]() {
    StopHostCommand(host, client).execute();
    stopHostAct->setEnabled(false);
    startHostAct->setEnabled(true);
    connectAct->setEnabled(true);
  });
  stopHostAct->setEnabled(false);
}

// Create the menubar
void MainWindow::createMenus() {

  // Attach all actions to File
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  // Attach all actions to Options
  editMenu = new QMenu(tr("&Edit"), this);
  editMenu->addAction(undoAct);
  editMenu->addAction(redoAct);
  editMenu->addSeparator();

  // Attach all actions to Tools
  toolsMenu = new QMenu(tr("&Tools"), this);
  toolSelectionMenu = new QMenu(tr("&Tools"), this);
  foreach (QAction *action, toolActs)
    toolSelectionMenu->addAction(action);
  toolsMenu->addMenu(toolSelectionMenu);
  toolsMenu->addAction(toolColorAct);
  toolsMenu->addAction(toolSizeAct);
  toolsMenu->addAction(toolOpacityAct);
  toolsMenu->addSeparator();

  connectionMenu = new QMenu(tr("&Connection"), this);
  connectionMenu->addAction(startHostAct);
  connectionMenu->addAction(stopHostAct);
  connectionMenu->addSeparator();
  connectionMenu->addAction(connectAct);
  connectionMenu->addAction(disconnectAct);
  connectionMenu->addSeparator();
  connectionMenu->addSeparator();

  menuBar()->addMenu(fileMenu);
  menuBar()->addMenu(editMenu);
  menuBar()->addMenu(toolsMenu);
  menuBar()->addMenu(connectionMenu);
}
