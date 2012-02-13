echo off
for /l %%i in (1,1,6) do (
    echo ---------------------------- test %%i -----------------------------------
    echo generating...
    gen_dense %%i > %%i
    echo done
    echo validating...
    validator < %%i
    echo done
    echo running correct...
    correct_sp < %%i > %%i.a
    echo done
    echo ----------------------------- end %%i -----------------------------------
)
   