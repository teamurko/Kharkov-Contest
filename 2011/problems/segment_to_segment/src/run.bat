set /A numTests=80
for /l %%i in (1,1,%numTests%) do (
    echo ---------------------------- test %%i -----------------------------------
    correct.exe < tests/%%i > tests/%%i.b
    check_d.exe tests/%%i tests/%%i.a tests/%%i.b
    del /S %%i.b
    echo ----------------------------- end %%i -----------------------------------
)
