echo off
for /l %%i in (1,1,400) do (
    del %%i
    del %%i.a
    del %%i.b
)
