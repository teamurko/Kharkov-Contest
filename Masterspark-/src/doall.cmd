@echo OFF
g++ -O2 ..\masterspark_in.cpp -o solution.exe
g++ -O2 ..\gen.cpp -o gen.exe


copy *.hand *.


gen 10 1 0 123 > 36
gen 10 1 1 13 > 37

gen 10 1 2 942 > 08
gen 10 1 5 1675 > 09
gen 10 1 7 273 > 10

gen 10 1 9 2123 > 11
gen 10 1 10 9123 > 12
gen 20 15 22 3123 > 13
gen 20 40 70 1273 > 14
gen 20 70 10 1223 > 15

gen 21 4 22 533 > 16
gen 0 1 2 8347 > 17
gen 0 1 2 548 > 18
gen 10 12 0 1593 > 19
gen 0 1 1  27 > 20

gen 0 1 1 57 > 21
gen 0 1 1 235 > 22
gen 0 1 1 9563 > 23
gen 99 100 0 246 > 24
gen 0 1 1 4586 > 25

gen 0 1 0 123 > 26
gen 0 1 1 23 > 27
gen 0 1 2 942 > 28
gen 0 1 5 1675 > 29
gen 0 1 7 273 > 30

gen 0 1 9 2123 > 31
gen 0 1 10 9123 > 32
gen 0 15 22 3123 > 33
gen 0 40 70 1273 > 34
gen 0 70 10 1223 > 35



for %%i in (??) do (
	copy %%i masterspark.in >nul
	echo %%i
	solution
	copy masterspark.out %%i.a >nul
)

