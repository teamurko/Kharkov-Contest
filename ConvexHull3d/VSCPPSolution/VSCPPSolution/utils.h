#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

namespace constants
{
    const double EPS = 1e-7;
}

#endif //UTILS_H_INCLUDED
