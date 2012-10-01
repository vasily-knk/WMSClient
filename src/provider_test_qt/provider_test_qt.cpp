#include "stdafx.h"
#include "provider_test_qt.h"

provider_test_qt::provider_test_qt(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
    , provider_("192.168.121.129")
    , zoom_(0)
    , x_(0)
    , y_(0)

    , timer_(new QTimer(this))
    , tile_requested_(false)
{
    updateTile();

    connect(timer_, SIGNAL(timeout()), this, SLOT(timerTick()));
    timer_->start(50);
}

provider_test_qt::~provider_test_qt()
{

}

void provider_test_qt::paintEvent(QPaintEvent *event)
{
    if (tile_ && tile_->ready())
    {
        QImage img(tile_t::WIDTH, tile_t::HEIGHT, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(this);

        unsigned char* ptr = tile_->get_data();
        for (size_t y = 0; y < tile_t::HEIGHT; ++y)
        {
            for (size_t x = 0; x < tile_t::WIDTH; ++x)
            {
                const QRgb rgb = qRgb(ptr[0], ptr[1], ptr[2]);
                img.setPixel(x, y, rgb);
                ptr += 3;
            }
        }

        painter.drawImage(QPointF(0, 0), img);
    }
}

void provider_test_qt::keyPressEvent(QKeyEvent *event)
{
    const size_t MAX_ZOOM = 100;
    
    bool tilechange = true;
    switch (event->key())
    {
    case Qt::Key_PageUp:
        if (zoom_ < MAX_ZOOM)
        {
            ++zoom_;
            x_ *= 2;
            y_ *= 2;
        }
        break;
    case Qt::Key_PageDown:
        if (zoom_ > 0)
        {
            --zoom_;
            x_ /= 2;
            y_ /= 2;
        }
        break;
    case Qt::Key_Left:
        --x_;
        break;
    case Qt::Key_Right:
        ++x_;
        break;
    case Qt::Key_Up:
        ++y_;
        break;
    case Qt::Key_Down:
        --y_;
        break;
    default:
        tilechange = false;
    }

    auto bound = [](int &val, const int min, const int max) -> void
    {
        val = std::min(val, max);
        val = std::max(val, min);
    };
    
    if (tilechange)
    {
        const size_t size = 1 << zoom_;
        bound(x_, 0, size - 1);
        bound(y_, 0, size - 1);

        updateTile();
    }

}

void provider_test_qt::updateTile()
{
    //setWindowTitle(QString::number(zoom_) + ": " + QString::number(x_) + ", " + QString::number(y_));
    setWindowTitle("Loading...");
    
    const size_t uses = tile_.use_count();

    tile_ = provider_.request_tile(tile_id_t(zoom_, x_, y_));
    tile_requested_ = true;

    qDebug() << "Tiles: " << tile_t::get_num_tiles();
}

void provider_test_qt::timerTick()
{
    if (tile_requested_ && tile_->ready())
    {
        update();
        setWindowTitle("Done.");
        tile_requested_ = false;
    }
}
