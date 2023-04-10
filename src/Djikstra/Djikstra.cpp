#include "matplotlibcpp.h"
#include <iostream>
#include <vector>
#include <queue>
#include "Nodes.h"

namespace plt = matplotlibcpp;



const int xmax = 30;
const int ymax = 30;



bool findpath = false;



bool isobstacle(Node* n, std::vector<int> ox, std::vector<int> oy)
{
    for(int i = 0; i < ox.size(); ++i)
        if(ox[i] == n->x && oy[i] == n->y)
            return true;
    return false;
}

void djiplan(Node* ni, int startx, int starty, int goalx, int goaly, std::vector<int> ox, std::vector<int> oy)
{
    Node* is = ni + startx * ymax + starty;
    Node* ig = ni + goalx * ymax + goaly;
    is->cost = 0;
    Node* curr = is;
    std::deque<Node*> openlist;
    std::deque<Node*> closelist;
    while (true)
    {
        if(curr == ig)
        {
            findpath = true;
            break;
        }
        std::vector<int> px = {curr->x};
        std::vector<int> py = {curr->y};
        plt::plot(px, py, "xc");
        plt::pause(0.001);
        for (Node* i : get_neighbours(curr, ymax))
        {
            if(not in_Nodes(i->x, i->y, xmax, ymax))
                continue;
            else if(isobstacle(i, ox, oy))
                continue; 
            else if(std::find(closelist.begin(), closelist.end(), i) != closelist.end())
                continue;
            else if(std::find(openlist.begin(), openlist.end(), i) != openlist.end())
            {
                if(curr->cost + calc_distance(i, curr) < i->cost)
                {
                    i->cost = curr->cost + calc_distance(i, curr);
                    i->parent = curr;
                }
            }
            else
            {
                i->cost = curr->cost + calc_distance(i, curr);
                i->parent = curr;
                openlist.push_back(i);
            }
        }
        if(openlist.size() == 0)
        {
            std::cout<<"openlist is empty"<<'\n';
            break;
        }
        closelist.push_back(curr);
        auto minn = openlist.begin();
        for(auto i = openlist.begin(); i != openlist.end(); ++i)
        {
            if((*i)->cost < (*minn)->cost)
                minn = i;
        }
        curr = *minn;
        openlist.erase(minn);
    }
}

std::tuple<std::vector<int>, std::vector<int>> calc_path(Node* ni, int startx, int starty, int goalx, int goaly)
{
    Node* p = ni + goalx * ymax + goaly;
    std::vector<int> px;
    std::vector<int> py;
    while(p->x != startx || p->y != starty)
    {
        px.push_back(p->x);
        py.push_back(p->y);
        p = p->parent;
    }
    px.push_back(p->x);
    py.push_back(p->y);
    return std::make_tuple(px, py);
}

int main()
{
    Node Nodes[xmax][ymax];
    for (int i = 0; i < xmax; ++i)
        for (int j = 0; j < ymax; ++j)
            Nodes[i][j] = Node(i, j);
    Node* ni = &Nodes[0][0];
    const int startx = 4;
    const int starty = 3;
    const int goalx = 25;
    const int goaly = 25;
    if (not in_Nodes(startx, starty, xmax, ymax) || not in_Nodes(goalx, goaly, xmax, ymax))
    {
        std::cout<<"Out of range!"<<'\n';
        return -1;
    }
    else
    {
        std::vector<int> ox, oy;
        for(int i = 0; i < xmax - 1; ++i)
        {
            int j = 0;
            ox.push_back(i);
            oy.push_back(j);
        }
        for(int j = 0; j < ymax - 1; ++j)
        {
            int i = xmax - 1;
            ox.push_back(i);
            oy.push_back(j);
        }
        for(int i = 1; i < xmax; ++i)
        {
            int j = ymax - 1;
            ox.push_back(i);
            oy.push_back(j);
        }
        for(int j = 1; j < ymax; ++j)
        {
            int i = 0;
            ox.push_back(i);
            oy.push_back(j);
        }
        for(int i = 0; i < 20; ++i)
        {
            int j = 9;
            ox.push_back(i);
            oy.push_back(j);
        }
        for(int i = 9; i < 30; ++i)
        {
            int j = 19;
            ox.push_back(i);
            oy.push_back(j);
        }
        plt::plot(ox, oy, ".k");
        std::vector<int> vsx ={startx};
        std::vector<int> vsy ={starty};
        std::vector<int> vgx ={goalx};
        std::vector<int> vgy ={goaly};
        plt::plot(vsx, vsy, "og");
        plt::plot(vgx, vgy, ".k");
        plt::grid(true);
        plt::axis("equal");
        djiplan(ni, startx, starty, goalx, goaly, ox, oy);
        if(findpath == true)
        {
            std::cout<<"find path!"<<'\n';
            std::tuple<std::vector<int>, std::vector<int>> tpp = calc_path(ni, startx, starty, goalx, goaly);
            std::vector<int> px = std::get<0>(tpp);
            std::vector<int> py = std::get<1>(tpp);
            plt::plot(px, py, "-r");
            plt::show();
            
        }
        else
            std::cout<<"can't find path!"<<'n';
    }
}