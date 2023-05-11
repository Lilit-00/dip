#include "graphwidget.h"
#include "graph_algorithm.h"
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

GraphWidget::GraphWidget(QWidget* parent)
	: GraphWidget(my::Graph(0), parent)
{}

GraphWidget::GraphWidget(const my::Graph& graph, QWidget* parent)
	: QGraphicsView(parent)
	, _scene(new QGraphicsScene(this))
	, _graph(graph)
{
    for (my::vertex_t v : _graph)
        for (my::edge_t e : my::Graph::vertex_iterator(_graph, v)) {
            if (_graph.is_connected(e))
                _graph.remove_edge(my::reverse_edge(e));
        }

	setScene(_scene);
	Initialize();

	repaint();
}

void GraphWidget::SetGraph(const my::Graph& graph) {
	_graph = graph;
	Initialize();
}

void GraphWidget::SetEdgesPen(const QPen& pen) {
    _edges_pen = pen;
    for(auto& edge : _m_edge) {
        QPen p = edge.second->pen();
        p.setWidth(pen.width());
        edge.second->setPen(p);
    }
    _scene->update();
}
//void GraphWidget::SetEdgesSize(const int size) {
//    _edges_pen.setWidth(size);
//    SetEdgesPen(_edges_pen);
//}

void GraphWidget::SetVertexSize(const int size) {
    _vertex_size = size;
    for (auto& vertex : _m_vertex)
        vertex->SetSize(_vertex_size);
    _scene->update();
}

QPen GraphWidget::EdgesPen() const {
    return _edges_pen;
}

void GraphWidget::AddEdge(const my::edge_t& e) {
	if(_graph.is_exist(e))
		return;
    _graph.add_edge(e);
    _m_edge[e] = new GraphicsEdgeItem(_m_vertex[e.first], _m_vertex[e.second]);
}

void GraphWidget::RemoveEdge(const my::edge_t& e) {

	if (_graph.is_exist(e) == false)
		return;
	_graph.remove_edge(e);
    delete _m_edge[e];
    _m_edge.erase(e);
}

void GraphWidget::RemoveVertex(const my::vertex_t & v) {
    // _graph.remove_vertex(v);
    // Initialize();
}

void GraphWidget::AddVertex() {
	// _graph.add_vertex();
}

size_t GraphWidget::graph_size() const {
    return _graph.size();
}

void GraphWidget::Initialize() {
	using namespace my;
    clear();
    _m_vertex.resize(_graph.size());
	for(const vertex_t v : _graph)
        _m_vertex[v] = new GraphicsVertexItem(QString::number(v));
    for(const vertex_t v : _graph)
        for (const edge_t e : Graph::vertex_iterator(_graph, v)) {
            _m_edge[e] = new GraphicsEdgeItem(_m_vertex[e.first], _m_vertex[e.second]);
            _scene->addItem(_m_edge[e]);
        }
    SetVertexPos();
}

void GraphWidget::clear() {
    for (auto p : _m_vertex)
        delete p;

    for (auto p : _m_edge)
        delete p.second;

    _m_vertex.clear();
    _m_edge.clear();
    _scene->clear();
}

void GraphWidget::SetVertexPos() {
    auto bpart = my::bipartite(_graph);
    if (bpart.empty()) {
        for(auto v : _m_vertex) {
            v->setPos(QPoint(rand() % (_graph.size() * 60) % 1200, rand() % (_graph.size() * 60) % 700));
            _scene->addItem(v);
        }
    } else {
        const int step = 50;
        int x_coordinate = 0;
        int distance = 200;
        for(const my::vertex_t v : _graph) {
            if (bpart[v]) {
                _m_vertex[v]->setPos(QPoint(x_coordinate += step, 0));
                _scene->addItem(_m_vertex[v]);
            }
        }
        x_coordinate = x_coordinate / 2 - (bpart.size() - x_coordinate / step) / 2 * step;
        for(const my::vertex_t v : _graph) {
            if (!bpart[v]) {
                _m_vertex[v]->setPos(QPoint(x_coordinate += step, distance));
                _scene->addItem(_m_vertex[v]);
            }
        }

    }
}

#include <iostream>

GraphicsVertexItem::GraphicsVertexItem(QGraphicsItem* parent)
	: GraphicsVertexItem("", parent)
{}

GraphicsVertexItem::GraphicsVertexItem(const QString& name, QGraphicsItem* parent) 
	: _name(name)
	, QGraphicsEllipseItem(parent)
    , _size(30)
{
    QGraphicsEllipseItem::setRect(0, 0, _size, _size);
	setBrush(Qt::black);
}

void GraphicsVertexItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	// std::cout << _name.toStdString() << " : pressed" << std::endl;
}

void GraphicsVertexItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	setPos(event->lastScenePos() - rect().center());
}

void GraphicsVertexItem::SetName(const QString& str) {
	_name = str;
}
QString GraphicsVertexItem::Name() const {
    return _name;
}

void GraphicsVertexItem::SetSize(int size) {
    _size = size;
}

void GraphicsVertexItem::paint(QPainter* painter,const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->setRenderHint(QPainter::Antialiasing, true);
	QGraphicsEllipseItem::paint(painter, option, widget);
    QGraphicsEllipseItem::setRect(0, 0, _size, _size);
	painter->save();
    painter->setPen(Qt::white);
    if (_size < 20)
        painter->drawText(rect(), "", {Qt::AlignHCenter | Qt::AlignVCenter});
    else
        painter->drawText(rect(), _name, {Qt::AlignHCenter | Qt::AlignVCenter});
	painter->restore();
}

GraphicsEdgeItem::GraphicsEdgeItem(GraphicsVertexItem* vs, GraphicsVertexItem* vd, bool IsDirected, const QPen& pen)
	: _source(vs)
	, _dest  (vd)
	, _is_directed(IsDirected)
{
    setPen(pen);
}

void GraphicsEdgeItem::SetColor(const QColor& color) {
    QPen p = pen();
    p.setColor(color);
    setPen(p);
}

my::edge_t GraphicsEdgeItem::Edge() const {
    return { static_cast<my::vertex_t>(_source->Name().toUInt()),
             static_cast<my::vertex_t>(  _dest->Name().toUInt()) };
}

void GraphicsEdgeItem::paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) {

    // QPen pen = painter->pen();
    // _pen.setWidth(1);
    // pen.setWidth(2);
	painter->setRenderHint(QPainter::Antialiasing, true);
    // QGraphicsLineItem::setPen(_pen);
	QGraphicsLineItem::setLine({_source->rect().center() + _source->pos(), _dest->rect().center() + _dest->pos()});
	// Ostart = _source->pos();
	// QPointF end   = _dest->pos();
	//if (!_source || !_dest)
 //       return;
	//QPointF sourcePoint = _source->rect().center() + _source->pos() + QPoint(10, 10);
	//QPointF destPoint = _dest->rect().center() + _dest->pos() + QPoint(10, 10);
 //   QLineF line(sourcePoint, destPoint);
 //   if (qFuzzyCompare(line.length(), qreal(0.)))
 //       return;

 //   double angle = std::atan2(-line.dy(), line.dx());
	//double arrowSize = 10;

 //   QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
 //                                             cos(angle - M_PI / 3) * arrowSize);
 //   QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
 //                                             cos(angle - M_PI + M_PI / 3) * arrowSize);

 //   painter->setBrush(Qt::black);
 //   painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	QGraphicsLineItem::paint(painter, option, widget);
}
