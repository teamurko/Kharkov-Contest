echo off
g++ -o gen gen.cpp
g++ -o correct n2_correct.cpp
g++ -o validator validator.cpp
set /A numTests=50
echo %numTests% > tests/all
for /l %%i in (1,1,%numTests%) do (
    echo ---------------------------- test %%i -----------------------------------
    echo generating...
    gen %%i > tests/%%i
    gen %%i >> tests/all
    echo done
    echo validating...
    validator < tests/%%i
    echo done
    del /S %%i
    echo ----------------------------- end %%i -----------------------------------
)
mv tests/all tests/1
echo 1 > tests/2
cat tests/maxtest1.in >> tests/2
echo 1 > tests/3
cat tests/maxtest2.in >> tests/3
echo 1 > tests/4
cat tests/maxtest3.in >> tests/4
echo running n2...
for /l %%i in (1, 1, 4) do (
    correct < tests/%%i > tests/%%i.a
)
echo done
   