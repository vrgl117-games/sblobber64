FROM anacierdem/libdragon

RUN apt-get update -yq
RUN apt-get install -yq imagemagick libsox-fmt-all sox golang-go

RUN cd /tmp && git clone https://github.com/sezero/mikmod.git && cd mikmod/libmikmod/n64 && make -f Makefile.n64 install

WORKDIR /game