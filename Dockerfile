FROM ubuntu:latest

RUN apt update && apt install -y git clang make
RUN apt install -y sudo

WORKDIR /tools
RUN git clone https://github.com/Nudeltruppe/CpuV2; \
    cd CpuV2; \
    bash install_tools.sh; \
    cd ..; \
    rm -rfv CpuV2

WORKDIR /root
ENTRYPOINT [ "/bin/bash" ]
