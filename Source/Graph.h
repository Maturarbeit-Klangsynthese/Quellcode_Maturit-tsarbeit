/*
  ==============================================================================

    Graph.h
    Created: 28 Sep 2024 8:49:43am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// hauptsächlich kopierter code, quelle in maturarbeit
class Graph
{
    int V;    // No. of vertices
 
    // Pointer to an array containing adjacency list
    std::list<int> *adj;
 
    // Vector to store indegree of vertices
    std::vector<int> indegree;
 
    // A function used by alltopologicalSort
    void alltopologicalSortUtil(std::vector<int>& res,
                                bool visited[]);
 
public:
    Graph(int V);   // Constructor
 
    // function to add an edge to graph
    void addEdge(int v, int w);
 
    // Prints all Topological Sorts
    void alltopologicalSort(); // Theoretisch bräuchte ich diese nicht ein mal
    
    std::vector<int> singletopologicalSort(); // die wichtige funktion!
};
