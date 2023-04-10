#include <string>
#include <vector>
#ifndef _Nodes_H_
#define _Nodes_H_

struct Node
{
    Node();
    Node(const int x, const int y);
    Node* parent = 0;
    double cost = 999999;
    std::string state = "";
    int x = 0;
	int y = 0;
};

std::vector<Node*> get_neighbours(Node* n, const int ymax);

bool in_Nodes(int x, int y, const int xmax, const int ymax);

double calc_distance(Node* node1, Node* node2);

#endif