#ifndef HEADER_FILE_CIRCUIT_BOARD 
#define HEADER_FILE_CIRCUIT_BOARD 


#include "graph.h"
#include "graph_algorithm.h"
#include "fraction.h"
// #include "dynamicmatrix.h"

#include <QWidget>
#include <QVector>
#include <QLine>
#include <QColor>
#include <QScrollArea>

#include "router.h"

namespace my {

class CircuitBoard : public QWidget 
{
   // Q_OBJECT

public:
    CircuitBoard(QWidget* = nullptr);

    void SetData(const std::pair<std::vector<QPointF>, std::vector<std::vector<my::Arc>>>&);

protected:
    void paintEvent(QPaintEvent*) override;
private:
    void rout_to_lines(const CBoard&, route, const QColor&);

private:
    std::vector<std::pair<QLineF, QColor>> _routes;
    std::vector<QPointF> _contacts;
    std::vector<QPointF> ___contacts;
    std::vector<QRectF>  _chips;
};

} // namespace my

#endif // HEADER_FILE_CIRCUIT_BOARD
