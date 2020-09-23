/*
 * gridview.hpp
 *
 *  Created on: Jan 17, 2019
 *      Author: jos
 */

#ifdef GTKGUI
#ifndef GRIDVIEW_HPP_
#define GRIDVIEW_HPP_

#include <gtkmm/drawingarea.h>
#include <pangomm.h>
#include "grid.hpp"

#define MAG 20

class GridView : public Gtk::DrawingArea
{
public:
    GridView(Grid &grid, int delay);
    virtual ~GridView();

protected:
    //Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
    bool on_timeout();

private:
    Grid &grid;

    void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, int rectangle_width, int rectangle_height, const char *text);
    void set_color(const Cairo::RefPtr<Cairo::Context> &cr, int id);
    void drawAgent(const Cairo::RefPtr<Cairo::Context> &cr, GridObject *o, int x, int y);
    void drawTile(const Cairo::RefPtr<Cairo::Context> &cr, GridObject *o, int x, int y);
};

#endif /* GRIDVIEW_HPP_ */
#endif /* NOGUI */
