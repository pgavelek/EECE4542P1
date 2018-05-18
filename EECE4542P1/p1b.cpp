// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <time.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

Graph bestGraph;
int conflicts;

struct VertexProperties
{
    pair<int,int> cell; // maze cell (x,y) value
    Graph::vertex_descriptor pred;
    bool visited;
    bool marked;
    int weight;
    int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
    int weight;
    bool visited;
    bool marked;
};

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
    int n, e;
    int j,k;

    fin >> n >> e;
    Graph::vertex_descriptor v;

    // Add nodes.
    for (int i = 0; i < n; i++)
        v = add_vertex(g);

    for (int i = 0; i < e; i++)
    {
        fin >> j >> k;
        add_edge(j,k,g);  // Assumes vertex list is type vecS
    }
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        g[*vItr].weight = w;
    }
}

void clearVisited(Graph &g){
    // Get a pair containing iterators pointing the beginning and end of the
    // list of nodes
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    // Loop over all nodes in the graph setting the visited value to false
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr){
        g[*vItr].visited = false;
    }
    
}

//Determines given a graph how many instances of two adjacent nodes sharing the same color there are
int returnConflicts(Graph &g) {
    int numConflicts = 0;
    //Clear all nodes marked as visited in the graph
    clearVisited(g);
    //Grab all of the vertices
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    //Cycle through all of the vertices
    for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr){
        //Grab the vertex descriptor of the current node
        Graph::vertex_descriptor v = *vItr;
        //If the current node has not been visited
        if (!g[v].visited){
            //Mark the current node as visited
            g[v].visited = true;
            //Grab all of the adjacent nodes to the current node
            pair<Graph::adjacency_iterator, Graph::adjacency_iterator> aItrRange = adjacent_vertices(v, g);
            //Cycle through all adjacent vertices
            for (Graph::adjacency_iterator aItr = aItrRange.first; aItr != aItrRange.second; ++aItr) {
                //Grab the vertex descriptor of the current node
                Graph::vertex_descriptor a = *aItr;
                //If this current adjacent node has not been checked for conflicts already and its color conflicts with the current node's color
                if (!g[a].visited && g[a].color == g[v].color) {
                    //Increment colors
                    numConflicts++;
                }
            }
        }
    }
    return numConflicts;
}

/**
 * Given a graph, the number of colors that are to be used, and the max node to color to,
 * color the graph's nodes exhaustively up to the max node.
 */
Graph exhaustColorToNode(Graph &g, int numColors, Graph::vertex_descriptor maxNode) {
    //Grab all of the vertices
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    //Vertex iterator declaration
    Graph::vertex_iterator vItr;
    //Find the desired node
    for (vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
        //If the desired node has been reached, break out of the loop
        if (*vItr == maxNode) {
            break;
        }
    }
    //Cycle through all the colors
    for (int currColor = 1; currColor <= numColors; currColor++) {
        //Grab the vertex descriptor of the current node
        Graph::vertex_descriptor v = *vItr;
        //Color current node with the current color
        g[v].color = currColor;
        //Reset the lower nodes to the first color
        for (Graph::vertex_iterator vItr2 = vItrRange.first; vItr2 != vItr; ++vItr2) {
            //Grab the vertex descriptor of the current node
            Graph::vertex_descriptor v2 = *vItr2;
            //Color current node with the current color
            g[v2].color = 1;
        }
        //Check if this layout is the best so far
        int currconflicts = returnConflicts(g);
        if (currconflicts < conflicts) {
            //Save the current graph to return
            bestGraph = g;
        }
        //If the current node is not the first node exhaust color to node
        if (vItr != vItrRange.first) {
            exhaustColorToNode(g, numColors, maxNode-1);
        }
    }
    //return the current graph
    return g;
}


//Tries every possible coloring of the nodes in a given graph given a certain number of colors and a time constraint to minimize the number of nodes that share
int exhaustiveColoring(Graph &g, int numColors, int t) {
    clock_t startTime = clock();
    //Grab all of the vertices
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    //Color all vertices with the first color
    for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr){
        //Grab the vertex descriptor of the current node
        Graph::vertex_descriptor v = *vItr;
        g[v].color = 1;
    }
    //Save this as the best graph so far
    bestGraph = g;
    //Update the number of conflicts for the best best graph
    conflicts = returnConflicts(g);
    //Cycle through all of the options for each node
    for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr){
        //Check that the current runtime is not past the specified max time
        if ((clock() - startTime) >= t) {
            return conflicts;
        }
        //Exhaustive cycle thorugh all of the coloring up to the current node
        exhaustColorToNode(g, numColors, *vItr);
    }
    //Return number of conflicts of best result
    return conflicts;
}


int main()
{
    char x;
    ifstream fin;
    string fileName;

    // Read the name of the graph from the keyboard or
    // hard code it here for testing.
    
    fileName = "/Users/wmeleis/2560-code/tree2/tree/graph1.txt";
    
    //   cout << "Enter filename" << endl;
    //   cin >> fileName;
    
    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    
    try
    {
        cout << "Reading graph" << endl;
        Graph g;
        int numColors;
        int numConflicts = -1;
        fin >> numColors;
        initializeGraph(g,fin);
        
        cout << "Num nodes: " << num_vertices(g) << endl;
        cout << "Num edges: " << num_edges(g) << endl;
        cout << endl;
        
        cout << g;
        
        numConflicts = exhaustiveColoring(g, numColors, 600);
        printSolution(bestGraph, numConflicts);
        
    }
    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}
