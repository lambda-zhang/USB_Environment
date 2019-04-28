FROM arm32v7/debian:sid
MAINTAINER ggxxde@163.com
RUN apt-get update && apt-get install -y libusb-1.0-0-dev pkg-config && apt-get clean
RUN mkdir /data
COPY release.arm.tgz /data/release.arm.tgz
WORKDIR /data
RUN find . && tar -xvmf release.arm.tgz && rm -rf release.arm.tgz && mkdir configs && touch configs/conf.ini && touch configs/conf-dev.ini &&ls -lh
EXPOSE 80
