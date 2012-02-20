#!/bin/bash

for i in {50000..50000}; do
    echo "Gen.."
    ./generator "$i" "$i" > input
    echo "Sol.."
    ./solution sol < input
done
