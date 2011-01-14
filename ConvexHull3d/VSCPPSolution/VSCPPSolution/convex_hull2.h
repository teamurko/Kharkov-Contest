#ifndef CH_2_H_INCLUDED
#define CH_2_H_INCLUDED

#include "point.h"
#include <algorithm>
#include <cassert>

class Compare 
{
public:
    Compare(const Point& point) : point_(point) {}
    virtual bool operator()(const Point& p1, const Point& p2) const = 0;
protected:
    Point point_;
};

class CompareLeft: public Compare
{
public:
    CompareLeft(const Point& point) : Compare(point) {}
    virtual bool operator()(const Point& p1, const Point& p2) const
    {
        return turnsLeft(point_, p1, p2);
    }
};

class CompareNotRight: public Compare
{
public:
    CompareNotRight(const Point& point) : Compare(point) {}
    virtual bool operator()(const Point& p1, const Point& p2) const
    {
        return !turnsLeft(point_, p1, p2);
    }
}; 

Id findTangent(const Points& points, const Compare& comp)
{
    assert(points.size() >= 3);
    size_t cur = 0, next = cur + 1;
    while (comp(points[cur], points[next])) {
        next = cur;
        cur = (cur + points.size() - 1) % points.size();
    }    
    while (!comp(points[cur], points[next])) {
        cur = next;
        next = (next + 1) % points.size();
    }
    return cur;
}

class PointsComparator
{
public:
    PointsComparator(const Point& point) : origin_(point) {}

    bool operator() (const Point& pointA, const Point& pointB) const
    {
        const Point& A = pointA - origin_, B = pointB - origin_;
        double det = det2(A, B);
        if (fabs(det) < constants::EPS) {
            return A.squaredLength() < B.squaredLength();
        } else {
            return det > 0;
        }            
    }
private:
    Point origin_;
};

namespace g2d 
{
    bool PointLess(const Point& a, const Point& b) 
    {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    }
    bool PointEq(const Point& a, const Point& b) 
    {
        return a.x() == b.x() && a.y() == b.y();
    }
} //2d

void convexHull2(Points points, Polygon* polygon)
{
    assert(points.size() > 3);
    sort(points.begin(), points.end(), g2d::PointLess);
    points.erase(unique(points.begin(), points.end(), g2d::PointEq), points.end());
    assert(points.size() > 2);
    Points::iterator originIterator = 
        min_element(points.begin(), points.end());
    Point origin = *originIterator;
    std::swap(*points.begin(), *originIterator);

    PointsComparator comparator(origin); 

    sort(points.begin()+1, points.end(), comparator); 
    
    typedef Points ConvexHull;

    ConvexHull convexHull;
    Points::const_iterator iter = points.begin();
    forn(i, 2) {
        convexHull.push_back(*iter++);
    }
    for ( ;iter != points.end(); ++iter) {
        while (convexHull.size() >= 2 && 
            !turnsLeft(convexHull[convexHull.size()-2],
                       convexHull[convexHull.size()-1],
                       *iter)) {
            convexHull.pop_back();
        }
        convexHull.push_back(*iter);
    }

    //remove intermediate points
    while (convexHull.size() >= 3 && 
        sameLine(convexHull[0], convexHull.back(), convexHull[convexHull.size()-2])) {
            convexHull.pop_back();
    }
    //TODO check if convexHull is ok, 
    //e.g. it does not contain tree points on the same line

    *polygon = Polygon(convexHull);
    forn(i, convexHull.size()) {
        polygon->addEdge(i, (i+1)%convexHull.size());
    }
}


#endif //CH_2_H_INCLUDED