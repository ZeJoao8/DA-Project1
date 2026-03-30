#ifndef EDMONDS_KARP_H
#define EDMONDS_KARP_H

#include <queue>
#include "../data_structures/Graph.h"
#include "../structs/Match.h"

/**
 * @file EdmondsKarp.h
 * @brief Edmonds-Karp maximum flow algorithm declarations.
 *
 * Provides a BFS-based implementation of the Ford-Fulkerson method for computing
 * maximum flow in a directed graph - the same presented during practical classes.
 *
 * Overall time complexity: O(V * E²).
 */

/**
 * @brief Visits a vertex during BFS if unvisited and residual capacity is positive.
 *
 * Marks @p w as visited, records the edge through which it was reached,
 * and enqueues it for further exploration.
 *
 * @param q        BFS queue of vertices to be explored.
 * @param e        Edge leading to vertex @p w.
 * @param w        Candidate vertex to visit.
 * @param residual Residual capacity available on edge @p e.
 *
 * @note Time complexity: O(1).
 */
void testAndVisit(std::queue<Vertex<int>*> &q, Edge<int> *e, Vertex<int> *w, double residual);

/**
 * @brief Adds a directed flow edge and its corresponding reverse edge to the graph.
 *
 * Creates a forward edge from @p src to @p dest with the given @p capacity,
 * and a backward edge from @p dest to @p src with capacity 0.
 * Both edges are linked via their reverse pointers to support residual graph operations.
 *
 * @param g        The flow graph.
 * @param src      Info value of the source vertex.
 * @param dest     Info value of the destination vertex.
 * @param capacity Capacity of the forward edge.
 *
 * @note Time complexity: O(V), where V is the number of vertices due to linear search performed during findVertex().
 */
void addFlowEdge(Graph<int>& g, int src, int dest, double capacity);

/**
 * @brief Finds an augmenting path from @p s to @p t via BFS on the residual graph.
 *
 * Traverses both forward edges (with remaining capacity) and backward edges
 * (with positive flow). Each visited vertex records the edge through which it
 * was reached, enabling path reconstruction.
 *
 * @param g Pointer to the flow graph.
 * @param s Pointer to the source vertex.
 * @param t Pointer to the sink vertex.
 * @return  @c true if an augmenting path to @p t was found; @c false otherwise.
 *
 * @note Time complexity: O(V + E), where V is the number of vertices and E the number of edges.
 */
bool findAugmentingPath(Graph<int> *g, Vertex<int> *s, Vertex<int> *t);

/**
 * @brief Computes the minimum residual capacity (bottleneck) along the augmenting path.
 *
 * Traces back from @p t to @p s using each vertex's recorded path edge.
 * For forward edges the residual is @c weight - @c flow; for backward edges it is @c flow.
 *
 * @param s Pointer to the source vertex.
 * @param t Pointer to the sink vertex.
 * @return  The bottleneck capacity along the path.
 *
 * @pre An augmenting path from @p s to @p t must have been found by findAugmentingPath().
 * @note Time complexity: O(V), where V is the number of vertices on the path.
 */
double findMinResidualAlongPath(Vertex<int> *s, Vertex<int> *t);

/**
 * @brief Augments flow along the path from @p s to @p t by @p f units.
 *
 * Traces back from @p t to @p s using each vertex's recorded path edge.
 * Forward edges have their flow increased by @p f; backward edges have it decreased.
 *
 * @param s Pointer to the source vertex.
 * @param t Pointer to the sink vertex.
 * @param f Flow amount to push (bottleneck value from findMinResidualAlongPath()).
 *
 * @pre An augmenting path must have been found and its bottleneck computed.
 * @note Time complexity: O(V), where V is the number of vertices on the path.
 */
void augmentFlowAlongPath(Vertex<int> *s, Vertex<int> *t, double f);

/**
 * @brief Computes the maximum flow from @p source to @p target using the Edmonds-Karp algorithm.
 *
 * Repeatedly finds shortest augmenting paths (BFS) in the residual graph and augments
 * flow along them until no augmenting path exists. All edge flows are reset to zero
 * before execution begins.
 *
 * @param g      Pointer to the flow graph.
 * @param source Info value of the source vertex.
 * @param target Info value of the sink vertex.
 * @return       The maximum flow value from @p source to @p target.
 *
 * @note Time complexity: O(V * E²), where V is the number of vertices and E the number of edges.
 */
double edmondsKarp(Graph<int> *g, int source, int target);

#endif // EDMONDS_KARP_H