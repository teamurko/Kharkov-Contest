#!/bin/bash

for i in {1..10}; do
    echo "Gen.."
    ./generator "$i" 10 1000000 > input
    echo "Sol.."
    ./solution sol < input
done
#for i in {11..20}; do
#    echo "Gen.."
#    ./generator "$i" 100000 1000000 > input
#    echo "Sol.."
#    ./solution sol < input
#done
