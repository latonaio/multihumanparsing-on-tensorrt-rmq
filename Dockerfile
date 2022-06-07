#FROM nvcr.io/nvidia/l4t-pytorch:r34.1.1-pth1.12-py3
FROM nvcr.io/nvidia/l4t-pytorch:r32.7.1-pth1.10-py3



RUN dpkg --configure -a
RUN apt install --fix-broken

RUN apt-get update -y
RUN apt-get install -y \
      apt-utils
RUN apt-get update -y && apt-get -y dist-upgrade
RUN apt-get install -y \
        libjansson-dev \
        librabbitmq-dev

RUN mkdir /jetson-inference
WORKDIR /jetson-inference

RUN bash




