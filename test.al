mov ci-1 vd0
jiz vd0 test
call test_func
lbl test
prn cspast_test_func\n eas

ext cd0



lbl test_func
mov cd10 vd0
mov cd4 vd1
mod vd0 vd1 vd0 
prn vd0 cs\n eas 
ret
