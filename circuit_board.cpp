#include "circuit_board.h"
#include "fraction.h"

#include <deque>
#include <list>
#include <unordered_set>
#include <set>

#include <QPainter>

namespace  my {

unsigned k = 110; // coefficiente of objects's size

class FPoint
{
public:
    FPoint() = default;
    FPoint(const FPoint&) = default;
    FPoint& operator=(const FPoint&) = default;
    FPoint(const Fraction& f1, const Fraction& f2)
        : _x(f1)
        , _y(f2) 
    {}
    FPoint(const QPointF& p) 
        : FPoint(Fraction::convert_double_to_fraction(p.x()), Fraction::convert_double_to_fraction(p.y()))
    {}
    inline Fraction x() const { return _x; }
    inline Fraction y() const { return _y; }
    bool operator==(const FPoint& p) const {
        return _x == p._x && p._y == _y;
    }
    bool operator!=(const FPoint& p) const {
        return !operator==(p);
    }
    explicit operator QPointF() const {
        return QPointF(_x.convert_fraction_to_double(), _y.convert_fraction_to_double());
    }
private:
    Fraction _x;
    Fraction _y;
};

class Chip 
{
public:
    Chip() = default;
    Chip(const my::vertex_t& v) : _number(v) {}
    Chip(const Chip&) = default;
    Chip& operator=(const Chip&) = default;

    size_t size() const {
        return (_contacts.size() / 4 + (_contacts.size() % 4 ? 1 : 0)) * 2 + 1;
    }
    size_t rsize() const {
        return (_contacts.size() + 3) / 4 + 1;
    }
    size_t contacts_count() const {
        return _contacts.size();
    }

    void add_contact(const my::vertex_t& v, my::Arc::Rotation r) {
        switch(r)
        {
            case my::Arc::NONE: _contacts.push_back(v); b_it = t_it = _contacts.end(); break;
            case my::Arc::ACW: _contacts.insert(--t_it, v); break;
            case my::Arc::CCW: _contacts.insert(b_it, v);   break;
        }

        //if (r == my::Arc::Rotation::ACW || r == my::Arc::Rotation::NONE){
        //    _contacts.push_front(v); 
        //} else {
        //    auto x = _contacts.back();
        //    _contacts.back() = v;
        //    _contacts.push_back(x);
        //}
    }
    inline void set_pos(const FPoint& p) { _position = p; }
    inline FPoint pos() const { return _position; }

    inline void set_num(const my::vertex_t& n) { _number = n; }
    inline my::vertex_t num() const { return _number; }

    QRectF rect() const {
        return {QPointF(_position.x().convert_fraction_to_double(), _position.y().convert_fraction_to_double()) * k, 
                QPointF((_position.x() + Fraction{(long)rsize(), inv_length}).convert_fraction_to_double(), 
                        (_position.y() + Fraction{(long)rsize(), inv_length}).convert_fraction_to_double()) * k};
    }
    void put_on_board(CBoard& board) const {
        FPoint left_top = _position;
        FPoint right_bot = {left_top.x() + Fraction{(long)rsize(), inv_length}, left_top.y() + Fraction{(long)rsize(), inv_length}};
        FPoint left_bot = {left_top.x(), right_bot.y()};
        FPoint right_top = {right_bot.x(), left_top.y()};

        FPoint p = left_top;
        while (p != left_bot) {
            board.put(p.y(), p.x(), CBPoint{inv_length, CBPoint::Direction::DOWN});
            p = {p.x(), p.y() + Fraction{1, inv_length}};
        }
        while (p != right_bot) {
            board.put(p.y(), p.x(), CBPoint{inv_length, CBPoint::Direction::RIGHT});
            p = {p.x() + Fraction{1, inv_length}, p.y()};
        }
        while (p != right_top) {
            board.put(p.y(), p.x(), CBPoint{inv_length, CBPoint::Direction::UP});
            p = {p.x(), p.y() - Fraction{1, inv_length}};
        }
        while (p != left_top) {
            board.put(p.y(), p.x(), CBPoint{inv_length, CBPoint::Direction::LEFT});
            p = {p.x() - Fraction{1, inv_length}, p.y()};
        }
    }

    FPoint contact_pos(const Chip& c, CBPoint::Direction* ptd = nullptr) const {
        FPoint left_top = _position;
        FPoint right_bot = {left_top.x() + Fraction{(long)rsize(), inv_length}, left_top.y() + Fraction{(long)rsize(), inv_length}};
        FPoint left_bot = {left_top.x(), right_bot.y()};
        FPoint right_top = {right_bot.x(), left_top.y()};
        size_t index = 0;
        for (auto it = _contacts.begin(); it != _contacts.end() && *it != c.num(); ++it, ++index);
        if (index == _contacts.size())
            return {};
        int dif = index % (_contacts.size() / 4 + 1) + 1;
        switch(index / (_contacts.size() / 4 + 1)) {
            case 0: if (ptd) { *ptd = CBPoint::Direction::RIGHT; }  return {left_top.x(), left_top.y() + Fraction(dif, inv_length)};   break;
            case 1: if (ptd) { *ptd = CBPoint::Direction::UP;    }  return {left_bot.x() + Fraction(dif, inv_length) , left_bot.y()};  break;
            case 2: if (ptd) { *ptd = CBPoint::Direction::LEFT;  }  return {right_bot.x(), right_bot.y() - Fraction(dif, inv_length)}; break;
            case 3: if (ptd) { *ptd = CBPoint::Direction::DOWN;  }  return {right_top.x() - Fraction(dif, inv_length), right_top.y()}; break;
        }
        return {};
    }

    inline bool operator==(const Chip& c) { return _number == c._number; }

    struct hash {
        inline std::size_t operator () (const Chip& c) const {
            return static_cast<size_t>(c._number);
        }
    };

private:
    my::vertex_t _number;

    std::list<my::vertex_t> _contacts;
    std::list<my::vertex_t>::iterator b_it;
    std::list<my::vertex_t>::iterator t_it;

    FPoint _position;
};

CircuitBoard::CircuitBoard(QWidget* parent)
    : QWidget(parent)
{
}

inline QPointF fractions_to_pointf(const std::pair<Fraction, Fraction>& rp) {
    return QPointF(static_cast<double>(rp.second) * k, static_cast<double>(rp.first) * k);
}

void CircuitBoard::rout_to_lines(const CBoard& board, route rp, const QColor& cl = Qt::black) {
    CBPosition start = rp.second;
    CBPoint	   start_point = board.get(start.first, start.second);
    while (start_point.direction != CBPoint::Direction::WITHOUT) {
        CBPosition s = start;
        CBPoint sp = start_point;
        while (start_point.direction == sp.direction) {
            s = get_pos(sp, s);
            sp = board.get(s.first, s.second);
        }
        _routes.push_back({QLineF(fractions_to_pointf(start), fractions_to_pointf(s)), cl});
        start = s;
        start_point = sp;
    }
}

void CircuitBoard::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    QRadialGradient gradient(QPointF(100, 100), 100);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(0, Qt::blue);
    QBrush brush(gradient); // Qt::black, Qt::RadialGradientPattern);
    painter.setBrush(brush);
    painter.setPen(pen);
    for(auto l : _routes) {
        pen.setColor(l.second);
        painter.setPen(pen);
        painter.drawLine(l.first);
    }
    for(auto c : _chips) {
        painter.drawRect(c);
    }
    painter.setPen(QPen{Qt::red});
    for (auto p : _contacts)
        painter.drawEllipse(QRectF{p - QPointF{3, 3}, p + QPointF{3, 3}});

    painter.setPen(QPen{Qt::black});
    for (auto p : ___contacts)
        painter.drawEllipse(QRectF{p - QPointF{2, 2}, p + QPointF{2, 2}});
}

void CircuitBoard::SetData(const std::pair<std::vector<QPointF>, std::vector<std::vector<my::Arc>>>& data) {
    if (data.first.empty() || data.second.empty())
        return;
    for (auto& vec : data.second) {
        for (auto arc : vec)
            std::cout << "(" << data.first[arc.edge.first].x() << ", " << data.first[arc.edge.first].y() << ")\t"
                      << "(" << data.first[arc.edge.second].x() << ", " << data.first[arc.edge.second].y() << ")"
                      << "\tedge[" << arc.edge.first << ", " << arc.edge.second << "]\t" << arc.rot << std::endl;
        std::cout << std::endl;
    }
    const QPointF& f = data.first.front();
    double min_x = f.x(), min_y = f.y(), max_y = f.y(), max_x = f.x();
    for(const QPointF& p : data.first) {
        min_x = std::min(min_x, p.x());
        max_x = std::max(max_x, p.x());
        min_y = std::min(min_y, p.y());
        max_y = std::max(max_y, p.y());
    }
    std::cout << min_x << " "
              << min_y << " "
              << max_x << " "
              << max_y << " "
              << std::endl;

    // std::unordered_map<my::vertex_t, Chip, Chip::hash> chips;
    std::vector<Chip> chips(data.first.size());
    for (auto& vec : data.second) {
        for(auto arc : vec) {
            Chip& cp = chips[arc.edge.first];
            cp.add_contact(arc.edge.second, my::Arc::Rotation::NONE);// arc.rot);
            QPointF pos((data.first[arc.edge.first].x() - min_x) / (max_x == min_x ? 1 : max_x - min_x) * (CBoardSize - 4), 
                        (data.first[arc.edge.first].y() - min_y) / (max_y == min_y ? 1 : max_y - min_y) * (CBoardSize - 4));
            pos = QPointF(pos.x() + ((double)CBoardSize - 6) / (CBoardSize - 4) + 1, pos.y() + ((double)CBoardSize - 6) / (CBoardSize - 4) + 1);
            cp.set_pos(pos);
            cp.set_num(arc.edge.first);

            Chip& cp2 = chips[arc.edge.second];
            cp2.add_contact(arc.edge.first, arc.rot);
            QPointF pos2((data.first[arc.edge.second].x() - min_x) / (max_x == min_x ? 1 : max_x - min_x) * (CBoardSize - 4), 
                         (data.first[arc.edge.second].y() - min_y) / (max_y == min_y ? 1 : max_y - min_y) * (CBoardSize - 4));
            pos2 = QPointF(pos2.x() + ((double)CBoardSize - 6) / (CBoardSize - 4) + 1, pos2.y() + ((double)CBoardSize - 6) / (CBoardSize - 4) + 1);
            cp2.set_pos(pos2);
            cp2.set_num(arc.edge.second);

        }
    }
    {
        std::list<Chip> lc;
        for (auto& cp : chips)
            if (cp.contacts_count()) lc.push_back(cp);
        chips.resize(lc.size());
        std::copy(lc.begin(), lc.end(), chips.begin());
    }
    //size_t max_cp_size;
    //for (Chip& cp : chips)
    //    max_cp_size = std::max(max_cp_size, cp.size());
    //for (auto& vec : data.second) {
    //    for(auto arc : vec) {
    //        Chip& cp = chips[arc.edge.first];
    //        QPointF pos = (QPointF)cp.pos();
    //        pos = QPointF(pos.x() + ((double)CBoardSize - 3 - max_cp_size) / (CBoardSize - 1), pos.y() + ((double)CBoardSize - 3 - max_cp_size) / (CBoardSize - 1));
    //        cp.set_pos(pos);

    //        Chip& cp2 = chips[arc.edge.second];
    //        QPointF pos2 = (QPointF)cp2.pos();
    //        pos2 = QPointF(pos2.x() + ((double)CBoardSize - 3 - max_cp_size) / (CBoardSize - 1), pos2.y() + ((double)CBoardSize - 3 - max_cp_size) / (CBoardSize - 1));
    //        cp2.set_pos(pos2);
    //    }
    //}

    inv_length = 1; 
    int32_t local_inv_length;
    for (auto it = chips.begin(); it != chips.end(); ++it)
        for(auto jt = std::next(it); jt != chips.end(); ++jt) {
            Chip ffc = *it, fsc = *jt;
            Fraction fx = {floor(ffc.pos().x() * inv_length), inv_length};
            Fraction sx = {floor(fsc.pos().x() * inv_length), inv_length};
            if(fx > sx) {
                std::swap(fx, sx);
                std::swap(ffc, fsc);
            }

            Chip sfc = *it, ssc = *jt;
            Fraction fy = {floor(sfc.pos().y() * inv_length), inv_length};
            Fraction sy = {floor(ssc.pos().y() * inv_length), inv_length};

            if(fy > sy) {
                std::swap(fy, sy);
                std::swap(sfc, ssc);
            }

            while((fx + Fraction(ffc.size(),inv_length) >= sx && 
                   fy + Fraction(sfc.size(),inv_length) >= sy) ||
                   fx + Fraction(ffc.size(),inv_length) >= CBoardSize ||
                   fy + Fraction(sfc.size(),inv_length) >= CBoardSize)
            {
                inv_length *= reduce_coefficient;
                fx = {floor(ffc.pos().x() * inv_length), inv_length};
                sx = {floor(fsc.pos().x() * inv_length), inv_length};

                fy = {floor(sfc.pos().y() * inv_length), inv_length};
                sy = {floor(ssc.pos().y() * inv_length), inv_length};
            }

        }
    local_inv_length = inv_length;

    _chips.clear();
    _contacts.clear();
    for(Chip& c : chips) {
        c.set_pos({{floor(c.pos().x() * inv_length), inv_length}, 
                   {floor(c.pos().y() * inv_length), inv_length}});
        _chips.push_back(c.rect());
    }

    //    {{{2, 2}, {4, 4}}, {{7, 2}, {10, 2}}},
    //    {{{4, 2}, {4, 4}}, {{5, 2}, {10, 2}}},
    //    {{{3, 2}, {4, 4}}, {{6, 2}, {10, 2}}},
    //    {{{5, 2}, {4, 4}}, {{4, 2}, {10, 2}}},
    //    {{{1, 2}, {4, 4}}, {{8, 2}, {10, 2}}},
    //    {{{6, 2}, {4, 4}}, {{3, 2}, {10, 2}}},
    //    {{{7, 2}, {4, 4}}, {{2, 2}, {10, 2}}},
    //    {{{8, 2}, {4, 4}}, {{1, 2}, {10, 2}}},
    //    {{{9, 2}, {4, 4}}, {{10, 2}, {10, 2}}},
    //    {{{10, 2}, {4, 4}}, {{9, 2}, {10, 2}}},
    //};
    // std::list<FPoint> tree;
     ___contacts.clear();
     _routes.clear();
     for (auto& vec : data.second) {
         inv_length = local_inv_length;
        CBoard board;
        chips_board.clear();
        for (const Chip& cp : chips)
            cp.put_on_board(chips_board);

        std::vector<route> routs; 
        std::vector<route> _routs; 
        std::vector<std::pair<route, CBPoint::Direction>> brouts; 
        
        for(auto arc : vec) {
            Chip& cp1 = chips[arc.edge.first];
            Chip& cp2 = chips[arc.edge.second];
            if(arc.rot == my::Arc::Rotation::NONE)
            {
                routs.push_back({{{cp1.contact_pos(cp2).y()}, {cp1.contact_pos(cp2).x()}}, 
                                 {{cp2.contact_pos(cp1).y()}, {cp2.contact_pos(cp1).x()}}});
                _contacts.push_back((QPointF)cp1.contact_pos(cp2) * k);
                _contacts.push_back((QPointF)cp2.contact_pos(cp1) * k);
            } else {
                ___contacts.push_back((QPointF)cp1.contact_pos(cp2) * k);
                ___contacts.push_back((QPointF)cp2.contact_pos(cp1) * k);

                my::FPoint start = cp1.contact_pos(cp2);
                my::FPoint end   = cp2.contact_pos(cp1);
                CBPoint::Direction dir;
                if(arc.rot != my::Arc::Rotation::ACW) {
                    std::swap(start, end);
                    cp2.contact_pos(cp1, &dir);
                } else {
                    cp1.contact_pos(cp2, &dir);
                }
                brouts.push_back({{{start.y(), start.x()}, {end.y(), end.x()}}, dir});
                _routs.push_back({{start.y(), start.x()}, {end.y(), end.x()}});
            }
        }
        QColor cl = {rand() % 255, rand() % 255, rand() % 255};
        wave_router(board, routs);
        // wave_router(board, _routs);
        for (auto r : routs)
            rout_to_lines(board, r, cl);
        // for (auto r : _routs)
        //    rout_to_lines(board, r, cl);

        // inv_length = local_inv_length;
        be_router(board, brouts);
        for (auto br : brouts)
            rout_to_lines(board, br.first, cl);
     }
     inv_length = local_inv_length;
     CBoard board;
     QColor cl = {rand() % 255, rand() % 255, rand() % 255};
     auto _not_routed = ::not_routed;
     p_wave_router(board);

     for (auto r : _not_routed)
         rout_to_lines(board, r, cl);

     inv_length = local_inv_length;
     board.clear();
     cl = {rand() % 255, rand() % 255, rand() % 255};
     _not_routed = ::not_routed;
     p_wave_router(board);
     for (auto r : _not_routed)
         rout_to_lines(board, r, cl);

     inv_length = local_inv_length;
     board.clear();
     cl = {rand() % 255, rand() % 255, rand() % 255};
     _not_routed = ::not_routed;
     p_wave_router(board);
     for (auto r : _not_routed)
         rout_to_lines(board, r, cl);

     repaint();
}

} // namespace my
