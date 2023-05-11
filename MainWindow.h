#ifndef HEADER_FILE_MAIN_WINDOW
#define HEADER_FILE_MAIN_WINDOW

#include <QMainWindow>

class QDockWidget;

class MainWindow : public QMainWindow
{
    //Q_OBJECT

public:
    MainWindow(QWidget* = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent*) override;

private:
    QDockWidget* _dock_widget;
};

#endif // HEADER_FILE_MAIN_WINDOW
