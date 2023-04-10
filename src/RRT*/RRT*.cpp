#include "matplotlibcpp.h"
#include "Random_tree.h"
#include <iostream>
#include <vector>
#include <ctime>

namespace plt = matplotlibcpp;

const double step = 1.0;
const double checkr = 10.0;

bool neartonew(Node p, std::size_t near, std::vector<Node>& tree, std::vector<obstacle> obstacles)
{
    bool b = false;
    double nx = step / calc_distance(p, tree[near]) * (p.x - tree[near].x) + tree[near].x;
    double ny = step / calc_distance(p, tree[near]) * (p.y - tree[near].y) + tree[near].y;
    Node n = Node(nx, ny);
    if(not inobstacle(tree[near], n, obstacles))
    {
        n.parent = near;
        n.cost = calc_distance(tree[near], n) + tree[near].cost;
        b = true;
    }
    for(std::size_t i = 0; i < tree.size(); ++i)
    {
        if(calc_distance(tree[i], n) <= checkr)
        {
            if(tree[i].cost + calc_distance(tree[i], n) < tree[near].cost + calc_distance(tree[near], n))
            {
                if(not inobstacle(tree[i], n, obstacles))
                {
                    n.parent = i;
                    n.cost = calc_distance(tree[i], n) + tree[i].cost;
                    b = true;
                }
            }
        }
    }
    if(b == true)
        tree.push_back(n);
    return b;
}

void rewire(std::vector<Node>& tree, std::vector<obstacle> obstacles)
{
    for(auto i : tree)
    {
        if(calc_distance(i, tree.back()) <= checkr)
        {
            if(i.cost > tree.back().cost + calc_distance(i, tree.back()))
            {
                i.parent = tree.size() - 1;
                i.cost = tree.back().cost + calc_distance(i, tree.back());
            }
        }
    }
}

void rrtstarplan(std::vector<Node>& tree, Node ng, std::vector<obstacle> obstacles)
{
    while(not reach(tree.back(), ng))
    {
        Node p;
        do
        {
            p = Node((rand() % 30), (rand() % 30));
        } while (intree(p, tree));
        std::size_t near = findnear(p, tree);
        if(neartonew(p, near, tree, obstacles))
        {
            std::vector<double> vpx {tree[near].x};
            std::vector<double> vpy {tree[near].y};
            plt::plot(vpx, vpy, ".k");
            plt::pause(0.001);
            rewire(tree, obstacles);
        }
    }
}

void findpath(std::vector<Node>& tree, Node ns, Node ng)
{
    std::vector<double> vx = {ng.x};
    std::vector<double> vy = {ng.y};
    Node i = tree.back();
    while(i.x != ns.x && i.y != ns.y)
    {
        vx.push_back(i.x);
        vy.push_back(i.y);
        i = tree[i.parent];
    }
    vx.push_back(ns.x);
    vy.push_back(ns.y);
    plt::plot(vx, vy, "-r");
}

int main()
{
    srand((unsigned int) time(NULL));
    double startx = 4;
    double starty = 3;
    double goalx = 25;
    double goaly = 25;
    Node ns = Node(startx, starty);
    Node ng = Node(goalx, goaly);
    std::vector<Node> tree = {ns};
    std::vector<Node>& tr = tree;
    obstacle o1 = obstacle(10, 10, 3);
    obstacle o2 = obstacle(15, 15, 2);
    obstacle o3 = obstacle(18, 18, 3);
    std::vector<obstacle> obstacles = {o1, o2, o3};
    std::vector<double> vsx ={startx};
    std::vector<double> vsy ={starty};
    std::vector<double> vgx ={goalx};
    std::vector<double> vgy ={goaly};
    plt::plot(vsx, vsy, "og");
    plt::plot(vgx, vgy, "og");
    plt::grid(true);
    plt::axis("equal");
    for(auto o: obstacles)
        drawcircle(o.x, o.y, o.r);
    rrtstarplan(tr, ng, obstacles);
    findpath(tr, ns, ng);
    plt::show();
}