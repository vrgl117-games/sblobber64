FROM ghcr.io/dragonminded/libdragon

ARG LIBDRAGON_COMMIT 07f1977bbb66a8f61d949983342c27915932d5a5 #05/27/26

# Cross-build under emulation can be flaky with parallel make, so keep jobs to one.
ENV MAKEFLAGS=-j1
RUN apt-get update -yq
RUN apt-get install -yq imagemagick libsox-fmt-all openmpt123 sox golang-go

RUN cd /tmp && git clone https://github.com/DragonMinded/libdragon.git && cd libdragon && git reset --hard $LIBDRAGON_COMMIT && make install && make tools-install

WORKDIR /game
