FROM gcc

RUN apt update; apt -y dist-upgrade
RUN apt install -y libwxgtk3.0-gtk3-dev libgtk-3-dev
WORKDIR /app
COPY . .
RUN make GUI=wx clean all
CMD ["./TileWorld"]
