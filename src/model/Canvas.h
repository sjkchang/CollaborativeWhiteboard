#ifndef CANVAS_H
#define CANVAS_H

#include "EditManager.h"
#include "Tool.h"
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

class ToolManager;
class EditManager;
class Tool;
class PencilTool;

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = nullptr, int width = 1024,
                  int height = 716);
  QImage image;
  QImage toolImage;
  QImage lastImage;
  EditManager *getEditManager() const;
  ToolManager *getToolManager() const;

  void setTool(Tool *tool);

  Tool *currentTool;

protected:
  void paintEvent(QPaintEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  void resizeImage(const QSize &newSize);

  bool toolActive;
  QPoint lastPoint;

  EditManager *editManager;
  ToolManager *toolManager;
};

#endif // CANVAS_H
