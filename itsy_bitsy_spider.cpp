//==================================================================================================================
//COT4400 Project #2
//Christopher Reiner
//See PDF in project folder for in-depth description.
//==================================================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <list>

//update the INPUTFILE definition per the required input file name
#define INPUTFILE "sample-spider-input.txt"

using namespace std;

//class for constructing and maintaining the graph
class Graph {
public:
    //functions
    Graph(int V);                                       // Constructor
    void addEdge(int v, int w);                         // function to add an edge to graph
    void DFS(int v);                                    // DFS traversal of the vertices reachable from v
    void
    bestPath();                                    // function to calculate the path traveled based on cell location

    //variables
    int V;                                              // No. of vertices
    list<int> *adj;                                     // Pointer to an array containing adjacency lists
    void DFSUtilityFunction(int v, bool visited[]);    // A recursive function used by DFS
    vector<int> pathVector;

    //variables
    int graphLevel;
    int graphRow;
    int graphColumn;
    int graphStartCell;
    int graphEndCell;
    bool endFound = false;
};

//constructor
Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}

//function to add edge to graph
void Graph::addEdge(int v, int w) {
    adj[v].push_back(w); // Add w to v’s list.
}

void Graph::DFSUtilityFunction(int v, bool visited[]) {

    if (endFound) return;

    if (v == graphEndCell) {
        endFound = true;
    }

    // Mark the current node as visited
    if (!visited[v]) {
        visited[v] = true;
        pathVector.push_back(v);
    }

    //Recur for all the adjacent vertices
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i) {
        if (!visited[*i])
            DFSUtilityFunction(*i, visited);
        if (endFound) return;
    }

}

// DFS traversal of the vertices reachable from v using recursive DFSUtil()
void Graph::DFS(int v) {
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++) visited[i] = false;

    // Call the recursive helper function to print DFS traversal
    DFSUtilityFunction(v, visited);
}

//function for interpreting the path followed from the vertices visited
void Graph::bestPath() {

    ofstream outputFile("output.txt", ios::out | ios::app);

    for (int i = 1; i < pathVector.size(); i++) {
        //cout << pathVector[i] << "-";
        if ((pathVector[i] - (graphRow * graphColumn) == pathVector[i - 1])) outputFile << "U ";
        else if ((pathVector[i] + (graphRow * graphColumn) == pathVector[i - 1])) outputFile << "D ";
        else if (pathVector[i] + 1 == pathVector[i - 1]) outputFile << "W ";
        else if (pathVector[i] - 1 == pathVector[i - 1]) outputFile << "E ";
        else if (pathVector[i] - graphColumn == pathVector[i - 1]) outputFile << "S ";
        else if (pathVector[i] + graphColumn == pathVector[i - 1]) outputFile << "N ";
    }
    outputFile << "\n";
}

//function for determining the coordinates of a cell - used for debugging
struct Point {
    int x;
    int y;
    int z;
};

//struct for organizing contents of graph vertices
struct vertex {
public:
    Point cellCoordinates;
    int identifier;
    bool north;
    bool east;
    bool south;
    bool west;
    bool up;
    bool down;
};

int main() {

    //variables for the main function
    string binaryInput;
    int numberOfMazes;
    int levels, rows, columns;
    int startLevel, startRow, startColumn;
    int endLevel, endRow, endColumn;

    //open input stream
    ifstream inFile;
    inFile.open(INPUTFILE);

    //input error checking
    if (!inFile) {
        cout << "Invalid Input File";
        exit(1);
    }

    //reads in input for # of mazes
    inFile >> numberOfMazes;

    //for loop for generating a graph for each maze in the input file
    for (int n = 0; n < numberOfMazes; n++) {
        inFile >> levels >> rows >> columns;
        int numVertices = levels * rows * columns;
        Graph mazeGraph(numVertices);
        mazeGraph.graphLevel = levels;
        mazeGraph.graphRow = rows;
        mazeGraph.graphColumn = columns;

        inFile >> startLevel >> startRow >> startColumn;
        mazeGraph.graphStartCell = ((startLevel * rows * columns) + (startRow * columns) + startColumn);

        inFile >> endLevel >> endRow >> endColumn;
        mazeGraph.graphEndCell = ((endLevel * rows * columns) + (endRow * columns) + endColumn);

        for (int l = 0; l < mazeGraph.graphLevel; l++) {
            for (int r = 0; r < mazeGraph.graphRow; r++) {
                for (int c = 0; c < mazeGraph.graphColumn; c++) {
                    inFile >> binaryInput;

                    vertex tempMazeCell{};

                    tempMazeCell.cellCoordinates.x = r;
                    tempMazeCell.cellCoordinates.y = c;
                    tempMazeCell.cellCoordinates.z = l;
                    tempMazeCell.identifier = ((l * mazeGraph.graphRow * columns) + (r * mazeGraph.graphColumn) + c);
                    tempMazeCell.north = binaryInput[0] != '0';
                    tempMazeCell.east = binaryInput[1] != '0';
                    tempMazeCell.south = binaryInput[2] != '0';
                    tempMazeCell.west = binaryInput[3] != '0';
                    tempMazeCell.up = binaryInput[4] != '0';
                    tempMazeCell.down = binaryInput[5] != '0';

                    if (tempMazeCell.north == 1) {
                        mazeGraph.adj[tempMazeCell.identifier].push_back(tempMazeCell.identifier - mazeGraph.graphRow);
                    }

                    if (tempMazeCell.east == 1) {
                        mazeGraph.adj[tempMazeCell.identifier].push_back(tempMazeCell.identifier + 1);
                    }

                    if (tempMazeCell.south == 1) {
                        mazeGraph.adj[tempMazeCell.identifier].push_back(tempMazeCell.identifier + mazeGraph.graphRow);
                    }

                    if (tempMazeCell.west == 1) {
                        mazeGraph.adj[tempMazeCell.identifier].push_back(tempMazeCell.identifier - 1);
                    }

                    if (tempMazeCell.up == 1) {
                        mazeGraph.adj[tempMazeCell.identifier].push_back(
                                tempMazeCell.identifier + (mazeGraph.graphRow * mazeGraph.graphColumn));
                    }

                    if (tempMazeCell.down == 1) {
                        mazeGraph.adj[tempMazeCell.identifier].push_back(
                                tempMazeCell.identifier - (mazeGraph.graphRow * mazeGraph.graphColumn));
                    }
                    if (c == mazeGraph.graphColumn - 1) cout << "\n";
                }
            }
        }
        //performs Depth First Search algorithm
        mazeGraph.DFS(mazeGraph.graphStartCell);

        //prints correct exit path to output file
        mazeGraph.bestPath();
    }
    return 0;
}
