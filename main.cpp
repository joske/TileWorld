#include "grid.hpp"
#include <time.h>
#include <stdlib.h>

#ifndef NOGUI
#include "gridview.hpp"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
using namespace Gtk;
#endif

int main(int argc, char** argv) {

    srand(time(NULL));
#ifndef NOGUI
    auto app = Gtk::Application::create(argc, argv, "TileWorld");

    Gtk::Window win;
    win.set_default_size(COLS * MAG, ROWS * MAG);
    win.set_title("DrawingArea");
#endif
    Grid* grid = new Grid(6, 20, 20);
#ifndef NOGUI
    GridView area(grid);
    win.add(area);
    area.show();
#endif
    grid->start();
#ifndef NOGUI
    return app->run(win);
#endif
}
