#include "stdafx.h"
#include "requester.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    requester r;
    r.go();

    return a.exec();
}
