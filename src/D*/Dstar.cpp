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
    double h = 999999;
    double k = 999999;
    std::string state = "new";
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
bool temp = false;

bool in_Nodes(int x, int y)
{
    return (x > 0 && y > 0 && x < xmax && y < ymax);
}

double calc_distance(Node* node1, Node* node2)
{
    double d;
    d = sqrt(pow(node1->x - node2->x, 2) + pow(node1->y - node2->y, 2));
	return d;
}

void insert(Node* j, std::deque<Node*>& openlist, double newh)
{
    if(j->state == "new")
    {
        j->k = newh;
        openlist.push_back(j);
    }
    else if(j->state == "open")
        j->k = std::min(j->k, newh);
    else
    {
        j->k = std::min(j->h, newh);
        openlist.push_back(j);
    }
    j->h = newh;
    j->state = "open";
}

void dstarplan(std::deque<Node*>& openlist)
{
    if(openlist.size() == 0)
    {
        return;
    }
    auto minn = openlist.begin();
    for(auto i = openlist.begin(); i != openlist.end(); ++i)
    {
        if((*i)->k < (*minn)->k)
            minn = i;
    }
    Node* curr = *minn;
    curr->state = "close";
    openlist.erase(minn);
    if(curr->k < curr->h )
    {
        for(Node* j : get_neighbours(curr))
        {
            if(not in_Nodes(j->x, j->y))
                continue;
            else if(j->state == "obstacle")
                continue;
            else if( curr->h > j->h + calc_distance(curr, j))
            {
                curr->h = j->h + calc_distance(curr, j);
                curr->parent = j;
            }
        }
    }
    if(curr->k == curr->h)
    {
        for(Node* j : get_neighbours(curr))
        {
            if(not in_Nodes(j->x, j->y))
                continue;
            else if(j->state == "obstacle")
                continue; 
            else if(j->state == "new" || (j->parent == curr && j->h != curr->h + calc_distance(curr, j)) || (j->parent != curr && j->h > curr->h + calc_distance(curr, j)))
            {
                j->parent = curr;
               insert(j, openlist, curr->h + calc_distance(curr, j));
            }
        }
    }
    else
    {
        for(Node* j : get_neighbours(curr))
        {
            if(j->state == "obstacle")
                continue;
            else if(j->state == "new" || (j->parent == curr && j->h != curr->h + calc_distance(curr, j)))
            {
                j->parent = curr;
                insert(j, openlist, curr->h + calc_distance(curr, j));
            }
            else if(j->parent != curr && j->h > curr->h + calc_distance(curr, j))
            {
                insert(curr, openlist, curr->h);
            }
            else if(j->parent != curr && curr->h > j->h + calc_distance(curr, j) && j->state == "close" && j->h > curr->k)
            {
                insert(j, openlist, j->h);
            }
        }
    }
}

void dynamic(Node* ni,Node* ig, Node* robot)
{
    while(true)
    {
        Node* p = ni + rand() % 30 * ymax + rand() % 30;
        if(p != ig && p != robot && p!= robot->parent)
        {
            p->h = 999999;
            p->k = 999999;
            p->state = "obstacle";
            std::vector<int> x {p->x};
            std::vector<int> y {p->y};
            plt::plot(x, y, ".k");
            break;
        }
    }
}

std::tuple<std::vector<int>, std::vector<int>> calc_path(Node* ni, int startx, int starty, int goalx, int goaly)
{
    Node* p = ni + startx * ymax + starty;
    std::vector<int> px;
    std::vector<int> py;
    while(p->x != goalx || p->y != goaly)
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
    //srand((unsigned int) time(NULL));
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
        for(int i = 0; i < ox.size(); ++i)
        {
            (ni + *(ox.begin() + i) * ymax + *(oy.begin() + i))->state = "obstacle";
        }
        std::vector<int> vsx ={startx};
        std::vector<int> vsy ={starty};
        std::vector<int> vgx ={goalx};
        std::vector<int> vgy ={goaly};
        plt::plot(vsx, vsy, "og");
        plt::plot(vgx, vgy, ".k");
        plt::grid(true);
        plt::axis("equal");
        

        std::deque<Node*> openlist;
        auto & ol = openlist;

        Node* is = ni + startx * ymax + starty;
        Node* ig = ni + goalx * ymax + goaly;

        ig->h = 0;
        ig->k = 0;
        
        openlist.push_back(ig);
        ig->state = "open";

        while(true)
        {
            dstarplan(ol);
            if(is->state == "close")
            {
                findpath = true;
                break;
            }  
        }
        
        if(findpath == true)
        {
            std::cout<<"find path!"<<'\n';
            std::tuple<std::vector<int>, std::vector<int>> tpp = calc_path(ni, startx, starty, goalx, goaly);
            std::vector<int> px = std::get<0>(tpp);
            std::vector<int> py = std::get<1>(tpp);
            plt::plot(px, py, "-r");
        }
        else
        {
            std::cout<<"can't find path!"<<'\n';
            return -1;
        }
        Node* robot = is;
        std::vector<int> rx {is->x};
        std::vector<int> ry {is->y};
        while(robot != ig)
        {
            for(int i = 0; i < 10; ++i)
            {
                dynamic(ni, ig, robot);
            }
            if(robot->parent->h == 999999)
            {
                for(int i = 0; i < 900; ++i)
                {
                    if((((ni + i)->parent) == (robot->parent)) && (ni + i)->state != "obstacle")
                    {
                        insert(ni + i, ol, 999999);
                    }
                }
                if(openlist.size() != 0)
                {
                    auto minn = openlist.begin();
                    for(auto i = openlist.begin(); i != openlist.end(); ++i)
                        {
                            if((*i)->k < (*minn)->k)
                                minn = i;
                        }
                    while(robot->h > (*minn)->k)
                    {
                        dstarplan(ol);
                        if(openlist.size() != 0)
                        {
                        for(auto i = openlist.begin(); i != openlist.end(); ++i)
                        {
                            if((*i)->k < (*minn)->k)
                                minn = i;
                        }
                        }
                    }
                }
                
            }
            robot = robot->parent;
            rx.push_back(robot->x);
            ry.push_back(robot->y);
            plt::plot(rx, ry, "-b");
            plt::pause(0.1);
                
        }
        plt::show();
    }
}