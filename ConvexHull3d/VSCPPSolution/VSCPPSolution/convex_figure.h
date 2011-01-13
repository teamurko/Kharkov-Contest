#ifndef CONVEX_FIGURE_H_INCLUDED
#define CONVEX_FIGURE_H_INCLUDED

#include "point.h"
#include "facet.h"
#include <vector>

typedef size_t Id;
typedef std::vector<Id> Ids;

class ConvexFigure 
{
public:
    typedef std::vector<Ids> AdjacencyList;

    ConvexFigure() {}

    ConvexFigure(const Points& points) : points_(points), 
            graph_(std::vector<std::vector<Id> >(points.size())) {}

    virtual ~ConvexFigure() {}

    void addEdge(Id from, Id to)
    {
        graph_[from].push_back(to);
    }

    void addEdge(const Edge& edge)
    {
        graph_[edge.from()].push_back(edge.to());
    }
     
    bool adjacent(Id from, Id to) const
    {
        forv(i, graph_[from]) {
            if (graph_[from][i] == to) return true;
        }
        return false;
    }

    const Ids& adjacentVertices(Id vertex) const 
    {
        return graph_[vertex];
    }

    const Point& operator[](size_t index) const 
    {
        return points_[index];        
    }

    Id indexOf(Id pointId) const 
    {
        forv(i, points_) {
            if (points_[i].id() == pointId) {
                return i;
            }
        }
        return std::numeric_limits<size_t>::max();
    }

    void clear() 
    {
        graph_.clear();
        points_.clear();
    }

    size_t size() const { return points_.size(); }

    const AdjacencyList& graph() const { return graph_; }

protected:
    AdjacencyList graph_;
    Points points_;       
};

namespace DFS {
typedef std::vector<bool> Used;

void dfs(Id v, const ConvexFigure::AdjacencyList& graph, Used& used) 
{
    if (used[v]) return;
    used[v] = true;
    forv(i, graph[v]) dfs(graph[v][i], graph, used);
}
} //DFS

class Polyhedron: public ConvexFigure
{
public:
    Polyhedron() {}
    Polyhedron(const Points& points) : ConvexFigure(points) {}
    virtual ~Polyhedron() {}

    Ids leaveReachedFrom(Id source) 
    {
        using namespace DFS;
        Used used(size());
        dfs(source, graph_, used);
        Ids mapIds(size());
        for(size_t i = 1; i < size(); ++i) {
            mapIds[i] = mapIds[i-1] + static_cast<size_t>(used[i-1]);
        }
        forv(i, graph_) {
            forv(j, graph_[i]) {
                graph_[i][j] = mapIds[graph_[i][j]];
            }
        }
        size_t cur = 0;
        forv(i, graph_) {
            if (used[i]) {
                graph_[cur++] = graph_[i];
            }
        }
        graph_.resize(cur);
        return mapIds;
    }

    void removeEdges(Id source, Id start, Id end) 
    {
        start = (start + 1) % graph_[source].size();
        rotate(graph_[source].begin(), graph_[source].begin() + start, graph_[source].end());
        end = (end + graph_[source].size() - start) % graph_[source].size();
        graph_[source].erase(graph_[source].begin(), graph_[source].begin() + end);
    }

    void merge(const Polyhedron& polyhedron, const Edges& edges) 
    {
        size_t curSize = size();
        forn(i, polyhedron.size()) {
            points_.push_back(polyhedron[i]);
            graph_.push_back(Ids());
        }
        
        forn(i, curSize) {
            const Ids& adj = polyhedron.adjacentVertices(i);
            forv(j, adj) {
                addEdge(i + curSize, adj[j] + curSize);
            }
        }
        
        forv(i, edges) {
            const Edge& edge = edges[i];
            addEdge(edge.from(), edge.to() + curSize);
            addEdge(edge.to() + curSize, edge.from());
        }
    }

private:
};

class Polygon: public  ConvexFigure
{
public:     
    Polygon() {}
    Polygon(const Points& points) : ConvexFigure(points) {}
    virtual ~Polygon() {}
    Points vertices() const
    {
        return points_;
    }

private:
};

#endif //CONVEX_FIGURE_H_INCLUDED