#include "mainwindow.h"
#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
#if 0
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app( argc, argv );
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    app.setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);
#endif

    Office2016Style* style = new Office2016Style();
    style->setTheme(Office2016Style::Colorful);
    app.setStyle(style);

    MainWindow w;
    w.show();
    return app.exec();

}



