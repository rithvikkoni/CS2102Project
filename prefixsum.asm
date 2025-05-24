adj 0x0000ff ; sp offset
adj -2 ; 
main: ldc 0 ; 
stl 0 ; current sum
ldc 0 ; 
stl 1 ; i = 0
loop: ldc size  
ldnl 0 ; size of array(n)
ldl 1  ; 
sub  ; 
brlz exit ;Exit if i >= n
ldc arr; base adress
ldl 1 ; i
add  ; curr index adress
ldnl 0 ; arr[i]
ldl 0  ;
add    ; new sum = currsum + arr[i]
stl 0  ; new sum
ldc arr ; 
ldl 1   ; 
add     ;
stl 2   ;
ldl 0   ;
ldl 2   ;
stnl 0  ; Store the sum back to arr[i]
ldl 1   ; Load i
adc 1   ; i++
stl 1   ;
br loop
exit: HALT
size: data 5 
arr: data 1  
     data 2
     data 3
     data 4
     data 5