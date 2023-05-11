#include "planargraphwidget.h"
// #include "graph_algorithm.h"

PlanarGraphWidget::PlanarGraphWidget(QWidget* parent)
    : PlanarGraphWidget(my::Graph(0), parent)
{}

PlanarGraphWidget::PlanarGraphWidget(const my::Graph& graph, QWidget* parent)
    : GraphWidget(graph, parent)
    , _mode(Mode::LAYERED)
{
    Initialize();
    _scene->advance();
}

void PlanarGraphWidget::Initialize() {
    GraphWidget::Initialize();
    _planar_graphs = my::graph_partitioning_in_planes(_graph);
    using namespace my;
    clear();
    _m_vertex.reserve(_graph.size());
    for(const vertex_t v : _graph)
        _m_vertex.push_back(new GraphicsVertexItem(QString::number(v)));

    for (const Graph& g : _planar_graphs)
        for(const vertex_t v : g)
            for (const edge_t e : Graph::vertex_iterator(g, v)) {
                _m_edge[e] = new GraphicsEdgeItem(_m_vertex[e.first], _m_vertex[e.second], false, {});
                _scene->addItem(_m_edge[e]);
            }


   // for(auto p : pvec) {
   //     p->setPos(QPoint(rand() % (_graph.size() * 30) % 700, rand() % (_graph.size() * 30) % 700));
   //     _scene->addItem(p);
   // }
    SetVertexPos();
}

void PlanarGraphWidget::ChangeEdgesColorsByMode() {
    using namespace my;
    if (_mode == Mode::LAYERED) {
        for (const Graph& g : _planar_graphs) {
            QColor random_color(rand() % 255, rand() % 255,rand() % 255);
            for(const vertex_t v : g)
                for (const edge_t e : Graph::vertex_iterator(g, v))
                    _m_edge[e]->SetColor(random_color);
        }
    } else {
        for (const Graph& g : _planar_graphs)
            for(const vertex_t v : g)
                for (const edge_t e : Graph::vertex_iterator(g, v))
                    _m_edge[e]->SetColor(Qt::black);
    }
    _scene->update();
}

void PlanarGraphWidget::SetMode(Mode mode) {
    //if (mode == _mode)
    //    return;
    _mode = mode;
    ChangeEdgesColorsByMode();
}

PlanarGraphWidget::Mode PlanarGraphWidget::mode() const {
    return _mode;
}

std::pair<std::vector<QPointF>, std::vector<std::vector<my::Arc>>> PlanarGraphWidget::RouteData() {
    std::pair<std::vector<QPointF>, std::vector<std::vector<my::Arc>>> route_data;

    for (auto vp : _m_vertex)
        route_data.first.push_back(vp->pos());
    for (my::Graph g : _planar_graphs)
        route_data.second.emplace_back(my::ordered_embadding(g));

   //  emit PlanarEmbedding(embeddings);
    return route_data;
}
