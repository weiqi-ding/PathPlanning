#include "Random_tree.h"
#include <cmath>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

Node::Node() = default;

Node::Node(const double x, const double y)
{
    this->x = x;
    this->y = y;
}

obstacle::obstacle() = default;

obstacle::obstacle(const double x, const double y, const double r)
{
    this->x = x;
    this->y = y;
    this->r = r;
}

double calc_distance(Node n1, Node n2)
{
    double d;
    d = sqrt(pow(n1.x - n2.x, 2) + pow(n1.y - n2.y, 2));
	return d;
}

bool reach(Node b, Node ng)
{
    if(calc_distance(b, ng) < 2)
        return true;
    return false;
}

bool inobstacle(Node n1, Node n2, std::vector<obstacle> obstacles)
{
    for(auto i : obstacles)
    {
        double a = sqrt(pow(n1.x - i.x, 2) + pow(n1.y - i.y, 2));
        double b = sqrt(pow(n2.x - i.x, 2) + pow(n2.y - i.y, 2));
        double c = sqrt(pow(n1.x - n2.x, 2) + pow(n1.y - n2.y, 2));
        double p = (a + b + c) / 2;
        double s = sqrt(p * (p - a) * (p - b) * (p - c));
        if(2 * s / c < i.r)
            return true;
    }
    return false;
}

void drawcircle(double x, double y, double r)
{
    double pi = 3.1415926;
    std::vector<double> vx;
    std::vector<double> vy;
    for(double i = 0; i < 360; i = i + 5)
    {
        vx.push_back(x + r * cos(i / 180 * pi));
        vy.push_back(y + r * sin(i / 180 * pi));
    }
    plt::plot(vx, vy, "-b");
}

bool intree(Node p, std::vector<Node>& tree)
{
    for(auto i: tree)
        if(i.x == p.x && i.y == p.y)
            return true;
    return false;
}

std::size_t findnear(Node p, std::vector<Node>& tree)
{
    auto near = tree.size() - 1;
    for(std::size_t i = 0; i < tree.size(); ++i)
        if(calc_distance(p, tree[i]) < calc_distance(tree[near], p))
            near = i;
    return near;
}
