* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Init Function (main)
  3:    LDA  7,0(7) 	jump to main
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> if
* -> Op
* -> Id
  4:     LD  0,-3(2) 	load id value
* <- Id
  5:     ST  0,-6(2) 	op: push left
* -> Id
  6:     LD  0,-4(2) 	load id value
* <- Id
  7:     LD  1,-6(2) 	op: load left
  8:    SUB  0,1,0 	op ==
  9:    JEQ  0,2(7) 	br if true
 10:    LDC  0,0(0) 	false case
 11:    LDA  7,1(7) 	unconditional jmp
 12:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Const
 14:    LDC  0,0(0) 	load const
* <- Const
 15:     ST  0,-2(2) 	assign: store value
* <- assign
* if: jump to end belongs here
 13:    JEQ  0,3(7) 	if: jmp to else
* -> assign
* -> Const
 17:    LDC  0,1(0) 	load const
* <- Const
 18:     ST  0,-2(2) 	assign: store value
* <- assign
 16:    LDA  7,2(7) 	jmp to end
* <- if
* -> if
* -> Op
* -> Id
 19:     LD  0,-3(2) 	load id value
* <- Id
 20:     ST  0,-6(2) 	op: push left
* -> Id
 21:     LD  0,-4(2) 	load id value
* <- Id
 22:     LD  1,-6(2) 	op: load left
 23:    SUB  0,1,0 	op <
 24:    JLT  0,2(7) 	br if true
 25:    LDC  0,0(0) 	false case
 26:    LDA  7,1(7) 	unconditional jmp
 27:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Op
* -> Id
 29:     LD  0,-4(2) 	load id value
* <- Id
 30:     ST  0,-6(2) 	op: push left
* -> Const
 31:    LDC  0,1(0) 	load const
* <- Const
 32:     LD  1,-6(2) 	op: load left
 33:    ADD  0,1,0 	op +
* <- Op
 34:     ST  0,-3(2) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Id
 35:     LD  0,-4(2) 	load id value
* <- Id
 36:     ST  0,-6(2) 	op: push left
* -> Op
* -> Op
* -> Const
 37:    LDC  0,3(0) 	load const
* <- Const
 38:     ST  0,-7(2) 	op: push left
* -> Const
 39:    LDC  0,5(0) 	load const
* <- Const
 40:     LD  1,-7(2) 	op: load left
 41:    MUL  0,1,0 	op *
* <- Op
 42:     ST  0,-7(2) 	op: push left
* -> Const
 43:    LDC  0,2(0) 	load const
* <- Const
 44:     LD  1,-7(2) 	op: load left
 45:    DIV  0,1,0 	op /
* <- Op
 46:     LD  1,-6(2) 	op: load left
 47:    ADD  0,1,0 	op +
* <- Op
 48:     ST  0,-4(2) 	assign: store value
* <- assign
* if: jump to end belongs here
 28:    JEQ  0,21(7) 	if: jmp to else
 49:    LDA  7,0(7) 	jmp to end
* <- if
* -> if
* -> Op
* -> Id
 50:     LD  0,-3(2) 	load id value
* <- Id
 51:     ST  0,-6(2) 	op: push left
* -> Id
 52:     LD  0,-4(2) 	load id value
* <- Id
 53:     LD  1,-6(2) 	op: load left
 54:    SUB  0,1,0 	op >
 55:    JGT  0,2(7) 	br if true
 56:    LDC  0,0(0) 	false case
 57:    LDA  7,1(7) 	unconditional jmp
 58:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Const
 60:    LDC  0,8(0) 	load const
* <- Const
 61:     ST  0,-3(2) 	assign: store value
* <- assign
* if: jump to end belongs here
 59:    JEQ  0,3(7) 	if: jmp to else
* -> assign
* -> Const
 63:    LDC  0,75(0) 	load const
* <- Const
 64:     ST  0,-4(2) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Op
* -> Id
 65:     LD  0,-4(2) 	load id value
* <- Id
 66:     ST  0,-6(2) 	op: push left
* -> Op
* -> Const
 67:    LDC  0,5(0) 	load const
* <- Const
 68:     ST  0,-7(2) 	op: push left
* -> Const
 69:    LDC  0,3(0) 	load const
* <- Const
 70:     LD  1,-7(2) 	op: load left
 71:    DIV  0,1,0 	op /
* <- Op
 72:     LD  1,-6(2) 	op: load left
 73:    MUL  0,1,0 	op *
* <- Op
 74:     ST  0,-6(2) 	op: push left
* -> Const
 75:    LDC  0,7(0) 	load const
* <- Const
 76:     LD  1,-6(2) 	op: load left
 77:    ADD  0,1,0 	op +
* <- Op
 78:     ST  0,-3(2) 	assign: store value
* <- assign
 62:    LDA  7,16(7) 	jmp to end
* <- if
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
 79:     LD  0,-5(2) 	load id value
* <- Id
 80:     ST  0,-6(2) 	op: push left
* -> Const
 81:    LDC  0,10(0) 	load const
* <- Const
 82:     LD  1,-6(2) 	op: load left
 83:    SUB  0,1,0 	op <=
 84:    JLE  0,2(7) 	br if true
 85:    LDC  0,0(0) 	false case
 86:    LDA  7,1(7) 	unconditional jmp
 87:    LDC  0,1(0) 	true case
* <- Op
* -> assign
* -> Op
* -> Id
 89:     LD  0,-5(2) 	load id value
* <- Id
 90:     ST  0,-6(2) 	op: push left
* -> Const
 91:    LDC  0,2(0) 	load const
* <- Const
 92:     LD  1,-6(2) 	op: load left
 93:    MUL  0,1,0 	op *
* <- Op
 94:     ST  0,-4(2) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Id
 95:     LD  0,-4(2) 	load id value
* <- Id
 96:     ST  0,-6(2) 	op: push left
* -> Const
 97:    LDC  0,4(0) 	load const
* <- Const
 98:     LD  1,-6(2) 	op: load left
 99:    ADD  0,1,0 	op +
* <- Op
100:     ST  0,-3(2) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Id
101:     LD  0,-5(2) 	load id value
* <- Id
102:     ST  0,-6(2) 	op: push left
* -> Const
103:    LDC  0,1(0) 	load const
* <- Const
104:     LD  1,-6(2) 	op: load left
105:    ADD  0,1,0 	op +
* <- Op
106:     ST  0,-5(2) 	assign: store value
* <- assign
107:    LDA  7,-29(7) 	jump to savedLoc1
 88:    JEQ  0,19(7) 	repeat: jmp to end
* <- repeat
* <- End Function
* End of execution.
108:   HALT  0,0,0 	
