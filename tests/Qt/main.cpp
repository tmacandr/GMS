#include "dcw_browse_main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dcw_browse_Main_Window w;
    w.show();
    return a.exec();
}
