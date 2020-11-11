FROM anacierdem/libdragon

RUN apt-get update -yq
RUN apt-get install -yq imagemagick libsox-fmt-all sox

WORKDIR /game