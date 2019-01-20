#include "grid.hpp"
#include "gridview.hpp"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

using namespace Gtk;

int main(int argc, char** argv) {

	auto app = Gtk::Application::create(argc, argv, "TileWorld");

	Gtk::Window win;
	win.set_default_size(COLS*MAG, ROWS*MAG);
	win.set_title("DrawingArea");

	Grid grid = Grid(1, 5, 5);
	GridView area(grid);
	win.add(area);
	area.show();
	grid.start();

	return app->run(win);
}
