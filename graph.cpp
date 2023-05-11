#include "graph.h"
#include <algorithm>

namespace my {

template<typename Type>
inline Type max(const std::pair<Type, Type>& p) {
    return std::max(p.first, p.second);
}

Graph::Graph(const size_t& size)
    : _matrix(size, row_t(size))
    // , _degres(size, 0)
{
    for (size_t i = 0; i < size; ++i)
        _matrix[i][i] = {i, i};
}

Graph::Graph(const std::initializer_list<edge_t>& il) 
    : Graph(max(std::max(il, [](auto& f, auto& s) { return max(f) < max(s); })))
{
    for (const edge_t& e : il) 
        add_edge(e);
}

bool Graph::add_edge(const vertex_t& u, const vertex_t& v) {
    if (u == v || std::max(u, v) >= size() || _matrix[u][v].is_valid())
        return false;
    _matrix[u][v] = {_matrix[u][u].prev, u};
    _matrix[u][u].prev = _matrix[u][_matrix[u][u].prev].next = v;
    // ++_degres[u];
    return true;
}

bool Graph::remove_edge(const vertex_t& u, const vertex_t& v) {
    if (std::max(u, v) >= size())
        return false;

    Node& node = _matrix[u][v];
    if (u == v || node.is_valid() == false)
        return false;
    _matrix[u][node.prev].next = node.next;
    _matrix[u][node.next].prev = node.prev;
    node = {invalid_vertex, invalid_vertex};
    // --_degres[u];
    return true;
}

bool Graph::remove_edge(const edge_t& e) {
    return remove_edge(e.first, e.second);
}

void Graph::remove_vertex(const vertex_t& v) {
    if (v >= size())
        return ;
    for (vertex_t w = 0; w < size(); ++w)
        remove_edge(w, v);
    for (Node& node = _matrix[v][v]; node.next != v; remove_edge(v, node.next));
}

void Graph::resize_matrix(size_t size) {
	for (vertex_t v = static_cast<vertex_t>(size); v < Graph::size(); ++v) 
		remove_vertex(v);
    _matrix.resize(size);
    for (row_t& r : _matrix)
        r.resize(size);
    if (_matrix.empty() == false) {
        for (vertex_t v = static_cast<vertex_t>(size - 1); _matrix[v][v].is_valid() == false; --v)
            _matrix[v][v] = {v, v};
    }
}

void Graph::resize(size_t size) {
    if (size < Graph::size()) {
        for (vertex_t v = static_cast<vertex_t>(size); v < Graph::size(); ++v) 
            remove_vertex(v);
    } 
    resize_matrix(size);
}

std::ostream& operator << (std::ostream& os, const my::Graph& g) {
     // for (const auto& row : g._matrix) {
     //     for (const Graph::Node& n : row) {
     //         if (n.is_valid())
     //             os << "{\e[31m" << n.prev << "\e[m, \e[31m" << n.next << "\e[m}" << " ";
     //         else
     //             os << "{" << "Ø" << ", " << "Ø" << "}" << " ";
     //     }
     //     os << std::endl;
     // }
     // for (const auto& d : g._degres) {
         // os << "{\e[34m " << d << " \e[m}" << " ";
     // }
     // os << std::endl;
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //for (size_t i = 0; i < g._matrix.size(); ++i) {
    //    for (size_t j = 0; j < g._matrix[i].size(); ++j) {
    //        if (i == j) {
    //            std::cout << "\x1b[32m0\x1b[m ";
    //        } else if(g._matrix[i][j].is_valid()) {
    //            std::cout << "\x1b[m1 ";
    //        } else {
    //            std::cout << "\x1b[31mX\x1b[m ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    for(auto v : g) {
        os << v << " : ";
        for(auto e : Graph::vertex_iterator(g, v)) {
            os << e.second << os.fill();
        }
        os << std::endl;
    }
    return os;
}

Graph::vertex_iterator Graph::begin() const {
    return Graph::vertex_iterator{*this, 0};
}

Graph::vertex_iterator Graph::end() const {
    return Graph::vertex_iterator{*this, size()};
}

} // namespace my
