#ifndef HEADER_FILE_PLANAR_GRAPH_WIDGET
#define HEADER_FILE_PLANAR_GRAPH_WIDGET

#include "graphwidget.h"
#include "graph_algorithm.h"

#include <list>
#include <QObject>


class PlanarGraphWidget : public GraphWidget
{
    // Q_OBJECT

public:
    PlanarGraphWidget(QWidget* = nullptr);
    PlanarGraphWidget(const my::Graph&, QWidget* = nullptr);

public:
    enum Mode { UNLAYERD, LAYERED };

    Mode mode() const;

public:
    void SetMode(Mode);
    std::pair<std::vector<QPointF>, std::vector<std::vector<my::Arc>>> RouteData();

signals:
    void PlanarEmbedding(const std::vector<std::vector<my::Arc>>&);

protected:
    void Initialize() override;
    void ChangeEdgesColorsByMode();
protected:
    std::list<my::Graph> _planar_graphs;
    Mode _mode;
};

#endif // HEADER_FILE_PLANAR_GRAPH_WIDGET
