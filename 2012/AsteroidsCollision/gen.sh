#!/bin/bash

if [ ! -d "tests" ]; then
    mkdir "tests"
fi

for i in {1..10}; do
    echo "Gen: $i"
    ./generator "$i" 10 10 100 > "tests/$i.in"
    echo "Sol.."
    ./solution sol < "tests/$i.in" > "tests/$i.out"
done

for i in {11..19}; do
    echo "Gen: $i"
    ./generator "$i" 100 3 1000 > "tests/$i.in"
    echo "Sol.."
    ./solution sol < "tests/$i.in" > "tests/$i.out"
done

for i in {20..25}; do
    echo "Gen: $i"
    ./generator "$i" 3 1000 100000 > "tests/$i.in"
    echo "Sol.."
    ./solution sol < "tests/$i.in" > "tests/$i.out"
done

for i in {26..50}; do
    echo "Gen: $i"
    ./generator "$i" 40000 40000 100000000 > "tests/$i.in"
    echo "Sol.."
    ./solution sol < "tests/$i.in" > "tests/$i.out"
done
