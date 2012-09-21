#include "StdAfx.h"
#include "grid_widget.h"
#include "google_projection.h"

grid_widget::grid_widget(QWidget *parent)
    : QWidget(parent)
    , dragging_(false)
    , pixmap("0.png")
    , level_(0)
{
    transform_.scale(1, -1);
}

grid_widget::~grid_widget()
{

}

void grid_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setWorldTransform(transform_);

    const QTransform inv = transform_.inverted();
    const QPointF p0 = inv.map(QPointF(0, 0));
    const QPointF p1 = inv.map(QPointF(size().width(), size().height()));

    const qreal grid_size = qreal(256);
    const QPoint i0 (::ceil(p0.x() / grid_size), ::ceil(p0.y() / grid_size));
    const QPoint i1 (::floor(p1.x() / grid_size), ::floor(p1.y() / grid_size));

/*
    const QPoint i0 (0, 0);
    const QPoint i1 (10, 10);
*/
    //QRect src_rect(0, 0, 256, 256); 
    for (int x = i0.x(); x < i1.x(); ++x)
    {
        for (int y = i1.y(); y < i0.y(); ++y)
        {
            const QRect rect(x * 256, y * 256, 256, 256);
            //painter.drawPixmap(rect.topLeft(), pixmap, src_rect);
            painter.drawRect(rect);

            const QString str = QString::number(x) + ", " + QString::number(y);
            painter.drawText(rect, str);
        }
    }

}

void grid_widget::mousePressEvent(QMouseEvent *event)
{
    last_moust_pos_ = event->pos();
    dragging_ = true;
}

void grid_widget::mouseReleaseEvent(QMouseEvent *event)
{
    dragging_ = false;
}

void grid_widget::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging_)
    {
        QPoint current_pos = event->pos();
        transform_.translate(current_pos.x() - last_moust_pos_.x(), -current_pos.y() + last_moust_pos_.y());
        last_moust_pos_ = event->pos();
        update();
    }
}
