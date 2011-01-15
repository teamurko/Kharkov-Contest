#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <iostream>

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

namespace constants
{
    const double EPS = 1e-9;
}

template<typename T>
void print(std::ostream& out, const std::vector<T>& vector)
{
    out << vector.size();
    forv(i, vector) {
        out << " " << vector[i];
    }
    out << std::endl;
}

#endif //UTILS_H_INCLUDED
