#include "graphwidget.h"
#include "planargraphwidget.h"
#include "graph_algorithm.h"
#include "test.h"
#include "MainWindow.h"
#include <QApplication>
#include <QTime>
#include "circuit_board.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
   // test_planarity_functions();

    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
