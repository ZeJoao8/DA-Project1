#ifndef EDMONDS_KARP_H
#define EDMONDS_KARP_H

#include <queue>
#include "../data_structures/Graph.h"
#include "../structs/Match.h"

/**
 * Declarações das funções para o algoritmo de Edmonds-Karp
 */

void testAndVisit(std::queue<Vertex<int>*> &q, Edge<int> *e, Vertex<int> *w, double residual);

void addFlowEdge(Graph<int>& g, int src, int dest, double capacity);

bool findAugmentingPath(Graph<int> *g, Vertex<int> *s, Vertex<int> *t, std::vector<Match>& matches);

double findMinResidualAlongPath(Vertex<int> *s, Vertex<int> *t);

void augmentFlowAlongPath(Vertex<int> *s, Vertex<int> *t, double f);

double edmondsKarp(Graph<int> *g, int source, int target);

#endif // EDMONDS_KARP_H