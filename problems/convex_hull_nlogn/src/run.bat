echo off
g++ -o gen gen.cpp
g++ -o correct nlogn_correct.cpp
g++ -o validator validator.cpp
set /A numTests=50
:: 102 tests constant hard coded, 102 = numTests * 2 + 2
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
echo 30 > tests/2
for /l %%i in (1, 1, 10) do (
    cat tests/maxtest1.in >> tests/2
    cat tests/maxtest2.in >> tests/2
    cat tests/maxtest3.in >> tests/2
)
echo running nlogn...
correct < tests/1 > tests/1.a
correct < tests/2 > tests/2.a
echo done
   