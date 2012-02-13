#ifndef CONVEX_FIGURE_H_INCLUDED
#define CONVEX_FIGURE_H_INCLUDED

#include "point.h"
#include "facet.h"
#include <vector>
#include <set>
#include <iterator>
#include <limits>
#include <algorithm>

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

    void addEdges(Id source, const Ids& adding)
    {
        Ids& adj = graph_[source];
        assert(adding[0] >= size());
        Ids::iterator iter = adj.begin();
        while (iter != adj.end() && *iter + size() != adding[0]) ++iter;
        if (iter != adj.end()) {
            std::rotate(adj.begin(), iter, adj.end());
        }
        Ids merged;
        Ids::const_iterator addIter = adding.begin();
        iter = adj.begin();
        while (addIter != adding.end() && iter != adj.end()) {
            assert(*addIter >= size());
            while (iter != adj.end() && *iter + size() != *addIter) {
                merged.push_back(*iter);
                ++iter;
            }
            if (iter == adj.end()) break;
            merged.push_back(*iter++);
            ++addIter;
            while (addIter != adding.end() && *addIter < size()) {
                merged.push_back(*addIter);
                ++addIter;
            }
        }
        std::copy(iter, adj.end(), std::back_inserter(merged));
        for(; addIter != adding.end(); ++addIter) {
            if (*addIter < size()) {
                merged.push_back(*addIter);
            }
        }
        adj = merged;
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

    const Points& points() const { return points_; }

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
                graph_[cur] = graph_[i];
                points_[cur++] = points_[i];
            }
        }
        graph_.resize(cur);
        points_.resize(cur);
        return mapIds;
    }

    void removeEdges(Id source, Id start, Id end) 
    {
        Ids& edges = graph_[source];
        Ids::iterator startIter = edges.begin();
        while (startIter != edges.end() && *startIter != start) ++startIter;
        if (startIter == edges.end()) {
            //TODO make tests for this case
            edges.clear();
        }
        else {
            rotate(edges.begin(), startIter+1, edges.end());
            startIter = edges.begin();
            while (*startIter != end) ++startIter;
            edges.erase(edges.begin(), startIter);
        }
    }

    //TODO refactoring
    void merge(const Polyhedron& polyhedron, Edges edges) 
    {
        forv(i, edges) {
            edges[i].setTo(edges[i].to() + size());
        }
        //check edges
        {
            std::set<Id> used;
            forv(i, edges) {
                used.insert(edges[i].from());
            }
            forv(i, edges) {
                assert(!used.count(edges[i].to()));
            }
        }
        size_t curSize = size();
        forn(i, polyhedron.size()) {
            points_.push_back(polyhedron[i]);
            graph_.push_back(Ids());
        }
        
        forv(i, polyhedron) {
            const Ids& adj = polyhedron.adjacentVertices(i);
            forv(j, adj) {
                addEdge(i + curSize, adj[j] + curSize);
            }
        }

        //making edges so that edges[0].from() != edges.back().from() 
        //provided that not all edges[i].from() are the same
        {
            Edges::iterator iter = edges.begin();
            while (iter != edges.end() && iter->from() == edges[0].from()) {
                ++iter;
            }
            if (iter != edges.end()) {
                std::rotate(edges.begin(), iter, edges.end());
            }
        }

        std::vector<Ids> addAdj(size());
        //adding edges to the first polyhedron
        Edges::const_iterator edgeIter = edges.begin();
        for(;edgeIter != edges.end();) {
            Edges::const_iterator edgeEnd = edgeIter;
            Ids adj;
            while (edgeEnd != edges.end() && edgeIter->from() == edgeEnd->from()) {
                adj.push_back(edgeEnd->to());
                edgeEnd++;
            }
            std::reverse(adj.begin(), adj.end());
            Id source = edgeIter->from();
            edgeIter = edgeEnd;
            Id addAfter;
            if (edgeIter == edges.end()) {
                addAfter = edges[0].from();
            } 
            else {
                addAfter = edgeIter->from();
            }
            addAdj[source].push_back(addAfter+size());
            forv(j, adj) {
                addAdj[source].push_back(adj[j]);
            }
        }

        //making edges so that edges[0].to() != edges.back().to() 
        //provided that not all edges[i].to() are the same
        {
            Edges::iterator iter = edges.begin();
            while (iter != edges.end() && iter->to() == edges[0].to()) {
                ++iter;
            }
            if (iter != edges.end()) {
                std::rotate(edges.begin(), iter, edges.end());
            }
        }

        //adding edges to the second polyhedron
        edgeIter = edges.begin();
        Id addAfter = edges.back().to();
        for(;edgeIter != edges.end();) {
            Edges::const_iterator edgeEnd = edgeIter;
            Ids adj;
            while (edgeEnd != edges.end() && edgeIter->to() == edgeEnd->to()) {
                adj.push_back(edgeEnd->from());
                edgeEnd++;
            }
            Id source = edgeIter->to();
            addAdj[source].push_back(addAfter+size());
            forv(j, adj) {
                addAdj[source].push_back(adj[j]);
            }
            addAfter = edgeIter->to();
            edgeIter = edgeEnd;
        }

        forv(i, addAdj) {
            if (addAdj[i].size() > 0) {
                addEdges(i, addAdj[i]);
            }
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

/*
        //TODO fix bug
        size_t shift = 0;
        while (shift < edges.size() && edges[shift].to() == edges.back().to()) {
            ++shift;
        }
        shift %= edges.size();
        rotate(edges.begin(), edges.begin() + shift, edges.end());
        Edges::const_iterator iter = edges.begin();
        while (iter != edges.end()) {
            Edges::const_iterator nextIter = iter;
            while (nextIter != edges.end() && nextIter->to() == iter->to()) {
                ++nextIter;
            }
            Id addAfter;
            if (iter != edges.begin()) {
                addAfter = (iter-1)->to();
            }
            else {
                addAfter = edges.back().to();
            }
            addEdges(iter->to() + curSize, addAfter, iter, nextIter);
            iter = nextIter;
//            const Edge& edge = edges[i];
//            addEdge(edge.to() + curSize, edge.from());
        }
        
        std::reverse(edges.begin(), edges.end());

        shift = 0;
        while (shift < edges.size() && edges[shift].from() == edges.back().from()) {
            ++shift;
        }
        shift %= edges.size();
        rotate(edges.begin(), edges.begin() + shift, edges.end());

        forv(i, edges) {
            const Edge& edge = edges[i];
            addEdge(edge.from(), edge.to() + curSize);
        }
*/
