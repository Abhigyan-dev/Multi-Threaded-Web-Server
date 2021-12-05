#!/bin/bash

for N  in {0..20}
do 
    ruby client.rb $N &
    #sleep 0.01
done
wait
