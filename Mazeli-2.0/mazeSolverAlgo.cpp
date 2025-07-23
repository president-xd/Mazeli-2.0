# include "mainApp.h"
# include "maze.h"


/////////all functions and classes related to solving Maze

bool isinbound(int x, int y, int xmax, int ymax) {
    return (0 <= x && x < xmax && 0 <= y && y < ymax);
}

//updates search path vector used to render nodes on screen.
//throws exception if terminate thread is issued
void updatesearchpath(Maze* m, std::pair<int, int> node, int depth) {
    //terminate thread user clicked stop simulation
    if (m->simulstat == false) {
        throw std::string("Terminate thread issued\n");
    }
    m->maxsearchpathdepth = std::max(m->maxsearchpathdepth, depth);
    m->searchpath.push_back({ node.first * m->grid_w,node.second * m->grid_h, depth });
    m->Refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

//updates final path vector used to render nodes on screen.
//throws exception if terminate thread is issued
void updatefinalpath(Maze* m, std::pair<int, int> node) {
    //terminate thread user clicked stop simulation
    if (m->simulstat == false) {
        throw std::string("Terminate thread issued\n");
    }
    m->finalpath.push_back({ node.first * m->grid_w,node.second * m->grid_h });
    m->Refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void solvemazeBFS(Maze* m) {

    int xmax = m->grid_col, ymax = m->grid_row;
    std::pair<int, int> start = { m->startCoord.x,m->startCoord.y };
    std::pair<int, int> goal = { m->goalCoord.x,m->goalCoord.y };
    auto maze = m->maze; //make copy and use as visited => if 1 then donot visit

    std::map<std::pair<int, int>, std::pair<int, int>> parent; // store parent of every node
    //parent[start] is not defined => retrace upto here

    //x , y , color
    std::queue<std::tuple<int, int, int>> Q;
    Q.push({ start.first,start.second,0 });
    maze[start.second][start.first] = 1;

    int x, y, dir[4][2] = { {1, 0},{0,-1} ,{-1,0},{0,1} };

    std::pair<int, int> p;
    int depth;
    int nodes_exp = 0;
    //init interface
    m->updatealgoinfo(wxT("0"), wxT("-"));
    updatesearchpath(m, start, 0);
    nodes_exp++;
    m->updatealgoinfo(wxString::Format("%d", nodes_exp), "");

    while (!Q.empty()) {
        std::tie(p.first, p.second, depth) = Q.front();
        Q.pop();

        if (p == goal) {
            //goal found
            int final_path_len = 0;
            while (true) {
                p = parent[p];
                if (p == start) {
                    final_path_len++;
                    m->updatealgoinfo("", wxString::Format("%d", final_path_len));
                    break;
                }
                //update on interface
                try {
                    updatefinalpath(m, p);
                    final_path_len++;
                    m->updatealgoinfo("", wxString::Format("%d", final_path_len));
                }
                catch (...) {
                    //terminate
                    return;
                }

            }
            break;
        }
        for (int i = 0; i < 4; i++) {
            x = p.first + dir[i][0];
            y = p.second + dir[i][1];
            //in bounds and not visited
            if (isinbound(x, y, xmax, ymax) && maze[y][x] == 0) {
                //mark as visited, add to fringe
                maze[y][x] = 1;
                Q.push({ x,y,depth + 1 });
                parent[{x, y}] = p;

                //update on interface
                try {
                    updatesearchpath(m, { x,y }, depth + 1);
                    nodes_exp++;
                    m->updatealgoinfo(wxString::Format("%d", nodes_exp), "");
                }
                catch (...) {
                    //terminate this thread
                    return;
                }

            }
        }
    }

}

void solvemazeBiBFS(Maze* m) {
    int xmax = m->grid_col, ymax = m->grid_row;
    std::pair<int, int> start = { m->startCoord.x,m->startCoord.y };
    std::pair<int, int> goal = { m->goalCoord.x,m->goalCoord.y };
    auto mazef = m->maze; //make copy and use as visited => if 1 then donot visit
    auto mazeb = m->maze; //make copy and use as visited => if 1 then donot visit

    std::map<std::pair<int, int>, std::pair<int, int>> parentf, parentb; // store parent of every node
    //parent[start] is not defined => retrace upto here

    //x , y , color
    std::queue<std::tuple<int, int, int>> Qf, Qb;
    Qf.push({ start.first,start.second,0 });
    mazef[start.second][start.first] = 1;

    Qb.push({ goal.first,goal.second,0 });
    mazeb[goal.second][goal.first] = 1;

    int xf, yf, xb, yb, dir[4][2] = { {1, 0},{0,-1} ,{-1,0},{0,1} };

    std::pair<int, int> pf, pb;
    int depthf, depthb;
    int nodes_exp = 2;
    //init interface
    m->updatealgoinfo(wxT("2"), wxT("-"));

    while (!(Qf.empty() || Qb.empty())) {
        std::tie(pf.first, pf.second, depthf) = Qf.front();
        Qf.pop();
        std::tie(pb.first, pb.second, depthb) = Qb.front();
        Qb.pop();

        for (int i = 0; i < 4; i++) {
            xf = pf.first + dir[i][0];
            yf = pf.second + dir[i][1];
            //in bounds and not visited
            if (isinbound(xf, yf, xmax, ymax) && mazef[yf][xf] == 0) {
                //mark as visited, add to fringe
                mazef[yf][xf] = 1;
                Qf.push({ xf,yf,depthf + 1 });
                parentf[{xf, yf}] = pf;

                //check if this is already discovered by back search
                if (mazeb[yf][xf] == 1) {
                    //intersecting node found
                    reconstruct_Bipath(m, parentf, parentb, { xf,yf }, { xf,yf });
                    return;
                }

                //update on interface
                try {
                    updatesearchpath(m, { xf,yf }, depthf + 1);
                    nodes_exp++;
                    m->updatealgoinfo(wxString::Format("%d", nodes_exp), "");
                }
                catch (...) {
                    //terminate this thread
                    return;
                }

            }

            xb = pb.first + dir[i][0];
            yb = pb.second + dir[i][1];
            //in bounds and not visited
            if (isinbound(xb, yb, xmax, ymax) && mazeb[yb][xb] == 0) {
                //mark as visited, add to fringe
                mazeb[yb][xb] = 1;
                Qb.push({ xb,yb,depthb + 1 });
                parentb[{xb, yb}] = pb;

                //check if this is already discovered by front search
                if (mazef[yb][xb] == 1) {
                    //intersecting node found
                    reconstruct_Bipath(m, parentf, parentb, { xb,yb }, { xb,yb });
                    return;
                }

                //update on interface
                try {
                    updatesearchpath(m, { xb,yb }, depthb + 1);
                    nodes_exp++;
                    m->updatealgoinfo(wxString::Format("%d", nodes_exp), "");
                }
                catch (...) {
                    //terminate this thread
                    return;
                }

            }
        }
    }
}

void reconstruct_Bipath(Maze* m, std::map<std::pair<int, int>, std::pair<int, int>>& parentf,
    std::map<std::pair<int, int>, std::pair<int, int>>& parentb, std::pair<int, int> pf,
    std::pair<int, int> pb) {

    std::pair<int, int> start = { m->startCoord.x,m->startCoord.y };
    std::pair<int, int> goal = { m->goalCoord.x,m->goalCoord.y };

    int final_path_len = 1;

    if (pf != start && pf != goal) {
        updatefinalpath(m, pf); //intersecting node
        pf = parentf[pf];
        pb = parentb[pb];
        final_path_len++;
    }

    //construct path from both direction
    bool pfvalid, pbvalid;
    while (true) {

        pfvalid = (pf != start);
        pbvalid = (pb != goal);

        if (!(pfvalid || pbvalid)) {
            m->updatealgoinfo("", wxString::Format("%d", final_path_len));
            return;
        }

        //update on interface
        try {
            if (pfvalid) {
                updatefinalpath(m, pf);
            }
            if (pbvalid) {
                updatefinalpath(m, pb);
            }
            m->updatealgoinfo("", wxString::Format("%d", final_path_len));
        }
        catch (...) {
            //terminate
            return;
        }

        if (pf != start) {
            pf = parentf[pf];
            final_path_len++;
        }
        if (pb != goal) {
            pb = parentb[pb];
            final_path_len++;
        }

    }
}

void solvemazeDFS(Maze* m) {
    int xmax = m->grid_col, ymax = m->grid_row;
    std::pair<int, int> start = { m->startCoord.x,m->startCoord.y };
    std::pair<int, int> goal = { m->goalCoord.x,m->goalCoord.y };
    auto maze = m->maze; //make copy and use as visited => if 1 then donot visit

    //x , y , neighbour , color
    std::stack<std::tuple<int, int, int, int>> S;
    S.push({ start.first,start.second,0,0 });
    maze[start.second][start.first] = 1;

    int nodes_exp = 1;
    m->updatealgoinfo(wxT("1"), wxT("-"));
    try {
        updatesearchpath(m, start, 0);
    }
    catch (...) {
        //terminate this thread
        return;
    }

    int x, y, dir[4][2] = { {1, 0},{0,-1} ,{-1,0},{0,1} };

    std::pair<int, int> p;
    int depth, neigh;


    while (!S.empty()) {
        std::tie(p.first, p.second, neigh, depth) = S.top();

        if (p == goal) {
            m->updatealgoinfo("", wxString::Format("%d", depth));
            break;
        }
        int i;
        for (i = neigh; i < 4; i++) {
            x = p.first + dir[i][0];
            y = p.second + dir[i][1];
            //in bounds and not visited
            if (isinbound(x, y, xmax, ymax) && maze[y][x] == 0) {
                //mark as visited, add to fringe
                neigh = i + 1;
                std::get<2>(S.top()) = neigh;
                maze[y][x] = 1;
                S.push({ x,y,0,depth + 1 });
                //update on interface
                try {
                    nodes_exp++;
                    m->updatealgoinfo(wxString::Format("%d", nodes_exp), "");
                    updatesearchpath(m, { x,y }, depth);
                }
                catch (...) {
                    //terminate this thread
                    return;
                }
                break;
            }
        }
        neigh = i + 1;
        //all neighbours visited
        if (neigh >= 5) {
            m->searchpath.pop_back();
            m->Refresh();
            S.pop();
        }

    }
}

//Functor for comparing two nodes based on f(n) in priority Queue for algorithms
class pQcomp {
protected:
    int gx, gy;
    int weight;
    int gparam;
    std::map<std::pair<int, int>, int>* depth;
public:
    pQcomp(int goalx, int goaly, std::map<std::pair<int, int>, int>* depth, int weight = 1, int gparam = 1) {
        gx = goalx;
        gy = goaly;
        this->depth = depth;
        this->weight = weight;
        this->gparam = gparam;
    }
    //heuristic for A* (manhaten distance)
    int mnhtndist(int x1, int y1, int x2, int y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    bool operator() (std::pair<int, int> a, std::pair<int, int> b) {
        int d1, d2, h1, h2;
        d1 = gparam * depth->at(a);
        d2 = gparam * depth->at(b);
        h1 = weight * mnhtndist(gx, gy, a.first, a.second);
        h2 = weight * mnhtndist(gx, gy, b.first, b.second);
        d1 += h1;
        d2 += h2;
        if (d1 == d2)
            return h1 > h2;
        return d1 > d2;
    }
};


//custom priority queue with decrease key for A*
class priorityQ {
    std::vector<std::pair<int, int>> pq;
    std::function<bool(std::pair<int, int>, std::pair<int, int>)> cmp;
public:
    priorityQ(const std::function<bool(std::pair<int, int>, std::pair<int, int>)>& comprator) {
        cmp = comprator;
    }
    void push(const std::pair<int, int>& val) {
        pq.push_back(val);
        std::push_heap(pq.begin(), pq.end(), cmp);
    }
    //do decrease key on given value using comparator,
    //update depth map before calling this function
    void decrease_key(const std::pair<int, int>& val) {
        //TODO : efficient O(log n)
        std::make_heap(pq.begin(), pq.end(), cmp);
    }
    bool empty() {
        return pq.empty();
    }
    std::pair<int, int> pop_top() {
        std::pop_heap(pq.begin(), pq.end(), cmp);
        auto p = pq.back();
        pq.pop_back();

        return p;
    }
};

void solvemazeheuristicHelper(Maze* m, priorityQ& pQ, std::map<std::pair<int, int>, int>& depthmap) {

    int xmax = m->grid_col, ymax = m->grid_row;
    std::pair<int, int> start = { m->startCoord.x,m->startCoord.y };
    std::pair<int, int> goal = { m->goalCoord.x,m->goalCoord.y };
    auto maze = m->maze; //make copy and use as CLOSED => if 1 then donot visit

    std::map<std::pair<int, int>, std::pair<int, int>> parent; // store parent of every node
    //parent[start] is not defined => retrace upto here


    depthmap[start] = 0;
    pQ.push(start);
    maze[start.second][start.first] = 1;

    int x, y, dir[4][2] = { {1, 0},{0,-1} ,{-1,0},{0,1} };

    std::pair<int, int> p;
    int depth;
    int nodes_exp = 0;
    m->updatealgoinfo(wxT("0"), wxT("-"));

    while (!pQ.empty()) {

        p = pQ.pop_top();
        depth = depthmap[p];
        maze[p.second][p.first] = 1; //mark node as CLOSED

        //update on interface
        try {
            updatesearchpath(m, p, depth);
            nodes_exp++;
            m->updatealgoinfo(wxString::Format("%d", nodes_exp), "");
        }
        catch (...) {
            //terminate 
            return;
        }

        if (p == goal) {
            //goal found
            int final_path_len = 0;
            while (true) {
                p = parent[p];
                if (p == start) {
                    final_path_len++;
                    m->updatealgoinfo("", wxString::Format("%d", final_path_len));
                    break;
                }
                try {
                    updatefinalpath(m, p);
                    final_path_len++;
                    m->updatealgoinfo("", wxString::Format("%d", final_path_len));
                }
                catch (...) {
                    //terminate
                    return;
                }

            }
            break;
        }
        for (int i = 0; i < 4; i++) {
            x = p.first + dir[i][0];
            y = p.second + dir[i][1];
            //in bounds and not in CLOSED
            if (isinbound(x, y, xmax, ymax) && maze[y][x] == 0) {

                //if value not in pQ (OPEN)
                if (depthmap.find({ x,y }) == depthmap.end()) {
                    depthmap[{x, y}] = depth + 1;
                    pQ.push({ x,y });
                    parent[{x, y}] = p;
                }
                //node already in pQ (OPEN) => do Relxation if new depth is smaller
                else if (depthmap[{x, y}] > depth + 1) {
                    depthmap[{x, y}] = depth + 1;
                    pQ.decrease_key({ x,y });
                    parent[{x, y}] = p; //assign new parent
                }

            }
        }
    }

}


void solvemazegreedybest(Maze* m) {
    std::map<std::pair<int, int>, int> depthmap;
    priorityQ pQ(pQcomp(m->goalCoord.x, m->goalCoord.y, &depthmap, 1, 0));
    solvemazeheuristicHelper(m, pQ, depthmap);
}

void solvemazeAstar(Maze* m) {
    std::map<std::pair<int, int>, int> depthmap;
    priorityQ pQ(pQcomp(m->goalCoord.x, m->goalCoord.y, &depthmap));
    solvemazeheuristicHelper(m, pQ, depthmap);
}

void solvemazeAstarweigh(Maze* m) {
    std::map<std::pair<int, int>, int> depthmap;
    priorityQ pQ(pQcomp(m->goalCoord.x, m->goalCoord.y, &depthmap, 5));
    solvemazeheuristicHelper(m, pQ, depthmap);
}
