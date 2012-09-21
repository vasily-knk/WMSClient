#include "stdafx.h"
#include "grid_widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    grid_widget window(NULL);
    window.show();
    return app.exec();    return 0;
}
