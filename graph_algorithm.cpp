#include "graph_algorithm.h"
#include "helper.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <stack>
#include <queue>
// #include <boost/graph/boyer_myrvold_planar_test.hpp>
// #include <boost/graph/adjacency_list.hpp>
#include <map>

#include "graphwidget.h" // ?@?@?@?@?@?@?@?@?@?@?@?@?@?@?!@#?$!@?#$!@?#$!@?#$!@#$?!?%$#%?$%^&?$%?^&%?^&?

namespace my  {

void  undirect_graph(Graph& g) {
    for (Graph::vertex_iterator v_it = g.begin(); v_it != g.end(); ++v_it)
        for (Graph::edge_iterator e_it = v_it.begin(); e_it != v_it.end(); ++e_it)
            g.add_edge(reverse_edge(*e_it));
}

bool is_undirected(const Graph& g) {
    for (Graph::vertex_iterator v_it = g.begin(); v_it != g.end(); ++v_it)
        for (Graph::edge_iterator e_it = v_it.begin(); e_it != v_it.end(); ++e_it)
            if (g.is_connected(*e_it) == false)
                return false;
    return true;
}

void remove_edges(Graph& g) {
    for (Graph::vertex_iterator v_it = g.begin(); v_it != g.end(); ++v_it)
        for (Graph::edge_iterator e_it = v_it.begin(); e_it != v_it.end();)
            g.disconnect((*e_it).first, (*e_it++).second);
}

size_t degre(const Graph& g) {
    size_t total = 0;
    for (Graph::vertex_iterator v_it = g.begin(); v_it != g.end(); ++v_it)
        for (Graph::edge_iterator e_it = v_it.begin(); e_it != v_it.end(); ++e_it) {
            ++total;
            if (g.is_connected(*e_it) == false)
                ++total;
        }

    return total / 2;
}

std::vector<bool> bipartite(Graph g) {
    undirect_graph(g);
    std::vector<bool> bvec(g.size());
    enum Color : int8_t { UNDEFINED = 0, BLACK = -1, WITH = 1 };
    std::vector<Color> colors_vec(g.size());
    for (const vertex_t& v : g) {
        if (colors_vec[v] == Color::UNDEFINED) {
            colors_vec[v] = Color::BLACK;
            bvec[v] = true;
            
            std::queue<vertex_t> q;
            q.push(v);
            while(q.empty() == false) {
                vertex_t u = q.front();
                q.pop();

                for (edge_t e : Graph::vertex_iterator(g, u)) {
                    if (colors_vec[e.second] != Color::UNDEFINED) {
                        if (colors_vec[e.second] == colors_vec[e.first])
                            return {};
                        continue;
                    }
                    q.push(e.second);
                    colors_vec[e.second] = Color(-colors_vec[e.first]);
                    bvec[e.second] = !bvec[e.first];
                }
            }
        }
    } 
    return bvec;
}



class PlanarityTesting
{
public:
    Graph& _graph;
    PlanarityTesting(Graph& g)
        : _graph(g)
        , _max_height(0)
        , _m_vertex(g.size())
        , _is_planar(true)
    {
        start_testing();
    }

    inline operator bool() { return _is_planar; }


    std::vector<Arc> get_ordered_embadding() {

        class Pwidget : public GraphWidget {
        public:
            Pwidget(PlanarityTesting& pt,  QWidget* p = nullptr) : GraphWidget(pt._graph, p), _this(pt) {
                // _planar_graphs = my::graph_partitioning_in_planes(graph);
                Initialize();
                _scene->advance();
            };
            PlanarityTesting& _this;
        protected:
            void Initialize() {
                using namespace my;
                _scene->clear();
                _m_vertex.clear();
                _m_vertex.reserve(_graph.size());
                for(const vertex_t v : _graph)
                    _m_vertex.push_back(new GraphicsVertexItem(QString::number(v)));


                for(const vertex_t v : GraphWidget::_graph)
                    for (const edge_t e : Graph::vertex_iterator(GraphWidget::_graph, v)) {
                        QColor color;
                        Vertex& vertex = _this._m_vertex[e.second];
                        if (_this._m_edge[e].side == Edge::Side::RIGHT)
                            color = e == vertex.parent_edge ? Qt::black : Qt::red;
                        else
                            color = (e == vertex.parent_edge) ? Qt::magenta : Qt::blue;
                        _m_edge[e] = new GraphicsEdgeItem(_m_vertex[e.first], _m_vertex[e.second]);
                        _scene->addItem(_m_edge[e]);
                    }
                SetVertexPos();
            }
        };

        if (_is_planar == false)
            return {};

        std::vector<Arc> embd;
        counting_sort_nesting_depth();       
        for (vertex_t v : _roots) {
            third_dfs_traversal(v, embd);
        }
        // Pwidget* lfwidget = new Pwidget(*this); // ----------------
        // lfwidget->show(); // -----------------
        return std::move(embd);
    }

private:
    void chgitem_inch() { // (:
        _current_subgraph = {0, 0};
        enum struct Calculated : bool { NO, YES };
        std::vector<Calculated> vec_c(_graph.size(), Calculated::NO);
        for (vertex_t v : _graph) {
            if (vec_c[v] == Calculated::NO)
                _current_subgraph.vertex_count += 1;
            vec_c[v] = Calculated::YES;
            for (edge_t e : Graph::vertex_iterator(_graph, v)) {
                if (vec_c[e.second] == Calculated::NO)
                    _current_subgraph.vertex_count += 1;
                _current_subgraph.edges_count += 1;
                if (_graph.is_connected(e) == false)
                    _current_subgraph.edges_count += 1;
                if (_current_subgraph.edges_count > 6 * _current_subgraph.vertex_count - 12 &&
                    _current_subgraph.vertex_count > 4)
                {
                    _is_planar = false;
                    return;
                }
                vec_c[e.second] = Calculated::YES;
            }
        }
    }

    void start_testing() {
        chgitem_inch(); // (:
        if (_is_planar == false)
            return;
        undirect_graph(_graph);

        for (vertex_t v : _graph) {
            if (_m_vertex[v].is_valid() == false) {
                _current_subgraph = {1, 0};
                _m_vertex[v] = {0, {v, v}};
                first_dfs_traversal(v);
                if (_current_subgraph.vertex_count == 1)
                    continue;
                //if (_current_subgraph.edges_count < 9 &&  _current_subgraph.vertex_count < 5) {
                //    continue;
                //} else if (_current_subgraph.edges_count > 3 * _current_subgraph.vertex_count - 6) {
                //    _is_planar = false;
                //     return;
                //}
                _roots.push_back(v);
            }
        }
        counting_sort_edges(); // complexity : O(N)
        // std::cout << _graph << std::endl;

        for (vertex_t v : _roots) {
           _stack = {}; // _stack.clear();
           _stack.push({invalid_interval, invalid_interval}); // -------------
           second_dfs_traversal(v);
           if (_is_planar == false)
               return;
           else {
               assert(_stack.size() == 1);
           }
        }
    }

    struct Interval {
        edge_t low;
        edge_t high;
        bool operator==(const Interval& other) const {
            return this->low == other.low && this->high == other.high;
        }
        bool operator!=(const Interval& other) const { return !operator==(other); }
    };
    static constexpr Interval invalid_interval = {invalid_edge, invalid_edge};

    struct ConflictingIntervals {
        Interval left;
        Interval right;
        bool operator==(const ConflictingIntervals& other) const {
            return this->left == other.left &&
                   this->right == other.right;
        }
        bool operator!=(const ConflictingIntervals& other) const { return !operator==(other); }
    };
    struct Edge {
        vertex_t lwpt, lwpt2;
        edge_t lwpt_edge;
        edge_t next = invalid_edge;
        //struct ConflictingIntervals stack_bottom; // 8 * unsigned long long
        size_t stack_bottom;
        enum Side : bool { LEFT, RIGHT } side = RIGHT;
    };
    struct Vertex {
        size_t height = static_cast<size_t>(-1);
        edge_t parent_edge = invalid_edge;
        inline bool is_valid() const {
            return height != static_cast<size_t>(-1);
        }
    };
    struct SubgraphInfo {
        size_t vertex_count, edges_count; // {1, 0}
    } _current_subgraph;


    void first_dfs_traversal(const vertex_t& v) {
        Vertex& parent_vertex = _m_vertex[v];
        Edge& parent_edge = _m_edge[parent_vertex.parent_edge];

        for (const edge_t& e : Graph::vertex_iterator(_graph, v)) {
            Edge& edge = _m_edge[e];
            Vertex& vertex = _m_vertex[e.second];

            _current_subgraph.edges_count += 1;
            orient(e);
            edge.lwpt = edge.lwpt2 = parent_vertex.height;
            if (vertex.is_valid() == false) {
                vertex.parent_edge = e;
                _current_subgraph.vertex_count += 1;
                vertex.height = parent_vertex.height + 1;
                if (_max_height < vertex.height)
                    _max_height = vertex.height;
                first_dfs_traversal(e.second);
            } else {
                edge.lwpt = vertex.height;
            }

            // determine nesting depth

            // if (true /* parent_edge is not root*/) {
               if (edge.lwpt < parent_edge.lwpt) {
                  parent_edge.lwpt2 = std::min(parent_edge.lwpt, edge.lwpt2);
                  parent_edge.lwpt  = std::min(parent_edge.lwpt, edge.lwpt);
               } else if (edge.lwpt == parent_edge.lwpt) {
                  parent_edge.lwpt2 = std::min(parent_edge.lwpt2, edge.lwpt2);
               } else {
                  parent_edge.lwpt2 = std::min(parent_edge.lwpt2, edge.lwpt);
               }
            //}
        }
    }

    void second_dfs_traversal(const vertex_t& v) {
        Vertex& parent_vertex = _m_vertex[v];
        Edge& parent_edge = _m_edge[parent_vertex.parent_edge];

        for (const edge_t& e : Graph::vertex_iterator(_graph, v)) {
            Edge& edge = _m_edge[e];
            Vertex& vertex = _m_vertex[e.second];

            // stack bottom[ei ] ← top(S)
            edge.stack_bottom = _stack.size(); // ------------------------------
            if (e == vertex.parent_edge) {
                second_dfs_traversal(e.second);
                if (_is_planar == false)
                    return;
            } else {
                edge.lwpt_edge = e;
                _stack.push({invalid_interval, {e, e}});
            }
            if (edge.lwpt < parent_vertex.height) {
                const edge_t& f_e = *Graph::vertex_iterator(_graph, v).begin();
                if (e == f_e) {
                    edge.lwpt_edge = _m_edge[f_e].lwpt_edge;
                } else {
                    adding_constraints(e);
                    if (_is_planar == false)
                        return;
                }
            }
        }

        if (is_root(parent_vertex.parent_edge) == false) {
            trimming_back_edges(parent_vertex.parent_edge.first);
            if (_is_planar == false)
                return;
            // side of e is side of a highest return edge
            if (parent_edge.lwpt < parent_vertex.parent_edge.first) {
                edge_t hl = _stack.top().left.high;
                edge_t hr = _stack.top().right.high;
                 // if hL != ⊥ and (hR = ⊥ or lowpt[hL ] > lowpt[hR ]) then
                if (is_root(hl) == false && (is_root(hr) || _m_edge[hl].lwpt > _m_edge[hr].lwpt))
                        parent_edge.next = hl;
                else
                        parent_edge.next = hr;
            }
        }
    }
    void adding_constraints(const edge_t& e) {
        ConflictingIntervals P = {invalid_interval, invalid_interval};
        Edge& edge = _m_edge[e];
        const Edge& f_edge = _m_edge[*Graph::vertex_iterator(_graph, e.first).begin()];
        do {
            ConflictingIntervals Q = _stack.top();
            _stack.pop();
            if (Q.left != invalid_interval)
                std::swap(Q.left, Q.right);
            if (Q.left != invalid_interval) {
                _is_planar = false;
                return;
            }
            if (_m_edge[Q.right.low].lwpt > f_edge.lwpt) { /* merge intervals */
                if (P.right == invalid_interval)
                    P.right.high = Q.right.high;
                else
                    _m_edge[P.right.low].next = Q.right.high; // --------next---------
                P.right.low = Q.right.low;
            } else {  /* make consistent */
                _m_edge[Q.right.low].next = f_edge.lwpt_edge; // ---------next---------
            }
        } while(_stack.size() != edge.stack_bottom);

        // 	merge conflicting return edges of e1 , . . . , ei−1 into P.L
        while (conflict(_stack.top().left, e) || conflict(_stack.top().right, e)) {
            ConflictingIntervals Q = _stack.top();
            _stack.pop();
            if (conflict(Q.right, e))
                std::swap(Q.left, Q.right);
            if (conflict(Q.right, e)) {
                _is_planar = false;
                return;
            } else  { /* merge interval below lowpt(ei ) into P.R */
            _m_edge[P.right.low].next = Q.right.high; // ---------next---------
            if (Q.right.low.first != Q.right.low.second) // if low is't root
                P.right.low = Q.right.low;
            }
            if (P.left == invalid_interval)
                P.left.high = Q.left.high;
            else
                _m_edge[P.left.low].next = Q.left.high; // -----------next---------
            P.left.low = Q.left.low;
        }
        if (P != ConflictingIntervals{invalid_interval, invalid_interval})
            _stack.push(P);
    }

    void trimming_back_edges(const vertex_t& u) {
        while (_stack.empty() == false && lowest(_stack.top()) == _m_vertex[u].height) {
            //	ConflictingIntervals P = _stack.top();
            // if P.L.low != ⊥ then side[P.L.low] ← −1
            if (_stack.top().left.low != invalid_edge) 
                _m_edge[_stack.top().left.low].side = Edge::Side::LEFT;
            _stack.pop();
        }
        if (_stack.empty())
            return;
        ConflictingIntervals P = _stack.top();
        _stack.pop();
        //::::::::::::::::::::::::::::I trim left interval:::::::::::::::::::::::::::
        while (P.left.high.first != P.left.high.second && P.left.high.second == u) {
            P.left.high = _m_edge[P.left.high].next;
        }
        if (P.left.high.first == P.left.high.second && P.left.low.first != P.left.low.second) {
            _m_edge[P.left.low].next = P.right.low; // -----------next---------
            _m_edge[P.left.low].side = Edge::Side::LEFT;  //	side[P.L.low] ← −1
            P.left.low = invalid_edge; //--------
        }

        //::::::::::::::::::::::::::::I trim right interval:::::::::::::::::::::::::::
        while (P.right.high.first != P.right.high.second && P.right.high.second == u) {
            P.right.high = _m_edge[P.right.high].next;
        }
        if (P.right.high.first == P.right.high.second && P.right.low.first != P.right.low.second) {
            _m_edge[P.right.low].next = P.left.low; // -------------next---------
            _m_edge[P.right.low].side = Edge::Side::RIGHT; //	side[P.L.low] ← 1
            P.right.low = invalid_edge; //-----------------
        }
        _stack.push(P);
    }

    inline bool is_root(const edge_t& e) {
        return e != invalid_edge && e.first == e.second;
    }

    inline bool conflict(const Interval& interval, const edge_t& edge) {
        return interval != invalid_interval && _m_edge[interval.high].lwpt > _m_edge[edge].lwpt;
    }

    size_t lowest(const ConflictingIntervals& interval) {
        if (interval == ConflictingIntervals{invalid_interval, invalid_interval})
            return static_cast<size_t>(-1);
        if (interval.left == invalid_interval)
            return _m_edge[interval.right.low].lwpt;
        if (interval.right == invalid_interval)
            return _m_edge[interval.left.low].lwpt;

        return std::min(_m_edge[interval.left.low].lwpt,
                        _m_edge[interval.right.low].lwpt);
    }

    // sort function with O(N) complexity
    void counting_sort_edges() {
    //   std::cout << "----------------before sorting----------------" << std::endl;
    //    std::cout << _graph << std::endl;

        std::vector<std::queue<edge_t>> vec_counts(2 * _max_height + 1); // ------------------- (+ 1) ??
        for (auto elem : _m_edge) {
            size_t nesting_depth = 2 * elem.second.lwpt;
            if (elem.second.lwpt2 < _m_vertex[elem.first.first].height) // chordal
                nesting_depth += 1;
            vec_counts[nesting_depth].push(elem.first);
        }
        remove_edges(_graph);
        for (std::queue<edge_t>& q : vec_counts)
            for (; q.empty() == false; q.pop())
                _graph.add_edge(q.front());

    //    std::cout << "----------------after sorting----------------" << std::endl;
    //    std::cout << _graph << std::endl;
    }

    // sort function with O(N) complexity
    void counting_sort_nesting_depth() {
    //   std::cout << "----------------before sorting----------------" << std::endl;
    //    std::cout << _graph << std::endl;

        // std::vector<std::queue<edge_t>> vec_counts(2 * _max_height + 1); // ------------------- (+ 1) ??
        std::map<int, std::queue<edge_t>> tree; 
        for (auto& elem : _m_edge) {
            if (elem.first == invalid_edge)
                continue;
            size_t nesting_depth = 2 * elem.second.lwpt;
            if (elem.second.lwpt2 < _m_vertex[elem.first.first].height) // chordal
                nesting_depth += 1;
            nesting_depth = nesting_depth * (sidef(elem.second) == Edge::Side::RIGHT ? 1 : -1);
            
            tree[nesting_depth].push(elem.first);
        }

        remove_edges(_graph);
        for (auto& q : tree)
            for (; q.second.empty() == false; q.second.pop())
                _graph.add_edge(q.second.front());

        //std::cout << "\x1b[32m";
        //std::cout << _graph << std::endl;
        //std::cout << "\x1b[m";
    }
    // integer sign(edge e)
    Edge::Side sidef(Edge& edge) {
        if(edge.next != invalid_edge) {
            edge.side = (edge.side == sidef(_m_edge[edge.next])) ? 
                Edge::Side::RIGHT : Edge::Side::LEFT;
            edge.next = invalid_edge;
        }
        return edge.side;
    }


    void third_dfs_traversal(const vertex_t& v, std::vector<Arc>& embd) {
        for (auto e_it = Graph::vertex_iterator(_graph, v).begin();
                  e_it != Graph::vertex_iterator(_graph, v).end();
                  ++e_it)
        {
            if (*e_it != _m_vertex[(*e_it).second].parent_edge &&
                sidef(_m_edge[*e_it]) == Edge::Side::LEFT)
            {
                embd.push_back({*e_it, Arc::Rotation::CCW});
            }
        }

        for (auto e_it = Graph::vertex_iterator(_graph, v).rbegin();
                  e_it != Graph::vertex_iterator(_graph, v).rend(); 
                  --e_it) 
        {
            if (*e_it == _m_vertex[(*e_it).second].parent_edge) {
                embd.push_back({*e_it, Arc::Rotation::NONE});
                third_dfs_traversal((*e_it).second, embd);
            } else if (sidef(_m_edge[*e_it]) == Edge::Side::RIGHT){
                // embd.push_back({*e_it, sidef(_m_edge[*e_it]) == Edge::Side::RIGHT ? Arc::Rotation::ACW : Arc::Rotation::CCW});
                embd.push_back({*e_it, Arc::Rotation::ACW});
            }
        }
    }

    inline void orient(const edge_t& e) { _graph.remove_edge({e.second, e.first}); }
    bool _is_planar;
    size_t _max_height;
    std::vector<Vertex> _m_vertex;
    std::unordered_map<edge_t, Edge, pair_hash> _m_edge;
    std::list<vertex_t> _roots;
    std::stack<ConflictingIntervals> _stack;
};

//bool is_planar(const my::Graph& g) {
//    using namespace boost;
//    typedef adjacency_list<vecS,
//                           vecS,
//                           undirectedS,
//                           property<vertex_index_t, int>
//                           > BoostGraph;
//
//    BoostGraph graph(g.size());
//    for (const vertex_t v : g)
//        for (const edge_t e : Graph::vertex_iterator(g, v))
//            add_edge(e.first, e.second, graph);
//    return boyer_myrvold_planarity_test(graph);
//
//}

bool planar(/*const*/ my::Graph& g) {
    return PlanarityTesting(const_cast<my::Graph&>(g));
}

std::vector<Arc> ordered_embadding(/*const*/ my::Graph& g) {
    return PlanarityTesting(const_cast<my::Graph&>(g)).get_ordered_embadding();
}


void distribution(std::list<my::Graph>& planar_graphs);

std::list<my::Graph> graph_partitioning_in_planes(const my::Graph& graph)
{
    std::list<my::Graph> out;

    // std::unordered_set<my::edge_t> edges;

    using edges_t = std::unordered_set<my::edge_t, pair_hash>;// std::list<my::edge_t>;
    edges_t edges;
    for (const vertex_t v : graph) {
        for (const edge_t e : Graph::vertex_iterator(graph, v)) {
            if (graph.is_connected(e) == false || e.first > e.second)
                // edges.push_back(e);
                edges.insert(e);
        }
    }

    for (edges_t avelcuk = {}; edges.empty() == false; std::swap(edges, avelcuk), avelcuk.clear()) {
        out.emplace_back(graph.size());
		my::Graph& graph_temp = out.back();
        for (auto e : edges)
        {
            graph_temp.connect(e.first, e.second);
            if (my::planar(graph_temp) == false) {
                graph_temp.disconnect(e.first, e.second);
                // avelcuk.push_back(e);
                avelcuk.insert(e);
            }
        }
    }
    for (auto& g : out)
        undirect_graph(g);
    //for (auto& g : out)
    //    std::cout << g << std::endl << std::endl;
    // distribution(out);
    return std::move(out);
}

//std::list<my::Graph> bm_graph_partitioning_in_planes(const my::Graph& graph)
//{
//    std::list<my::Graph> out;
//
//    using edges_t = std::list<my::edge_t>;
//    edges_t edges;
//    for (const vertex_t v : graph)
//        for (const edge_t e : Graph::vertex_iterator(graph, v))
//            edges.push_back(e);
//
//
//    for (edges_t avelcuk = {}; edges.empty() == false; edges = avelcuk, avelcuk.clear()) {
//        my::Graph graph_temp(graph.size());
//        // graph_temp.vertexes = graph.vertexes;
//        for (auto e : edges)
//        {
//            graph_temp.add_edge(e);
//            if (my::is_planar(graph_temp) == false) {
//                graph_temp.remove_edge(e);
//                avelcuk.push_back(e);
//            }
//        }
//        out.push_back(graph_temp);
//    }
//    return out;
//}

void distribution(std::list<my::Graph>& planar_graphs) {
    if (planar_graphs.size() < 2)
        return;

    size_t averige = 0;
    std::vector<size_t> degres;
    for (auto& g : planar_graphs) {
        assert(my::planar(g));
        degres.push_back(degre(g));
        averige += degres.back();
    }
    averige /= planar_graphs.size();

    std::unordered_map<edge_t, Graph*, pair_hash> edges;
    size_t index = 0;
    for (my::Graph& g: planar_graphs) {
        if (degres[index] > averige) {
            for (const vertex_t v : g) {
                for (const edge_t e : Graph::vertex_iterator(g, v)) {
                    edges[e]= &g;
                }
        }
    }

    index = 0;
    for (auto it = planar_graphs.begin(); it != planar_graphs.end(); ++it, ++index) {
        if (degres[index] < averige) {
            for (auto& e : edges) {
                    it->connect(e.first.first, e.first.second);
                    if (my::planar(*it) == false) {
                        e.second->disconnect(e.first.first, e.first.second);
                        edges.erase(e.first);
                        // it->connect(e.second.first, e.second.second);
                        ++degres[index];
                        if (degres[index] > averige) {
                            break;
                        }
                    } else {
                        it->disconnect(e.first.first, e.first.second);
                    }
                }
                if (degres[index] > averige) {
                    break;
                }
            }
        }
    }
    std::cout << std::endl;
    index = 0;
    for (const my::Graph& g : planar_graphs) {
        std::cout << "degre :\t" << degres[index] << "\t" << degre(g) << std::endl;
        ++index;
    }
}

Graph random_graph(const size_t& s, const double& k) {
    my::Graph rg(s);
    size_t edges_max_count = s * (s - 1) / 2;
    if (edges_max_count == 0)
        edges_max_count = 1;

    for (my::vertex_t v = 0; v < rg.size(); ++v) {
        for (my::vertex_t u = v + 1; u < rg.size(); ++u) {
            if (rand() % edges_max_count <= edges_max_count * k)
                rg.connect(u, v);
        }
    }
    return rg;
}

Graph random_bipartite_graph(const size_t& l, const size_t& r, const double& k) {
    my::Graph rg(l + r);
    size_t edges_max_count = l * r;
    if (edges_max_count == 0)
        edges_max_count = 1;

    for (my::vertex_t v = 0; v < l; ++v) {
        for (my::vertex_t u = l; u < rg.size(); ++u) {
            if (rand() % edges_max_count <= edges_max_count * k)
                rg.connect(u, v);
        }
    }
    return rg;

}

} // namespace my
