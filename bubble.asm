ldc 0x1000 
a2sp 
adj -7
ldc size
ldnl 0
stl 0; size of the array
ldc 1
stl 1; i = 0
ldc 0
stl 2; j = 0
ldc arr
stl 3;storing base address in 3
firstloop:
	ldl 0
	ldl 1
	sub
	brz exit
    ldl 1  ; j = i
	stl 2
    br secondloop
secondloop:
	ldl 2
	brz incri
	ldl 3   ; arr
	ldl 2	;j
	add
	ldnl 0	;arr[j]
	stl 4	
	ldl 2
	ldc 1
	sub	;j-1
	ldl 3	; arr
	add	;
	ldnl 0;arr[j-1]
	stl 5
    ldl 4
	ldl 5
	sub	; (arr[j]-arr[j-1])
	brlz swap

decrj:	
	;j--
	ldl 2
	adc -1
	stl 2
    br secondloop

swap: ldl 3	;array
	ldl 2	;j
	add	;array+j
	stl 6
	ldl 5
	ldl 6
	stnl 0	
    adc -1	;array+j-1
	stl 6
	ldl 4
	ldl 6
	stnl 0	
    br decrj
incri:
	ldl 1
	adc 1
	stl 1
	br firstloop
exit:	HALT
size: data 8
arr:  data 6
    data 4
    data 3
    data 7
    data 2
	data -40
	data 20
	data 0