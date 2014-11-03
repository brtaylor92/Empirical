#ifndef EMP_POINT_2D_H
#define EMP_POINT_2D_H

#include <cmath>

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  A simple class to track points in 2-Dimensional space.
//

namespace emp {

  template <typename TYPE> class Point {
  private:
    TYPE x;
    TYPE y;
    // @CAO store magnitude?  Lazy eval?

  public:
    Point() : x(0.0), y(0.0) { ; }                      // Default = 0,0
    Point(const Point & _in) : x(_in.x), y(_in.y) { ; } // Copy constructor
    Point(TYPE _x, TYPE _y) : x(_x), y(_y) { ; }        // Two ints -> x and y
    Point(const Point & _in, TYPE new_magnitude) {
      const double mag_shift = ((double) new_magnitude) / ((double) _in.Magnitude());
      x = _in.x * mag_shift;
      y = _in.y * mag_shift;
    }
    ~Point() { ; }

    const Point & operator=(const Point & _in) { x = _in.x; y = _in.y; return *this; }

    inline TYPE GetX() const { return x; }
    inline TYPE GetY() const { return y; }

    double Magnitude() const { return sqrt( x*x + y*y ); }
    bool AtOrigin() { return x == 0 && y == 0; }
    bool NonZero() { return x != 0 || y != 0; }

    // Determine a new point, but don't change this one
    Point GetMidpoint(const Point & p2) const { return Point((x+p2.x)/2, (y+p2.y)/2); }
    Point GetRot90() const { return Point(y, -x); }
    Point GetRot180() const { return Point(-x, -y); }
    Point GetRot270() const { return Point(-y, x); }

    Point operator+(const Point & _in) const { return Point(x + _in.x , y + _in.y); }
    Point operator-(const Point & _in) const { return Point(x - _in.x , y - _in.y); }
    Point operator*(double mult) const { return Point(((double) x) * mult, ((double) y) * mult); }
    Point operator/(double div) const { return Point(((double) x) / div, ((double) y) / div); }
    Point operator*(int mult) const { return Point(x * mult, y * mult); }
    Point operator/(int div) const { return Point(x / div, y / div); }

    // Modify this point.
    Point & Set(TYPE _x, TYPE _y) { x=_x; y=_y; return *this; }
    Point & Translate(TYPE shift_x, TYPE shift_y) { x += shift_x; y += shift_y; return *this; }
    Point & TranslateX(TYPE shift) { x += shift; return *this; }
    Point & TranslateY(TYPE shift) { y += shift; return *this; }
    Point & ToOrigin() { x = 0; y = 0; return *this; }
    Point & NegateX() { x = -x; return *this; }
    Point & NegateY() { y = -y; return *this; }
    
    Point & operator+=(const Point & _in) { x += _in.x; y += _in.y; return *this; }
    Point & operator-=(const Point & _in) { x -= _in.x; y -= _in.y; return *this; }
    Point & operator*=(double mult) { x *= mult; y *= mult; return *this; }
    Point & operator/=(double val) { if (val != 0.0) { x /= val;  y /= val; }; return *this; }
    Point & operator*=(int mult) { x *= mult; y *= mult; return *this; }
    Point & operator/=(int val) { if (val != 0.0) { x /= val;  y /= val; }; return *this; }

    Point & Rot90() { return Set(y, -x); }
    Point & Rot180() { return Set(-x, -y); }
    Point & Rot270() { return Set(-y, x); }

    // Square-roots are slow to calculate; if we're just doing comparisons, square-distance
    // is usualy going to be sufficient.
    TYPE SquareDistance(const Point & _in) const {
      const TYPE x_dist = x - _in.x;
      const TYPE y_dist = y - _in.y;
      return x_dist*x_dist + y_dist*y_dist;
    }

    TYPE Distance(const Point & _in) { return sqrt( SquareDistance(_in) ); }
  };

};

#endif