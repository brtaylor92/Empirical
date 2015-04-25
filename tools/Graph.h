#ifndef EMP_GRAPH_H
#define EMP_GRAPH_H

//////////////////////////////////////////////////////////////////////////////////////////
//
//  A simple, fast graph class
//

#include <assert.h>
#include <iostream>
#include <vector>

#include "BitVector.h"

namespace emp {

  class Graph {
  public:
    class Node {
    private:
      BitVector edge_set;
    public:
      Node(int32_t num_nodes) : edge_set(num_nodes) { ; }
      Node(const Node & in_node) : edge_set(in_node.edge_set) { ; }
      ~Node() { ; }

      Node & operator=(const Node & in_node) { edge_set = in_node.edge_set; return *this; }

      bool HasEdge(int32_t to) const { return edge_set[to]; }
      void AddEdge(int32_t to) { edge_set.Set(to, true); }
      void RemoveEdge(int32_t to) { edge_set.Set(to, false); }
      void SetEdge(int32_t to, bool val) { edge_set.Set(to, val); }
      const BitVector & GetEdgeSet() const { return edge_set; }

      int32_t GetDegree() const { return edge_set.CountOnes(); }
      int32_t GetMaskedDegree(const BitVector & mask) const { return (mask & edge_set).CountOnes(); }
    };

  private:
    std::vector<Node> nodes;

  public:
    Graph(int32_t num_nodes=0) : nodes(num_nodes, num_nodes) { ; }
    Graph(const Graph & in_graph) : nodes(in_graph.nodes) { ; }
    ~Graph() { ; }

    Graph & operator=(const Graph & in_graph) { nodes = in_graph.nodes; return *this; }

    int32_t GetSize() const { return (int32_t) nodes.size(); }
    int32_t GetEdgeCount() const {
      int32_t edge_count = 0;
      for (int32_t i = 0; i < (int32_t) nodes.size(); i++) edge_count += nodes[i].GetDegree();
      return edge_count;
    }

    const BitVector & GetEdgeSet(int32_t id) const {
      assert(id >= 0 && id < (int32_t) nodes.size());
      return nodes[id].GetEdgeSet();
    }
    int32_t GetDegree(int32_t id) const {
      assert(id >= 0 && id < (int32_t) nodes.size());
      return nodes[id].GetDegree();
    }
    int32_t GetMaskedDegree(int32_t id, const BitVector & mask) const {
      assert(id >= 0 && id < (int32_t) nodes.size());
      return nodes[id].GetMaskedDegree(mask);
    }


    bool HasEdge(int32_t from, int32_t to) const {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t)nodes.size());
      return nodes[from].HasEdge(to);
    }
    void AddEdge(int32_t from, int32_t to) {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      nodes[from].AddEdge(to);
    }
    void RemoveEdge(int32_t from, int32_t to) {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      nodes[from].RemoveEdge(to);
    }
    void SetEdge(int32_t from, int32_t to, bool val) {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      nodes[from].SetEdge(to, val);
    }


    bool HasEdgePair(int32_t from, int32_t to) const {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      return nodes[from].HasEdge(to) && nodes[to].HasEdge(from);
    }
    void AddEdgePair(int32_t from, int32_t to) {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      nodes[from].AddEdge(to);
      nodes[to].AddEdge(from);
    }
    void RemoveEdgePair(int32_t from, int32_t to) {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      nodes[from].RemoveEdge(to);
      nodes[to].RemoveEdge(from);
    }
    void SetEdgePairs(int32_t from, int32_t to, bool val) {
      assert(from >= 0 && from < (int32_t) nodes.size() && to >= 0 && to < (int32_t) nodes.size());
      nodes[from].SetEdge(to, val);
      nodes[to].SetEdge(from, val);
    }


    void Print32_tSym(std::ostream & os=std::cout) {
      os << GetSize() << " " << (GetEdgeCount()/2) << std::endl;
      for (int32_t from = 0; from < (int32_t) nodes.size(); from++) {
        for (int32_t to=from+1; to < (int32_t) nodes.size(); to++) {
          if (HasEdge(from, to) == false) continue;
          os << from << " " << to << std::endl;
        }
      }
    }

  };

}

#endif
