echo off
for /l %%i in (1,1,50) do (
    echo ---------------------------- test %%i -----------------------------------
    echo generating...
    gen %%i > input.txt
    echo done
    echo validating...
    validator < input.txt
    echo done
    echo running simple...
    simple < input.txt > simple.out
    echo done
    echo running n2...
    n2 < input.txt > n2.out
    echo done
    echo running nlogn...
    VSCPPSolution.exe < input.txt > nlogn.out
    echo done
    fc simple.out n2.out
    fc simple.out nlogn.out
    echo ----------------------------- end %%i -----------------------------------
)
   