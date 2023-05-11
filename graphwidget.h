#ifndef HEADER_FILE_GRAPH_WIDGET
#define HEADER_FILE_GRAPH_WIDGET

#include <QGraphicsView>
#include <QGraphicsEllipseItem>

#include "graph.h"
#include "helper.h"

class GraphicsVertexItem;
class GraphicsEdgeItem : public QGraphicsLineItem
{
public:
    GraphicsEdgeItem(GraphicsVertexItem*, GraphicsVertexItem* , bool = false, const QPen& = {Qt::black});
    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* = nullptr) override;
    void SetColor(const QColor&);
    my::edge_t Edge() const;
private:
    GraphicsVertexItem* _source, *_dest;
    bool _is_directed;
    // QPen _pen;
};

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
	GraphWidget(QWidget* = nullptr);
	GraphWidget(const my::Graph&, QWidget* = nullptr);

	void SetGraph(const my::Graph&);
    void SetEdgesPen(const QPen&);
    void SetVertexSize(const int);
    // virtual void SetEdgesSize(const int);
    QPen EdgesPen() const;

    virtual	void AddEdge(const my::edge_t&);
    virtual	void RemoveEdge(const my::edge_t&);
    virtual	void RemoveVertex(const my::vertex_t&);
    virtual	void AddVertex();

    size_t graph_size() const;
protected:
    virtual void Initialize();
    void SetVertexPos();
    void clear();

    // void Initialize(const my::Graph&);


protected:
	QGraphicsScene* _scene;
    std::vector<GraphicsVertexItem*> _m_vertex;
    std::unordered_map<my::edge_t, GraphicsEdgeItem*, pair_hash> _m_edge;
    my::Graph _graph;

    QPen _edges_pen;
    int _vertex_size;
};

class GraphicsVertexItem : public QGraphicsEllipseItem
{
public:
	GraphicsVertexItem(QGraphicsItem* = nullptr);
	GraphicsVertexItem(const QString&, QGraphicsItem* = nullptr);
	void SetName(const QString&);
    QString Name() const;
    void SetSize(int size);
	void mousePressEvent(QGraphicsSceneMouseEvent*) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
	void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* = nullptr) override;
private:
	QString _name;
    int _size;
};



#endif // HEADER_FILE_GRAPH_WIDGET
