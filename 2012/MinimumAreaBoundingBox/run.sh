#!/bin/bash

for i in {1..1000}; do
    echo "Gen.."
    ./generator "$i" 1000 > input
    echo "Sol.."
    ./solution check < input
done
