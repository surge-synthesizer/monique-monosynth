#!/bin/bash
konsole -e /bin/sh -c "cd $(dirname "$0") && echo 'BUILD LINUX MONIQUE 64 STANDALONE' && make clean CONFIG=x64 && make CONFIG=x64 -j3 && make strip CONFIG=x64 && notify-send 'BUILD LINUX MONIQUE 64 STANDALONE - DONE'" 
