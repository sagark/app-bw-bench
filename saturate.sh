#!/bin/bash

for ((i=0; i<10; i++)) do
    ./bandwidth-1.2.1/bandwidth64 &
done
