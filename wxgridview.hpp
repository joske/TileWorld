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
    BasicDrawPane(wxFrame* parent, int agents, int tiles, int holes, int obst, int delay);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnTimer(wxCommandEvent& event);
    
    void render(wxDC& dc);
    
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
private:
    Grid grid;
    wxTimer *timer;
};

class MyApp : public wxApp
{
public:
    MyApp() {};
    virtual ~MyApp() {};

    virtual bool OnInit();
    wxFrame *frame;
    BasicDrawPane * drawPane;
};

#endif
#endif