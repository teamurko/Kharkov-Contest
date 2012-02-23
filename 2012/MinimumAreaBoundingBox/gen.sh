#!/bin/bash

if [ ! -d "tests" ]; then
    mkdir "tests"
fi
for i in {1..20}; do
    echo "Gen.."
    ./generator "$i" 40000 100000000 > "tests/$i.in"
    echo "Sol.."
    ./solution sol < "tests/$i.in" > "tests/$i.out"
done
