#ifndef GRID_WIDGET_H
#define GRID_WIDGET_H

#include <QObject>

class grid_widget : public QWidget
{
    Q_OBJECT

public:
    grid_widget(QWidget *parent);
    ~grid_widget();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QTransform transform_;
    bool dragging_;
    QPoint last_moust_pos_;

    QPixmap pixmap;

    int level_;
    //co
};

#endif // GRID_WIDGET_H
