#include "stdafx.h"
#include "provider_test_qt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    provider_test_qt w;
    w.show();

    auto res = a.exec();

    return res;
}
