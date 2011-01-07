#ifndef FACET_H_INCLUDED
#define FACET_H_INCLUDED

#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

class Edge
{
public:
    Edge(size_t from, size_t to) : from_(from), to_(to) {}
    size_t from() const { return from_; }
    size_t to() const { return to_; }
private:
    size_t from_;
    size_t to_;
};

bool operator<(const Edge& edgeA, const Edge& edgeB) 
{
    return edgeA.from() < edgeB.from() || 
        (edgeA.from() == edgeB.from() &&
         edgeA.to() < edgeB.to());
}

class Facet
{
public:
    typedef std::vector<size_t> Indices;

    Facet() {}
    Facet(const Indices& indices)
    {
        vertices_ = indices;
    }

    size_t operator[](size_t index) const
    {
        assert(index < size());
        return vertices_[index];
    }

    void add(size_t vertexIndex) 
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
     
private:
    Indices vertices_;                        
};

typedef std::vector<Facet> Facets;

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
