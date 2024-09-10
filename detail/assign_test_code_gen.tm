* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Init Function (funOne)
  4:     ST  0,-1(2) 	store return address from ac
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Op
* -> Const
  5:    LDC  0,1(0) 	load const
* <- Const
  6:     ST  0,-4(2) 	op: push left
* -> Op
* -> Op
* -> Const
  7:    LDC  0,5(0) 	load const
* <- Const
  8:     ST  0,-5(2) 	op: push left
* -> Const
  9:    LDC  0,7(0) 	load const
* <- Const
 10:     LD  1,-5(2) 	op: load left
 11:    MUL  0,1,0 	op *
* <- Op
 12:     ST  0,-5(2) 	op: push left
* -> Const
 13:    LDC  0,2(0) 	load const
* <- Const
 14:     LD  1,-5(2) 	op: load left
 15:    DIV  0,1,0 	op /
* <- Op
 16:     LD  1,-4(2) 	op: load left
 17:    ADD  0,1,0 	op +
* <- Op
 18:     ST  0,-3(2) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Id
 19:     LD  0,-3(2) 	load id value
* <- Id
 20:     ST  0,-4(2) 	op: push left
* -> Const
 21:    LDC  0,4(0) 	load const
* <- Const
 22:     LD  1,-4(2) 	op: load left
 23:    ADD  0,1,0 	op +
* <- Op
 24:     ST  0,-2(2) 	assign: store value
* <- assign
 25:    LDA  1,0(2) 	save current fp into ac1
 26:     LD  2,0(2) 	make fp = ofp
 27:     LD  7,-1(1) 	return to caller
* <- End Function
* -> Init Function (main)
  3:    LDA  7,24(7) 	jump to main
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Op
* -> Id
 28:     LD  0,-2(2) 	load id value
* <- Id
 29:     ST  0,-4(2) 	op: push left
* -> Const
 30:    LDC  0,1(0) 	load const
* <- Const
 31:     LD  1,-4(2) 	op: load left
 32:    ADD  0,1,0 	op +
* <- Op
 33:     ST  0,-3(2) 	assign: store value
* <- assign
* <- End Function
* End of execution.
 34:   HALT  0,0,0 	
