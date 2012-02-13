echo off
g++ -o gen gen.cpp
g++ -o correct simple.cpp
g++ -o validator validator.cpp
for /l %%i in (1,1,50) do (
    echo ---------------------------- test %%i -----------------------------------
    echo generating...
    gen %%i > tests/%%i
    echo done
    echo validating...
    validator < tests/%%i
    echo done
    echo running simple...
    correct < tests/%%i > tests/%%i.a
    echo done
    echo ----------------------------- end %%i -----------------------------------
)
   