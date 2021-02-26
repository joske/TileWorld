from gcc

RUN apt update; apt -y dist-upgrade
run apt install -y libwxgtk3.0-gtk3-dev libgtk-3-dev
WORKDIR /app
