#include "Canvas.h"
#include "ImageDiff.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtWidgets>

Canvas::Canvas(QWidget *parent, int width, int height) : QWidget(parent) {
  // Roots the widget to the top left even if resized
  setAttribute(Qt::WA_StaticContents);
  QSize imageSize(width, height);
  image = QImage(width, height, QImage::Format_RGB32);
  image.fill(qRgb(255, 255, 255));
  resizeImage(imageSize);

  editManager = new EditManager(this);
  toolManager = new ToolManager(this);
  toolActive = false;
}

EditManager *Canvas::getEditManager() const { return editManager; }
ToolManager *Canvas::getToolManager() const { return toolManager; }
/*
Mouse Event Handlers
*/
void Canvas::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    toolImage = QImage(image);
    toolActive = true;
    currentTool->start(event->pos());
  }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
  if ((event->buttons() & Qt::LeftButton))
    currentTool->update(event->pos());
  update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    currentTool->end(event->pos());
    toolActive = false;

    ImageDiff edit = ImageDiff(image, toolImage);
    editManager->makeEdit(edit);

    update();
  }
}

void Canvas::setTool(Tool *tool) { this->currentTool = tool; }

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  QRect dirtyRect = event->rect();

  if (toolActive) {
    painter.drawImage(dirtyRect, toolImage, dirtyRect);
  } else {
    painter.drawImage(dirtyRect, image, dirtyRect);
  }
}

void Canvas::resizeImage(const QSize &newSize) {
  // Check if we need to redraw the image
  if (image.size() == newSize)
    return;

  // Create a new image to display and fill it with white
  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));

  // Draw the image
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), image);
  image = newImage;
}
