g++ ..\lines_vg.cpp -o solution.exe
g++ ..\check.cpp -o check.exe

for %%i in (??) do (
    solution.exe <%%i >%%i.a
    check.exe %%i %%i.a %%i.a
)

