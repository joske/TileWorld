# TileWorld

Simulation of a gridworld where there are agents trying to push tiles into holes. Project from my master of ai years, but this time implemented in c++.

Requires gtkmm or wxwidgets for GUI, but can be compiled without.

GTK: make GUI=gtk all

wxWidgets: make GUI=wx all

no GUI: make all

On ubuntu/debian: apt-get install libwxgtk3.0-gtk3-dev. 

On MacOS: brew install wxwidgets (requires homebrew) (or use the Docker info below).

Not tested on Windows (who cares, but in theory should work with the Docker setup).

Docker:

docker build -t tileworld .

Default entrypoint is Gtk build

macOS (Xquartz needs to run with network access enabled and with xhost +):

docker run -ti -e DISPLAY=host.docker.internal:0 --rm --init tileworld 

linux (complicated as it needs to forward the X display socket & xauth):

docker run -ti -e DISPLAY=$DISPLAY  -v /tmp/.X11-unix:/tmp/.X11-unix:rw --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --network=host --privileged --rm --init tileworld

To run WxWidgets:

docker run -ti -e DISPLAY=host.docker.internal:0 --rm --init tileworld /app/TileWorldWx
