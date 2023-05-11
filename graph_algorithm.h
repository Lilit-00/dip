#ifndef HEADER_FILE_GRAPH_ALGORITHM
#define HEADER_FILE_GRAPH_ALGORITHM

#include "graph.h"
#include "graph_parser.h"

#include <list>

namespace my  {

void orient_graph(Graph&);
void  undirect_graph(Graph&);

// Graph depth_first_serach(const Graph& g);
std::vector<bool> bipartite(Graph g);

std::list<my::Graph> graph_partitioning_in_planes(const my::Graph&);
std::list<my::Graph> bm_graph_partitioning_in_planes(const my::Graph& graph);

bool planar(/*const*/ Graph&);
bool is_planar(const my::Graph& g);


struct Arc {
    edge_t edge;
    enum Rotation { ACW = 1, NONE = 0, CCW = -1 } rot;
};

std::vector<Arc> ordered_embadding(/*const*/ my::Graph& g);

inline edge_t reverse_edge(const edge_t& e) {
    return {e.second, e.first};
}

Graph random_graph(const size_t&, const double&);
Graph random_bipartite_graph(const size_t&, const size_t&, const double&);

} // namespace my

#endif
