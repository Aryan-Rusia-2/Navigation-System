/*
Name:  Aryan Rusia
SID:   1669003
CCID:  rusia
CMPUT 275 Winter 2022
Assignment 1 - Trivial Navigation System (Part 1)
-------------------------------------------
*/

#include <unordered_map>
#include <utility>
#include "dijkstra.h"
#include "digraph.h"
#include "wdigraph.h"
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

void dijkstra(const WDigraph &graph, int startVertex, unordered_map<int, PIL> &tree)
{
    /*
    Description : Dijkstra's algorithm is used to find the shortest path / cost from one
                  node to another nodes. It determines the least cost path, given the 
                  start node

    Arguements :
             i)const WDigraph &graph : An instance of class WDigraph
            ii) int startVertex : It gives the starting vertex/node.
           iii) unordered_map<int, PIL> &tree : Unordered_map which will
                store the least cost path
                

    Returns :
           void : (none)

    */

    int cost;         // Stores the inital cost (distance)
    int cost_changed; // Stores the change in cost (change in distance)
    int new_cost;     // Stores the final cost (distance)
    int node;         // STores the curretn node
    int last_node;    // Stores the predecessor node

    // Declaringa a priority queue that works for min-heap.
    priority_queue<long_int_int, vector<long_int_int>, greater<long_int_int>> p_queue;

    //
    p_queue.push(long_int_int(0, int_int(startVertex, startVertex)));

    // Proceed only if the size of queue is greater than zero.
    if (p_queue.size() >= 0)
    {
        // Run while loop till p_queue is not empty
        while (!p_queue.empty())
        {
            // All three of these are stores as (long long, (int, int))
            // where: i) long long represents cost, ii) first int represents
            // curretn node, iii) second int represents last node.

            // Predecessor node
            last_node = p_queue.top().second.second;

            // current node
            node = p_queue.top().second.first;

            // initial cost
            cost = p_queue.top().first;

            // Removing element from the top of the queue.
            p_queue.pop();

            // If the current node is not present in tree, then store it
            // along with the last_node and new_cost.
            if (tree.find(node) == tree.end())
            {
                // Traverse over all the nodes and edges.
                for (auto i = graph.neighbours(node); i != graph.endIterator(node); i++)
                {
                    // cost of edge from current node to i
                    cost_changed = graph.getCost(node, *i);

                    // Final cost will be equal to the inital cost + cost_changed
                    new_cost = cost + cost_changed;

                    // Add current node, last_node and the new_cost to the priority queue.
                    p_queue.push(long_int_int(new_cost, int_int(*i, node)));
                }
            }
            // Continue, if current node is already stored/present in the tree.
            else
            {
                continue;
            }

            // Store last_node and cost in the PIL
            tree[node] = PIL(last_node, cost);
        }
    }
}