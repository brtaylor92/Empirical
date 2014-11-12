#ifndef EMP_PHYSICS_2D_H
#define EMP_PHYSICS_2D_H

///////////////////////////////////////////////////////////////////////////////////////////////
//
//  Physics2D - handles movement and collissions in a simple 2D world.
//

#include <vector>
#include <unordered_set>

#include "Surface2D.h"

namespace emp {

  template <typename BODY_TYPE, typename BODY_INFO, typename BASE_TYPE> class Physics2D {
  private:
    Surface2D<BODY_TYPE, BODY_INFO, BASE_TYPE> passive_surface; // Contains bodies only external acted upon
    Surface2D<BODY_TYPE, BODY_INFO, BASE_TYPE> active_surface;  // Contains bodies that can initate actions

  public:
    Physics2D(BASE_TYPE width, BASE_TYPE height, BASE_TYPE max_org_diameter=100) 
      : passive_surface(width, height, width / max_org_diameter, height / max_org_diameter)
      , active_surface(width, height, width / max_org_diameter, height / max_org_diameter)
    { ; }
    ~Physics2D() { ; }

    Physics2D & AddActiveBody(BODY_TYPE * in_body) { active_surface.AddBody(in_body); return *this; }
    Physics2D & AddPassiveBody(BODY_TYPE * in_body) { passive_surface.AddBody(in_body); return *this; }

    const std::unordered_set<BODY_TYPE *> & GetActiveBodySet() const {
      return active_surface.GetBodySet();
    }
    const std::unordered_set<BODY_TYPE *> & GetPassiveBodySet() const {
      return passive_surface.GetBodySet();
    }
  };

};

#endif
