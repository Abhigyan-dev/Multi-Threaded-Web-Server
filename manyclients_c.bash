#!/bin/bash

for N  in {0..89}
do 
    ./a.out ./data/$N.html &
    sleep 0.01
done
wait
