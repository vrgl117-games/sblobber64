FROM ghcr.io/dragonminded/libdragon

# Latest release tag (V14.2.0-17896385267) at time of update
ARG LIBDRAGON_COMMIT 806fc505d55ef123237d1dda0cb5149221482035

# Cross-build under emulation can be flaky with parallel make, so keep jobs to one.
ENV MAKEFLAGS=-j1
RUN apt-get update -yq
RUN apt-get install -yq imagemagick libsox-fmt-all sox golang-go

RUN cd /tmp && git clone https://github.com/DragonMinded/libdragon.git && cd libdragon && git reset --hard $LIBDRAGON_COMMIT && make install && make tools-install
RUN cd /tmp && git clone https://github.com/sezero/mikmod.git && cd mikmod/libmikmod/n64 && make -f Makefile.n64 install

WORKDIR /game
