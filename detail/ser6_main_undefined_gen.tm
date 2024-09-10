* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Init Function (funOne)
  4:     ST  0,-1(2) 	store return address from ac
* -> Function Call (output)
* -> Const
  5:    LDC  0,1(0) 	load const
* <- Const
  6:    OUT  0,0,0 	print value
* <- End Function
* -> Init Function (funTwo)
  7:     ST  0,-1(2) 	store return address from ac
* -> Function Call (output)
* -> Const
  8:    LDC  0,2(0) 	load const
* <- Const
  9:    OUT  0,0,0 	print value
* <- End Function
* -> Init Function (funThree)
 10:     ST  0,-1(2) 	store return address from ac
* -> Function Call (output)
* -> Const
 11:    LDC  0,3(0) 	load const
* <- Const
 12:    OUT  0,0,0 	print value
* <- End Function
* End of execution.
 13:   HALT  0,0,0 	
