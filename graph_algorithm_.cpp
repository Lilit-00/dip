#include "graph_algorithm.h"

#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace my  {

inline edge_t reverse_edge(const edge_t& e) {
    return {e.second, e.first};
}

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

bool is_bipartite(const Graph& g) {
    // Graph& graph = const_cast<Graph&>(g);
    enum Color : int8_t { UNDEFINED = 0, BLACK = -1, WITH = 1 };
    std::vector<Color> colors_vec(g.size());
    for (const vertex_t& v : g) {
        if (colors_vec[v] == Color::UNDEFINED) {
            colors_vec[v] = Color::BLACK;
            
            std::queue<vertex_t> q;
            q.push(v);
            while(q.empty() == false) {
                vertex_t u = q.front();
                q.pop();

                for (edge_t e : Graph::vertex_iterator(g, u)) {
                    if (colors_vec[e.second] != Color::UNDEFINED) {
                        if (colors_vec[e.second] == colors_vec[e.first])
                            return false;
                        continue;
                    }
                    q.push(e.second);
                    colors_vec[e.second] = Color(-colors_vec[e.first]);
                }
            }
        }
    } 
    return true;
}




} // namespace my
