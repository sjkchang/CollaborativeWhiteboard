#ifndef TOOL_H
#define TOOL_H

#include "Canvas.h"
#include <QColor>
#include <QImage>
#include <QList>
#include <QPainter>
#include <QPoint>
#include <QtDebug>

class Canvas;

class Tool {
public:
  Tool(QImage &image) : image(image){};

  virtual void start(QPoint position) = 0;
  virtual void update(QPoint position) = 0;
  virtual void end(QPoint position) = 0;

  void setBrushSize(int size) { brushSize = size; }
  void setOpacity(int opacity) { this->opacity = opacity; }
  void setColor(QColor color) { this->color = color; }
  int getBrushSize() { return brushSize; }
  int getOpacity() { return opacity; }
  QColor getColor() { return color; }

protected:
  int brushSize = 5;
  int opacity = 255;
  QColor color = Qt::black;

  QImage &image;
};

class PencilTool : public Tool {
public:
  PencilTool(QImage &image) : Tool(image){};
  void start(QPoint position) override;
  void update(QPoint position) override;
  void end(QPoint position) override;
  virtual void drawLine(QPoint start, QPoint end);

private:
  QList<QPoint> points;
  bool isDrawing = false;
  QPoint lastPoint;
};

class EraserTool : public PencilTool {
public:
  EraserTool(QImage &image) : PencilTool(image){};

  void drawLine(QPoint start, QPoint end) override;
};

class RectangleTool : public Tool {
public:
  RectangleTool(QImage &image) : Tool(image){};
  void start(QPoint position) override;
  void update(QPoint position) override;
  void end(QPoint position) override;

private:
  QPoint startPoint;
  QPoint endpoint;
  bool isDrawing = false;
  QImage originalImage;

  void drawRectangle(QPoint start, QPoint end);
};

class ToolManager {
public:
  ToolManager(Canvas *canvas);

  void setCanvasTool(QString name);
  Tool *getTool(QString name) { return tools[name]; }
  QList<QString> getToolNames() { return tools.keys(); }

  void setBrushSize(int size);
  void setOpacity(int opacity);
  void setColor(QColor color);
  QColor getColor();
  int getBrushSize();
  int getOpacity();

private:
  QHash<QString, Tool *> tools;
  Canvas *canvas;
};

#endif // TOOL_H