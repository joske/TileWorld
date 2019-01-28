/*
 * gridview.cpp
 *
 *  Created on: Jan 17, 2019
 *      Author: jos
 */

#ifndef NOGUI
#include "gridview.hpp"
#include <cairomm/context.h>
#include <cmath>
#include <glibmm/main.h>
#include <string>

GridView::GridView(Grid& grid, int delay) : grid(grid) {
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &GridView::on_timeout), delay);
}

GridView::~GridView() {
}

bool GridView::on_timeout() {
    // force our program to redraw the entire clock.
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    grid.update();
    return true;
}

bool GridView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->set_line_width(2);
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int x = c * MAG;
            int y = r * MAG;
            cr->set_source_rgb(0, 0, 0);
            GridObject* o = grid.getObject(c, r);
            if (o != NULL) {
                switch (o->getType()) {
                    case AGENT:
                        drawAgent(cr, o, x, y);
                        break;
                    case HOLE:
                        cr->arc(x + MAG/2, y + MAG/2, MAG / 2, 0, 2 * M_PI);
                        cr->fill();
                        break;
                    case TILE:
                        drawTile(cr, o, x, y);
                        break;
                    case OBSTACLE:
                        cr->rectangle(x, y, MAG, MAG);
                        cr->fill();
                        break;
                }
            }
            cr->stroke();
        }
    }
    const vector<Agent*>& agents = grid.getAgents();
    const int x = COLS * MAG + 20;
    const int y = 20;
    for_each(agents.begin(), agents.end(),[&](Agent* agent) { 
	ostringstream buf;
        int id = agent->getId();
        set_color(cr, id);
        buf << "Agent(" << id << "): " << agent->getScore();
        draw_text(cr, x, y + id * MAG, buf.str().c_str());
    });
    return true;
}

void GridView::drawTile(const Cairo::RefPtr<Cairo::Context>& cr, GridObject* o,
        int x, int y) {
    Tile* tile = reinterpret_cast<Tile*>(o);
    cr->arc(x + MAG/2, y + MAG/2, MAG / 2, 0, 2 * M_PI);
    cr->begin_new_sub_path();
    draw_text(cr, x + MAG / 4, y, to_string(tile->getScore()).c_str());
}

void GridView::drawAgent(const Cairo::RefPtr<Cairo::Context>& cr, GridObject* o,
        int x, int y) {
    Agent* agent = reinterpret_cast<Agent*>(o);
    set_color(cr, agent->getId());
    cr->rectangle(x, y, MAG, MAG);
    if (agent->hasTile()) {
        cr->begin_new_sub_path();
        cr->arc(x + MAG/2, y + MAG/2, MAG / 2, 0, 2 * M_PI);
        cr->begin_new_sub_path();
        draw_text(cr, x + MAG / 4, y, to_string(agent->getTile()->getScore()).c_str());
    }
}

void GridView::set_color(const Cairo::RefPtr<Cairo::Context>& cr, int id) {
    switch (id) {
        case 0:
            cr->set_source_rgb(0, 0, 255);
            break;
        case 1:
            cr->set_source_rgb(255, 0, 0);
            break;
        case 2:
            cr->set_source_rgb(0, 255, 0);
            break;
        case 3:
            cr->set_source_rgb(128, 128, 0);
            break;
        case 4:
            cr->set_source_rgb(0, 128, 128);
            break;
        case 5:
            cr->set_source_rgb(128, 0, 128);
            break;
    }
}

void GridView::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y,
        const char* text) {
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    Pango::FontDescription font;

    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);

    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    auto layout = create_pango_layout(text);

    layout->set_font_description(font);

    int text_width;
    int text_height;

    //get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->move_to(x, y);

    layout->show_in_cairo_context(cr);
}
#endif
