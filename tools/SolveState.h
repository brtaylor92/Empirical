#ifndef EMP_SOLVE_STATE_H
#define EMP_SOLVE_STATE_H

//////////////////////////////////////////////////////////////////////////////////////////
//
//  SolveState is used as part of a branching solver to keep track of the current state.
//

#include "BitVector.h"

namespace emp {

  class SolveState {
  private:
    BitVector in_vector;   // Objects included for sure
    BitVector unk_vector;  // Objects yet to be decided on

  public:
    SolveState(int32_t state_size=0) : in_vector(state_size), unk_vector(state_size) {
      unk_vector.SetAll();
    }
    SolveState(const SolveState & in) : in_vector(in.in_vector), unk_vector(in.unk_vector) { ; }
    ~SolveState() { ; }

    SolveState & operator=(const SolveState & in) {
      in_vector = in.in_vector;
      unk_vector = in.unk_vector;
      return *this;
    }

    int32_t GetSize() const { return in_vector.GetSize(); }

    bool IsIn(int32_t id) const { return in_vector[id]; }
    bool IsUnk(int32_t id) const { return unk_vector[id]; }
    bool IsOut(int32_t id) const { return !(IsIn(id) | IsUnk(id)); }

    bool IsFinal() const { return unk_vector.None(); }

    int32_t CountIn() const { return in_vector.CountOnes(); }
    int32_t CountUnk() const { return unk_vector.CountOnes(); }
    int32_t CountOut() const { return in_vector.GetSize() - CountIn() - CountUnk(); }

    const BitVector & GetInVector() const { return in_vector; }
    const BitVector & GetUnkVector() const { return unk_vector; }
    BitVector GetOutVector() const { return ~in_vector & ~unk_vector; }
    
    int32_t GetNextUnk(int32_t prev_unk) const {
      return unk_vector.FindBit(prev_unk+1);
    }

    void Include(int32_t id) {
      assert(id >= 0 && id < (int32_t) in_vector.size());
      unk_vector.Set(id, false);
      in_vector.Set(id, true);
    }

    void Exclude(int32_t id) {
      assert(id >= 0 && id < (int32_t) in_vector.size());
      unk_vector.Set(id, false);
    }

    // Change our mind about a potentially included node
    void ForceExclude(int32_t id) {
      unk_vector.Set(id, false);
      in_vector.Set(id, false);
    }
    
    void IncludeSet(const BitVector & inc_set) {
      assert(inc_set.GetSize() == in_vector.GetSize());
      in_vector |= inc_set;
      unk_vector &= ~inc_set;
    }

    void ExcludeSet(const BitVector & inc_set) {
      assert(inc_set.GetSize() == in_vector.GetSize());
      unk_vector &= ~inc_set;
    }
  };

};

#endif
