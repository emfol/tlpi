#!/bin/sh

docker container create -it --name tlpi --hostname tlpi -v "$(pwd):/usr/local/src/tlpi" -p 8192:8192/tcp ubuntu:jammy /bin/sh
