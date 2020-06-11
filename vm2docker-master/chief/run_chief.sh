#!/bin/sh
make clean
rm -rf constants/__pycache__
rm -f constants/*_wrap* constants/*~ constants/.~* constants/mypython constants/*.pyc
rm -f constants/core
rm -f constants/*.o constants/*.bundle constants/_agent.so
rm -f constants/agent.py
make
chmod +x vm2docker.py
rm -rf /tmp/tmp*
./vm2docker.py --debug 10.139.76.197 49153