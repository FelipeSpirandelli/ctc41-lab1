* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Init Function (main)
  3:    LDA  7,0(7) 	jump to main
* -> declare vector
  4:    LDA  0,-2(2) 	guard addr of vector
  5:     ST  0,-2(2) 	store addr of vector
* <- declare vector
* -> declare vector
  6:    LDA  0,-13(2) 	guard addr of vector
  7:     ST  0,-13(2) 	store addr of vector
* <- declare vector
* -> declare vector
  8:    LDA  0,-24(2) 	guard addr of vector
  9:     ST  0,-24(2) 	store addr of vector
* <- declare vector
* -> declare var
* <- declare var
* -> assign
* -> Const
 10:    LDC  0,0(0) 	load const
* <- Const
 11:     ST  0,-35(2) 	assign: store value
* <- assign
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
 12:     LD  0,-35(2) 	load id value
* <- Id
 13:     ST  0,-36(2) 	op: push left
* -> Const
 14:    LDC  0,10(0) 	load const
* <- Const
 15:     LD  1,-36(2) 	op: load left
 16:    SUB  0,1,0 	op <
 17:    JLT  0,2(7) 	br if true
 18:    LDC  0,0(0) 	false case
 19:    LDA  7,1(7) 	unconditional jmp
 20:    LDC  0,1(0) 	true case
* <- Op
* -> assign vector
* -> Vector
* -> Const
 22:    LDC  0,1(0) 	load const
* <- Const
 23:     LD  1,-13(2) 	get the address of the vector
 24:     LD  3,-35(2) 	get the value of the index
 25:    LDC  4,1(0) 	load 1
 26:    ADD  3,3,4 	sub 3 by 1
 27:    SUB  1,1,3 	get the address
 28:     ST  0,0(1) 	get the value of the vector
* -> assign vector
* -> Vector
* -> Const
 29:    LDC  0,1(0) 	load const
* <- Const
 30:     LD  1,-24(2) 	get the address of the vector
 31:     LD  3,-35(2) 	get the value of the index
 32:    LDC  4,1(0) 	load 1
 33:    ADD  3,3,4 	sub 3 by 1
 34:    SUB  1,1,3 	get the address
 35:     ST  0,0(1) 	get the value of the vector
* -> assign
* -> Op
* -> Id
 36:     LD  0,-35(2) 	load id value
* <- Id
 37:     ST  0,-36(2) 	op: push left
* -> Const
 38:    LDC  0,1(0) 	load const
* <- Const
 39:     LD  1,-36(2) 	op: load left
 40:    ADD  0,1,0 	op +
* <- Op
 41:     ST  0,-35(2) 	assign: store value
* <- assign
 42:    LDA  7,-31(7) 	jump to savedLoc1
 21:    JEQ  0,21(7) 	repeat: jmp to end
* <- repeat
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
 43:     LD  0,-35(2) 	load id value
* <- Id
 44:     ST  0,-36(2) 	op: push left
* -> Const
 45:    LDC  0,10(0) 	load const
* <- Const
 46:     LD  1,-36(2) 	op: load left
 47:    SUB  0,1,0 	op <
 48:    JLT  0,2(7) 	br if true
 49:    LDC  0,0(0) 	false case
 50:    LDA  7,1(7) 	unconditional jmp
 51:    LDC  0,1(0) 	true case
* <- Op
* -> assign vector
* -> Vector
* -> Op
* -> Id
* -> Vector
 53:     LD  0,-13(2) 	get the address of the vector
 54:     LD  3,-35(2) 	get the value of the index
 55:    LDC  4,1(0) 	load 1
 56:    ADD  3,3,4 	sub 3 by 1
 57:    SUB  0,0,3 	get the address
 58:     LD  0,0(0) 	get the value of the vector
* <- Id
 59:     ST  0,-36(2) 	op: push left
* -> Id
* -> Vector
 60:     LD  0,-24(2) 	get the address of the vector
 61:     LD  3,-35(2) 	get the value of the index
 62:    LDC  4,1(0) 	load 1
 63:    ADD  3,3,4 	sub 3 by 1
 64:    SUB  0,0,3 	get the address
 65:     LD  0,0(0) 	get the value of the vector
* <- Id
 66:     LD  1,-36(2) 	op: load left
 67:    ADD  0,1,0 	op +
* <- Op
 68:     LD  1,-2(2) 	get the address of the vector
 69:     LD  3,-35(2) 	get the value of the index
 70:    LDC  4,1(0) 	load 1
 71:    ADD  3,3,4 	sub 3 by 1
 72:    SUB  1,1,3 	get the address
 73:     ST  0,0(1) 	get the value of the vector
 74:    LDA  7,-32(7) 	jump to savedLoc1
 52:    JEQ  0,22(7) 	repeat: jmp to end
* <- repeat
* <- End Function
* End of execution.
 75:   HALT  0,0,0 	
