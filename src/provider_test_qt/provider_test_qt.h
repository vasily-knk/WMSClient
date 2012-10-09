#ifndef PROVIDER_TEST_QT_H
#define PROVIDER_TEST_QT_H

#include "../wms/wms/tile_provider.h"
#include "../wms/lru_cache.h"

class provider_test_qt : public QWidget
{
    Q_OBJECT

public:
    provider_test_qt(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~provider_test_qt();

protected:
    void keyPressEvent(QKeyEvent *event);

protected slots:
    void timerTick();

private:
    void updateTile();
    void initInterface();
    void updateImage();
    void updateList();

    static inline QString tileIdToString(const wms::tile_id_t &id);
private:
    wms::tile_provider provider_;
    shared_ptr<const wms::tile_t> tile_;
    shared_ptr<wms::lru_cache> cache_;

    int zoom_, x_, y_;

    QTimer *timer_;
    QLabel *screen_, *location_;
    QListView *cache_list_;
    QStringListModel list_model_;

    bool tile_requested_;
    std::stringstream tile_provider_debug_;
};

#endif // PROVIDER_TEST_QT_H
