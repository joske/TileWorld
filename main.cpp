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
#endif

int main(int argc, char **argv)
{
    int agents = 6;
    int tiles = 20;
    int holes = 20;
    int obst = 40;
    int delay = 200;

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
    Grid grid(agents, tiles, holes, obst);
#ifdef GTKGUI
    auto app = Gtk::Application::create(argc, argv, "TileWorld");

    Gtk::Window win;
    win.set_default_size((COLS * MAG) + 200, ROWS * MAG); // space for the scores
    win.set_title("TileWorld");
    GridView area(grid, delay);
    win.add(area);
    area.show();
    return app->run(win);
#else // GTKGUI
#ifdef WXGUI
    MyApp* myApp = new MyApp(grid, delay);
    wxApp::SetInstance(myApp);
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    wxTheApp->OnExit();
    wxEntryCleanup();
#else // WXGUI
    grid.start();    
#endif // WXGUI
#endif // GTKGUI
}
