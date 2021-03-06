#include "grid.hpp"
#include <time.h>
#include <stdlib.h>

#ifdef GTKGUI
#include "gtkgridview.hpp"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
using namespace Gtk;
#endif

#ifdef WXGUI
#include "wxgridview.hpp"

IMPLEMENT_APP(MyApp)
#else
int main(int argc, char **argv)
{
    int agents = 1;
    int tiles = 20;
    int holes = 20;
    int obst = 20;
    int delay = 300;

    if (argc != 1)
    {
        // parameters passed
        if (argc != 6)
        {
            cerr << "usage: " << argv[0] << " <agents> <tiles> <holes> <obstacles> <delay>" << endl;
            return 1;
        }
        agents = atoi(argv[1]);
        tiles = atoi(argv[2]);
        holes = atoi(argv[3]);
        obst = atoi(argv[4]);
        delay = atoi(argv[5]);
    }

    srand(time(NULL));
#ifdef GTKGUI
    auto app = Gtk::Application::create(argc, argv, "TileWorld");

    Gtk::Window win;
    win.set_default_size((COLS * MAG) + 200, ROWS * MAG); // space for the scores
    win.set_title("TileWorld");
#endif
    Grid grid(agents, tiles, holes, obst);
#ifdef GTKGUI
    GridView area(grid, delay);
    win.add(area);
    area.show();
#endif
    grid.start();
#ifdef GTKGUI
    return app->run(win);
#endif
}
#endif /* WXGUI */
