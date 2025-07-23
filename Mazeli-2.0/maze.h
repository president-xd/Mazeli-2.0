#pragma once

#include <wx/dcbuffer.h>
#include <wx/listctrl.h>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <tuple>
#include <thread>
#include <chrono>

wxDEFINE_EVENT(CUSTOM_EVENT, wxCommandEvent);

class CustomEvent {
    wxCommandEvent event;
    wxEvtHandler* dest; //destination object
public:

    CustomEvent() {
        event.SetEventType(CUSTOM_EVENT);
        dest = nullptr;
    }
    CustomEvent(int enumID) {
        event.SetEventType(CUSTOM_EVENT);
        event.SetId(enumID);
        dest = nullptr;
    }
    void setId(int enumID) {
        event.SetId(enumID);
    }
    void setDest(wxEvtHandler* dest) {
        this->dest = dest;
    }
    void postEvent() {
        wxPostEvent(dest, event);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////
class priorityQ;// defined in mazesolvealgo.cpp


class Maze : public wxScrolled<wxPanel> {
    int window_w, window_h;
    int grid_w, grid_h;
    int grid_row, grid_col;

    std::set<std::pair<int, int>> obstacle;
    std::vector<std::tuple<int, int, int>> searchpath;//x , y , depth(color)
    int maxsearchpathdepth; // store max search path depth(max color)
    std::vector<std::pair<int, int>> finalpath;
    std::vector<std::vector<bool>> maze;

    wxPoint startCoord, goalCoord;
    bool setstart, setgoal; //set true if next left click is going to set start/goal
    bool simulstat; //disable button on simulstat == true
    int currentAlgoidx; //algo to run from list

    bool setStartGoal(const wxPoint& coord);
    bool isinbound(const wxPoint& coord);
    bool markCell(wxPoint coord);
    bool unMarkCell(wxPoint coord);
    void logmaze();


    //Solving Algorithm helper function
    friend void startSimulation(Maze* m); //run on thread
    friend void updatesearchpath(Maze* m, std::pair<int, int> node, int depth);
    friend void updatefinalpath(Maze* m, std::pair<int, int> node);

    //all Solving ALgorithms
    friend void solvemazeBFS(Maze* m);
    friend void solvemazeBiBFS(Maze* m);
    friend void reconstruct_Bipath(Maze* m, std::map<std::pair<int, int>, std::pair<int, int>>& parentf,
        std::map<std::pair<int, int>, std::pair<int, int>>& parentb, std::pair<int, int> pf,
        std::pair<int, int> pb);
    friend void solvemazeDFS(Maze* m);
    //solvemazeheuristicHelper() not to be used stand alone
    friend void solvemazeheuristicHelper(Maze* m, priorityQ& pQ, std::map<std::pair<int, int>, int>& depthmap);
    friend void solvemazegreedybest(Maze* m);
    friend void solvemazeAstar(Maze* m);
    friend void solvemazeAstarweigh(Maze* m);
    //friend void solvemazeAstarpxWD(Maze* m);
    //stores list for dropdown
    static std::vector<std::function<void(Maze*)>> solvealgolist;
    static std::vector<std::string> solvealgonamelist;

public:
    //Events for Communication
    CustomEvent toggleSSBtnEvt;
    //pass Running Algo info to interface
    std::function<void(wxString, wxString)> updatealgoinfo;

    Maze(wxWindow* parent, wxWindowID id);
    ~Maze();
    void initMaze();

    void clearAll(wxCommandEvent& evt);
    void clearSearch(wxCommandEvent& evt);
    void setStart(wxCommandEvent& evt);
    void setGoal(wxCommandEvent& evt);
    void setCurrentAlgoIdx(wxCommandEvent& evt);
    void genrateMazeQ(wxCommandEvent& evt);

    void onMouseMove(wxMouseEvent& evt);
    void onLeftBtn(wxMouseEvent& evt);
    void onRightBtn(wxMouseEvent& evt);
    void onSolveMazeBtn(wxCommandEvent& evt);
    void onPaint(wxPaintEvent& evt);

    std::vector<std::string>* getAlgoList() { return &solvealgonamelist; }

    DECLARE_EVENT_TABLE()
};
///////////////////////////////////////////////////////////////////////////

void startSimulation(Maze* m);
void solvemazeBFS(Maze* m);
void solvemazeBiBFS(Maze* m);
void reconstruct_Bipath(Maze* m, std::map<std::pair<int, int>, std::pair<int, int>>& parentf,
    std::map<std::pair<int, int>, std::pair<int, int>>& parentb, std::pair<int, int> pf,
    std::pair<int, int> pb);
void solvemazeDFS(Maze* m);
void solvemazeheuristicHelper(Maze* m, priorityQ& pQ, std::map<std::pair<int, int>, int>& depthmap);
void solvemazeAstar(Maze* m);
void solvemazeAstarweigh(Maze* m);
//void solvemazeAstarpxWD(Maze* m);
void solvemazegreedybest(Maze* m);

///////////////////////////////////////////////////////////////////////////

class SidePanel : public wxPanel {
    std::string btnSSLabel[2] = { "Start Simulation >>","Stop Simulation ||" };
    wxButton* dev_button;
    wxButton* btnClearAll;
    wxButton* btnClearSearch;
    wxButton* btnSetStart;
    wxButton* btnSetGoal;
    wxButton* btnSolveMaze;
    wxChoice* choiceAlgo;
    wxListView* infoBox;
    wxButton* btnGenMaze;
    wxButton* btnHelp;
public:

    SidePanel(wxWindow* parent, wxWindowID id);
    void toggleSSBtn(wxCommandEvent& evt);
    void setChoiceAlgoList(std::vector<std::string>* choicelist);
    void updateAlgoInfo(wxString nodes_exp, wxString final_path_len);
};

/////////////////////////////////////////////////////////////////////////

class MainFrame : public wxFrame {
    Maze* grid;
    wxBoxSizer* boxsz;
    SidePanel* spanel;
    void initConnections();

public:
    MainFrame();
    void onHelpBtn(wxCommandEvent& evt);
};