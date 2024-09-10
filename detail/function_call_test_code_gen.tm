* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Init Function (f)
  4:     ST  0,-1(2) 	store return address from ac
* -> Param
* <- Param
* -> Param
* <- Param
* -> return
* -> Op
* -> Id
  5:     LD  0,-2(2) 	load id value
* <- Id
  6:     ST  0,-4(2) 	op: push left
* -> Id
  7:     LD  0,-3(2) 	load id value
* <- Id
  8:     LD  1,-4(2) 	op: load left
  9:    ADD  0,1,0 	op +
* <- Op
 10:    LDA  1,0(2) 	save current fp into ac1
 11:     LD  2,0(2) 	make fp = ofp
 12:     LD  7,-1(1) 	return to caller
* <- return
* <- End Function
* -> Init Function (g)
 13:     ST  0,-1(2) 	store return address from ac
* -> Param
* <- Param
* -> return
* -> Op
* -> Id
 14:     LD  0,-2(2) 	load id value
* <- Id
 15:     ST  0,-3(2) 	op: push left
* -> Const
 16:    LDC  0,1(0) 	load const
* <- Const
 17:     LD  1,-3(2) 	op: load left
 18:    ADD  0,1,0 	op +
* <- Op
 19:    LDA  1,0(2) 	save current fp into ac1
 20:     LD  2,0(2) 	make fp = ofp
 21:     LD  7,-1(1) 	return to caller
* <- return
* <- End Function
* -> Init Function (main)
  3:    LDA  7,18(7) 	jump to main
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Function Call (f)
 22:     ST  2,-5(2) 	Guard fp
* -> Id
 23:     LD  0,-2(2) 	load id value
* <- Id
 24:     ST  0,-7(2) 	Store value of func argument
* -> Function Call (g)
 25:     ST  2,-8(2) 	Guard fp
* -> Id
 26:     LD  0,-3(2) 	load id value
* <- Id
 27:     ST  0,-10(2) 	Store value of func argument
 28:    LDA  2,-8(2) 	change fp
 29:    LDC  0,31(0) 	Guard return adress
 30:    LDA  7,-18(7) 	jump to function
* <- Function Call
 31:     ST  0,-8(2) 	Store value of func argument
 32:    LDA  2,-5(2) 	change fp
 33:    LDC  0,35(0) 	Guard return adress
 34:    LDA  7,-31(7) 	jump to function
* <- Function Call
 35:     ST  0,-4(2) 	assign: store value
* <- assign
* <- End Function
* End of execution.
 36:   HALT  0,0,0 	
