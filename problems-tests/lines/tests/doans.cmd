g++ ..\correct.cpp -o correct.exe
g++ ..\check.cpp -o check.exe

for %%i in (??) do (
    correct.exe <%%i >%%i.a
    check.exe %%i %%i.a %%i.a
)

