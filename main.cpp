#include "titleeditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TitleEditor w;
    w.show();
    return a.exec();
}
