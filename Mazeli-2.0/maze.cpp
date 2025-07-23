#include "mainApp.h"
#include "maze.h"
#include <random> // Add this include at the top of the file

std::random_device rd;
std::mt19937 g(rd());
//extern std::ofstream flog;

std::vector<std::function<void(Maze*)>> Maze::solvealgolist =
{ solvemazeBFS,solvemazeDFS,solvemazeAstar,
  solvemazegreedybest,solvemazeBiBFS

};

std::vector<std::string> Maze::solvealgonamelist =
{ "BFS","DFS","A* Consistent","Greedy Best First","Bidirectional BFS"
};

enum btnid {
    BTN_CLEARALL = 99, BTN_CLEARSEARCH, BTN_SETSTART, BTN_SOLVEMAZE, BTN_SETGOAL, BTN_GENMAZE, BTN_HELP,
    CHOICE_ALGO,
    CST_TOGGLESSBTN
};

//////////////////////////////////////////////////////////////////////////

void renderStart(wxDC& dc, const wxPoint& point, const wxSize& size) {
    dc.SetBrush(*wxYELLOW_BRUSH);
    dc.DrawRectangle(point, size);
}

void renderGoal(wxDC& dc, const wxPoint& point, const wxSize& size) {
    dc.SetBrush(*wxGREEN_BRUSH);
    dc.DrawRectangle(point, size);
}

//////////////////////////////////////////////////////////////////

bool Maze::setStartGoal(const wxPoint& coord) {
    if (setstart && coord != goalCoord) {
        maze[coord.y][coord.x] = 0;
        startCoord = coord;
        setstart = false;
        return true;
    }
    if (setgoal && coord != startCoord) {
        maze[coord.y][coord.x] = 0;
        goalCoord = coord;
        setgoal = false;
        return true;
    }
    return false;
}
bool Maze::isinbound(const wxPoint& coord) {
    return (0 <= coord.x && coord.x < grid_col && 0 <= coord.y && coord.y < grid_row);
}
bool Maze::markCell(wxPoint coord) {
    //return false if already marked or coordinates out of bound
    coord.x = coord.x / grid_w;
    coord.y = coord.y / grid_h;

    //if not in bonuds
    if (!isinbound(coord))
        return false;
    //mark start/goal if action needs to be performed
    if (setStartGoal(coord))
        return true;

    if (coord == startCoord || coord == goalCoord)
        return false;
    if (maze[coord.y][coord.x] == 0) {
        obstacle.insert({ coord.x * grid_w , coord.y * grid_h });
        maze[coord.y][coord.x] = 1;
        return true;
    }
    return false;
}
bool Maze::unMarkCell(wxPoint coord) {
    //return false if already unmarked or coordinates out of bound
    coord.x = coord.x / grid_w;
    coord.y = coord.y / grid_h;
    if (coord == startCoord || coord == goalCoord)
        return false;
    if (coord.x < grid_col && coord.y < grid_row && maze[coord.y][coord.x] == 1) {
        obstacle.erase({ coord.x * grid_w , coord.y * grid_h });
        maze[coord.y][coord.x] = 0;
        return true;
    }
    return false;
}

Maze::Maze(wxWindow* parent, wxWindowID id) : wxScrolled<wxPanel>(parent, id) {
    grid_w = 20;
    grid_h = 20;
    grid_row = 35;
    grid_col = 60;
    window_w = 1000;
    window_h = 1000;
    startCoord = wxPoint(0, 0);
    goalCoord = wxPoint(grid_col - 1, grid_row - 1);
    setstart = false;
    setgoal = false;
    simulstat = false;
    maxsearchpathdepth = 1;
    currentAlgoidx = 0;

    //set custom events
    toggleSSBtnEvt.setId(CST_TOGGLESSBTN);

    initMaze();

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetVirtualSize(wxSize(window_w, window_h));
    EnableScrolling(true, true);
    SetScrollRate(10, 10);
    ShowScrollbars(wxSHOW_SB_ALWAYS, wxSHOW_SB_ALWAYS);

    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Maze::onLeftBtn));
    Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Maze::onRightBtn));
    Connect(wxEVT_MOTION, wxMouseEventHandler(Maze::onMouseMove));
}

BEGIN_EVENT_TABLE(Maze, wxScrolled<wxPanel>)
EVT_PAINT(Maze::onPaint)
END_EVENT_TABLE()


Maze::~Maze() {
    simulstat = false;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Maze::initMaze() {
    maze.clear();
    maze.resize(grid_row);
    for (auto& row : maze)
        row.resize(grid_col);
}

void Maze::clearAll(wxCommandEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    obstacle.clear();
    searchpath.clear();
    finalpath.clear();
    initMaze();
    Refresh();
}
void Maze::clearSearch(wxCommandEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    searchpath.clear();
    finalpath.clear();
    Refresh();
}
void Maze::setStart(wxCommandEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    startCoord = wxDefaultPosition;
    setstart = true;
    Refresh();
}
void Maze::setGoal(wxCommandEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    goalCoord = wxDefaultPosition;
    setgoal = true;
    Refresh();
}

void Maze::setCurrentAlgoIdx(wxCommandEvent& evt) {
    currentAlgoidx = evt.GetSelection();
}

void Maze::onMouseMove(wxMouseEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    if (evt.LeftIsDown()) {
        if (markCell(CalcUnscrolledPosition(evt.GetPosition())))
            Refresh();
    }
    else if (evt.RightIsDown()) {
        if (unMarkCell(CalcUnscrolledPosition(evt.GetPosition())))
            Refresh();
    }
}
void Maze::onLeftBtn(wxMouseEvent& evt) {

    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    //convert client coordinates to virtual position(position used by wxDC)
    if (markCell(CalcUnscrolledPosition(evt.GetPosition())))
        Refresh();
}
void Maze::onRightBtn(wxMouseEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;
    //convert client coordinates to virtual position(position used by wxDC)
    if (unMarkCell(CalcUnscrolledPosition(evt.GetPosition())))
        Refresh();
}
void Maze::logmaze() {
    //log to file
    /*
    for (auto& row : maze) {
        for (bool i : row)
            flog << i << ' ';
        flog << '\n';
    }
    flog << '\n';
    flog << "start " << startCoord.x << " " << startCoord.y << '\n';
    flog << "goal " << goalCoord.x << " " << goalCoord.y << '\n';
    /**/
}

void startSimulation(Maze* m) {
    m->toggleSSBtnEvt.postEvent();
    m->simulstat = true;
    m->solvealgolist[m->currentAlgoidx](m);
    m->simulstat = false;
    m->toggleSSBtnEvt.postEvent();
}

void Maze::onSolveMazeBtn(wxCommandEvent& evt) {
    if (simulstat) {
        //issue terminate thread
        simulstat = false;
        return;
    }

    //check if startCoord and goalCoord are valid
    if (!isinbound(startCoord)) {
        wxMessageDialog err(nullptr, wxT("Please make sure Start Point is placed on grid.\n"));
        err.ShowModal();
        return;
    }
    if (!isinbound(goalCoord)) {
        wxMessageDialog err(nullptr, wxT("Please make sure Goal Point is placed on grid.\n"));
        err.ShowModal();
        return;
    }

    //init and reset
    maxsearchpathdepth = 1; //init max search depth
    clearSearch(evt); //clear and refresh search/final path vector

    std::thread ss(startSimulation, this);
    ss.detach();
}


void Maze::onPaint(wxPaintEvent& evt) {

    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);
    dc.Clear();

    //horizontal lines
    for (int i = 0; i <= grid_row; i++)
        dc.DrawLine(0, i * grid_h, grid_w * grid_col, i * grid_h);
    //vertical lines
    for (int i = 0; i <= grid_col; i++)
        dc.DrawLine(i * grid_w, 0, i * grid_w, grid_h * grid_row);

    //search path - HEAVY
    int px, py, color, maxdepth;
    maxdepth = maxsearchpathdepth;
    for (const auto& p : searchpath) {
        std::tie(px, py, color) = p;
        float r = color / (float)maxdepth;
        dc.SetBrush(wxBrush(wxColor(50 + (int)(100 * r), 55 + (int)(200 * r), 150)));
        dc.DrawRectangle({ px,py }, { grid_w,grid_h });
    }
    //obstacles
    dc.SetBrush(*wxBLACK_BRUSH);
    for (const auto& p : obstacle)
        dc.DrawRectangle({ p.first,p.second }, { grid_w,grid_h });

    //start , goal
    renderStart(dc, { startCoord.x * grid_w, startCoord.y * grid_h }, { grid_w,grid_h });
    renderGoal(dc, { goalCoord.x * grid_w, goalCoord.y * grid_h }, { grid_w,grid_h });

    //final path
    dc.SetBrush(wxBrush(wxColor(251, 153, 2)));
    for (const auto& p : finalpath)
        dc.DrawRectangle({ p.first,p.second }, { grid_w,grid_h });

}


struct DisjointSet {
    std::vector<wxPoint> parent;
    std::vector<int> size;
    int maxx, maxy;

    DisjointSet(int maxx, int maxy) {
        this->maxx = maxx;
        this->maxy = maxy;
        parent.resize(maxx * maxy);
        size.resize(maxx * maxy);
        fill(size.begin(), size.end(), 1);
        for (int i = 0; i < maxx; i++) {
            for (int j = 0; j < maxy; j++) {
                parent[i * maxy + j] = { i,j };
            }
        }
    }

    wxPoint find_set(wxPoint v) {
        if (v == parent[v.x * maxy + v.y])
            return v;
        return parent[v.x * maxy + v.y] = find_set(parent[v.x * maxy + v.y]);
    }

    void union_set(wxPoint a, wxPoint b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (size[a.x * maxy + a.y] < size[b.x * maxy + b.y])
                std::swap(a, b);
            parent[b.x * maxy + b.y] = a;
            size[a.x * maxy + a.y] += size[b.x * maxy + b.y];
        }
    }
};

void Maze::genrateMazeQ(wxCommandEvent& evt) {
    //simulation is on.. dont allow user to click
    if (simulstat)
        return;

    //init and fill with walls
    clearAll(evt);
    for (auto& row : maze)
        std::fill(row.begin(), row.end(), 1);

    setstart = false;
    setgoal = false;
    startCoord = { rand() % grid_col / 3, rand() % grid_row / 3 };
    do {
        //very unlikely
        goalCoord = { 2 * grid_col / 3 + rand() % grid_col / 3,2 * grid_row / 3 + rand() % grid_row / 3 };
    } while (goalCoord == startCoord);

    maze[startCoord.y][startCoord.x] = 0;
    maze[goalCoord.y][goalCoord.x] = 0;

    DisjointSet S(grid_col, grid_row);
    wxPoint dir[4] = { {1, 0},{0,-1} ,{-1,0},{0,1} };
    int randomch[4] = { 0,1,2,3 };
    //approx limit on removing random walls
    int limitwalls = grid_col * grid_row / 3, totalwalls = grid_col * grid_row;

    wxPoint rn, tn, tn1, tn2, rn1 = startCoord, rn2 = goalCoord;
    rn = { rand() % grid_col, rand() % grid_row };

    while (S.find_set(startCoord) != S.find_set(goalCoord)) {
        if (!(rand() % 2))
            std::shuffle(randomch, randomch + 4, g);
        for (int i = 0; i < 4; i++) {
            tn1 = rn1 + dir[randomch[i]];
            if (isinbound(tn1)) {
                S.union_set(rn1, tn1);
                rn1 = tn1;
                maze[rn1.y][rn1.x] = 0;
                totalwalls--;
                break;
            }
        }
        for (int i = 0; i < 4; i++) {
            tn2 = rn2 + dir[randomch[i]];
            if (isinbound(tn2)) {
                S.union_set(rn2, tn2);
                rn2 = tn2;
                maze[rn2.y][rn2.x] = 0;
                totalwalls--;
                break;
            }
        }
    }

    while (totalwalls >= limitwalls) {
        if (!(rand() % 2))
            std::shuffle(randomch, randomch + 4, g);
        if (!(rand() % 5))
            rn = { rand() % grid_col, rand() % grid_row };
        for (int i = 0; i < 4; i++) {
            tn = rn + dir[randomch[i]];
            if (isinbound(tn)) {
                S.union_set(rn, tn);
                totalwalls -= maze[rn.y][rn.x] + maze[tn.y][tn.x];
                maze[rn.y][rn.x] = 0;
                maze[tn.y][tn.x] = 0;
                rn = tn;
                break;
            }
        }
    }

    for (int i = 0; i < grid_row; i++)
        for (int j = 0; j < grid_col; j++)
            if (maze[i][j] == 1)
                obstacle.insert({ j * grid_w , i * grid_h });
    //render
    Refresh();

}

////////////////////////////////////////////////////////
//  void DrawHeader(wxDC& dc, int window_w, int window_h) {
//         // Code to draw header
//         int headerHeight = 50; // Height of the header block
//         wxBrush headerBrush(wxColour(255, 0, 0)); // Red header background color
//         dc.SetBrush(headerBrush);
//         dc.SetPen(*wxTRANSPARENT_PEN);
//         dc.DrawRectangle(0, 0, window_w, headerHeight);

//         // Draw Header Text
//         wxFont headerFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
//         dc.SetFont(headerFont);
//         dc.SetTextForeground(wxColour(255, 255, 255)); // White text color

//         wxString headerText = "MAZELI Solver Developed by Haveli";
//         wxSize textSize = dc.GetTextExtent(headerText);
//         int textX = (window_w - textSize.GetWidth()) / 2; // Center text horizontally
//         int textY = (headerHeight - textSize.GetHeight()) / 2; // Center text vertically within the header

//         dc.DrawText(headerText, textX, textY);
//     }



SidePanel::SidePanel(wxWindow* parent, wxWindowID id) : wxPanel(parent, id) {
    wxBoxSizer* grid = new wxBoxSizer(wxVERTICAL);
    dev_button = new wxButton(this, BTN_CLEARALL, wxT("Mazeli Solver Devoloped By Team HAVELI"));
    btnClearAll = new wxButton(this, BTN_CLEARALL, wxT("Clear All"));
    btnClearSearch = new wxButton(this, BTN_CLEARSEARCH, wxT("Clear Search"));
    btnSetStart = new wxButton(this, BTN_SETSTART, wxT("Set Start"));
    btnSetGoal = new wxButton(this, BTN_SETGOAL, wxT("Set Goal"));
    btnSolveMaze = new wxButton(this, BTN_SOLVEMAZE, wxString(btnSSLabel[0]));

    choiceAlgo = new wxChoice(this, CHOICE_ALGO);

    infoBox = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

    wxString col1 = "Parameter";
    infoBox->InsertColumn(0, col1);

    infoBox->SetColumnWidth(0, infoBox->GetFont().GetPixelSize().y * col1.length() + 20);
    infoBox->InsertColumn(1, wxT("Value"));

    infoBox->InsertItem(0, wxT("Nodes Explored"));
    infoBox->InsertItem(1, wxT("Solution Length"));
    infoBox->SetItem(0, 1, wxT("-"));
    infoBox->SetItem(1, 1, wxT("-"));

    btnGenMaze = new wxButton(this, BTN_GENMAZE, wxT("Genrate Random Maze"));
    btnHelp = new wxButton(this, BTN_HELP, wxT("Help / Features"));

    int vspace = 10;
    grid->AddSpacer(20);
    grid->Add(dev_button, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(20);
    grid->Add(btnHelp, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(20);
    grid->Add(infoBox, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace);
    grid->Add(btnClearAll, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace);
    grid->Add(btnClearSearch, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace);
    grid->Add(btnSetStart, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace);
    grid->Add(btnSetGoal, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace);
    grid->Add(btnSolveMaze, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace);
    grid->Add(choiceAlgo, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);
    grid->AddSpacer(vspace * 3);
    grid->Add(btnGenMaze, 0, wxSHAPED | wxALIGN_CENTER_HORIZONTAL);

    SetSizer(grid);


}

void SidePanel::setChoiceAlgoList(std::vector<std::string>* choicelist) {
    for (int i = 0; i < choicelist->size(); i++)
        choiceAlgo->Append(choicelist->at(i));
    choiceAlgo->SetSelection(0);
}

void SidePanel::toggleSSBtn(wxCommandEvent& evt) {
    if (btnSolveMaze->GetLabel() == wxString(btnSSLabel[0])) {
        btnSolveMaze->SetLabel(wxString(btnSSLabel[1]));
    }
    else {
        btnSolveMaze->SetLabel(wxString(btnSSLabel[0]));
    }
    Refresh();
}

//updates values on interface, if given string is empty then
//function ignores it
void SidePanel::updateAlgoInfo(wxString nodes_exp, wxString final_path_len) {
    if (nodes_exp.length() != 0)
        infoBox->SetItem(0, 1, nodes_exp);
    if (final_path_len.length() != 0)
        infoBox->SetItem(1, 1, final_path_len);
    Refresh();
}

////////////////////////////////////////////////////////////////////////////


void MainFrame::initConnections() {
    Bind(wxEVT_BUTTON, &Maze::clearAll, grid, BTN_CLEARALL);
    Bind(wxEVT_BUTTON, &Maze::clearSearch, grid, BTN_CLEARSEARCH);
    Bind(wxEVT_BUTTON, &Maze::setStart, grid, BTN_SETSTART);
    Bind(wxEVT_BUTTON, &Maze::onSolveMazeBtn, grid, BTN_SOLVEMAZE);
    Bind(wxEVT_BUTTON, &Maze::setGoal, grid, BTN_SETGOAL);
    Bind(wxEVT_CHOICE, &Maze::setCurrentAlgoIdx, grid, CHOICE_ALGO);
    Bind(wxEVT_BUTTON, &Maze::genrateMazeQ, grid, BTN_GENMAZE);
    Bind(wxEVT_BUTTON, &MainFrame::onHelpBtn, this, BTN_HELP);

    Bind(CUSTOM_EVENT, &SidePanel::toggleSSBtn, spanel, CST_TOGGLESSBTN);
    grid->toggleSSBtnEvt.setDest(spanel);
    grid->updatealgoinfo = std::bind(&SidePanel::updateAlgoInfo, spanel, std::placeholders::_1, std::placeholders::_2);
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Maze Solver - Mazeli 2.0", wxPoint(0, 0), wxSize(1500, 800)) {

    // Set the application icon
#ifdef _WIN32
    wxIcon icon;
    if (icon.LoadFile(wxT("101"), wxBITMAP_TYPE_ICO_RESOURCE)) {
        SetIcon(icon);
    }
#endif

    grid = new Maze(this, wxID_ANY);
    spanel = new SidePanel(this, wxID_ANY);
    spanel->setChoiceAlgoList(grid->getAlgoList());

    boxsz = new wxBoxSizer(wxHORIZONTAL);
    boxsz->Add(grid, 5, wxEXPAND);
    boxsz->Add(spanel, 1, wxEXPAND);

    SetSizer(boxsz);
    initConnections();
}

void MainFrame::onHelpBtn(wxCommandEvent& evt) {
    wxMessageDialog Help(nullptr,
        wxT("\
1. Use left mouse button to draw walls on grid(click and drag).\n\
2. Use right mouse button to erase walls on grid(click and drag).\n\
3. Click \"Set Start\" and then click(left mouse button) anywhere on grid to place \"Start Point\", Goal can be placed similarly.\n\
4. Click \"Clear All\" to clear both walls and search path.\n\
5. Click \"Clear Search\" to clear search path without erasing walls.\n\
6. Use Drop Down to select algorithm.\n\
7. Click \"Start Simulation\" to Start simulation.\n\
8. To stop a running algorithm click \"Stop Simulation\".\n"));
    Help.ShowModal();
}


wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

