#include <iostream>   // std::endl, setw.
#include <iomanip>    // setprecision etc...
#include <algorithm>  // copy.
#include <vector>     // field.
#include <utility>    // pair
#include <iterator>   // ostream_iterator
#include <complex>    // xy.
#include <cassert>    // assert.
#include <cmath>      // sqrt, floor.
#include <cstdio>     // g++ 4.4.0

using namespace std;

template<typename K>
struct Coordinate {
  typedef K value_type;
  Coordinate() : x(0), y(0), z(0) {}
  Coordinate(value_type x, value_type y, value_type z) : x(x), y(y), z(z) {}
  ~Coordinate() {}
  Coordinate& operator+=(const Coordinate& p) {
    x+=p.x; y+=p.y; z+=p.z; return *this;
  }
  Coordinate& operator-=(const Coordinate& p) {
    x-=p.x; y-=p.y; z-=p.z; return *this;
  }
  Coordinate& operator*=(const K k) {
    x *= k; y *= k; z *= k; return *this;
  }

  value_type innerProduct(const Coordinate& p) const {
    return x * p.x + y * p.y + z * p.z;
  }

  Coordinate outerProduct(const Coordinate& p) const {
    return Coordinate(y * p.z - z * p.y,
		      z * p.x - x * p.z,
		      x * p.y - y * p.x);
  }

  value_type norm() const {
    return innerProduct(*this);
  }

  static Coordinate unitVector(int i) {
    if (i == 0)
      return Coordinate(1, 0, 0);
    else if (i == 1)
      return Coordinate(0, 1, 0);
    else if (i == 2)
      return Coordinate(0, 0, 1);
    else
      assert(false);
  }

  value_type x, y, z;
};

template<typename K>
Coordinate<K> operator+(const Coordinate<K>& lhs, const Coordinate<K>& rhs) {
  Coordinate<K> tmp(lhs);
  tmp += rhs;
  return tmp;
}

template<typename K>
Coordinate<K> operator-(const Coordinate<K>& lhs, const Coordinate<K>& rhs) {
  Coordinate<K> tmp(lhs);
  tmp -= rhs;
  return tmp;
}

template<typename K>
Coordinate<K> operator*(K lhs, const Coordinate<K>& rhs) {
  Coordinate<K> tmp(rhs);
  tmp *= lhs;
  return tmp;
}

template<typename K>
Coordinate<K> operator-(const Coordinate<K>& x) {
  return Coordinate<K>(-x.x, -x.y, -x.z);
}

template<typename K>
bool operator==(const Coordinate<K>& lhs, const Coordinate<K>& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}


typedef Coordinate<int> Coord;
typedef complex<double> xy;

struct Problem {
  vector<Coord> cube;
  Coord start, goal;
  Problem() {}
  ~Problem() {}
  void clear() {cube.clear();}
};

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<VVI> VVVI;

typedef vector<double> VD;
typedef vector<VD> VVD;

struct Vertex {
  int cube;  // -1 for start/goal.
  Coord coord;
  VI surface;
  Vertex() : cube(-1) {}
};

struct Surface {
  int cube;
  vector<pair<int, int> > neighbor;  // surface#, direction.
  VI vertex;  // in ccw order.   5th/6th entries are the start and the goal.
  Coord center;  // useful.
  Coord normal;  // useful.
  Coord dir[2];  // useful.
  Surface() : cube(-1) {}
};

class Solver {
public:
  Solver() {}
  ~Solver() {}
  double solve(const Problem& p);

  // Build internal structure based on the given set of cubes.
  void buildVerticesAndSurfaces(const vector<Coord>& cube);

  // Add a vertex located at the surface of the cubes.
  // Returns the id of the new vertex if succeeded.
  // Returns -1 if the vertex is not on any surfaces.
  int addVertexOnSurface(const Coord& position);

  // You should call this before calling netDistance.
  void setNetworkDistance();

  double netDistance(int start_id, int goal_id) const;

  bool canWalkNet(xy start, xy goal,
		  int surface_start, int surface_start_dir,
		  int surface_goal, int surface_goal_dir) const;

  double netDistanceWithFixedSurface(const xy& start_diff, const xy& goal_diff,
				     int surface_start, int surface_start_dir,
				     int surface_goal, int surface_goal_dir,
				     double sup) const;

  // Another utilities.
  // Checks if p is an appropriate problem.
  bool isProblem(const Problem& p);

private:
  Coord getGridShift(const vector<Coord>& cube);
  void cube2Grid(const vector<Coord>& cube, VVVI* gridp);
  void addVertices(const vector<Coord>& cube,
		   vector<Vertex>* vertexp);
  void addSurfaces(const vector<Coord>& cube, const VVVI& grid,
		   vector<Surface>* surfacep,
		   vector<Vertex>* vertexp);
  void addSurfaceRelations(const vector<Coord>& cube, const VVVI& grid,
			   const vector<Vertex>& vertex,
			   vector<Surface>* surfacep);
  void connectSurfaces(const vector<Vertex>& vertex,
		       vector<Surface>* surfacep,
		       int surface1, int surface2);
  void fillOutDistanceOnSurface(const vector<Surface>& surface,
				const vector<Vertex>& vertex,
				VVD* distancep);
  int addVertexOnSurfaceInternal(const Coord& position,
				 vector<Surface>* surfacep,
				 vector<Vertex>* vertexp,
				 VVD* distancep);

  double netDistanceBetweenCenters(int start_id, int goal_id,
				   double sup) const;
  double netDistanceFromCornerToCenter(int start_id, int goal_id,
				       double sup) const;
  double netDistanceBetweenCorners(int start_id, int goal_id,
				   double sup) const;
  
  void world2Grid(const Coord& p, Coord* q);
  int getGrid(const VVVI& grid, const Coord& p) {
    assert(0<=p.x);
    assert(p.x < grid.size());
    assert(0<=p.y);
    assert(p.y < grid[p.x].size());
    assert(0<=p.z);
    assert(p.z < grid[p.x][p.y].size());
    return grid[p.x][p.y][p.z];
  }

  VVVI grid_;
  Coord grid_shift_;
  vector<Vertex> vertex_;
  vector<Surface> surface_;
  VVD direct_distance_;
  VVD network_distance_;
};

const double EPS=1.0e-10;
#define EQ(x, y) (fabs((x)-(y)) < EPS)
#define NE(x, y) (!EQ(x, y))
#define LT(x, y) ((x) - (y) < -EPS)
#define GT(x, y) LT(y, x)
#define LE(x, y) (!GT(x, y))
#define GE(x, y) (!LT(x, y))

const int kLength = 100;
const double kInfty = 1000.0 * kLength;
const int kDir4X[4] = {1, 0, -1, 0};
const int kDir4Y[4] = {0, 1, 0, -1};
const int kDir8X[8] = {1, 1, 0, -1, -1, -1,  0,  1};
const int kDir8Y[8] = {0, 1, 1,  1,  0, -1, -1, -1};

void getMinCoordinateForGrid(const vector<Coord>& cube, Coord* result) {
  const int infty = kLength * (cube.size() + 2);
  int minx = infty;
  int miny = infty;
  int minz = infty;
  for (size_t i = 0; i < cube.size(); ++i) {
    minx = min(minx, cube[i].x - kLength/2);
    miny = min(miny, cube[i].y - kLength/2);
    minz = min(minz, cube[i].z - kLength/2);
  }
  *result = Coord(minx, miny, minz);
}

Coord Solver::getGridShift(const vector<Coord>& cube) {
  Coord shift;
  getMinCoordinateForGrid(cube, &shift);
  // place one line for sentinel.
  return -shift + Coord(kLength, kLength, kLength);
}

void Solver::world2Grid(const Coord& p, Coord* q) {
  *q = p + grid_shift_ - Coord(kLength/2, kLength/2, kLength/2);
  q->x /= kLength;
  q->y /= kLength;
  q->z /= kLength;
}

void Solver::cube2Grid(const vector<Coord>& cube, VVVI* gridp) {
  VVVI& grid = *gridp;
  const int size = cube.size() + 2;
  grid = VVVI(size, VVI(size, VI(size, -1)));
  for (int i = 0; i < cube.size(); ++i) {
    Coord pos;
    world2Grid(cube[i], &pos);
    assert(grid[pos.x][pos.y][pos.z] == -1);
    grid[pos.x][pos.y][pos.z] = i;
  }
}

Coord corner2CoordShift(int corner) {
  return (kLength / 2) *
    Coord((corner&1) * 2 - 1,
	  ((corner>>1)&1) * 2 - 1,
	  ((corner>>2)&1) * 2 - 1);
}

int coordShift2Corner(const Coord& p) {
  return ((p.x + 1) >> 1) |
         (((p.y + 1) >> 1) << 1) |
         (((p.z + 1) >> 1) << 2);
}

int normal2Surfaceid(const Coord& p) {
  int result = 0;
  int v = 0;
  if (p.x != 0) {
    result = 0;
    v = p.x;
  } else if (p.y != 0) {
    result = 2;
    v = p.y;
  } else if (p.z != 0) {
    result = 4;
    v = p.z;
  } else {
    assert(false);
  }
  return result + (v > 0 ? 1 : 0);
}

void Solver::addVertices(const vector<Coord>& cube, vector<Vertex>* vertexp) {
  vector<Vertex>& vertex = *vertexp;
  vertex = vector<Vertex>(cube.size() * 8);
  for (int cur = 0; cur < cube.size(); ++cur) {
    for (int corner = 0; corner < 8; ++corner) {
      Vertex& v = vertex[cur * 8 + corner];
      v.coord = cube[cur] + corner2CoordShift(corner);
      v.cube = cur;
    }
  }
}

void Solver::addSurfaces(const vector<Coord>& cube, const VVVI& grid,
			 vector<Surface>* surfacep,
			 vector<Vertex>* vertexp) {
  vector<Surface>& surface = *surfacep;
  vector<Vertex>& vertex = *vertexp;
  surface = vector<Surface>(cube.size() * 6);  // clear surface.
  for (int cur = 0; cur < cube.size(); ++cur) {
    Coord grid_coord;
    world2Grid(cube[cur], &grid_coord);
    for (int dir = 0; dir < 3; ++dir) {
      // dir: x, y, z
      for (int pm = 0; pm < 2; ++pm) {
	// -x, +x..
	const Coord unit = (pm * 2 - 1) * Coord::unitVector(dir);
	if (getGrid(grid, grid_coord + unit) >= 0)
	  continue;  // can't walk on this surface.
	int surface_id = cur * 6 + dir * 2 + pm;
	Surface& sur = surface[surface_id];
	sur.cube = cur;
	sur.normal = unit;
	sur.center = cube[cur] + (kLength / 2) * unit;
	sur.neighbor = vector<pair<int, int> >(4, make_pair(-1, -1));

	const int dir2 = (dir + 1) % 3;
	const Coord unit2 = Coord::unitVector(dir2 % 3);
	const Coord unit3 = unit.outerProduct(unit2);
	sur.dir[0] = unit2;
	sur.dir[1] = unit3;
	// Set the vertices on the surface.
	// The vertices should align in counter-clockwise.
	// 2__1
	// |__|
	// 3  0
	for (int d2 = 0; d2 < 4; ++d2) {
	  const Coord to_corner = unit +
                                  kDir8X[(d2 * 2 + 7) & 7] * unit2 +
                                  kDir8Y[(d2 * 2 + 7) & 7] * unit3;
	  const int vertex_id = cur * 8 + coordShift2Corner(to_corner);
	  sur.vertex.push_back(vertex_id);
	  vertex[vertex_id].surface.push_back(surface_id);
	}
      }
    }
  }
}

void Solver::connectSurfaces(const vector<Vertex>& vertex,
			     vector<Surface>* surfacep,
			     int surface1, int surface2) {
  Surface& s1 = surfacep->at(surface1);
  Surface& s2 = surfacep->at(surface2);
  // dir1=0, dir2=0 => s1[0] = s2[3], s1[1] = s2[2]
  // dir1=1, dir2=0 => s1[1] = s2[0], s1[2] = s2[3]
  // dir1=2, dir2=0 => s1[2] = s2[1], s1[3] = s2[0]
  // dir1=0, dir2=1 => s1[0] = s2[2], s1[1] = s2[1]
  // dir1=0, dir2=2 => s1[0] = s2[1], s1[1] = s2[0]
  // dir1=1, dir2=1 => s1[1] = s2[3], s1[2] = s2[2]
  for (int dir1 = 0; dir1 < 4; ++dir1) {
    for (int dir2 = 0; dir2 < 4; ++dir2) {
      const int index2 = ((3 + dir1 - dir2) + 4) & 3;
      if (vertex[s1.vertex[dir1]].coord == vertex[s2.vertex[index2]].coord &&
	  vertex[s1.vertex[(dir1 + 1) & 3]].coord ==
	  vertex[s2.vertex[(index2 + 3) & 3]].coord) {
	assert(s1.neighbor[dir1].first < 0);
	s1.neighbor[dir1] = make_pair(surface2, dir2);
	return;
      }
    }
  }
  assert(false);
}

void Solver::addSurfaceRelations(const vector<Coord>& cube, const VVVI& grid,
				 const vector<Vertex>& vertex,
				 vector<Surface>* surfacep) {
  vector<Surface>& surface = *surfacep;
  for (int cur = 0; cur < cube.size(); ++cur) {
    Coord grid_coord;
    world2Grid(cube[cur], &grid_coord);
    for (int dir = 0; dir < 3; ++dir) {
      // dir: x, y, z
      for (int pm = 0; pm < 2; ++pm) {
	// -x, +x..
	const Coord unit = (pm * 2 - 1) * Coord::unitVector(dir);
	if (getGrid(grid, grid_coord + unit) >= 0)
	  continue;  // can't walk on this surface.
	int surface_id = cur * 6 + dir * 2 + pm;
	Surface& sur = surface[surface_id];

	const int dir2 = (dir + 1) % 3;
	const Coord unit2 = Coord::unitVector(dir2 % 3);
	const Coord unit3 = unit.outerProduct(unit2);

	// Add surfaces, in clockwise order.
	for (int d2 = 0; d2 < 4; ++d2) {
	  const Coord unit_d2 = kDir4X[d2] * unit2  + kDir4Y[d2] * unit3;
	  int next_surface_id;
	  if (getGrid(grid, grid_coord + unit_d2) < 0) {
	    // edge on the same cube across different surfaces.
	    next_surface_id = cur * 6 +
                              normal2Surfaceid(unit_d2);
	  } else if (getGrid(grid, grid_coord + unit_d2 + unit) < 0) {
	    // edge between different cubes, straight.
	    next_surface_id = getGrid(grid, grid_coord + unit_d2) * 6 +
	                      dir * 2 + pm;
	  } else {
	    // edge between different cubes, L-shape.
	    next_surface_id = getGrid(grid, grid_coord + unit_d2 + unit) * 6 +
	      normal2Surfaceid(-unit_d2);
	  }
	  connectSurfaces(vertex, surfacep, surface_id, next_surface_id);
	}
	// Ensure that surface_id has for neighbors.
	for (int i = 0; i < 4; ++i) {
	  assert(sur.neighbor[i].first >= 0);
	}
      }
    }
  }
}

void Solver::fillOutDistanceOnSurface(const vector<Surface>& surface,
					const vector<Vertex>& vertex,
					VVD* distancep) {
  VVD& distance = *distancep;
  for (int surface_id = 0; surface_id < surface.size(); ++surface_id) {
    if (surface[surface_id].cube < 0)  // This surface is not used.
      continue;
    const Surface& sur = surface[surface_id];
    // Set distance on the surface.
    for (int i = 0; i < 4; ++i) {
      for (int j = i; j < 4; ++j) {
	const int v1_id = sur.vertex[i];
	const int v2_id = sur.vertex[j];
	distance[v1_id][v2_id] = distance[v2_id][v1_id] =
	  sqrt(double ((vertex[v1_id].coord - vertex[v2_id].coord).norm()));
      }
    }
    // connect distances.
    for (int i = 0; i < 4; ++i) {
      const int neighbor_id = sur.neighbor[i].first;
      const Surface& neighbor = surface[neighbor_id];
      for (int j = 0; j < 4; ++j) {
	const int v1_id = sur.vertex[j];
	for (int k =0; k < 4; ++k) {
	  const int v2_id = neighbor.vertex[k];
	  if ((vertex[v1_id].coord - vertex[v2_id].coord).norm() == 0) {
	    distance[v1_id][v2_id] = distance[v2_id][v1_id] = 0;
	  }
	}
      }
    }
  }
}

int Solver::addVertexOnSurfaceInternal(const Coord& position,
				       vector<Surface>* surfacep,
				       vector<Vertex>* vertexp,
				       VVD* distancep) {
  vector<Surface>& surface = *surfacep;
  vector<Vertex>& vertex = *vertexp;
  VVD& distance = *distancep;
  if (distance.size() <= vertex.size()) {
    // Need to extend distance.
    const int old_size = distance.size();
    const int new_size = old_size + old_size / 2;
    for (int i = 0; i < old_size; ++i) {
      distance[i].resize(new_size);
      for (int j = old_size; j < new_size; ++j) {
	distance[i][j] = kInfty;
      }
    }
    distance.resize(new_size);
    for (int i = old_size; i < new_size; ++i) {
      distance[i] = VD(new_size, kInfty);
      distance[i][i] = 0;
    }
  }
  vertex.push_back(Vertex());
  Vertex& v = vertex.back();
  v.coord = position;
  const int result = vertex.size() - 1;

  for (int surface_id = 0; surface_id < surface.size(); ++surface_id) {
    if (surface[surface_id].cube < 0)  // This surface is not used.
      continue;
    Surface& sur = surface[surface_id];
    if (sur.normal.innerProduct(v.coord - sur.center) != 0)
      continue;  // not on the same surface.
    bool inside = true;
    for (int i = 0; i < 4; ++i) {
      if ((vertex[sur.vertex[i]].coord - v.coord).
	  outerProduct(vertex[sur.vertex[(i + 1) & 3]].coord - v.coord).
	  innerProduct(sur.normal) <= 0) {
	inside = false;
	break;
      }
    }
    if (!inside)
      continue;
    v.surface.push_back(surface_id);
    sur.vertex.push_back(result);
    for (int i = 0; i < sur.vertex.size(); ++i) {
      const int corner_id = sur.vertex[i];
      distance[result][corner_id] = distance[corner_id][result] =
	sqrt(double ((v.coord - vertex[corner_id].coord).norm()));
    }
    return result;
  }
  // When the vertex is not on any surface.
  return -1;
}

void floydWarshall(const VVD& distance1, VVD* far_distancep) {
  VVD& dest = *far_distancep;
  dest = distance1;  // copy
  for (int i = 0; i < dest.size(); ++i)
    dest[i][i] = 0;
  for (int k = 0; k < dest.size(); ++k) {
    for (int i = 0; i < dest.size(); ++i) {
      for (int j = 0; j < dest.size(); ++j) {
	dest[i][j] = min(dest[i][j], dest[i][k] + dest[k][j]);
      }
    }
  }
}

// returns true if you can walk.
bool Solver::canWalkNet(xy start, xy goal,
			int surface_start, int surface_start_dir,
			int surface_goal, int surface_goal_dir) const {
  int tile_goal_x = static_cast<int>(floor(real(goal))) / kLength;
  int tile_goal_y = static_cast<int>(floor(imag(goal))) / kLength;
  if (static_cast<int>(floor(real(goal))) % kLength == 0) {
    assert(static_cast<int>(floor(imag(goal))) % kLength == 0);
    --tile_goal_x;
    --tile_goal_y;
  }
  vector<pair<double, int> > todo;
  for (int i = 0; i < tile_goal_x; ++i) {
    const double t = ((i + 1) * kLength - real(start)) / real(goal - start);
    todo.push_back(make_pair(t, 0));
  }
  for (int i = 0; i < tile_goal_y; ++i) {
    const double t = ((i + 1) * kLength - imag(start)) / imag(goal - start);
    todo.push_back(make_pair(t, 1));
  }
  sort(todo.begin(), todo.end());
  // Checks if it passes lattice points.
  for (int i = 1; i < todo.size(); ++i) {
    if (EQ(todo[i].first, todo[i - 1].first))
      return false;
  }
  int sur = surface_start;
  int dir = surface_start_dir;
  for (int i = 0; i < todo.size(); ++i) {
    if (todo[i].second == 0) {  // go to +x
      // dir = 0 => neighbor[0] with dir := dir
      // dir = 1 => neighbor[3] with dir += 1
      // dir = 2 => neighbor[2] with dir += 2
      const pair<int, int>& next = surface_[sur].neighbor[(4 - dir)& 3];
      sur = next.first;
      dir = (dir + next.second) & 3;
    } else {  // go to +y
      // dir = 0 => neighbor[1] with dir := dir
      // dir = 1 => neighbor[0] with dir += 1
      const pair<int, int>& next = surface_[sur].neighbor[(5 - dir) & 3];
      sur = next.first;
      dir = (dir + next.second) & 3;
    }
  }
  return sur == surface_goal && dir == surface_goal_dir;
}

double Solver::netDistanceWithFixedSurface(
                   const xy& start_diff, const xy& goal_diff,
		   int surface_start, int surface_start_dir,
		   int surface_goal, int surface_goal_dir,
		   double sup) const {
  double result = sup;
  for (int y = 0;; ++y) {
    // The shortest distance can be x == 0 and x == 1.
    // To take the estimation of shortest distance, we only take
    // the differences of y coordinate.
    if (LT(result,
	   abs(imag(goal_diff - start_diff) + kLength * y))) {
      if (y == 0)
	continue;
      else
	return result;
    }
    for (int x = 0;; ++x) {
      double new_length = abs(goal_diff - start_diff +
			      xy(kLength * x, kLength * y));
      if (LT(result, new_length)) {
	if (x == 0)
	  continue;  // Try  ==1 as well because x==0 can be farther than x==1. 
	else
	  break;
      }
      if (canWalkNet(start_diff,
		     goal_diff + xy(kLength * x, kLength * y),
		     surface_start, surface_start_dir,
		     surface_goal, surface_goal_dir)) {
	result = new_length;
	break;
      }
    }
  }
}

xy relativePosition(const Surface& surface, const Vertex& vertex) {
  Coord diff = vertex.coord - surface.center;
  return xy(surface.dir[0].innerProduct(diff),
	    surface.dir[1].innerProduct(diff));
}

int relativeDir(const Surface& surface, const int vertex_id) {
  for (int i = 0; i < 4; ++i) {
    if (surface.vertex[i] == vertex_id)
      return i;
  }
  assert(false);
  return -1;
}

double Solver::netDistance(int start_id, int goal_id) const {
  double sup = network_distance_[start_id][goal_id];
  double result;
  if (vertex_[start_id].cube < 0) {  // start is not on corner
    if (vertex_[goal_id].cube < 0)  // neigher start nor goal are on corner.
      result = netDistanceBetweenCenters(start_id, goal_id, sup);
    else
      result = netDistanceFromCornerToCenter(goal_id, start_id, sup);
  } else {
    if (vertex_[goal_id].cube < 0)
      result = netDistanceFromCornerToCenter(start_id, goal_id, sup);
    else
      result = netDistanceBetweenCorners(start_id, goal_id, sup);
  }
  return result;
}

double Solver::netDistanceBetweenCenters(int start_id, int goal_id,
					 double sup) const {
  const Vertex& start = vertex_[start_id];
  const Vertex& goal = vertex_[goal_id];
  double result = sup;
  assert(start.surface.size() == 1);
  assert(goal.surface.size() == 1);
  xy start_diff = relativePosition(surface_[start.surface.front()], start);
  for (int start_dir = 0; start_dir < 4; ++start_dir) {
    xy goal_diff = relativePosition(surface_[goal.surface.front()], goal);
    for (int goal_dir = 0; goal_dir < 4; ++goal_dir) {
      result = netDistanceWithFixedSurface(
		   start_diff + xy(kLength / 2, kLength / 2),
		   goal_diff + xy(kLength / 2, kLength / 2),
		   start.surface.front(), start_dir,
		   goal.surface.front(), goal_dir, result);
      goal_diff *= xy(0, 1);
    }
    start_diff *= xy(0, 1);
  }
  return result;
}

double Solver::netDistanceFromCornerToCenter(int start_id, int goal_id,
					     double sup) const {
  const Vertex& start = vertex_[start_id];
  const Vertex& goal = vertex_[goal_id];
  double result = sup;
  assert(goal.surface.size() == 1);
  for (int surf1_id = 0; surf1_id < start.surface.size(); ++surf1_id) {
    const Surface& surf1 = surface_[start.surface[surf1_id]];
    int start_dir = 3 - relativeDir(surf1, start_id);  // 3:0, 2:1, 1:2
    xy goal_diff = relativePosition(surface_[goal.surface.front()], goal);
    for (int goal_dir = 0; goal_dir < 4; ++goal_dir) {
      result = netDistanceWithFixedSurface(
		   xy(0, 0),
		   goal_diff + xy(kLength / 2, kLength / 2),
		   start.surface[surf1_id], start_dir,
		   goal.surface.front(), goal_dir, result);
      goal_diff *= xy(0, 1);
    }
  }
  return result;
}

double Solver::netDistanceBetweenCorners(int start_id, int goal_id,
					 double sup) const {
  const Vertex& start = vertex_[start_id];
  const Vertex& goal = vertex_[goal_id];
  double result = sup;
  for (int surf1_id = 0; surf1_id < start.surface.size(); ++surf1_id) {
    const Surface& surf1 = surface_[start.surface[surf1_id]];
    int start_dir = 3 - relativeDir(surf1, start_id);  // 3:0, 2:1, 1:2
    for (int surf2_id = 0; surf2_id < goal.surface.size(); ++surf2_id) {
      const Surface surf2 = surface_[goal.surface[surf2_id]];
      int goal_dir = (5 - relativeDir(surf2, goal_id)) & 3; // 0:1, 1:0, 2:3
      result = netDistanceWithFixedSurface(
		   xy(0, 0), xy(kLength, kLength),
		   start.surface[surf1_id], start_dir,
		   goal.surface[surf2_id], goal_dir, result);

    }
  }
  return result;
}

void Solver::buildVerticesAndSurfaces(const vector<Coord>& cube) {
  grid_shift_ = getGridShift(cube);
  cube2Grid(cube, &grid_);
  addVertices(cube, &vertex_);
  addSurfaces(cube, grid_, &surface_, &vertex_);
  addSurfaceRelations(cube, grid_, vertex_, &surface_);
  const int distance_size = vertex_.size() + 2 + vertex_.size() / 2;
  direct_distance_ = VVD(distance_size, VD(distance_size, kInfty));
  fillOutDistanceOnSurface(surface_, vertex_, &direct_distance_);
}

int Solver::addVertexOnSurface(const Coord& position) {
  return addVertexOnSurfaceInternal(position, &surface_, &vertex_,
				    &direct_distance_);
}

void Solver::setNetworkDistance() {
  floydWarshall(direct_distance_, &network_distance_);
}


double Solver::solve(const Problem& p_original) {
  Problem prob(p_original);
  buildVerticesAndSurfaces(prob.cube);
  const int start = addVertexOnSurface(prob.start);
  const int goal  = addVertexOnSurface(prob.goal);
  assert(start >= 0);
  assert(goal >= 0);
  setNetworkDistance();
  for (int i = 0; i < vertex_.size(); ++i) {
    for (int j = i + 1; j < vertex_.size(); ++j) {
      direct_distance_[i][j] = direct_distance_[j][i]
	= netDistance(i, j);
    }
  }
  setNetworkDistance();
  return network_distance_[start][goal];
}

bool Solver::isProblem(const Problem& p) {
  buildVerticesAndSurfaces(p.cube);
  if (addVertexOnSurface(p.start) < 0 || addVertexOnSurface(p.goal) < 0) {
    return false;
  }
  setNetworkDistance();
  for (int i = 0; i < vertex_.size(); ++i) {
    for (int j = i + 1; j < vertex_.size(); ++j) {
      if (netDistance(i, j) >= p.cube.size() * 3. * kLength)
	return false;  // not connected.
    }
  }
  return true;
}

bool readProblem(Problem* p) {
  int n;
  p->clear();
  cin >> n;
  if (n == 0)
    return false;
  for (int i = 0; i < n; ++i) {
    int x, y, z;
    cin >> x >> y >> z;
    p->cube.push_back(Coord(x, y, z));
  }
  int x, y, z;
  cin >> x >> y >> z;
  p->start = Coord(x, y, z);
  cin >> x >> y >> z;
  p->goal = Coord(x, y, z);
  return true;
}

int main(void) {
	freopen ("j.in", "r", stdin);
	freopen ("j.out", "w", stdout);
  Problem p;
  cout.setf(ios::fixed);
  cout << setprecision(10);
  readProblem(&p);
  Solver s;
  cout << s.solve(p) << endl;
}
