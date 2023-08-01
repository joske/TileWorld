#ifdef WXGUI

#include <memory>
#include <sstream>
#include <string>

#include "./grid.hpp"
#include "./wxgridview.hpp"

bool MyApp::OnInit() {
  wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  frame = new wxFrame((wxFrame *)NULL, -1, wxT("TileWorld"), wxPoint(50, 50),
                      wxSize((COLS * MAG) + 200, ROWS * MAG));

  drawPane = new BasicDrawPane((wxFrame *)frame, grid, delay);
  sizer->Add(drawPane, 1, wxEXPAND);
  frame->SetSizer(sizer);
  frame->SetAutoLayout(true);

  frame->Show();
  return true;
}

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()

BasicDrawPane::BasicDrawPane(wxFrame *parent, Grid &grid, int delay)
    : wxPanel(parent), grid(grid) {
  timer = new wxTimer(this, 1);
  Connect(wxEVT_TIMER, wxCommandEventHandler(BasicDrawPane::OnTimer));
  timer->Start(delay);
}

void BasicDrawPane::OnTimer(wxCommandEvent &event) {
  grid.update();
  Refresh();
  Update();
}

void BasicDrawPane::paintEvent(wxPaintEvent &evt) {
  wxPaintDC dc(this);
  render(dc);
}

void BasicDrawPane::paintNow() {
  wxClientDC dc(this);
  render(dc);
}

void set_text_color(wxDC &dc, const wxColor &color) {
  dc.SetTextForeground(color);
}

const wxColor set_color(wxDC &dc, int id) {
  wxColor color;
  switch (id) {
  case 0:
    color = wxColor(0, 0, 255);
    break;
  case 1:
    color = wxColor(255, 0, 0);
    break;
  case 2:
    color = wxColor(0, 255, 0);
    break;
  case 3:
    color = wxColor(128, 128, 0);
    break;
  case 4:
    color = wxColor(0, 128, 128);
    break;
  case 5:
    color = wxColor(128, 0, 128);
    break;
  }
  dc.SetPen(wxPen(color, 2));
  return color;
}

void draw_text(wxDC &dc, int x, int y, const char *text, const wxColor &color) {
  set_text_color(dc, color);
  dc.DrawText(wxString::FromUTF8(text), x, y);
}

void draw_text(wxDC &dc, int x, int y, const char *text) {
  draw_text(dc, x, y, text, wxColor(0, 0, 0));
}

void drawTile(wxDC &dc, shared_ptr<GridObject> o, int x, int y) {
  shared_ptr<Tile> tile = static_pointer_cast<Tile>(o);
  dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
  dc.DrawCircle(wxPoint(x + MAG / 2, y + MAG / 2), MAG / 2);
  draw_text(dc, x + MAG / 4, y, to_string(tile->getScore()).c_str());
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
}

void drawAgent(wxDC &dc, shared_ptr<GridObject> o, int x, int y) {
  shared_ptr<Agent> agent = static_pointer_cast<Agent>(o);
  const wxColor &color = set_color(dc, agent->getId());
  dc.DrawRectangle(x, y, MAG, MAG);
  if (agent->hasTile) {
    dc.DrawCircle(wxPoint(x + MAG / 2, y + MAG / 2), MAG / 2);
    draw_text(dc, x + MAG / 4, y,
              to_string(agent->getTile()->getScore()).c_str(), color);
  }
}

void drawObstacle(wxDC &dc, int x, int y) {
  dc.SetBrush(*wxBLACK_BRUSH);
  dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
  dc.DrawRectangle(x, y, MAG, MAG);
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
}

void drawHole(wxDC &dc, int x, int y) {
  dc.SetBrush(*wxBLACK_BRUSH);
  dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
  dc.DrawCircle(wxPoint(x + MAG / 2, y + MAG / 2), MAG / 2);
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void BasicDrawPane::render(wxDC &dc) {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int x = c * MAG;
      int y = r * MAG;
      shared_ptr<GridObject> o = grid.getObject(c, r);
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
  for (shared_ptr<Agent> agent : grid.getAgents()) {
    ostringstream buf;
    int id = agent->getId();
    buf << "Agent(" << id << "): " << agent->getScore();
    const wxColor &color = set_color(dc, id);
    draw_text(dc, x, y + id * MAG, buf.str().c_str(), color);
  }
}

#endif
