#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(153,153,153));
    w.setPalette(palette);
    w.show();

    return a.exec();
}
