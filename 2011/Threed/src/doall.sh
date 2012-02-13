#!/bin/sh

gcc cptest.c -o cptest
g++ genLargeYes.cpp -O2 -o genLargeYes
g++ genRand.cpp -O2 -o genRand
g++ genSqrtNo.cpp -O2 -o genSqrtNo
g++ solution_ag.cpp -O2 -o solution

./cptest <01.hand >01
./cptest <02.hand >02

echo "Generating test #3"
./genRand 3 10 adfoiadreiqporeq >03
echo "Generating test #4"
./genRand 3 10 eopirerqpioewfqw >04
echo "Generating test #5"
./genRand 4 10 adfsp4qwerfasana >05
echo "Generating test #6"
./genRand 10 100 adf43434r434na >06
echo "Generating test #7"
./genRand 100 1000 da5434fwefas >07
echo "Generating test #8"
./genRand 1000 10000 eaffaaaafd >08
echo "Generating test #9"
./genSqrtNo 10 10 dafiapofadsap >09
echo "Generating test #10"
./genSqrtNo 30 100 ioafnafakj >10
echo "Generating test #11"
./genSqrtNo 57 1000 4fandasav >11
echo "Generating test #12"
./genSqrtNo 218 10000 lfdskld >12
echo "Generating test #13"
./genSqrtNo 1234 100000 43oel >13
echo "Generating test #14"
./genLargeYes 3 10 afapofadsap >14
echo "Generating test #15"
./genLargeYes 30 100 ioanafazj >15
echo "Generating test #16"
./genLargeYes 57 1000 4faan av >16
echo "Generating test #17"
./genLargeYes 218 100000 dskld >17
echo "Generating test #18"
./genLargeYes 1234 1000000 4kl >18
echo "Generating test #19"
./genSqrtNo 2000 1000000 dafdafiapofadap >19
echo "Generating test #20"
./genSqrtNo 2000 1000000 ioaadfffnafaakj >20
echo "Generating test #21"
./genSqrtNo 3000 1000000 4fananadfddasav >21
echo "Generating test #22"
./genSqrtNo 3000 1000000 kfalfdskfdafald >22
echo "Generating test #23"
./genSqrtNo 4000 1000000 43oefjsklfadffa >23
echo "Generating test #24"
./genLargeYes 2000 1000000 dafdafipofadap >24
echo "Generating test #25"
./genLargeYes 2000 1000000 ioaadffnafaakj >25
echo "Generating test #26"
./genLargeYes 3000 1000000 4fananadddasav >26
echo "Generating test #27"
./genLargeYes 3000 1000000 kfalfdsfdafald >27
echo "Generating test #28"
./genLargeYes 4000 1000000 43oefjsklfaffa >28
echo "Generating test #29"
./genSqrtNo 5000 1000000 dafdaqerpofadap >29
echo "Generating test #30"
./genSqrtNo 6000 1000000 ioawerffnafaakj >30
echo "Generating test #31"
./genSqrtNo 10000 1000000 4fnanadfddasav >31
echo "Generating test #32"
./genSqrtNo 10000 1000000 adfanadfddasav >32
echo "Generating test #33"
./genSqrtNo 10000 1000000 dafaerqwerfwqe >33
echo "Generating test #34"
./genSqrtNo 10000 1000000 dasifdndaffadq >34
echo "Generating test #35"
./genSqrtNo 10000 1000000 oorfosdmvafdmm >35
echo "Generating test #36"
./genSqrtNo 10000 1000000 4eorewflkremaa >36
echo "Generating test #37"
./genSqrtNo 10000 1000000 4eorawflq3rdfa >37
echo "Generating test #38"
./genLargeYes 10000 1000000 adfanadfadsdaav >38
echo "Generating test #39"
./genLargeYes 10000 1000000 adfanadfddaav >39
echo "Generating test #40"
./genLargeYes 10000 1000000 dafaerqwefwqe >40
echo "Generating test #41"
./genLargeYes 10000 1000000 dasifdnaffadq >41
echo "Generating test #42"
./genLargeYes 10000 1000000 oorfodmvafdmm >42
echo "Generating test #43"
./genLargeYes 10000 1000000 4eoewflkremaa >43
echo "Generating test #44"
./genLargeYes 10000 1000000 4orawflq3rdfa >44

mkdir ../tests

for i in ??; do
mv $i threed.in
./solution
mv threed.in ../tests/$i
mv threed.out ../tests/$i.a
echo Generated test $i.
done

rm genLargeYes
rm genRand
rm genSqrtNo
rm solution
rm cptest
