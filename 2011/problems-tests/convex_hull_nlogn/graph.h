#include "convex_figure.h"

#include <fstream>
#include <string>

namespace Graph {
    void writeToFile(const std::string& file, const ConvexFigure::AdjacencyList& graph)
    {
        std::ofstream out(file.c_str());
        using std::endl;
        out << "digraph Polyhedron {" << endl;
        forv(i, graph) {
            out << "v" << i << "[name = " << i << "]" << endl;
        }
        forv(i, graph) {
            const Ids& adj = graph[i];
            forv(j, adj) {
                out << "v" << i << " -> v" << adj[j] << endl;
            }
        }
        out << "}" << endl;
        out.close();
    }
} //Graph
