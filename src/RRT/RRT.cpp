#include <iostream>
#include "matplotlibcpp.h"
#include <ctime>
#include "Random_tree.h"

namespace plt = matplotlibcpp;

const double xmax = 30.0;
const double ymax = 30.0;
const double step = 1.0;

bool grow(Node p, std::size_t near, std::vector<Node>& tree, std::vector<obstacle> obstacles)
{
    double nx = step / calc_distance(p, tree[near]) * (p.x - tree[near].x) + tree[near].x;
    double ny = step / calc_distance(p, tree[near]) * (p.y - tree[near].y) + tree[near].y;
    Node n = Node(nx, ny);
    if(not inobstacle(n, tree[near], obstacles))
    {
        n.parent = near;
        tree.push_back(n);
        return true;
    }
    return false;
}


void rrtplan(std::vector<Node>& tree, Node ng, std::vector<obstacle> obstacles)
{
    while(not reach(tree.back(), ng))
    {
        Node p;
        do
        {
            p = Node((rand() % 30), (rand() % 30));
        } while (intree(p, tree));
        std::size_t near = findnear(p, tree);
        if(grow(p, near, tree, obstacles))
        {
            std::vector<double> vpx {tree[near].x};
            std::vector<double> vpy {tree[near].y};
            plt::plot(vpx, vpy, ".k");
            plt::pause(0.001);
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
    rrtplan(tr, ng, obstacles);
    findpath(tr, ns, ng);
    plt::show();
}