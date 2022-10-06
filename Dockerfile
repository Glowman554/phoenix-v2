FROM ubuntu:latest

RUN apt update && apt install -y git clang make
RUN apt install -y sudo

WORKDIR /tools
RUN git clone https://github.com/Nudeltruppe/CpuV2 /tools
RUN bash install_tools.sh

WORKDIR /root
ENTRYPOINT [ "/bin/bash" ]
