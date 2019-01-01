FROM fedora:28

RUN \
    dnf install -y cmake gcc-c++ make gdb git \
    && cd /home \
    && git clone https://github.com/nlohmann/json.git \
    && cd /home/json \
    && mkdir build \
    && cd build \
    && cmake -DJSON_BuildTests=OFF .. \
    && make install
