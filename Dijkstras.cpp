#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <string.h>
#include <algorithm>

using namespace std;

#define MAXDISTANCE 100000

class Edge;
class Node;

void Dijkstras();
string findShortestPath(string destinationId);

vector<Node *> nodesList;
vector<Edge *> edgesList;

/*
Node represent for the location or the point

Attributes:
id : Unique id for classified each different node
coordinateX : coordinate X for the node
coordinateY : coordinate Y for the node
distanceFromStart : distance of the node from the staring node
isVisited : Check if this node already peform relaxation
previous : previous connected from starting point with minimum distacen
*/
class Node
{
public:
    string id;
    int coordinateX;
    int coordinateY;
    double distanceFromStart;
    bool isVisited;
    Node *previous;

    Node()
    {
    }

    /*
    Constructor initialize all the distanceFrom start for all node to infinity
    then push the node to the nodesList
    */

    Node(string id, int coorX, int coorY)
        : id(id), coordinateX(coorX), coordinateY(coorY), isVisited(false), previous(NULL)
    {
        distanceFromStart = MAXDISTANCE;
        nodesList.push_back(this);
    }
};

/*
Edge between two node

Attributes:
node1 : Connected node 1
node2 : Connected node 2
distanceAdj : distance between nodes
*/
class Edge
{
public:
    Node *node1;
    Node *node2;
    double distanceAdj;

    /*
    Constructor find two point in the nodeList note with the node id
    */
    Edge(string node1Id, string node2Id)
    {

        for (size_t i{0}; i < nodesList.size(); i++)
        {
            if (nodesList.at(i)->id == node1Id)
            {
                node1 = nodesList.at(i);
            }
            if (nodesList.at(i)->id == node2Id)
            {
                node2 = nodesList.at(i);
            }
        }
        this->connect(this->node1, this->node2);
    }

    /*
    Connect two points by calculate the distance between two point and
    push the edge to the edgesList
    */
    void connect(Node *n1, Node *n2)
    {
        distanceAdj = sqrt(pow((n1->coordinateX - n2->coordinateX), 2) + pow((n1->coordinateY - n2->coordinateY), 2));
        cout << "Distance between points: " << distanceAdj << '\n';
        edgesList.push_back(this);
    }
};

/*
    Find the shortes path from the staring point for all other nodes
    by peform relaxation .

    Arguments:
    minNode : Node with the miminum distance from the starting point
    minDistance : Minimum distance from the starting point
    currentNode=:The node choose in the loop to find the minNode

    distance : Distance between the minNode with the connected note
    minNodeDistance : minNode distance from the starting point
*/
void Dijkstras()
{
    Node *minNode = nodesList.at(0);
    double minDistance{MAXDISTANCE};

    // Loop through all node to find the smallest path between points
    for (size_t i{0}; i < nodesList.size(); i++)
    {

        // Find the minimum distanceFromStart node

        for (size_t j{0}; j < nodesList.size(); j++)
        {
            Node *currentNode = nodesList[j];

            // // Check if the current node already be relaxation and the distanceFromStart is smaller than minNode
            if (!currentNode->isVisited && currentNode->distanceFromStart < minDistance)
            {
                // If true assign currentNode to minMode
                minNode = currentNode;
                minDistance = minNode->distanceFromStart;
            }
        }

        for (size_t k{0}; k < edgesList.size(); k++)
        {
            double distance{0};
            double minNodeDistance{0};

            // If the current edge have node 1 and havent peform relaxtion yet
            if (minNode == edgesList.at(k)->node1 && !edgesList.at(k)->node2->isVisited)
            {
                // Calculate the distance between currentNode with the connected note
                distance = minNode->distanceFromStart + edgesList.at(k)->distanceAdj;
                minNodeDistance = edgesList.at(k)->node2->distanceFromStart;

                /*
                1. If the distance between node is smaller than the currentNode distanceFromStart
                2. Assign the distance between node to the currentNode distance from start
                3. Assign edges previous to the minNode
                */
                if (distance < minNodeDistance)
                {
                    edgesList.at(k)->node2->distanceFromStart = distance;
                    edgesList.at(k)->node2->previous = minNode;
                }
            }

            // If the current edge have node 2 and havent peform relaxtion yet
            else if (minNode == edgesList.at(k)->node2 && !edgesList.at(k)->node1->isVisited)
            {
                // Calculate the distance between currentNode with the connected note
                distance = minNode->distanceFromStart + edgesList.at(k)->distanceAdj;
                minNodeDistance = edgesList.at(k)->node1->distanceFromStart;

                /*
                1. If the distance between node is smaller than the currentNode distanceFromStart
                2. Assign the distance between node to the currentNode distance from start
                3. Assign edges previous to the minNode
                */
                if (distance < minNodeDistance)
                {
                    edgesList.at(k)->node1->distanceFromStart = distance;
                    edgesList.at(k)->node1->previous = minNode;
                }
            }
        }

        // Dont peform relaxation process on minimum node in the future
        minNode->isVisited = true;
        minDistance = MAXDISTANCE;
    }
}

/*
Find the shortest path from the starting point to the destination point

Arguments:
destinationId: destination point Id
previous : Previous connected node with the shortest path for the current node
shortestPath : shortest path string

Return:
String with the shortes path from the starting point to the destination point

*/
string findShortestPath(string destinationId)
{
    Node *previous;
    string shortestPath{};

    // Find the desintation point
    for (size_t i{0}; i < nodesList.size(); i++)
    {
        if (nodesList.at(i)->id == destinationId)
        {
            previous = nodesList.at(i);
        }
    }

    cout << "Shortest Distance to " << destinationId << " : " << previous->distanceFromStart << "\n";

    // Exit the loop if the precious is null
    while (previous)
    {
        shortestPath += previous->id + "->";
        previous = previous->previous;
    }
    return shortestPath;
}

int main()
{

    // User input number of point
    int numberOfPoints{};
    cout << "Input number of points: ";
    cin >> numberOfPoints;

    string id{};
    int coordinateX{};
    int coordinateY{};

    // Input point id, coordinate X, coordinate Y
    for (int i{0}; i < numberOfPoints; i++)
    {
        cout << "\nPoint " << i + 1 << " ID: ";
        cin >> id;
        cout << "Point " << i + 1 << " coordinate X: ";
        cin >> coordinateX;
        cout << "Point " << i + 1 << " coordinate Y: ";
        cin >> coordinateY;
        Node *node = new Node(id, coordinateX, coordinateY);
    }

    // Input number of connection edges
    int numberOfConnection{};
    cout << "\nNumber of connection edges: ";
    cin >> numberOfConnection;

    // Input two connection point
    string node1{};
    string node2{};
    for (int i{}; i < numberOfConnection; i++)
    {
        cout << "\nEdges " << i + 1 << "\n";
        cout << "Node 1 ID: ";
        cin >> node1;
        cout << "Node 2 ID: ";
        cin >> node2;
        Edge *edge = new Edge(node1, node2);
    }

    string startingPointId{};
    cout << "Select the starting point ID: ";
    cin >> startingPointId;

    // Set the starting poing distanceFromStart to 0
    for (int i{}; i < nodesList.size(); i++)
    {
        if (nodesList.at(i)->id == startingPointId)
        {
            nodesList.at(i)->distanceFromStart = 0;
        }
    }

    // Call the Dijkstras function to calculate the shortest path from starting point
    Dijkstras();

    // User input the destination
    string destinationId{};
    cout << "\nSelect the destination ID: ";
    cin >> destinationId;

    // Find the shortest path
    string shortestPath = findShortestPath(destinationId);
    cout << shortestPath;

    return 0;
}
