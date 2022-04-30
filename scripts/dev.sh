#!/bin/bash

#bash scripts/sasswatch.sh &
while :; do
    cmake -Bbuild -S. 
    make -j 2 -C build
    ./build/everdrive &
    pid=$!
    inotifywait -e modify $(find -mindepth 1 -maxdepth 1 ! -name "build" ! -name ".git" ! -name "everdrive.db")
    kill $pid
done
