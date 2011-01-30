echo off
g++ -o correct_sp correct_sp.cpp
javac CorrectSp.java
for /l %%i in (1, 1, 40) do (
    echo ---------------------------- test %%i -----------------------------------
    echo generating...
    gen %%i > %%i
    echo done
    correct_sp < %%i > %%i.a
    java CorrectSp < %%i > %%i.b
    fc %%i.a %%i.b
    echo ----------------------------- end %%i -----------------------------------
)
   