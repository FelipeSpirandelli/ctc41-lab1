* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Init Function (gdc)
  4:     ST  0,-1(2) 	store return address from ac
* -> Param
* <- Param
* -> Param
* <- Param
* -> if
* -> Op
* -> Id
  5:     LD  0,-3(2) 	load id value
* <- Id
  6:     ST  0,-4(2) 	op: push left
* -> Const
  7:    LDC  0,0(0) 	load const
* <- Const
  8:     LD  1,-4(2) 	op: load left
  9:    SUB  0,1,0 	op ==
 10:    JEQ  0,2(7) 	br if true
 11:    LDC  0,0(0) 	false case
 12:    LDA  7,1(7) 	unconditional jmp
 13:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> return
* -> Id
 15:     LD  0,-2(2) 	load id value
* <- Id
 16:    LDA  1,0(2) 	save current fp into ac1
 17:     LD  2,0(2) 	make fp = ofp
 18:     LD  7,-1(1) 	return to caller
* <- return
* if: jump to end belongs here
 14:    JEQ  0,5(7) 	if: jmp to else
* -> return
* -> Function Call (gdc)
 20:     ST  2,-4(2) 	Guard fp
* -> Id
 21:     LD  0,-3(2) 	load id value
* <- Id
 22:     ST  0,-6(2) 	Store value of func argument
* -> Op
* -> Id
 23:     LD  0,-2(2) 	load id value
* <- Id
 24:     ST  0,-7(2) 	op: push left
* -> Op
* -> Op
* -> Id
 25:     LD  0,-2(2) 	load id value
* <- Id
 26:     ST  0,-8(2) 	op: push left
* -> Id
 27:     LD  0,-3(2) 	load id value
* <- Id
 28:     LD  1,-8(2) 	op: load left
 29:    DIV  0,1,0 	op /
* <- Op
 30:     ST  0,-8(2) 	op: push left
* -> Id
 31:     LD  0,-3(2) 	load id value
* <- Id
 32:     LD  1,-8(2) 	op: load left
 33:    MUL  0,1,0 	op *
* <- Op
 34:     LD  1,-7(2) 	op: load left
 35:    SUB  0,1,0 	op -
* <- Op
 36:     ST  0,-7(2) 	Store value of func argument
 37:    LDA  2,-4(2) 	change fp
 38:    LDC  0,40(0) 	Guard return adress
 39:    LDA  7,-36(7) 	jump to function
* <- Function Call
 40:    LDA  1,0(2) 	save current fp into ac1
 41:     LD  2,0(2) 	make fp = ofp
 42:     LD  7,-1(1) 	return to caller
* <- return
 19:    LDA  7,23(7) 	jmp to end
* <- if
* <- End Function
* -> Init Function (main)
  3:    LDA  7,39(7) 	jump to main
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Function Call (input)
 43:     IN  0,0,0 	read input
 44:     ST  0,-2(2) 	assign: store value
* <- assign
* -> assign
* -> Function Call (input)
 45:     IN  0,0,0 	read input
 46:     ST  0,-3(2) 	assign: store value
* <- assign
* -> Function Call (output)
* -> Function Call (gdc)
 47:     ST  2,-4(2) 	Guard fp
* -> Id
 48:     LD  0,-2(2) 	load id value
* <- Id
 49:     ST  0,-6(2) 	Store value of func argument
* -> Id
 50:     LD  0,-3(2) 	load id value
* <- Id
 51:     ST  0,-7(2) 	Store value of func argument
 52:    LDA  2,-4(2) 	change fp
 53:    LDC  0,55(0) 	Guard return adress
 54:    LDA  7,-51(7) 	jump to function
* <- Function Call
 55:    OUT  0,0,0 	print value
* <- End Function
* End of execution.
 56:   HALT  0,0,0 	
