#include "wxgridview.hpp"
#include "grid.hpp"
#include <string>
#include <sstream>

#ifdef WXGUI

bool MyApp::OnInit()
{
    int agents = 6;
    int tiles = 20;
    int holes = 20;
    int obst = 20;
    int delay = 300;

    if (argc != 1) {
        // parameters passed
        if (argc != 6) {
            cerr << "usage: " << argv[0] << " <agents> <tiles> <holes> <obstacles> <delay>" << endl;
            return 1;
        }
        agents = atoi(argv[1]);
        tiles = atoi(argv[2]);
        holes = atoi(argv[3]);
        obst = atoi(argv[4]);
        delay = atoi(argv[5]);
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("TileWorld"), wxPoint(50,50), wxSize((COLS * MAG) + 200, ROWS * MAG));
 	
    drawPane = new BasicDrawPane( (wxFrame*) frame, agents, tiles, holes, obst, delay);
    sizer->Add(drawPane, 1, wxEXPAND);
	
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
	
    frame->Show();
    return true;
} 

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()

BasicDrawPane::BasicDrawPane(wxFrame* parent, int agents, int tiles, int holes, int obst, int delay) :
wxPanel(parent), grid(agents, tiles, holes, obst)
{
    timer = new wxTimer(this, 1);
    Connect(wxEVT_TIMER, wxCommandEventHandler(BasicDrawPane::OnTimer));
    timer->Start(delay);
}

void BasicDrawPane::OnTimer(wxCommandEvent& event)
{
    grid.update();
    Refresh();
    Update();
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void set_color(wxDC&  dc, int id) {
    switch (id) {
        case 0:
            dc.SetPen( wxPen( wxColor(0,0,255), 2 ) );
            break;
        case 1:
            dc.SetPen( wxPen( wxColor(255, 0, 0), 2 ) );
            break;
        case 2:
            dc.SetPen( wxPen( wxColor(0, 255, 0), 2 ) );
            break;
        case 3:
            dc.SetPen( wxPen( wxColor(128, 128, 0), 2 ) );
            break;
        case 4:
            dc.SetPen( wxPen( wxColor(0, 128, 128), 2 ) );
            break;
        case 5:
            dc.SetPen( wxPen( wxColor(128, 0, 128), 2 ) );
            break;
    }
}

void draw_text(wxDC&  dc, int x, int y, const char* text) {
    dc.DrawText(wxString::FromUTF8(text), x, y); 
}

void drawTile(wxDC&  dc, GridObject* o,
        int x, int y) {
    Tile* tile = reinterpret_cast<Tile*>(o);
    dc.SetPen( wxPen( wxColor(0, 0, 0), 2 ) );
    dc.DrawCircle( wxPoint(x + MAG/2, y + MAG/2), MAG/2);
    draw_text(dc, x + MAG / 4, y, to_string(tile->getScore()).c_str());
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
}

void drawAgent(wxDC&  dc, GridObject* o,
        int x, int y) {
    Agent* agent = reinterpret_cast<Agent*>(o);
    set_color(dc, agent->getId());
    dc.DrawRectangle(x, y, MAG, MAG);
    if (agent->hasTile()) {
        dc.DrawCircle( wxPoint(x + MAG/2, y + MAG/2), MAG/2);
        draw_text(dc, x + MAG / 4, y, to_string(agent->getTile()->getScore()).c_str());
    }
}

void drawObstacle(wxDC&  dc, int x, int y) {
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen( wxPen( wxColor(0, 0, 0), 2 ) );
    dc.DrawRectangle(x, y, MAG, MAG);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
}

void drawHole(wxDC&  dc, int x, int y) {
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen( wxPen( wxColor(0, 0, 0), 2 ) );
    dc.DrawCircle( wxPoint(x + MAG/2, y + MAG/2), MAG/2);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void BasicDrawPane::render(wxDC&  dc)
{
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int x = c * MAG;
            int y = r * MAG;
            GridObject* o = grid.getObject(c, r);
            if (o != NULL) {
                switch (o->getType()) {
                    case AGENT:
                        drawAgent(dc, o, x, y);
                        break;
                    case HOLE:
                        drawHole(dc, x, y);
                         break;
                    case TILE:
                        drawTile(dc, o, x, y);
                        break;
                    case OBSTACLE:
                        drawObstacle(dc, x, y);
                        break;
                }
            }
        }
    }
    const int x = COLS * MAG + 20;
    const int y = 20;
    for (Agent* agent : grid.getAgents()) {
	    ostringstream buf;
        int id = agent->getId();
        buf << "Agent(" << id << "): " << agent->getScore();
        set_color(dc, id);
        draw_text(dc, x, y + id * MAG, buf.str().c_str());
    }
}


#endif