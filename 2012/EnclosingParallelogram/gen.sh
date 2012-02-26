#!/bin/bash

if [ ! -d "tests" ]; then
    mkdir "tests"
else
    rm tests/*
fi
for i in {1..10}; do
    echo "Gen.."
    ./generator "$i" 40000 100000000 > "tests/$i.in"
    echo "Sol.."
    ./solution sol < "tests/$i.in" > "tests/$i.out"
    ./checker "tests/$i.in" "tests/$i.out" "tests/$i.out"
done
