#ifndef HEADER_FILE_GRAPH
#define HEADER_FILE_GRAPH

#include <vector>
#include <initializer_list>
#include <iostream>
#include <cassert>

namespace my {

using vertex_t = unsigned long long int;
using edge_t   = std::pair<vertex_t, vertex_t>;
using degre_t  = vertex_t;

static constexpr vertex_t invalid_vertex = static_cast<vertex_t>(-1);
static constexpr edge_t   invalid_edge   = {invalid_vertex, invalid_vertex};

class Graph
{
private:
    struct Node
    {
        vertex_t prev = invalid_vertex;
        vertex_t next = invalid_vertex;
        inline bool is_valid() const;
    };

    using row_t    = std::vector<Node>;
    using matrix_t = std::vector<row_t>;

public:
    Graph(const size_t&);
    Graph(const std::initializer_list<edge_t>&);

    Graph(const Graph&) = default;
    Graph& operator=(const Graph&) = default;
    Graph(Graph&&) = default;
    Graph& operator=(Graph&&) = default;

    inline bool add_edge(const edge_t&);
    bool add_edge(const vertex_t&, const vertex_t&);
    bool remove_edge(const vertex_t&, const vertex_t&);
    bool remove_edge(const edge_t&);

    void add_vertex();
    void remove_vertex(const vertex_t&);

    inline void connect(const vertex_t&, const vertex_t&);
    inline void disconnect(const vertex_t&, const vertex_t&);

    inline bool is_exist(const edge_t&) const;
    inline bool is_exist(const vertex_t&, const vertex_t&) const;
    inline bool is_connected(const edge_t&) const;
    inline bool is_connected(const vertex_t&, const vertex_t&) const;

    void resize(size_t size);

    inline degre_t degre(const vertex_t&) const;

    // void shrink();
    inline size_t size() const;

private:
    void resize_matrix(size_t size);

public:
	class edge_iterator;

	class vertex_iterator
	{
        friend class Graph;
	public:
		inline vertex_iterator(const Graph&, const vertex_t&);
		inline vertex_iterator() = default;

        vertex_iterator(const vertex_iterator&) = default;
        vertex_iterator& operator=(const vertex_iterator&) = default;

		inline vertex_iterator operator++();
		inline vertex_iterator operator++(int);

		inline vertex_iterator operator--();
		inline vertex_iterator operator--(int);

		edge_iterator begin() const;
		edge_iterator end() const;

        edge_iterator rbegin() const;
        edge_iterator rend() const;

		inline operator vertex_t() const;
        inline vertex_t operator*() const;

	private:
        const Graph&    _graph;
		vertex_t _vertex;
	};

	class edge_iterator
	{
        friend class vertex_iterator;
	public:
        inline edge_iterator(const Graph&, const vertex_t&);
        // edge_iterator() = default;
        edge_iterator(const edge_iterator&) = default;
        edge_iterator& operator=(const edge_iterator&) = default;

		inline edge_iterator operator++();
		inline edge_iterator operator++(int);

		inline edge_iterator operator--();
		inline edge_iterator operator--(int);

        inline edge_t operator*() const;

        inline bool operator==(const edge_iterator&) const;
        inline bool operator!=(const edge_iterator&) const;

        // inline vertex_t owner_vertex() const;
        // inline vertex_t _vertex() const; 
    private:
        const Graph& _graph;
        edge_t  _edge;
	};
	
	vertex_iterator begin() const;
	vertex_iterator end() const;
    
public:
    friend std::ostream& operator << (std::ostream& os, const my::Graph& g);

private:
    matrix_t _matrix; // Adjacency matrix
};

//-----------------------------inline implementations-----------------------------//
 // #include "graph_inline_implementations.h"

inline bool Graph::Node::is_valid() const { 
    return prev != invalid_vertex && next != invalid_vertex;
}

inline bool Graph::add_edge(const edge_t& e) {
    return add_edge(e.first, e.second);
}

inline size_t Graph::size() const {
    return _matrix.size();
}

//inline degre_t Graph::degre(const vertex_t& v) const {
//    return _degres[v];
//}

inline void Graph::connect(const vertex_t& u, const vertex_t& v) {
    add_edge(u, v);
    add_edge(v, u);
}

inline void Graph::disconnect(const vertex_t& u, const vertex_t& v) {
    remove_edge(u, v); 
    remove_edge(v, u);
}

inline bool Graph::is_exist(const edge_t& e) const {
    return is_exist(e.first, e.second);
}

inline bool Graph::is_exist(const vertex_t& u, const vertex_t& v) const {
    return _matrix[u][v].is_valid();
}

inline bool Graph::is_connected(const edge_t& e) const {
    return is_connected(e.first, e.second);
}

inline bool Graph::is_connected(const vertex_t& u, const vertex_t& v) const {
    return is_exist(u, v) && is_exist(v, u);
}

//-----------------------------iterators-----------------------------//

// inline Graph::vertex_iterator::vertex_iterator()
// 	: _vertex(invalid_vertex)
// {}

inline Graph::vertex_iterator::vertex_iterator(const Graph& g, const vertex_t& v)
	: _graph(g)
    , _vertex(v)
{}

inline Graph::vertex_iterator Graph::vertex_iterator::operator++() {
	++_vertex;
    return *this; 
}

inline Graph::vertex_iterator Graph::vertex_iterator::operator++(int) {
    vertex_iterator temp = *this;
    ++_vertex;
	return temp;
}

inline Graph::vertex_iterator Graph::vertex_iterator::operator--() {
	--_vertex;
    return *this;
}

inline Graph::vertex_iterator Graph::vertex_iterator::operator--(int) {
    vertex_iterator temp = *this;
	--_vertex;
    return temp;
}


inline Graph::vertex_iterator::operator vertex_t() const {
    return _vertex;
}

inline vertex_t Graph::vertex_iterator::operator*() const {
    return _vertex; 
}

inline Graph::edge_iterator Graph::vertex_iterator::begin() const {
    return ++edge_iterator(_graph, _vertex);
}

inline Graph::edge_iterator Graph::vertex_iterator::end() const {
    return edge_iterator(_graph, _vertex);
}


inline Graph::edge_iterator Graph::vertex_iterator::rbegin() const {
    return --edge_iterator(_graph, _vertex);
}

inline Graph::edge_iterator Graph::vertex_iterator::rend() const {
    return edge_iterator(_graph, _vertex);
}

inline Graph::edge_iterator::edge_iterator(const Graph& g, const vertex_t& v) 
    : _graph(g)
    , _edge(v, v)
{
    assert(v < g.size());
}

inline Graph::edge_iterator Graph::edge_iterator::operator++() {
    _edge.second = _graph._matrix[_edge.first][_edge.second].next;
    return *this;
}

inline Graph::edge_iterator Graph::edge_iterator::operator++(int) {
    edge_iterator temp = *this;
    this->operator++();
    return temp;
}

inline Graph::edge_iterator Graph::edge_iterator::operator--() {
    _edge.second = _graph._matrix[_edge.first][_edge.second].prev;
    return *this;
}

inline Graph::edge_iterator Graph::edge_iterator::operator--(int) {
    edge_iterator temp = *this;
    this->operator--();
    return temp;
}

inline bool Graph::edge_iterator::operator==(const edge_iterator& it) const {
    return &_graph == &it._graph && 
            _edge.first == it._edge.first && 
            _edge.second == it._edge.second;
}    

inline bool Graph::edge_iterator::operator!=(const edge_iterator& it) const {
    return !operator==(it);
}

inline edge_t Graph::edge_iterator::operator*() const { 
    return _edge;
}


} // namespace my


#endif // HEADER_FILE_GRAPH
