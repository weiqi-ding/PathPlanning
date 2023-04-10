#include "matplotlibcpp.h"
#include <iostream>
#include <vector>
#include <queue>

namespace plt = matplotlibcpp;

struct Node
{
    Node() = default;
    Node(const int x, const int y)
    {
        this->x = x;
        this->y = y;
    }
    Node* parent = nullptr;
    double g = 999999;
    double h = 999999;
    double cost = 999999;
    std::string state = "";
    int x = 0;
	int y = 0;
};

const int xmax = 30;
const int ymax = 30;

std::vector<Node*> get_neighbours(Node* n)
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

bool findpath = false;

bool in_Nodes(int x, int y)
{
    return (x > 0 && y > 0 && x < xmax && y < ymax);
}

double calc_g(Node* node1, Node* node2)
{
    double g;
    g = sqrt(pow(node1->x - node2->x, 2) + pow(node1->y - node2->y, 2));
	return g;
}

double calc_h(Node* node1, Node* node2)
{
    double h;
    h = sqrt(pow(node1->x - node2->x, 2) + pow(node1->y - node2->y, 2));
	return h;
}

bool isobstacle(Node* n, std::vector<int> ox, std::vector<int> oy)
{
    for(int i = 0; i < ox.size(); ++i)
        if(ox[i] == n->x && oy[i] == n->y)
            return true;
    return false;
}

void astarplan(Node* ni, int startx, int starty, int goalx, int goaly, std::vector<int> ox, std::vector<int> oy)
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
        for (Node* i : get_neighbours(curr))
        {
            if(not in_Nodes(i->x, i->y))
                continue;
            else if(isobstacle(i, ox, oy))
                continue; 
            else if(std::find(closelist.begin(), closelist.end(), i) != closelist.end())
                continue;
            else if(std::find(openlist.begin(), openlist.end(), i) != openlist.end())
            {
                if(curr->g + calc_g(i, curr) < i->g)
                {
                    i->g = curr->g + calc_g(i, curr);
                    i->cost = i->g + i->h;
                    i->parent = curr;
                }
            }
            else
            {
                i->g = curr->g + calc_g(i, curr);
                i->h = calc_h(i, ig);
                i->cost = i->g + i->h;
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
    if (not in_Nodes(startx, starty) || not in_Nodes(goalx, goaly))
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
        astarplan(ni, startx, starty, goalx, goaly, ox, oy);
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