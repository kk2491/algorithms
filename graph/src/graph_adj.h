//
// Created by jun on 6/6/17.
//
// implementation of the directed and undirected graph using adjacency list
//

#ifndef GRAPH_GRAPH_ADJ_H
#define GRAPH_GRAPH_ADJ_H


#include <iostream>
#include <list>
#include <vector>

namespace graph {
  //
  // node in a linked list
  //
  struct AdjListNode {
    int value;  // value of the node
    int weight;  // weight of the node
    AdjListNode* next;  // pointer to the next node
  };

  //
  // head of a linked list
  //
  struct AdjListHead {
    bool visited;  // flag indicate whether a vertex has been visited
    AdjListNode* head;  // head of linked list
  };

}


class GraphAdj {
protected:

  int n_vertex_;  // number of vertices

  // a vector of linked list head nodes plus some other information
  std::vector<graph::AdjListHead> adj_list_;

  //
  // add an edge (first, second)
  // The nodes in the linked list are sorted in ascending order
  // Note: the sorting will not increase the complexity since it is
  //       anyhow necessary to traverse the list to check the existence
  //       of the identical node. For the identical node, we only
  //       increase the weight.
  //
  // @param first: first vertex
  // @param second: second vertex
  // @param weight: weight of source node
  //
  void addEdge(int first, int second, int weight=1);

  //
  // delete the edge (first, second)
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: weight of the removed node
  //
  int delEdge(int first, int second);

  //
  // clear the linked list and release memory
  //
  // @param vertex: vertex value
  //
  void clearList(int vertex);

public:

  //
  // constructor
  //
  // @param n: total number of vertices
  //
  GraphAdj(int n);

  //
  // destructor
  //
  virtual ~GraphAdj();

  //
  // get No. of vertices in the graph
  //
  int size() const;

  //
  // get No. of edges in the graph
  //
  virtual int countEdge() const;

  //
  // get a vector of vertices with non-empty linked lists
  //
  std::vector<int> getNonEmptyList() const;

  //
  // get the head of the linked list
  //
  // @param vertex: value of the vertex
  //
  // @return: head of the linked list
  //
  graph::AdjListHead getList(int vertex) const;

  //
  // check whether two vertices are connected
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: true for connected and false for unconnected. Throw an error
  //          if the two vertices are only partially connected.
  virtual bool isConnected(int first, int second) const;

  //
  // connect two vertices
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: true for success and false for already connected vertices
  //
  virtual bool connect(int first, int second);

  //
  // disconnect two vertices
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: number of edges between the two vertices
  //
  virtual int disconnect(int first, int second);

  //
  // collapse the vertex src to the vertex dst and empty the linked list for src
  //
  // @param src: value of the source vertex
  // @param dst: value of the destination vertex
  //
  virtual void collapse(int src, int dst);

  //
  // Apply depth-first-search (DFS) from a vertex
  //
  // @param vertex: starting vertex
  //
  // @return: a vector of sink vertices, ordered by finding time
  //
  virtual std::vector<int> DFS(int vertex);

  //
  // display the graph
  //
  void display();

};


class UdGraphAdj: public GraphAdj {
public:
  //
  // constructor
  //
  UdGraphAdj(int n);

  //
  // destructor
  //
  ~UdGraphAdj();

  //
  // get No. of edges in the graph
  //
  int countEdge() const;

  //
  // check whether two vertices are connected
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: true for connected and false for unconnected. Throw an error
  //          if the two vertices are only partially connected.
  bool isConnected(int first, int second) const;

  //
  // connect two vertices
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: true for success and false for already connected vertices
  //
  bool connect(int first, int second);


  // disconnect two vertices
  //
  // @param first: the first vertex
  // @param second: the second vertex
  //
  // @return: number of edges between the two vertices
  //
  int disconnect(int first, int second);

  //
  // collapse the vertex src to the vertex dst and empty the linked list for src
  //
  // @param src: value of the source vertex
  // @param dst: value of the destination vertex
  //
  void collapse(int src, int dst);
};

#endif //GRAPH_GRAPH_ADJ_H
