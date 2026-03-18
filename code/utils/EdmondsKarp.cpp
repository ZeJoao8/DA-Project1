#include "data_structures/Graph.h"

template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    // Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
    }

   // TO DO
    std::queue< Vertex<T>*> q;
    q.push(s);
    s->setVisited(true);

    while (!q.empty()){
      Vertex<T> *cur = q.front();
      q.pop(); if (cur == t) return true;

      for (auto e : cur -> getAdj()) {
          Vertex<T> *cur_vis = e -> getDest();
          double res_cap = e -> getWeight() - e -> getFlow();
          testAndVisit(q, e, cur_vis, res_cap);
      }

      for (auto e: cur -> getIncoming()) {
          Vertex<T> *cur_vis = e -> getOrig();
          double res_cap = e -> getFlow();
          testAndVisit(q, e, cur_vis, res_cap);
      }

    }
    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}

// Function to find the minimum residual capacity along the augmenting path
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;
    Vertex<T> *v = t; // Começar no T!

    while (v != s) {
        Edge<T>* e = v->getPath();

        // Se o destino da aresta for o nó atual, viemos na direção normal (Forward)
        if (e->getDest() == v) {
            double res_cap = e->getWeight() - e->getFlow();
            if (res_cap < f) f = res_cap;
            v = e->getOrig(); // Recuar para a origem da aresta
        }
        // Se a origem da aresta for o nó atual, viemos "em contramão" (Backward)
        else {
            double res_cap = e->getFlow();
            if (res_cap < f) f = res_cap;
            v = e->getDest(); // Recuar para o destino da aresta (que na verdade foi de onde viemos)
        }
    }
    return f;
}
// Function to augment flow along the augmenting path with the given flow value
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    Vertex<T> *v = t;

    while (v != s) {
        Edge<T>* e = v->getPath();

        if (e->getDest() == v) { // Forward
            e->setFlow(e->getFlow() + f);
            v = e->getOrig();
        }
        else { // Backward
            e->setFlow(e->getFlow() - f);
            v = e->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
template <class T>
void edmondsKarp(Graph<T> *g, int source, int target) {
    // Find source and target vertices in the graph
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);

    for (auto& v: g -> getVertexSet()) {
        v -> setVisited(false);
        v -> setPath(nullptr);
        for (auto& e: v -> getAdj()) {
            e -> setFlow(0);
        }
    }
    while (findAugmentingPath(g, s, t)) {
        double flow = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s,t, flow);
    }

}