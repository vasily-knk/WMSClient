#include "stdafx.h"
#include "provider_test_qt.h"
#include "../tiles_provider/lru_cache.h"
#include "../tiles_provider/wms_png_provider.h"
#include "../tiles_provider/disk_png_cache.h"

provider_test_qt::provider_test_qt(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
    , zoom_(0)
    , x_(0)
    , y_(0)

    , timer_(new QTimer(this))
    , tile_requested_(false)

    , cache_(new lru_cache(5, 10))
{
    shared_ptr<wms_png_provider> wms(new wms_png_provider("192.168.121.129", provider_.io_service()));
    shared_ptr<disk_png_cache> disk_cache(new disk_png_cache("./cache", provider_.io_service()));

    provider_.add_provider(cache_);
    provider_.add_provider(disk_cache);
    provider_.add_provider(wms);

    initInterface();

    updateTile();

    connect(timer_, SIGNAL(timeout()), this, SLOT(timerTick()));
    timer_->start(50);
}

provider_test_qt::~provider_test_qt()
{

}

/*
void provider_test_qt::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (tile_ && tile_->ready())
    {
        QImage img(tile_t::WIDTH, tile_t::HEIGHT, QImage::Format_ARGB32_Premultiplied);

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

    QString str = QString::number(zoom_) + " (" + QString::number(x_) + ", " + QString::number(y_) + ")";
    painter.drawText(0, 256, str);
}
*/

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

    tile_id_t id (zoom_, x_, y_);
    tile_ = provider_.request_tile(id);
    tile_requested_ = true;
    location_->setText(tileIdToString(id));

    QStringList string_list;
    //for (auto it = cache_->get_map().get_map().begin(); it != cache_->get_map().get_map().end(); ++it)
    BOOST_FOREACH(auto &v, cache_->get_map())
        string_list.push_back(tileIdToString(v.first));

    list_model_.setStringList(string_list);

    qDebug() << "Tiles: " << tile_t::get_num_tiles();
}

void provider_test_qt::timerTick()
{
    if (tile_requested_ && tile_->ready())
    {
        updateImage();
        
        update();
        setWindowTitle("Done.");
        tile_requested_ = false;
    }
}

void provider_test_qt::initInterface()
{
    QGridLayout *layout = new QGridLayout;
    
    screen_ = new QLabel(this);
    //screen_->setPixmap(QPixmap(tile_t::WIDTH, tile_t::HEIGHT));
    layout->addWidget(screen_, 0, 0);

    location_ = new QLabel(this);
    layout->addWidget(location_, 1, 0);

    cache_list_ = new QListView(this);
    cache_list_->setModel(&list_model_);
    cache_list_->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(cache_list_, 0, 1, 2, 1);

    setLayout(layout);
}

void provider_test_qt::updateImage()
{
    QImage img(tile_t::WIDTH, tile_t::HEIGHT, QImage::Format_ARGB32_Premultiplied);

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

    QPixmap pixmap;
    pixmap.convertFromImage(img);

    screen_->setPixmap(pixmap);
}

QString provider_test_qt::tileIdToString(const tile_id_t &id)
{
    return QString::number(id.zoom) + " (" + QString::number(id.x) + ", " + QString::number(id.y) + ")";
}
