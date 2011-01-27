gen > input.txt

copy input.txt %1
nsi.exe  < input.txt > output.txt
copy output.txt %1.a

pdistBline_vg.exe < input.txt > output.txt
check input.txt output.txt %1.a

more %1.a