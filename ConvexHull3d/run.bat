echo off
for /l %%i in (1,1,19) do (
    echo ---------------------------- test %%i -----------------------------------
    gen %%i > input.txt
    validator < input.txt
    simple < input.txt > simple.out
    n2 < input.txt > n2.out
    fc simple.out n2.out
    echo ----------------------------- end %%i -----------------------------------
)   