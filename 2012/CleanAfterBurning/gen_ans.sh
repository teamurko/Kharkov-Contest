#!/bin/bash
tests=`ls tests | egrep '*.in'`
for tst in $tests; do
    name=`echo "$tst" | sed 's/\.in//g'`
    ./solution sol < "tests/$tst" > "tests/$name.out"
done
