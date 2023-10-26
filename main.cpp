#include <QtWidgets/QSplashScreen>
// #include <QtCore/QThread>


#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/imgs/splash.jpg"));
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    
    splash->showMessage(QObject::tr("Setting up the main window..."),
                        topRight, Qt::black);
    MainWindow win;

//     splash->showMessage(QObject::tr("Testing windows (5s)..."),
//                         topRight, Qt::black);
//     QThread::sleep(2);
    // QTime timer;
    // timer.start();
    // while (timer.elapsed() < 5000)
    //     app.processEvents();

    win.show();
    splash->finish(&win);
    delete splash;

    return app.exec();
}