#include "test.h"
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <chrono>
#include <thread>


void test_planarity_functions() {
    unsigned long long btime = 0;
    unsigned long long mtime = 0;
    unsigned long long btime_planar = 0;
    unsigned long long mtime_planar = 0;
    unsigned long long mtotal = 0;
    unsigned long long btotal = 0;

    qDebug()<<"kkk";
    qDebug()<< "Test range"<<"\tP_myTime" << "\tP_bmTime" << "\tP_my_Total" << "\tP_bm_Total" << "\tNP_myTime" << "\tNP_bmTime"<< "\tNP_my_Total" << "\tNP_bm_Total\n";

    for (size_t s = 10; s < 200; s += 10) {
        for (double k = 0.001; k < 0.2; k += 0.01) {
            // std::cout << "[" << s << " " << k << "]" <<std::endl;

            using namespace my;
            using namespace boost;
            typedef adjacency_list<vecS,
                                   vecS,
                                   undirectedS,
                                   property<vertex_index_t, int>
                                   > BoostGraph;

            my::Graph g = random_graph(s, k);
            BoostGraph graph(g.size());
            for (const vertex_t v : g)
                for (const edge_t e : Graph::vertex_iterator(g, v))
                    add_edge(e.first, e.second, graph);



            auto start = std::chrono::high_resolution_clock::now();
            // bool b_is_planar = boyar(g);

            bool b_is_planar = boyer_myrvold_planarity_test(graph);


            auto end = std::chrono::high_resolution_clock::now();
            auto function_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            start = std::chrono::high_resolution_clock::now();
            bool b_planar = planar(g);
            end = std::chrono::high_resolution_clock::now();
            auto my_function_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            if (b_planar) {
                mtime_planar += my_function_time.count();
                btime_planar += function_time.count();
            } else {
                mtime += my_function_time.count();
                btime += function_time.count();
            }

            if (b_planar != b_is_planar) {
              //   std::cout << "\x1b[31mfail \x1b[m" << std::endl;
               //  std::cout << g << std::endl;
                 planar(g);
                 return;
            }
        }
        if (s % 20 == 0) {
             // std::cout << (b_is_planar ? "(planar)" : "(not planar)") << " random test " << s << " :" << std::flush;
             // static auto mtotal = my_function_time.count();
             // static auto btotal = function_time.count();
             // std::cout << "\tmy: " << my_function_time.count() << "   (" << (mtotal += my_function_time.count()) << ")";
             // std::cout << "\tbm: " << function_time.count()<< "   (" << (btotal += function_time.count()) << ")";
//             std::cout << "planar->" << "\trandom tests in [" << s - 199  << ", "<< s << "] range :" << std::flush;
//             std::cout << "\tmy: " << mtime_planar << "   (" << (mtotal += mtime_planar) << ")";
//             std::cout << "\t\tbm: " << btime_planar << "   (" << (btotal += btime_planar) << ")";
//             std::cout << " -> \x1b[32mpass\x1b[m " << std::endl;

//             std::cout << "(not planar)" << "\trandom tests in [" << s - 199  << ", "<< s << "] range :" << std::flush;
//             std::cout << "\tmy: " << mtime << "   (" << (mtotal += mtime) << ")";
//             std::cout << "\t\tbm: " << btime << "   (" << (btotal += btime) << ")";
//             std::cout << " -> \x1b[32mpass\x1b[m " << std::endl;

//            std::cout << s-199 <<  " - " << s << std::flush;
//            std::cout << "\t\t" << mtime_planar;
//            std::cout << "\t\t" << btime_planar;
//            std::cout << "\t\t" << (mtotal += mtime_planar);
//            std::cout << "\t\t" << (btotal += btime_planar);
//            std::cout << "\t\t" << mtime;
//            std::cout << "\t\t" << btime;
//            std::cout << "\t\t" << (mtotal += mtime);
//            std::cout << "\t\t" << (btotal += btime)<< std::endl;

            qDebug() << s-199 <<  " - " << s
             << "\t\t" << mtime_planar
             << "\t\t" << btime_planar
             << "\t\t" << (mtotal += mtime_planar)
             << "\t\t" << (btotal += btime_planar)
             << "\t\t" << mtime
             << "\t\t" << btime
             << "\t\t" << (mtotal += mtime)
             << "\t\t" << (btotal += btime) << "\n";
             btime = mtime = 0;
        }
    }
}
