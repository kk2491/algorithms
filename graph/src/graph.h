//
// Created by jun on 6/6/17.
//

#ifndef CONTRACTION_GRAPH_H
#define CONTRACTION_GRAPH_H


#include <iostream>
#include <list>
#include <vector>

//
// adjacency list node
//
struct AdjListNode {
  int value;
  AdjListNode* next;
};

//
// adjacency list, only the head node is required here
//
struct AdjList {
  AdjListNode* head;
};

class Graph {
private:

  int n_vertex_;  // number of vertices
  int n_edge_;  // number of edges
  AdjList* adj_list_;  // an array of adjacency list

  void validateVertex(int v);

public:

  //
  // constructor
  //
  Graph(int n);

  //
  // destructor
  //
  ~Graph();

  //
  // add an edge with two vertices a and b
  //
  void addEdge(int a, int b);

  //
  // delete node b in the linked list for a
  //
  void delEdgeNode(int a, int b);

  //
  // delete an edge with two vertices a and b
  //
  void delEdge(int a, int b);

  //
  //
  //
  void contract(int a, int b);

  //
  // delete all the edges connected to vertex b
  //
  void delVertexEdge(int v);

  //
  // print the graph
  //
  void printGraph();

};


#endif //CONTRACTION_GRAPH_H
