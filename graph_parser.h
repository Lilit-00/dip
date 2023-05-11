#
#ifndef HEADER_FILE_GRAPH_PARSER
#define HEADER_FILE_GRAPH_PARSER

#include "graph.h"
#include <fstream>

std::istream& operator>>(std::istream& input, my::Graph& graph);

#endif
