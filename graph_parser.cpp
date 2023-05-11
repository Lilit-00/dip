#include "graph_parser.h"
#include "graph_algorithm.h"

#include <unordered_map>
#include <unordered_set>
#include <exception>

#include "helper.h"

void shorten(my::Graph& graph) {
    std::unordered_map<my::vertex_t, my::vertex_t> mp;
    size_t dif = 0;
    my::Graph tgraph = graph;
    my::undirect_graph(tgraph);
    for (auto v_it = tgraph.begin(); v_it != tgraph.end(); ++v_it) {
        if (v_it.begin() == v_it.end()) {
            ++dif;
        }
        mp[*v_it] = *v_it - dif;
    }
    my::Graph new_graph(graph.size() - dif);
    for (auto v : graph)
        for (auto e : my::Graph::vertex_iterator(graph, v))
            new_graph.add_edge(mp[e.first], mp[e.second]);
    graph = std::move(new_graph);
}

std::istream& operator>>(std::istream& input, my::Graph& graph) {
	std::unordered_map<my::vertex_t, std::unordered_set<my::vertex_t>> adj_set;
	my::vertex_t max_vertex = 0;

	skip_spaces(input);
	while (input.eof() == false) {
		my::vertex_t vertex;
		input >> vertex;
		if (vertex > max_vertex)
			max_vertex = vertex;

		char seperator;
		input >> seperator;
		if (seperator != ':')
			return input;

		skip_spaces(input);
		while (input.eof() == false && input.peek() != '\n') {
			skip_spaces(input);
			if (is_digit(input.peek()) == false)
				return input;

			my::vertex_t neighbor;
			input >> neighbor;

			if (neighbor > max_vertex)
				max_vertex = neighbor;

			adj_set[vertex].insert(neighbor);

			skip_spaces(input);

			if (input.peek() == ',')
				input.get(seperator);
			else if (input.eof() == false && input.peek() != '\n')
				return input;
		}
		skip_eol(input);
	}
	my::Graph new_graph((size_t)max_vertex + 1);
	for (auto& st : adj_set)
		for (my::vertex_t v : st.second)
			new_graph.add_edge(st.first, v);
	graph = std::move(new_graph);

    shorten(graph);

    return input;
}

