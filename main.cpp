#include "colorreduce.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    colorReduce w;
    w.show();

    return a.exec();
}
