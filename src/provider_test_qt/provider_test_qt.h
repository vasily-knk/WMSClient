#ifndef PROVIDER_TEST_QT_H
#define PROVIDER_TEST_QT_H

#include "../tiles_provider/tile_provider2.h"

class provider_test_qt : public QWidget
{
    Q_OBJECT

public:
    provider_test_qt(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~provider_test_qt();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected slots:
    void timerTick();

private:
    void updateTile();
private:
    tile_provider2 provider_;
    shared_ptr<const tile_t> tile_;

    int zoom_, x_, y_;

    QTimer *timer_;
    bool tile_requested_;
};

#endif // PROVIDER_TEST_QT_H
