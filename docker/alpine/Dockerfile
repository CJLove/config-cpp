# Multi-stage Dockerfile to build a "configdev" image containing the watch example
#
# From top directory:
# docker build -t configdev:latest -f docker/alpine/Dockerfile .
#
FROM alpinedev:latest as builder

ADD . /home/config-cpp

RUN \
    cd /home/config-cpp \
    && rm -rf build \
    && mkdir build \
    && cd build \
    && cmake -D JSON_SUPPORT=ON -D YAML_SUPPORT=OFF -D CMAKE_INSTALL_PREFIX=$PWD/.. .. \
    && make watcher \
    && make install

FROM alpine:3.7

RUN \
    apk add --nocache libstdc++ \
    && mkdir -p /etc/config

COPY --from=builder /home/config-cpp/inputs/json/config.json /etc/config/
COPY --from=builder /home/config-cpp/lib/lib*.so /usr/lib
COPY --from=builder /home/config-cpp/bin/watcher /usr/bin/

ENTRYPOINT [ "/usr/bin/watcher" ]