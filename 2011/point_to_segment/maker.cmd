gen > input.txt

copy input.txt %1
sol.exe  < input.txt > output.txt
copy output.txt %1.a
more %1.a