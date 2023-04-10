#include "Nodes.h"
#include <iostream>
#include <cmath>
Node::Node() = default;

Node::Node(const int x, const int y)
{
    this->x = x;
    this->y = y;
}

std::vector<Node*> get_neighbours(Node* n, const int ymax)
{
    std::vector<Node*> neighbours;
	neighbours.push_back(n - 1);
	neighbours.push_back(n + 1);
	neighbours.push_back(n - ymax);
	neighbours.push_back(n + ymax);
	neighbours.push_back(n - ymax - 1);
    neighbours.push_back(n - ymax + 1);
	neighbours.push_back(n + ymax - 1);
	neighbours.push_back(n + ymax + 1);
	return neighbours;
}

bool in_Nodes(int x, int y, const int xmax, const int ymax)
{
    return (x > 0 && y > 0 && x < xmax && y < ymax);
}

double calc_distance(Node* node1, Node* node2)
{
    double d;
    d = sqrt(pow(node1->x - node2->x, 2) + pow(node1->y - node2->y, 2));
	return d;
}
