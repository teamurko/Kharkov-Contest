#!/bin/bash

g++ -O2 masterspark_in.cpp -o solution.exe
g++ -O2 gen.cpp -o gen.exe

gcc gen_hand01.c  -o gh01.exe
gcc gen_hand02.c  -o gh02.exe
gcc gen_hand03.c  -o gh03.exe
gcc gen_hand04.c  -o gh04.exe
gcc gen_hand05.c  -o gh05.exe
gcc gen_hand06.c  -o gh06.exe
gcc gen_hand07.c  -o gh07.exe

./gh01.exe > 01
./gh02.exe > 02
./gh03.exe > 03
./gh04.exe > 04
./gh05.exe > 05
./gh06.exe > 06
./gh07.exe > 07

./gen.exe 10 1 0 123 > 36
./gen.exe 10 1 1 13 > 37

./gen.exe 10 1 2 942 > 08
./gen.exe 10 1 5 1675 > 09
./gen.exe 10 1 7 273 > 10

./gen.exe 10 1 9 2123 > 11
./gen.exe 10 1 10 9123 > 12
./gen.exe 20 15 22 3123 > 13
./gen.exe 20 40 70 1273 > 14
./gen.exe 20 70 10 1223 > 15

./gen.exe 21 4 22 533 > 16
./gen.exe 0 1 2 8347 > 17
./gen.exe 0 1 2 548 > 18
./gen.exe 10 12 0 1593 > 19
./gen.exe 0 1 1  27 > 20

./gen.exe 0 1 1 57 > 21
./gen.exe 0 1 1 235 > 22
./gen.exe 0 1 1 9563 > 23
./gen.exe 99 100 0 246 > 24
./gen.exe 0 1 1 4586 > 25

./gen.exe 0 1 0 123 > 26
./gen.exe 0 1 1 23 > 27
./gen.exe 0 1 2 942 > 28
./gen.exe 0 1 5 1675 > 29
./gen.exe 0 1 7 273 > 30

./gen.exe 0 1 9 2123 > 31
./gen.exe 0 1 10 9123 > 32
./gen.exe 0 15 22 3123 > 33
./gen.exe 0 40 70 1273 > 34
./gen.exe 0 70 10 1223 > 35

mkdir ../tests
                                       
for i in ??; do
  cp $i masterspark.in
  ./solution.exe
  mv masterspark.out  $i.a
  rm masterspark.in
  mv $i ../tests
  mv $i.a ../tests
done

rm *.exe

