echo off
for /l %%i in (51,1,52) do (
    echo ---------------------------- test %%i -----------------------------------
    echo generating...
    gen %%i > input.txt
    echo done
    echo running n2...
    n2 < input.txt > n2.out
    echo done
    echo ----------------------------- end %%i -----------------------------------
)
   