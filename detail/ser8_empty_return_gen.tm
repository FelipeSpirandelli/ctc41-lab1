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
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Const
  5:    LDC  0,1(0) 	load const
* <- Const
  6:     ST  0,-4(2) 	assign: store value
* <- assign
* -> assign
* -> Const
  7:    LDC  0,1(0) 	load const
* <- Const
  8:     ST  0,-6(2) 	assign: store value
* <- assign
* -> if
* -> Op
* -> Id
  9:     LD  0,-3(2) 	load id value
* <- Id
 10:     ST  0,-7(2) 	op: push left
* -> Const
 11:    LDC  0,0(0) 	load const
* <- Const
 12:     LD  1,-7(2) 	op: load left
 13:    SUB  0,1,0 	op ==
 14:    JEQ  0,2(7) 	br if true
 15:    LDC  0,0(0) 	false case
 16:    LDA  7,1(7) 	unconditional jmp
 17:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Id
 19:     LD  0,-2(2) 	load id value
* <- Id
 20:     ST  0,-5(2) 	assign: store value
* <- assign
* if: jump to end belongs here
 18:    JEQ  0,3(7) 	if: jmp to else
* -> assign
* -> Function Call (gdc)
 22:     ST  2,-7(2) 	Guard fp
* -> Id
 23:     LD  0,-3(2) 	load id value
* <- Id
 24:     ST  0,-9(2) 	Store value of func argument
* -> Op
* -> Id
 25:     LD  0,-2(2) 	load id value
* <- Id
 26:     ST  0,-10(2) 	op: push left
* -> Op
* -> Op
* -> Id
 27:     LD  0,-2(2) 	load id value
* <- Id
 28:     ST  0,-11(2) 	op: push left
* -> Id
 29:     LD  0,-3(2) 	load id value
* <- Id
 30:     LD  1,-11(2) 	op: load left
 31:    DIV  0,1,0 	op /
* <- Op
 32:     ST  0,-11(2) 	op: push left
* -> Id
 33:     LD  0,-3(2) 	load id value
* <- Id
 34:     LD  1,-11(2) 	op: load left
 35:    MUL  0,1,0 	op *
* <- Op
 36:     LD  1,-10(2) 	op: load left
 37:    SUB  0,1,0 	op -
* <- Op
 38:     ST  0,-10(2) 	Store value of func argument
 39:    LDA  2,-7(2) 	change fp
 40:    LDC  0,42(0) 	Guard return adress
 41:    LDA  7,-38(7) 	jump to function
* <- Function Call
 42:     ST  0,-5(2) 	assign: store value
* <- assign
 21:    LDA  7,21(7) 	jmp to end
* <- if
* -> assign
* -> Const
 43:    LDC  0,1(0) 	load const
* <- Const
 44:     ST  0,-4(2) 	assign: store value
* <- assign
* -> assign
* -> Const
 45:    LDC  0,1(0) 	load const
* <- Const
 46:     ST  0,-6(2) 	assign: store value
* <- assign
* -> return
 47:    LDA  1,0(2) 	save current fp into ac1
 48:     LD  2,0(2) 	make fp = ofp
 49:     LD  7,-1(1) 	return to caller
* <- return
* <- End Function
* -> Init Function (main)
  3:    LDA  7,46(7) 	jump to main
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Function Call (input)
 50:     IN  0,0,0 	read input
 51:     ST  0,-2(2) 	assign: store value
* <- assign
* -> assign
* -> Function Call (input)
 52:     IN  0,0,0 	read input
 53:     ST  0,-3(2) 	assign: store value
* <- assign
* -> assign
* -> Function Call (gdc)
 54:     ST  2,-4(2) 	Guard fp
* -> Id
 55:     LD  0,-2(2) 	load id value
* <- Id
 56:     ST  0,-6(2) 	Store value of func argument
* -> Id
 57:     LD  0,-3(2) 	load id value
* <- Id
 58:     ST  0,-7(2) 	Store value of func argument
 59:    LDA  2,-4(2) 	change fp
 60:    LDC  0,62(0) 	Guard return adress
 61:    LDA  7,-58(7) 	jump to function
* <- Function Call
 62:     ST  0,-2(2) 	assign: store value
* <- assign
* -> Function Call (output)
* -> Id
 63:     LD  0,-2(2) 	load id value
* <- Id
 64:    OUT  0,0,0 	print value
* <- End Function
* End of execution.
 65:   HALT  0,0,0 	
