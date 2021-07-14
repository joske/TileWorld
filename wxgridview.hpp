#ifndef __WXGRIDVIEW_HPP__
#define __WXGRIDVIEW_HPP__

#ifdef WXGUI
#include <wx/wx.h>
#include <wx/sizer.h>
#include "grid.hpp"

#define MAG 20

class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame *parent, Grid &grid, int delay);

    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void OnTimer(wxCommandEvent &event);

    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
private:
    Grid &grid;
    wxTimer *timer;
};

class MyApp : public wxApp
{
public:
    MyApp(Grid &grid, int delay) : grid(grid), delay(delay) {}
    virtual ~MyApp(){};

    virtual bool OnInit();
    Grid &grid;
    int delay;
    wxFrame *frame;
    BasicDrawPane *drawPane;
};

#endif
#endif