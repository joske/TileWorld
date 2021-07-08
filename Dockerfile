FROM gcc

RUN apt update; apt -y dist-upgrade
RUN apt install -y libwxgtk3.0-gtk3-dev libgtk-3-dev libgtkmm-3.0-dev
WORKDIR /app
COPY . .
RUN make GUI=wx clean all
RUN mv TileWorld TileWorldWx
RUN make GUI=gtk clean all
RUN mv TileWorld TileWorldGtk
CMD ["./TileWorldGtk"]
