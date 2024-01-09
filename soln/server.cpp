#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "dijkstra.h"
#include "digraph.h"

using namespace std;

// Globally defining Character Flags
const char A_char = 'A';
const char R_char = 'R';
const char E_char = 'E';
const char W_char = 'W';
char prog_end_flag;
char user_given_flag;

// Point Data Structure
struct Point
{
    // Default Constructor
    Point() = default;

    Point(long long &input_Latitude, long long &input_lon) : Latitude(input_Latitude), Longitude(input_lon){};

    // Declaring two long long variables representing latitude and longitude
    long long Latitude;
    long long Longitude;
};

// Forward declaration of function "readGraph()".
void readGraph(string filename, WDigraph &graph, unordered_map<int, Point> &points);

// Forward declaration of function "Client_input()".
void Client_input(int &id_startNode, int &id_endNode, unordered_map<int, Point> &points);

// Forward declaration of function "Nearest_Vertex()".
int Nearest_Vertex(Point &location, unordered_map<int, Point> &points);

// Forward declaration of function "manhattan()".
long long manhattan(const Point &pt1, const Point &pt2);

// Forward declaration of function "if_no_path()".
void if_no_path(list<int> &shortest_path, unordered_map<int, PIL> tree, int id_endNode);

// Forward declaration of function "program_end()".
void program_end();

int main()
{
    // Creating an instance of class Wdigraph
    WDigraph graph;

    // Declaring an int variable containing the id of starting vertex/node.
    int id_startNode;

    // Declaring an int variable containing the id of end vertex/node.
    int id_endNode;

    // This variable contains the name of file, which describes the map of Edmonton
    string file = "edmonton-roads-2.0.1.txt";

    // Declaring an unordered_map which will store the least cost path (Shortest Path)
    unordered_map<int, PIL> tree;

    // Declaring an unordered_map to store the lat/lon of the nodes.
    unordered_map<int, Point> points;

    // Declaring a list to read the shortest shortest_path.
    list<int> shortest_path;

    // Caliing function "readGraph" to read the amp of edmonton from the provided file
    readGraph(file, graph, points);

    // Calling function "Client_input" to take the input from user / client.
    Client_input(id_startNode, id_endNode, points);

    // Calling function "dijkstra" to find the least cost path (Shortest Distance)
    dijkstra(graph, id_startNode, tree);

    // Calling function "if_no_path" to check if a path exists between two user given vertices.
    if_no_path(shortest_path, tree, id_endNode);

    // This if condition constructs a path from start vertex to end vertex on the basis of tree.
    if (tree.find(id_endNode) != tree.end())
    {
        // Traversing from end vertex to start vertex and adding the vertices of the least cost path.
        for (int i = id_endNode; i != id_startNode; i = tree[i].first)
        {
            shortest_path.push_front(i);
        }

        shortest_path.push_front(id_startNode);

        //  First line of O/P, where we prints N along the size of shortest path
        cout << "N" << ' ' << shortest_path.size() << "\n";
    }

    // Declaring an int variable storing the size of the path
    int size = shortest_path.size();

    // The following snippet of code prints W along with lat / lon of in-path vertices, 
    // as it gets acknowledgement(A) from the user
    if (size != 0)
    {
        long long final_lat, final_lon;

        // Traversing over the shortest path to print lat/lon of every vertex
        for (auto &nodes : shortest_path)
        {
            // Take input from user
            cin >> user_given_flag;

            // Storing the values of final longitude /latitude of the vertices which exit in the shortest path.
            final_lat = points[nodes].Latitude;
            final_lon = points[nodes].Longitude;

            if (user_given_flag == A_char) // Check if user inputted flag is 'A' or not.
            {
                // OUtput formal --> W Latitude Longitude
                cout << "W" << ' ' << final_lat << ' ' << final_lon << endl;
            }
        }

        // Calling function "program_end" to take acknowledgement from user and end the program
        program_end();
    }

    return 0;
}

void readGraph(string filename, WDigraph &graph, unordered_map<int, Point> &points)
{
    /*
        Description : This function reads the file and builds the graph. In our assignment,
                      it builds a graph based on the "edmonton-roads-2.0.1.txt" file.

        Arguements :
           i) string filename :
           ii) WDigraph &graph : An instance of class WDigraph
           iii) unordered_map<int, Point> &points : Unordered_map storing the lat/lon of the vertices

        Returns :
         void : (none)


    */

    // ifstream is used to open and read the input file
    ifstream input_file;
    input_file.open(filename);

    // Declaring string variables which will store the input line
    string input_line;
    string diff_sections;

    // Unknown input lines, so i used a while loop
    while (getline(input_file, input_line))
    {
        if (input_line == "") // break the loop if input line if empty

            break;

        else
        {
            // Reads different sections of input_line
            stringstream line(input_line);

            int str = input_line.find("E"); // Finding location of E

            int str2 = input_line.find("V"); // Finding loaction of V

            getline(line, diff_sections, ','); // 1st part of input_line

            // If it's an edge, then follow this format : E,start,end,name
            if (str == 0)
            {
                // Reading the start of an edge, and also converting it from string to integer
                getline(line, diff_sections, ',');
                int edge_start = stoi(diff_sections);

                // Reading the end of an edge, and converting it from string to integer
                getline(line, diff_sections, ',');
                int edge_end = stoi(diff_sections);

                // Add the edge_start, end_edge and manhattan distance between them to the graph
                graph.addEdge(edge_start, edge_end, manhattan(points[edge_start], points[edge_end]));
            }

            // If it's a vertex, then follow this format : V, ID, Latitude, Longitude
            else if (str2 == 0)
            {

                // Reading the id of the vertex, and converting it from string to integer
                getline(line, diff_sections, ',');
                int vertex_id = stoi(diff_sections);

                // Adding the vertex to the graph
                graph.addVertex(vertex_id);

                // Reading the latitude and converting it to long long type
                getline(line, diff_sections, ',');
                long double Latitude_raw = stold(diff_sections);
                long long Latitude = static_cast<long long>(Latitude_raw * 100000);

                // Reading the latitude and converting it to long long type
                getline(line, diff_sections, ',');
                long double lon_raw = stold(diff_sections);
                long long Longitude = static_cast<long long>(lon_raw * 100000);

                // Storing the vertex_id along with its lat/lon in the struct Point.
                points[vertex_id] = Point(Latitude, Longitude);
            }
        }
    }

    // Close the input file
    input_file.close();

    return;
}

void Client_input(int &id_startNode, int &id_endNode, unordered_map<int, Point> &points)
{
    /*
        Description : This function takes input from the user.The correct input format is :
                      R start_Lat start_Lon End_Lat End_Lon.
                       
        Arguements :

           i) int &id_startNode : GIves us the ID of starting NOde.

           ii) int &id_endNode : Gives us the ID of end NOde.

           iii) unordered_map<int, Point> &point : Unordered_map storing the lat/lon of the nodes
                                                   

        Returns :
         void : (none)


    */

    // declaring two point variables storing start_point and the end_point
    Point start_point, end_point;

    // Taking input from the user
    char character_R;
    cin >> character_R;

    if (character_R == R_char) // If the input is "R", then it's a valid request
    {
        // Declaring start_Latitude and taking it as input from user
        long long start_Latitude;
        cin >> start_Latitude;

        // Declaring start_longitude and taking it as input from user
        long long start_longitude;
        cin >> start_longitude;

        // Stores the Start point as inputted by the user
        start_point = Point(start_Latitude, start_longitude);

        // Vertex Id of the Start Point
        id_startNode = Nearest_Vertex(start_point, points);

        // Declaring end_Latitude and taking it as input from user
        long long end_Latitude;
        cin >> end_Latitude;

        // Declaring end_longitude and taking it as input from user
        long long end_longitude;
        cin >> end_longitude;

        // Stores the End Point as inputted by the user
        end_point = Point(end_Latitude, end_longitude);

        // Vertex Id of end point
        id_endNode = Nearest_Vertex(end_point, points);
    }
}

void if_no_path(list<int> &shortest_path, unordered_map<int, PIL> tree, int id_endNode)
{
    /*
        Description : Determines if a path exists between two user inputted vertices. 
                      If no path exists, then end the program.

        Arguements :

             i) list<int> &path : It's a list which contains the least cost path

             ii) unordered_map<int, PIL> tree : Unordered_map storing the latitude and 
                                                longitude of the concerned vertices
             
             iii) int id_endNode : int variable which gives us the ID of End Node.

        Returns :
         void : (none)


    */

    // This if conditions checks if a path exists between two user given vertices.
    if (tree.find(id_endNode) == tree.end())
    {
        // Print N 0 , if there's no path
        cout << "N 0\n";

        // End program, once acknowledged from the user
        program_end();
        return;
    }

    return;
}

long long manhattan(const Point &pt1, const Point &pt2)
{
    /*
        Description : This function determines the manhattan distance between 
                      two given vertices.

        Arguements :
            i) const Point &pt1 : One of the vertices and the pt1 stores the latitude and 
                                  longitude of the that vertex

            ii)  const Point &pt2 : Another vertex and pt2 stores the latitude and 
                                   longitude of the that vertex

        Returns :
          
           long long : returns the final manhattan distance 

    */

    // Storing absolute distance bewteen the latitudes in abs_lat
    long long abs_lat = abs(pt1.Latitude - pt2.Latitude);

    // Storing absolute distance bewteen the longitudes in abs_lon
    long long abs_lon = abs(pt1.Longitude - pt2.Longitude);

    // Add abs_lat and abs_lon to get the final manhattan distance
    long long manhattan_dist = abs_lat + abs_lon;

    return manhattan_dist;
}

int Nearest_Vertex(Point &location, unordered_map<int, Point> &points)
{
    /*
        Description : This function finds the nearest vertex to the user inputted start 
                      and end vertex.

        Arguements :

            i) Point &location : It gives us the map's latitude and longitude

            ii) unordered_map<int, Point> &points : Unordered_map storing the latitude and longitude
                                                    of the vertices

        Returns :

            int : retursn the nearest node / vertex to the provided point.
    */

    // Declaring a pair to store the nearest vertex the manhattan distance
    pair<long long, int> nearest_node;

    // This variable acts as a flag.
    int checker = 1;

    // Traversing through all the points
    for (auto &nodes : points)
    {
        // Stores the manhattan distance with the associated nearest node.
        long long nearest_point = manhattan(nodes.second, location);

        // By default, first vertex is always the closest.
        if (checker == 1 || nearest_point < nearest_node.first)
        {
            nearest_node.second = nodes.first;
            nearest_node.first = nearest_point;

            // Flag if its the first vertex
            if (checker == 1)
            {
                checker = 0;
            }
        }
    }
    return nearest_node.second;
}

void program_end()
{
    /*
        Description : This funtion end the program whenever called

        Arguements :
            void : (none)

        Returns :
          void : (none)


    */

    // Taking input from the user to end the program
    cin >> prog_end_flag;

    // I input == 'A', then print E and end the program.
    if (prog_end_flag == A_char)
    {
        cout << "E" << '\n';
    }
}
