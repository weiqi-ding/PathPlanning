#ifndef _RANDOM_TREE_H_
#define _RANDOM_TREE_H_
#include <vector>


struct Node
{
    Node();
    Node(const double x, const double y);
    std::size_t parent = 0;
    double x = 0;
	double y = 0;
    double cost = 0;
};

struct obstacle
{
    obstacle();
    obstacle(const double x, const double y, const double r);
    double x = 0;
    double y = 0;
    double r = 0;
};

double calc_distance(Node n1, Node n2);

bool reach(Node b, Node ng);

bool inobstacle(Node n1, Node n2, std::vector<obstacle> obstacles);

void drawcircle(double x, double y, double r);

bool intree(Node p, std::vector<Node>& tree);

std::size_t findnear(Node p, std::vector<Node>& tree);

#endif