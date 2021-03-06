#ifndef EMP_PHYSICS_2D_H
#define EMP_PHYSICS_2D_H

///////////////////////////////////////////////////////////////////////////////////////////////
//
//  Physics2D - handles movement and collissions in a simple 2D world.
//

#include <vector>
#include <unordered_set>
#include <functional>
using namespace std::placeholders;

#include "Surface2D.h"

namespace emp {

  template <typename BODY_TYPE, typename BODY_INFO, typename BASE_TYPE> class Physics2D {
  private:
    Surface2D<BODY_TYPE, BODY_INFO, BASE_TYPE> surface;    // Contains bodies that can collide.
    Surface2D<BODY_TYPE, BODY_INFO, BASE_TYPE> background; // Contains bodies that can't collide.

  public:
    Physics2D(BASE_TYPE width, BASE_TYPE height, BASE_TYPE max_org_diameter=20) 
      : surface(width, height)
      , background(width, height)
    { ; }
    ~Physics2D() { ; }

    Physics2D & AddBody(BODY_TYPE * in_body) { surface.AddBody(in_body); return *this; }
    Physics2D & AddBackground(BODY_TYPE * in_body) { background.AddBody(in_body); return *this; }

    bool TestPairCollision(BODY_TYPE & body1, BODY_TYPE & body2) {
      if (body1.IsLinked(body2)) return false;  // Linked bodies can overlap.

      const Point<BASE_TYPE> dist = body1.GetCenter() - body2.GetCenter();
      const BASE_TYPE sq_pair_dist = dist.SquareMagnitude();
      const BASE_TYPE radius_sum = body1.GetRadius() + body2.GetRadius();
      const BASE_TYPE sq_min_dist = radius_sum * radius_sum;

      // If there was no collision, return false.
      if (sq_pair_dist >= sq_min_dist) { return false; }

      if (sq_pair_dist == 0.0) {
        // If the shapes are on top of each other, we have a problem.  Shift one!
        body2.Translate(emp::Point<BASE_TYPE>(0.01, 0.01));
      }

      // @CAO If objects can phase or explode, identify that here.
      
      // Re-adjust position to remove overlap.
      const double true_dist = sqrt(sq_pair_dist);
      const double overlap_dist = ((double) radius_sum) - true_dist;
      const double overlap_frac = overlap_dist / true_dist;
      const Point<BASE_TYPE> cur_shift = dist * (overlap_frac / 2.0);
      body1.AddShift(cur_shift, overlap_dist);
      body2.AddShift(-cur_shift, overlap_dist);

      // @CAO if we have inelastic collisions, we just take the weighted average of velocites
      // and let the move together.
      
      // Assume elastic: Re-adjust velocity to reflect bounce.
      double x1, y1, x2, y2;
  
      if (dist.GetX() == 0) {
        x1 = body1.GetVelocity().GetX();  y1 = body2.GetVelocity().GetY();
        x2 = body2.GetVelocity().GetX();  y2 = body1.GetVelocity().GetY();

        body1.SetVelocity(Point<BASE_TYPE>(x1, y1));
        body2.SetVelocity(Point<BASE_TYPE>(x2, y2));
      }
      else if (dist.GetY() == 0) {
        x1 = body2.GetVelocity().GetX();  y1 = body1.GetVelocity().GetY();
        x2 = body1.GetVelocity().GetX();  y2 = body2.GetVelocity().GetY();

        body1.SetVelocity(Point<BASE_TYPE>(x1, y1));
        body2.SetVelocity(Point<BASE_TYPE>(x2, y2));
      }
      else {
        const Point<BASE_TYPE> rel_velocity(body2.GetVelocity() - body1.GetVelocity());
        double normal_a = dist.GetY() / dist.GetX();
        x1 = ( rel_velocity.GetX() + normal_a * rel_velocity.GetY() )
          / ( normal_a * normal_a + 1 );
        y1 = normal_a * x1;
        x2 = rel_velocity.GetX() - x1;
        y2 = - (1 / normal_a) * x2;

        body2.SetVelocity(body1.GetVelocity() + Point<BASE_TYPE>(x2, y2));
        body1.SetVelocity(body1.GetVelocity() + Point<BASE_TYPE>(x1, y1));
      }
  

      return true;      
    }

    void Update() {
      // Handle movement of bodies
      auto & body_set = surface.GetBodySet();
      for (BODY_TYPE * cur_body : body_set) {
        cur_body->BodyUpdate(0.5);   // Let a body change size or shape, as needed.
        cur_body->ProcessStep(0.0125);  // Update position and velocity.
      }

      // Handle collisions
      auto collide_fun = std::bind(&Physics2D::TestPairCollision, this, _1, _2);
      surface.TestCollisions(collide_fun);

      // Determine which bodies we should remove.
      int cur_size = (int) body_set.size();
      for (int i = 0; i < cur_size; i++) {
        const double cur_pressure = body_set[i]->CalcPressure();

        // @CAO Arbitrary pressure threshold!
        if (cur_pressure > 2.0) {
          static bool popped = false;
          if (popped == false) {
            popped = true;
            emp::Alert("First Pop! Num orgs=", body_set.size());
          }

          // Too much pressure!  We need to burst this cell.
          delete body_set[i];
          cur_size--;
          body_set[i] = body_set[cur_size];
          --i;
        }
      }
      body_set.resize(cur_size);
    }

    // Access to bodies
    std::vector<BODY_TYPE *> & GetBodySet() {
      return surface.GetBodySet();
    }
    std::vector<BODY_TYPE *> & GetBackgroundSet() {
      return background.GetBodySet();
    }

    // Access to bodies in a const physics...
    const std::vector<BODY_TYPE *> & GetConstBodySet() const {
      return surface.GetConstBodySet();
    }
    const std::vector<BODY_TYPE *> & GetConstBackgroundSet() const {
      return background.GetConstBodySet();
    }
  };

};

#endif
