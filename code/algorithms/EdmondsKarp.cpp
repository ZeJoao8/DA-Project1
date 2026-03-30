#include "EdmondsKarp.h"
#include "../data_structures/Graph.h"

void testAndVisit(std::queue<Vertex<int> *> &q, Edge<int> *e, Vertex<int> *w,
                  double residual) {
  // Check if the vertex 'w' is not visited and there is residual capacity
  if (!w->isVisited() && residual > 0) {
    // Mark 'w' as visited, set the path through which it was reached, and
    // enqueue it
    w->setVisited(true);
    w->setPath(e);
    q.push(w);
  }
}

void addFlowEdge(Graph<int> &g, int src, int dest, double capacity) {
  auto e1 = g.findVertex(src)->addEdge(g.findVertex(dest), capacity);
  auto e2 = g.findVertex(dest)->addEdge(g.findVertex(src), 0);
  e1->setReverse(e2);
  e2->setReverse(e1);
}

// Function to find an augmenting path using Breadth-First Search
bool findAugmentingPath(Graph<int> *g, Vertex<int> *s, Vertex<int> *t) {
  // Mark all vertices as not visited
  for (auto v : g->getVertexSet()) {
    v->setVisited(false);
  }

  std::queue<Vertex<int> *> q;
  q.push(s);
  s->setVisited(true);

  while (!q.empty()) {
    Vertex<int> *cur = q.front();
    q.pop();
    if (cur == t)
      return true;

    for (auto e : cur->getAdj()) {
      Vertex<int> *cur_vis = e->getDest();
      double res_cap = e->getWeight() - e->getFlow();
      testAndVisit(q, e, cur_vis, res_cap);
    }

    for (auto e : cur->getIncoming()) {
      Vertex<int> *cur_vis = e->getOrig();
      double res_cap = e->getFlow();
      testAndVisit(q, e, cur_vis, res_cap);
    }
  }
  // Return true if a path to the target is found, false otherwise
  return t->isVisited();
}

// Function to find the minimum residual capacity along the augmenting path
double findMinResidualAlongPath(Vertex<int> *s, Vertex<int> *t) {
  double f = INF;
  Vertex<int> *v = t; // It must start in t otherwise an error will occur

  while (v != s) {
    Edge<int> *e = v->getPath();

    // If dest of the current edge is our current node -> (Forward)
    if (e->getDest() == v) {
      double res_cap = e->getWeight() - e->getFlow();
      if (res_cap < f)
        f = res_cap;
      v = e->getOrig(); // Back to the origin
    }
    // (Backward)
    else {
      double res_cap = e->getFlow();
      if (res_cap < f)
        f = res_cap;
      v = e->getDest(); // Back to the dest of the edge
    }
  }
  return f;
}
// Function to augment flow along the augmenting path with the given flow value
void augmentFlowAlongPath(Vertex<int> *s, Vertex<int> *t, double f) {
  Vertex<int> *v = t;

  while (v != s) {
    Edge<int> *e = v->getPath();

    if (e->getDest() == v) { // Forward
      e->setFlow(e->getFlow() + f);
      v = e->getOrig();
    } else { // Backward
      e->setFlow(e->getFlow() - f);
      v = e->getDest();
    }
  }
}

// Main function implementing the Edmonds-Karp algorithm
double edmondsKarp(Graph<int> *g, int source, int target) {
  // Find source and target vertices in the graph
  Vertex<int> *s = g->findVertex(source);
  Vertex<int> *t = g->findVertex(target);

  for (auto &v : g->getVertexSet()) {
    v->setVisited(false);
    v->setPath(nullptr);
    for (auto &e : v->getAdj()) {
      e->setFlow(0);
    }
  }

  double maxFlow = 0;
  while (findAugmentingPath(g, s, t)) {
    double flow = findMinResidualAlongPath(s, t);
    augmentFlowAlongPath(s, t, flow);
    maxFlow += flow;
  }
  return maxFlow;
}