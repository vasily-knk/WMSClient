#ifndef PROVIDER_TEST_QT_H
#define PROVIDER_TEST_QT_H

#include "../tiles_provider/tile_provider.h"

class lru_cache;

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

    static inline QString tileIdToString(const tile_id_t &id);
private:
    tile_provider provider_;
    shared_ptr<const tile_t> tile_;
    shared_ptr<lru_cache> cache_;

    int zoom_, x_, y_;

    QTimer *timer_;
    QLabel *screen_, *location_;
    QListView *cache_list_;
    QStringListModel list_model_;

    bool tile_requested_;
};

#endif // PROVIDER_TEST_QT_H
