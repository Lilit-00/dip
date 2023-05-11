#include "MainWindow.h"
#include "graphwidget.h"
#include "planargraphwidget.h"
#include "graph_algorithm.h"
#include "circuit_board.h"

#include <QDockWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QMouseEvent>
#include <QBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QInputDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(800, 500);
    setWindowTitle("Diploma Work");
    _dock_widget = new QDockWidget(this);
    _dock_widget->setFeatures(QDockWidget::DockWidgetMovable);

    //my::CircuitBoard* circuit_board = new my::CircuitBoard;
    //circuit_board->show();

    // QPushButton*	button	= new QPushButton("change mode", _dock_widget);
    QComboBox*      combo   = new QComboBox(_dock_widget);
    //QPushButton*	route_button = new QPushButton("route", _dock_widget);
    QPushButton*	random_graph_button = new QPushButton("generate random graph", _dock_widget);
    QPushButton*	browse_file = new QPushButton("browse", _dock_widget);

    QSpinBox*		edge_spinbox	= new QSpinBox(_dock_widget);
    QSpinBox*		vertex_spinbox	= new QSpinBox(_dock_widget);

    QLabel*         mode_label = new QLabel("mode");
    QLabel*         edge_size_label = new QLabel("edges size");
    QLabel*         vertex_size_label = new QLabel("vertex size");

    QLineEdit*		line_edit = new QLineEdit("file path", _dock_widget);

    QScrollArea*	scrollarea	= new QScrollArea(_dock_widget);
    //     QVBoxLayout*	vlayout	= new QVBoxLayout;
    QGridLayout*	layout = new QGridLayout;
    // QSpacerItem*    spacer = new QSpacerItem()

    combo->addItem("normal");
    combo->addItem("layered");

    edge_spinbox->setMaximum(10);
    edge_spinbox->setMinimum(1);

    vertex_spinbox->setMaximum(40);
    vertex_spinbox->setMinimum(15);
    vertex_spinbox->setValue(30);

    _dock_widget->setWidget(scrollarea);

    scrollarea->setLayout(layout);

    // vlayout->addWidget(button);
    // vlayout->addWidget(edge_spinbox);
    // vlayout->addWidget(route_button);

    layout->addWidget(line_edit, 0, 0);
    layout->addWidget(browse_file, 0, 1);

    layout->addWidget(mode_label, 2, 0);
    layout->addWidget(combo, 2, 1);
    layout->addWidget(random_graph_button, 1, 0, 1, 2);
    //layout->addWidget(button, 0, 0);
    layout->addWidget(edge_size_label, 3, 0);
    layout->addWidget(edge_spinbox, 3, 1);
    layout->addWidget(vertex_size_label, 4, 0);
    layout->addWidget(vertex_spinbox, 4, 1);
   // layout->addWidget(route_button, 5, 0, 1, 2);
    addDockWidget(Qt::LeftDockWidgetArea, _dock_widget);

    PlanarGraphWidget* planar_graph_widget = new PlanarGraphWidget(my::Graph(0), this);
    // planar_graph_widget->hide();

    setCentralWidget(planar_graph_widget);

    connect(combo, &QComboBox::textActivated, this, [=](const QString& str){
        if (str == "normal")
            planar_graph_widget->SetMode(PlanarGraphWidget::Mode::UNLAYERD);
        else
            planar_graph_widget->SetMode(PlanarGraphWidget::Mode::LAYERED);
    });

    //connect(button, &QPushButton::pressed, this, [=](){
    //    if (planar_graph_widget->mode() == PlanarGraphWidget::Mode::LAYERED)
    //        planar_graph_widget->SetMode(PlanarGraphWidget::Mode::UNLAYERD);
    //    else
    //        planar_graph_widget->SetMode(PlanarGraphWidget::Mode::LAYERED);
    //});
    connect(random_graph_button, &QPushButton::pressed, this, [=](){
        bool ok = false;
        int vertex_count = QInputDialog::getInt(_dock_widget,
                                                "random graph generation",
                                                "vertex count",
                                                planar_graph_widget->graph_size(), 5, 25, 1,
                                                &ok);
        if (ok) {
            // planar_graph_widget->SetGraph(my::random_bipartite_graph(vertex_count / 2, vertex_count / 2, 0.7));
            planar_graph_widget->SetGraph(my::random_graph(vertex_count, 0.7));
            combo->setCurrentText("normal");
        }
    });
    connect(edge_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value){
        QPen pen = planar_graph_widget->EdgesPen();
        pen.setWidth(value);
        planar_graph_widget->SetEdgesPen(pen);
    });
    connect(vertex_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), planar_graph_widget, &PlanarGraphWidget::SetVertexSize);
//    connect(route_button, &QPushButton::pressed, this, [=](){
//        QScrollArea* scroll_area = new QScrollArea;
//        my::CircuitBoard* circuit_board = new my::CircuitBoard(scroll_area);
//        circuit_board->setWindowTitle("Diploma Work");
//        scroll_area->setWidget(circuit_board);
//        circuit_board->resize(1500, 1500);
//        scroll_area->show();
//        // circuit_board->show();
//        std::pair<std::vector<QPointF>, std::vector<std::vector<my::Arc>>> data = planar_graph_widget->RouteData();
//        circuit_board->SetData(data);
//    });
    connect(browse_file, &QPushButton::pressed, this, [=] () {
        my::Graph graph(0);

        QString fileName = QFileDialog::getOpenFileName(this);
        if (fileName.isEmpty()) return;
        std::ifstream file(fileName.toStdString());
        file >> graph;
        if (graph.size() == 0) return;

        planar_graph_widget->SetGraph(graph);
        combo->setCurrentText("normal");
    });
    connect(line_edit, &QLineEdit::editingFinished, this, [=] () {
        const QString& path = line_edit->text();
        my::Graph graph(0);

        if (path.isEmpty()) return;
        std::ifstream file(path.toStdString());
        if (file.good() == false) return;
        file >> graph;
        if (graph.size() == 0) return;

        planar_graph_widget->SetGraph(graph);
        combo->setCurrentText("normal");
    });
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    std::cout << event->pos().x() << " " << event->pos().y() << std::endl;
}
