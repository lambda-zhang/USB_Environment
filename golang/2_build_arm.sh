#!/bin/bash
CGO_ENABLED=1 GOOS=linux GOARCH=arm go build -ldflags "-w" -o main main.go

rm -rf release.arm.tgz
tar -czvmf release.arm.tgz main.arm  webpage/dist/

gitid=$(git rev-parse --short HEAD)
echo "docker build -t lambdazhang/usbenv:"${gitid}" ."
