#include "Tool.h"

void PencilTool::start(QPoint position) {
  lastPoint = position;
  isDrawing = true;
}

void PencilTool::update(QPoint position) {
  drawLine(lastPoint, position);
  lastPoint = position;
};

void PencilTool::end(QPoint position) {
  drawLine(lastPoint, position);
  isDrawing = false;
};

void PencilTool::drawLine(QPoint start, QPoint end) {
  QPainter painter(&image);
  QColor color = QColor(this->color.red(), this->color.green(),
                        this->color.blue(), this->opacity);
  painter.setPen(
      QPen(color, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawLine(start, end);
};

void EraserTool::drawLine(QPoint start, QPoint end) {
  QPainter painter(&image);
  QColor color = QColor(255, 255, 255, this->opacity);
  painter.setPen(
      QPen(color, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawLine(start, end);
};

void RectangleTool::start(QPoint position) {
  startPoint = position;
  isDrawing = true;
  originalImage = QImage(image);
};
void RectangleTool::update(QPoint position) {
  endpoint = position;
  image = QImage(originalImage);

  drawRectangle(startPoint, endpoint);
};

void RectangleTool::end(QPoint position) {
  endpoint = position;
  isDrawing = false;
  image = QImage(originalImage);

  drawRectangle(startPoint, endpoint);
};

void RectangleTool::drawRectangle(QPoint start, QPoint end) {
  QPainter painter(&image);
  QColor color = QColor(this->color.red(), this->color.green(),
                        this->color.blue(), this->opacity);
  painter.setPen(
      QPen(color, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

  painter.drawRect(QRect(start, end));
};

ToolManager::ToolManager(Canvas *canvas) : canvas(canvas) {
  tools.insert("Pencil", new PencilTool(canvas->toolImage));
  tools.insert("Rectangle", new RectangleTool(canvas->toolImage));
  tools.insert("Eraser", new EraserTool(canvas->toolImage));
};

void ToolManager::setCanvasTool(QString name) {
  Tool *tool = tools[name];
  if (tool) {
    canvas->setTool(tool);
  }
};

void ToolManager::setBrushSize(int size) {
  canvas->currentTool->setBrushSize(size);
}
void ToolManager::setOpacity(int opacity) {
  canvas->currentTool->setOpacity(opacity);
}
void ToolManager::setColor(QColor color) {
  canvas->currentTool->setColor(color);
}

QColor ToolManager::getColor() { return canvas->currentTool->getColor(); }
int ToolManager::getBrushSize() { return canvas->currentTool->getBrushSize(); }
int ToolManager::getOpacity() { return canvas->currentTool->getOpacity(); }