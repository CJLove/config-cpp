# Dockerfile that can produce an "alpinedev" image that can be used in
# a multi-stage Dockerfile to produce a Fedora-based docker image
#
# docker build -t alpinedev:latest .
#
FROM alpine:3.7

RUN apk add --no-cache \
    bash git g++ cmake make \
    && cd /home \
    && git clone https://github.com/nlohmann/json.git \
    && cd /home/json \
    && mkdir build \
    && cd build \
    && cmake -DJSON_BuildTests=OFF .. \
    && make install
