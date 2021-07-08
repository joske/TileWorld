# TileWorld

Simulation of a gridworld where there are agents trying to push tiles into holes. Project from my master of ai years, but this time implemented in c++.

Requires gtkmm or wxwidgets for GUI, but can be compiled without.

GTK: make GUI=gtk all

wxWidgets: make GUI=wx all

no GUI: make all

On ubuntu/debian: apt-get install libwxgtk3.0-gtk3-dev. 

On MacOS: brew install wxwidgets (requires homebrew).

Not tested on Windows.

Docker:

docker build -t tileworld .

Default entrypoint is Gtk build

docker run -ti -e DISPLAY=host.docker.internal:0 --rm --init tileworld

To run WxWidgets:

docker run -ti -e DISPLAY=host.docker.internal:0 --rm --init tileworld /app/TileWorldWx