//
// Created by jun on 6/6/17.
//
// implementation of the directed and undirected graph using adjacency list
//
// base class:
// - GraphAdj()
//  derived class:
//    - UdGraphAdj()
//
//
#ifndef GRAPH_GRAPH_ADJ_H
#define GRAPH_GRAPH_ADJ_H

#include <iostream>
#include <assert.h>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>


namespace graph {
  //
  // node in a linked list, which represents edges
  //
  template <class T>
  struct Edge {
    T value;  // value of the head vertex of the edge
    int weight;  // weight of the edge
    double distance;  // length of the edge
    Edge<T>* next;  // pointer to the next edge in the same linked list
  };

  //
  // vertex of the graph, containing head node of the linked list
  //
  template <class T>
  struct GraphAdjVertex {
    T value;  // value of the vertex
    bool visited;  // flag indicate whether a vertex has been visited
    Edge<T>* next;  // head node of the linked list
  };

  //
  // construct a new Edge
  //
  // @param value: value of the head vertex of the edge
  // @param weight: weight of the edge
  // @param distance: length of the edge
  //
  // @return: pointer to the new edge
  //
  template <class T>
  Edge<T>* newEdge(T value, int weight=1, double distance=1.0){

    Edge<T>* new_edge =new Edge<T>;
    new_edge->value = value;
    new_edge->weight = weight;
    new_edge->distance = distance;
    new_edge->next = NULL;

    return new_edge;
  }

  //
  // construct a new GraphAdjVertex
  //
  // @param value: value of the vertex
  //
  // @return: pointer to the new vertex
  //
  template <class T>
  GraphAdjVertex<T>* newGraphAdjVertex(T value) {

    GraphAdjVertex<T>* new_vertex = new GraphAdjVertex<T>;
    new_vertex->value = value;
    new_vertex->visited = false;
    new_vertex->next = NULL;

    return new_vertex;
  }

  //
  // reverse the directed graph
  //
  // @param graph: a graph object
  //
  template <class G>
  G reverseGraph(G const &graph) {
    G graph_reversed;

    for (auto i = 0; i < graph.size(); ++i) {
      auto value = graph.getVertexByIndex(i)->value;
      auto current_node = graph.getVertexByIndex(i)->next;
      while ( current_node ) {
        graph_reversed.connect(current_node->value, value);
        current_node = current_node->next;
      }
    }

    return graph_reversed;
  }
}

/*
 * Directed graph
 */

template <class T>
class GraphAdj {

protected:
  //
  // get the pointer to a vertex
  //
  // @param value: the vertex value
  //
  // @return: pointer to the vertex
  //
  graph::GraphAdjVertex<T>* getVtx(T value) {
    auto search = hash_.find(value);
    if ( search == hash_.end() ) {
      return NULL;
    } else {
      return &vertices_[search->second];
    }
  }

  //
  // reset all the vertices to unvisited
  //
  void reset() {
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      it->visited = false;
    }
  }

  //
  // clear (release memory) the linked list belong to a vertex
  //
  // @param vertex: the vertex value
  //
  void clearList(T value) {
    graph::GraphAdjVertex<T>* v = getVtx(value);
    if ( !v->next ) { return; }

    graph::Edge<T>* current = v->next;

    while ( current ) {
      v->next = current->next;
      delete current;
      current = v->next;
    }
  }

  // hash table that converts the vertex value to the vector index
  std::unordered_map<T, int> hash_;

  // vector of verticies
  std::vector<graph::GraphAdjVertex<T>> vertices_;

  //
  // Add an edge from tail to head. The nodes in the linked
  // list are sorted in ascending order.
  //
  // Public method connect() is implemented to add an edge for both
  // directed graph and undirected graph
  //
  // Note: the sorting will not increase the complexity since it is
  //       anyhow necessary to traverse the list to check the existence
  //       of the identical node. For the identical node, we only
  //       increase the weight.
  //
  // @param tail: the tail vertex value
  // @param head: the head vertex value
  // @param weight: the weight of edge
  // @param distance: the length of the edge
  //
  void addEdge(T tail, T head, int weight, double distance) {
    assert(tail != head);

    graph::GraphAdjVertex<T> * v_tail = getVtx(tail);
    if ( !v_tail ) {
      graph::GraphAdjVertex<T>* new_vertex = graph::newGraphAdjVertex<T>(tail);
      vertices_.push_back(*new_vertex);
      hash_.insert(std::pair<T, int> (tail, vertices_.size() - 1));
    }

    graph::GraphAdjVertex<T>* v_head = getVtx(head);
    if ( !v_head ) {
      graph::GraphAdjVertex<T>* new_vertex = graph::newGraphAdjVertex<T>(head);
      vertices_.push_back(*new_vertex);
      hash_.insert(std::pair<T, int> (head, vertices_.size() - 1));
    }

    // the address may have changed due to the rearrangement of the vector
    v_tail = getVtx(tail);
    v_head = getVtx(head);

    graph::Edge<T>* new_edge = graph::newEdge(head, weight, distance);

    graph::Edge<T>* current = v_tail->next;
    graph::Edge<T>* previous = v_tail->next;

    if ( !v_tail->next ) {
      v_tail->next = new_edge;
    } else if ( head == v_tail->next->value ) {
      v_tail->next->weight += weight;
      delete new_edge;
      return;
    } else if ( head < v_tail->next->value ) {
      // insert the new edge in the head
      v_tail->next = new_edge;
      v_tail->next->next = current;
    } else {
      // insert the new edge in the middle
      current = current->next;
      while ( current ) {
        if ( current->value == head ) {
          current->weight += weight;
          delete new_edge;
          return;
        } else if ( current->value > head ) {
          new_edge->next = current;
          previous->next = new_edge;
          return;
        } else {
          current = current->next;
          previous = previous->next;
        }
      }

      // insert the new edge in the tail
      previous->next = new_edge;
    }
  }

  //
  // Delete the edge from tail to head
  //
  // Public method disconnect() is implemented to delete an edge for both
  // directed graph and undirected graph
  //
  // @param tail: the tail vertex value
  // @param head: the head vertex value
  //
  // @return: weight of the removed node
  //
  int delEdge(T tail, T head) {
    assert(tail != head);

    int weight;
    graph::GraphAdjVertex<T>* v_tail = getVtx(tail);
    if ( !v_tail ) { return 0; }
    graph::Edge<T>* current = v_tail->next;
    graph::Edge<T>* previous = v_tail->next;

    if ( v_tail->next->value == head ) {
      // if node is the head node
      v_tail->next = current->next;
      weight = current->weight;
      delete current;
      return weight;
    } else {
      current = current->next;
      while ( current ) {
        if ( current->value == head ) {
          previous->next = current->next;
          weight = current->weight;
          delete current;
          return weight;
        } else {
          previous = previous->next;
          current = current->next;
        }
      }
    }

    return 0;
  }

public:
  //
  // constructor
  //
  GraphAdj() {
    hash_.max_load_factor(0.5);
  }

  //
  // copy constructor
  // TODO:: improve it
  //
  GraphAdj(const GraphAdj<T>& g) {
    graph::GraphAdjVertex<T> adj_list;

    for (int i = 0; i < g.size(); ++i) {
      adj_list.visited = false;
      adj_list.next = NULL;

      vertices_.push_back(adj_list);
    }

    for ( auto it1 = vertices_.begin(), it2 = g.vertices_.begin();
          it2 != g.vertices_.end(); ++it1, ++it2 ) {

      it1->visited = it2->visited;
      it1->value = it2->value;

      graph::Edge<T>* current_node1 = it1->next;
      graph::Edge<T>* current_node2 = it2->next;

      while ( current_node2 ) {
        graph::Edge<T>* new_node = new graph::Edge<T>(*current_node2);
        if ( !it1->next ) {
          it1->next = new_node;
          current_node1 = new_node;
        } else {
          current_node1->next = new_node;
          current_node1 = current_node1->next;
        }
        current_node2 = current_node2->next;
      }
    }

    hash_ = g.hash_;
  }

  //
  // destructor
  // TODO: check memory leak
  //
  virtual ~GraphAdj() {
    for (auto it = vertices_.begin(); it != vertices_.end() ; ++it) {
      clearList(it->value);
    }
  }

  //
  // get No. of vertices in the graph
  //
  std::size_t size() const { return vertices_.size(); }

  //
  // get the pointer to a const vertex by vertex value
  //
  // @param value: the vertex value
  //
  // @return: pointer to the const vertex
  //
  graph::GraphAdjVertex<T> const* getVertex(T value) const {
    auto search = hash_.find(value);
    if ( search == hash_.end() ) {
      return NULL;
    } else {
      return &vertices_[search->second];
    }
  }

  //
  // get the index of a vertex with value "value"
  //
  // @param value: the vertex value
  //
  // @return: the vertex index in the vector
  //
  int const getVertexIndex(T value) const {
    auto search = hash_.find(value);
    if ( search == hash_.end() ) {
      return -1;
    } else {
      return search->second;
    }
  }

  //
  // get the pointer to a const vertex by index of the vertex vector
  //
  // @param value: the index
  //
  // @return: pointer to the const vertex
  //
  graph::GraphAdjVertex<T> const* getVertexByIndex(int index) const {
    if ( index < 0 || index >= vertices_.size() ) {
      return NULL;
    } else {
      return &vertices_[index];
    }
  }

  //
  // get No. of edges in the graph
  //
  virtual int countEdge() const {
    int count = 0;
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      graph::Edge<T>* current = it->next;
      while ( current ) {
        count += current->weight;
        current = current->next;
      }
    }

    return count;
  }
  //
  // get a vector of vertices with at least one edge
  //
  std::vector<T> getConnectedVertices() const {
    std::vector<T> connectedVertices;

    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      if ( it->next ) {
        connectedVertices.push_back(it->value);
      }
    }

    return connectedVertices;
  }

  //
  // check whether two vertices are connected
  //
  // @param tail: the tail vertex value
  // @param head: the head vertex value
  //
  // @return: true for connected and false for unconnected. Throw an error
  //          if the two vertices are only partially connected.
  //
  virtual bool isConnected(T tail, T head) {
    graph::GraphAdjVertex<T>* v_tail = getVtx(tail);
    if ( !v_tail) { return false; }

    if ( tail == head ) { return true; }

    bool connected = false;

    graph::Edge<T>* current = v_tail->next;

    while ( current ) {
      if ( current->value == head ) {
        connected = true;
      }
      current = current->next;
    }

    return connected;
  }

  //
  // connect two vertices
  //
  // @param tail: the tail vertex value
  // @param head: the head vertex value
  // @param weight: the weight of the edge
  // @param distance: the length of the edge
  //
  // @return: true for success and false for already connected vertices
  //
  virtual bool connect(T tail, T head, int weight=1, double distance=1.0) {
    if ( isConnected(tail, head) ) { return false; }

    addEdge(tail, head, weight, distance);

    return true;
  }

  //
  // disconnect two vertices
  //
  // @param tail: the tail vertex value
  // @param head: the head vertex value
  //
  // @return: number of edges between the two vertices
  //
  virtual int disconnect(T tail, T head) {
    int weight = delEdge(tail, head);

    return weight;
  }

  //
  // collapse the vertex src to the vertex dst and empty the linked list for src
  //
  // @param src: the value of the source vertex
  // @param dst: the value of the destination vertex
  //
  virtual void collapse(T src, T dst) {};

  //
  // Breath-first-search (BFS) starting from a vertex
  //
  // @param vertex: the starting vertex value
  //
  // @return: a vector of visited vertices, ordered by finding time
  //
  std::vector<T> breathFirstSearch(T value) {
    graph::GraphAdjVertex<T>* v = getVtx(value);
    if ( !v ) { return std::vector<T> {}; }

    // the container for visited vertices in finding sequence
    std::vector<T> visited;
    std::queue<T> track;

    track.push(value);
    visited.push_back(value);
    v->visited = true;
    while (!track.empty()) {
      graph::GraphAdjVertex<T>* current_head = getVtx(track.front());
      track.pop();

      graph::Edge<T>* current_node = current_head->next;
      // find all children vertices which have not been visited yet
      while (current_node) {
        if ( !getVtx(current_node->value)->visited ) {
          track.push(current_node->value);
          visited.push_back(current_node->value);
          getVtx(current_node->value)->visited = true;
        }
        current_node = current_node->next;
      }
    }

    reset();
    return visited;
  }

  //
  // Depth-first-search (DFS) starting from a vertex
  //
  // @param value: starting vertex value
  //
  // @return: a vector of sink vertices, ordered by finding time
  //
  std::vector<T> depthFirstSearch(T value) {
    graph::GraphAdjVertex<T>* v = getVtx(value);
    if ( !v ) { return std::vector<T> {}; }

    // the container for sink vertices in finding sequence
    std::vector<T> sink;
    std::stack<T> tracker;

    tracker.push(value);
    v->visited = true;
    T current_vertex_value = tracker.top();
    // search the vertex reachable from the current vertex
    while ( true ) {
      bool retreat = true;  // a flag indicating whether to retreat to the last vertex
      graph::Edge<T>* current_node = v->next;
      // find the next reachable vertex which has not been visited
      while ( current_node ) {
        graph::GraphAdjVertex<T>* next_v = getVtx(current_node->value);
        if ( !next_v->visited ) {
          next_v->visited = true;
          tracker.push(current_node->value);

          retreat = false;
          break;
        }
        current_node = current_node->next;
      }
      // if a sink vertex is found
      if ( retreat ) {
        sink.push_back(current_vertex_value);
        tracker.pop();
      }

      if ( tracker.empty() ) {
        break;
      } else {
        current_vertex_value = tracker.top();
        v = getVtx(current_vertex_value);
        v->visited = true;
      }
    }

//    reset();

    return sink;
  }

  //
  // display the graph
  //
  void display() const {
    std::cout << "------------------------------" << std::endl;

    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      graph::Edge<T>* pprint = it->next;

      std::cout << "Vertex [" << it->value << "] (visited = "
                << it->visited << " )";
      while (pprint) {
        std::cout << " -> " << pprint->value
                  << " (" << pprint->weight << ',' << pprint->distance << ")";
        pprint = pprint->next;
      }

      std::cout << std::endl;
    }
  }

};


/*
 * Undirected graph
 */

template <class T>
class UdGraphAdj: public GraphAdj<T> {
private:
  using GraphAdj<T>::vertices_;
  using GraphAdj<T>::hash_;
  using GraphAdj<T>::addEdge;
  using GraphAdj<T>::delEdge;
  using GraphAdj<T>::clearList;
  using GraphAdj<T>::getVtx;

public:
  //
  // constructor - call default constructor of GraphAdj()
  //
  UdGraphAdj() {}

  //
  // destructor - call default destructor
  //
  ~UdGraphAdj() {}

  //
  // get No. of edges in the graph
  //
  int countEdge() const {
    int count = 0;
    for (int i=0; i<vertices_.size(); ++i) {
      graph::Edge<T>* current = vertices_[i].next;
      while ( current ) {
        count += current->weight;
        current = current->next;
      }
    }

    assert( count % 2 == 0 );

    return count/2;
  }

  //
  // check whether two vertices are connected
  //
  // @param first: the first vertex value
  // @param second: the second vertex value
  //
  // @return: true for connected and false for unconnected. Throw an error
  //          if the two vertices are only partially connected.
  //
  bool isConnected(T first, T second) {
    graph::GraphAdjVertex<T>* v_first = getVtx(first);
    graph::GraphAdjVertex<T>*  v_second = getVtx(second);
    // if any one of the two vertices is empty
    if ( !v_first || !v_second ) { return false; }

    if ( first == second ) { return true; }

    bool connected1 = false;
    bool connected2 = false;

    graph::Edge<T>* current = v_first->next;

    while ( current ) {
      if ( current->value == second ) {
        connected1 = true;
      }
      current = current->next;
    }

    current = v_second->next;

    while ( current ) {
      if ( current->value == first ) {
        connected2 = true;
      }
      current = current->next;
    }

    if ( connected1 && connected2 ) {
      return true;
    } else if ( connected1 || connected2 ) {
      throw std::invalid_argument("two vertices are only partially connected");
    } else {
      return false;
    }
  }

  //
  // connect two vertices
  //
  // @param first: the first vertex value
  // @param second: the second vertex value
  // @param weight: the weight of the edge
  // @param distance: the distance of the edge
  //
  // @return: true for success and false for already connected vertices
  //
  bool connect(T first, T second, int weight=1, double distance=1.0) {
    assert(first != second);

    if ( isConnected(first, second) ) { return false; }

    // we need to add a node for the lists a and b respectively
    addEdge(first, second, weight, distance);
    addEdge(second, first, weight, distance);

    return true;
  }

  //
  // disconnect two vertices
  //
  // @param first: the first vertex value
  // @param second: the second vertex value
  //
  // @return: number of edges between the two vertices
  //
  int disconnect(T first, T second) {

    assert(first != second);

    int weight1 = delEdge(first, second);
    int weight2 = delEdge(second, first);

    assert(weight1 == weight2);

    return weight1;
  }
  //
  // collapse the vertex src to the vertex dst and empty the linked list for src
  //
  // @param src: value of the source vertex
  // @param dst: value of the destination vertex
  //
  void collapse(T src, T dst) {
    assert(src != dst);

    // first remove loops
    disconnect(src, dst);

    // In each collapse, the No. of edges will be reduced only by the No. of edges
    // between the source and destination vertices!

    // change the value of nodes with value src to dst
    graph::Edge<T>* current = getVtx(src)->next;
    while ( current ) {
      // change the node with value 'src' in other linked lists to 'dst'
      // TODO:: this could be fast since the linked list is traversed twice here for code simplicity
      int weight = delEdge(current->value, src);
      addEdge(current->value, dst, weight, 1);

      // add node in the linked list 'src' to the linked list 'dst'
      addEdge(dst, current->value, current->weight, 1);

      current = current->next;
    }

    // delete linked list and release memory
    clearList(src);

  }
};

#endif //GRAPH_GRAPH_ADJ_H
