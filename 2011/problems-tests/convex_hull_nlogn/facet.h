#ifndef FACET_H_INCLUDED
#define FACET_H_INCLUDED

#include "plane.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <string>

typedef size_t Id;
typedef std::vector<Id> Ids;

class Edge
{
public:
    Edge(Id from, Id to) : from_(from), to_(to) {}
    Id from() const { return from_; }
    Id to() const { return to_; }
    void setFrom(Id from) { from_ = from; }
    void setTo(Id to) { to_ = to; }
private:
    Id from_;
    Id to_;
};

bool operator<(const Edge& edgeA, const Edge& edgeB) 
{
    return edgeA.from() < edgeB.from() || 
        (edgeA.from() == edgeB.from() &&
         edgeA.to() < edgeB.to());
}

typedef std::vector<Edge> Edges;

class Facet
{
public:
    Facet() {}
    Facet(const Ids& indices)
    {
        vertices_ = indices;
    }

    Id operator[](Id index) const
    {
        assert(index < size());
        return vertices_[index];
    }

    void add(Id vertexIndex) 
    {
        vertices_.push_back(vertexIndex);
    }

    size_t size() const
    {
        return vertices_.size();
    }

    void order() 
    {
        size_t shift = static_cast<size_t>(
                std::min_element(vertices_.begin(), vertices_.end()) 
                        - vertices_.begin());
        std::rotate(vertices_.begin(), vertices_.begin() + shift, vertices_.end());
    }

    friend std::ostream& operator<<(std::ostream& out, const Facet& facet);

    std::string str() const 
    {
        std::ostringstream out;
        out << size() << " ";
        out << this->operator[](0) << " " << this->operator[](1) << " " << this->operator[](2);
        return out.str();
    }

    void mapIds(const Ids& map) 
    {
        forv(i, vertices_) {
            vertices_[i] = map[vertices_[i]];
        }
    }
     
private:
    Ids vertices_;                        
};

typedef std::vector<Facet> Facets;

bool below(const Points& points, const Plane& plane)
{
    for(size_t i = 0; i < points.size(); ++i) {
        if (plane.signedDistance(points[i]) > constants::EPS) {
            return false;
        }
    }
    return true;
}

bool isFacet(const Points& points, const Facet& facet) 
{
    Plane plane(points[facet[0]], points[facet[1]], points[facet[2]]);
    return below(points, plane);
}

std::ostream& operator<<(std::ostream& out, const Facet& facet)
{
    out << facet.size();
    for(size_t i = 0; i < facet.size(); ++i) {
        out << " " << facet[i];
    }
    return out;
}

bool operator<(const Facet& facetA, const Facet& facetB)
{
    if (facetA.size() != facetB.size()) {
        return facetA.size() < facetB.size();
    }        
    for(size_t i = 0; i < facetA.size(); ++i) {
        if (facetA[i] != facetB[i]) {
            return facetA[i] < facetB[i];
        }
    }
    return false;
}


Facet makeFacet(size_t a, size_t b, size_t c) 
{
    Facet facet;
    facet.add(a);
    facet.add(b);
    facet.add(c);
    facet.order();
    return facet;
}

#endif //FACET_H_INCLUDED
