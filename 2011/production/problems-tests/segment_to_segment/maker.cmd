gen > input.txt

copy input.txt %1
correct.exe  < input.txt > output.txt
copy output.txt %1.a

correct2.exe < input.txt > output.txt
check input.txt output.txt %1.a

more %1.a