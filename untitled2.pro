QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MainWindow.cpp \
    circuit_board.cpp \
    dynamicmatrix.cpp \
    fraction.cpp \
    graph.cpp \
    graph_algorithm.cpp \
    graph_parser.cpp \
    graphwidget.cpp \
    helper.cpp \
    main.cpp \
    planargraphwidget.cpp \
    router.cpp \
    test.cpp

HEADERS += \
    MainWindow.h \
    boost/fusion/container/list/detail/cpp03/preprocessed/list10_fwd.hpp \
    boost/graph/accounting.hpp \
    boost/graph/adj_list_serialize.hpp \
    boost/graph/adjacency_iterator.hpp \
    boost/graph/adjacency_list.hpp \
    boost/graph/adjacency_list_io.hpp \
    boost/graph/adjacency_matrix.hpp \
    boost/graph/astar_search.hpp \
    boost/graph/bandwidth.hpp \
    boost/graph/bc_clustering.hpp \
    boost/graph/bellman_ford_shortest_paths.hpp \
    boost/graph/betweenness_centrality.hpp \
    boost/graph/biconnected_components.hpp \
    boost/graph/bipartite.hpp \
    boost/graph/boyer_myrvold_planar_test.hpp \
    boost/graph/boykov_kolmogorov_max_flow.hpp \
    boost/graph/breadth_first_search.hpp \
    boost/graph/bron_kerbosch_all_cliques.hpp \
    boost/graph/buffer_concepts.hpp \
    boost/graph/chrobak_payne_drawing.hpp \
    boost/graph/circle_layout.hpp \
    boost/graph/closeness_centrality.hpp \
    boost/graph/clustering_coefficient.hpp \
    boost/graph/compressed_sparse_row_graph.hpp \
    boost/graph/connected_components.hpp \
    boost/graph/copy.hpp \
    boost/graph/core_numbers.hpp \
    boost/graph/create_condensation_graph.hpp \
    boost/graph/cuthill_mckee_ordering.hpp \
    boost/graph/cycle_canceling.hpp \
    boost/graph/dag_shortest_paths.hpp \
    boost/graph/degree_centrality.hpp \
    boost/graph/depth_first_search.hpp \
    boost/graph/dijkstra_shortest_paths.hpp \
    boost/graph/dijkstra_shortest_paths_no_color_map.hpp \
    boost/graph/dimacs.hpp \
    boost/graph/directed_graph.hpp \
    boost/graph/dll_import_export.hpp \
    boost/graph/dominator_tree.hpp \
    boost/graph/eccentricity.hpp \
    boost/graph/edge_coloring.hpp \
    boost/graph/edge_connectivity.hpp \
    boost/graph/edge_list.hpp \
    boost/graph/edmonds_karp_max_flow.hpp \
    boost/graph/edmunds_karp_max_flow.hpp \
    boost/graph/erdos_renyi_generator.hpp \
    boost/graph/exception.hpp \
    boost/graph/exterior_property.hpp \
    boost/graph/filtered_graph.hpp \
    boost/graph/find_flow_cost.hpp \
    boost/graph/floyd_warshall_shortest.hpp \
    boost/graph/fruchterman_reingold.hpp \
    boost/graph/geodesic_distance.hpp \
    boost/graph/graph_archetypes.hpp \
    boost/graph/graph_as_tree.hpp \
    boost/graph/graph_concepts.hpp \
    boost/graph/graph_mutability_traits.hpp \
    boost/graph/graph_selectors.hpp \
    boost/graph/graph_stats.hpp \
    boost/graph/graph_traits.hpp \
    boost/graph/graph_utility.hpp \
    boost/graph/graphml.hpp \
    boost/graph/graphviz.hpp \
    boost/graph/grid_graph.hpp \
    boost/graph/gursoy_atun_layout.hpp \
    boost/graph/hawick_circuits.hpp \
    boost/graph/howard_cycle_ratio.hpp \
    boost/graph/incremental_components.hpp \
    boost/graph/is_kuratowski_subgraph.hpp \
    boost/graph/is_straight_line_drawing.hpp \
    boost/graph/isomorphism.hpp \
    boost/graph/iteration_macros.hpp \
    boost/graph/iteration_macros_undef.hpp \
    boost/graph/johnson_all_pairs_shortest.hpp \
    boost/graph/kamada_kawai_spring_layout.hpp \
    boost/graph/king_ordering.hpp \
    boost/graph/kruskal_min_spanning_tree.hpp \
    boost/graph/labeled_graph.hpp \
    boost/graph/leda_graph.hpp \
    boost/graph/lookup_edge.hpp \
    boost/graph/loop_erased_random_walk.hpp \
    boost/graph/make_biconnected_planar.hpp \
    boost/graph/make_connected.hpp \
    boost/graph/make_maximal_planar.hpp \
    boost/graph/matrix_as_graph.hpp \
    boost/graph/max_cardinality_matching.hpp \
    boost/graph/maximum_adjacency_search.hpp \
    boost/graph/maximum_weighted_matching.hpp \
    boost/graph/mcgregor_common_subgraphs.hpp \
    boost/graph/mesh_graph_generator.hpp \
    boost/graph/metis.hpp \
    boost/graph/metric_tsp_approx.hpp \
    boost/graph/minimum_degree_ordering.hpp \
    boost/graph/named_function_params.hpp \
    boost/graph/named_graph.hpp \
    boost/graph/neighbor_bfs.hpp \
    boost/graph/numeric_values.hpp \
    boost/graph/one_bit_color_map.hpp \
    boost/graph/overloading.hpp \
    boost/graph/page_rank.hpp \
    boost/graph/planar_canonical_ordering.hpp \
    boost/graph/planar_face_traversal.hpp \
    boost/graph/plod_generator.hpp \
    boost/graph/point_traits.hpp \
    boost/graph/prim_minimum_spanning_tree.hpp \
    boost/graph/profile.hpp \
    boost/graph/properties.hpp \
    boost/graph/property_iter_range.hpp \
    boost/graph/push_relabel_max_flow.hpp \
    boost/graph/r_c_shortest_paths.hpp \
    boost/graph/random.hpp \
    boost/graph/random_layout.hpp \
    boost/graph/random_spanning_tree.hpp \
    boost/graph/read_dimacs.hpp \
    boost/graph/relax.hpp \
    boost/graph/reverse_graph.hpp \
    boost/graph/rmat_graph_generator.hpp \
    boost/graph/sequential_vertex_coloring.hpp \
    boost/graph/simple_point.hpp \
    boost/graph/sloan_ordering.hpp \
    boost/graph/small_world_generator.hpp \
    boost/graph/smallest_last_ordering.hpp \
    boost/graph/ssca_graph_generator.hpp \
    boost/graph/st_connected.hpp \
    boost/graph/stanford_graph.hpp \
    boost/graph/stoer_wagner_min_cut.hpp \
    boost/graph/strong_components.hpp \
    boost/graph/subgraph.hpp \
    boost/graph/successive_shortest_path_nonnegative_weights.hpp \
    boost/graph/tiernan_all_cycles.hpp \
    boost/graph/topological_sort.hpp \
    boost/graph/topology.hpp \
    boost/graph/transitive_closure.hpp \
    boost/graph/transitive_reduction.hpp \
    boost/graph/transpose_graph.hpp \
    boost/graph/tree_traits.hpp \
    boost/graph/two_bit_color_map.hpp \
    boost/graph/two_graphs_common_spanning_trees.hpp \
    boost/graph/undirected_dfs.hpp \
    boost/graph/undirected_graph.hpp \
    boost/graph/use_mpi.hpp \
    boost/graph/vector_as_graph.hpp \
    boost/graph/vertex_and_edge_range.hpp \
    boost/graph/vf2_sub_graph_iso.hpp \
    boost/graph/visitors.hpp \
    boost/graph/wavefront.hpp \
    boost/graph/write_dimacs.hpp \
    circuit_board.h \
    dynamicmatrix.h \
    fraction.h \
    graph.h \
    graph_algorithm.h \
    graph_inline_implementations.h \
    graph_parser.h \
    graphwidget.h \
    helper.h \
    planargraphwidget.h \
    router.h \
    test.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
