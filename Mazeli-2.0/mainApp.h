#pragma once
#include <wx/wx.h>
#include <fstream>

//extern std::ofstream flog;
class MainApp : public wxApp {

public:
    virtual bool OnInit();
    ~MainApp() {
        //flog.close();
    }
};

#pragma once
