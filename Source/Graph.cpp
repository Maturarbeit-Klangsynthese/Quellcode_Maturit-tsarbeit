/*
  ==============================================================================

    Graph.cpp
    Created: 28 Sep 2024 8:49:43am
    Author:  Maximilian

  ==============================================================================
*/

#include "Graph.h"

//  Constructor of graph
Graph::Graph(int V)
{
    this->V = V;
    adj = new std::list<int>[V];
 
    // Initialising all indegree with 0
    for (int i = 0; i < V; i++)
        indegree.push_back(0);
}
 
//  Utility function to add edge
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v's list.
 
    // increasing inner degree of w by 1
    indegree[w]++;
}
 
//  Main recursive function to print all possible
//  topological sorts
void Graph::alltopologicalSortUtil(std::vector<int>& res,
                                   bool visited[])
{
    // To indicate whether all topological are found
    // or not
    bool flag = false;
 
    for (int i = 0; i < V; i++)
    {
        //  If indegree is 0 and not yet visited then
        //  only choose that vertex
        if (indegree[i] == 0 && !visited[i])
        {
            //  reducing indegree of adjacent vertices
            std::list<int>:: iterator j;
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                indegree[*j]--;
 
            //  including in result
            res.push_back(i);
            visited[i] = true;
            alltopologicalSortUtil(res, visited);
 
            // resetting visited, res and indegree for
            // backtracking
            visited[i] = false;
            res.erase(res.end() - 1);
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                indegree[*j]++;
 
            flag = true;
        }
    }
 
    //  We reach here if all vertices are visited.
    //  So we print the solution here
    if (!flag)
    {
        for (int i = 0; i < res.size(); i++)
            std::cout << res[i] << " ";
        std::cout << std::endl;
    }
}
 
//  The function does all Topological Sort.
//  It uses recursive alltopologicalSortUtil()
void Graph::alltopologicalSort()
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
 
    std::vector<int> res;
    alltopologicalSortUtil(res, visited);
}

// Function to return a single topological sort
std::vector<int> Graph::singletopologicalSort()
{
    std::vector<int> topoSort; // Vector to store the topological order
    std::queue<int> q;         // Queue to store vertices with indegree 0
 
    // Push all vertices with indegree 0 into the queue
    for (int i = 0; i < V; i++)
    {
        if (indegree[i] == 0)
            q.push(i);
    }
 
    while (!q.empty())
    {
        // Get the front element from the queue and add it to topoSort
        int u = q.front();
        q.pop();
        topoSort.push_back(u);
 
        // Reduce the indegree of all adjacent vertices of u
        for (auto& v : adj[u])
        {
            indegree[v]--;
 
            // If indegree becomes 0, add it to the queue
            if (indegree[v] == 0)
                q.push(v);
        }
    }
 
    // If topological sort includes all vertices, return it
    if (topoSort.size() == V)
        return topoSort;
 
    // If there is a cycle, return an empty vector
    return std::vector<int>();
}
