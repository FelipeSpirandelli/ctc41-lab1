* TINY Compilation to TM Code
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     LD  2,0(0) 	load maxaddress from location 0
  2:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> declare vector
  3:    LDC  0,10(0) 	load global position to ac
  4:    LDC  5,0(0) 	load 0
  5:     ST  0,10(5) 	store ac in global_position_aux
* <- declare vector
* -> Init Function (minloc)
  7:     ST  0,-1(2) 	store return address from ac
* -> Param vet
* <- Param vet
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
* -> Id
  8:     LD  0,-3(2) 	load id value
* <- Id
  9:     ST  0,-7(2) 	assign: store value
* <- assign
* -> assign
* -> Id
* -> Vector
 10:     LD  0,-2(2) 	get the address of the vector
 11:     LD  3,-3(2) 	get the value of the index
 12:    LDC  4,1(0) 	load 1
 13:    ADD  3,3,4 	sub 3 by 1
 14:    SUB  0,0,3 	get the address
 15:     LD  0,0(0) 	get the value of the vector
* <- Id
 16:     ST  0,-6(2) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Id
 17:     LD  0,-3(2) 	load id value
* <- Id
 18:     ST  0,-8(2) 	op: push left
* -> Const
 19:    LDC  0,1(0) 	load const
* <- Const
 20:     LD  1,-8(2) 	op: load left
 21:    ADD  0,1,0 	op +
* <- Op
 22:     ST  0,-5(2) 	assign: store value
* <- assign
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
 23:     LD  0,-5(2) 	load id value
* <- Id
 24:     ST  0,-8(2) 	op: push left
* -> Id
 25:     LD  0,-4(2) 	load id value
* <- Id
 26:     LD  1,-8(2) 	op: load left
 27:    SUB  0,1,0 	op <
 28:    JLT  0,2(7) 	br if true
 29:    LDC  0,0(0) 	false case
 30:    LDA  7,1(7) 	unconditional jmp
 31:    LDC  0,1(0) 	true case
* <- Op
* -> if
* -> Op
* -> Id
* -> Vector
 33:     LD  0,-2(2) 	get the address of the vector
 34:     LD  3,-5(2) 	get the value of the index
 35:    LDC  4,1(0) 	load 1
 36:    ADD  3,3,4 	sub 3 by 1
 37:    SUB  0,0,3 	get the address
 38:     LD  0,0(0) 	get the value of the vector
* <- Id
 39:     ST  0,-8(2) 	op: push left
* -> Id
 40:     LD  0,-6(2) 	load id value
* <- Id
 41:     LD  1,-8(2) 	op: load left
 42:    SUB  0,1,0 	op <
 43:    JLT  0,2(7) 	br if true
 44:    LDC  0,0(0) 	false case
 45:    LDA  7,1(7) 	unconditional jmp
 46:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Id
* -> Vector
 48:     LD  0,-2(2) 	get the address of the vector
 49:     LD  3,-5(2) 	get the value of the index
 50:    LDC  4,1(0) 	load 1
 51:    ADD  3,3,4 	sub 3 by 1
 52:    SUB  0,0,3 	get the address
 53:     LD  0,0(0) 	get the value of the vector
* <- Id
 54:     ST  0,-6(2) 	assign: store value
* <- assign
* -> assign
* -> Id
 55:     LD  0,-5(2) 	load id value
* <- Id
 56:     ST  0,-7(2) 	assign: store value
* <- assign
* if: jump to end belongs here
 47:    JEQ  0,10(7) 	if: jmp to else
 57:    LDA  7,0(7) 	jmp to end
* <- if
* -> assign
* -> Op
* -> Id
 58:     LD  0,-5(2) 	load id value
* <- Id
 59:     ST  0,-8(2) 	op: push left
* -> Const
 60:    LDC  0,1(0) 	load const
* <- Const
 61:     LD  1,-8(2) 	op: load left
 62:    ADD  0,1,0 	op +
* <- Op
 63:     ST  0,-5(2) 	assign: store value
* <- assign
 64:    LDA  7,-42(7) 	jump to savedLoc1
 32:    JEQ  0,32(7) 	repeat: jmp to end
* <- repeat
* -> return
* -> Id
 65:     LD  0,-7(2) 	load id value
* <- Id
 66:    LDA  1,0(2) 	save current fp into ac1
 67:     LD  2,0(2) 	make fp = ofp
 68:     LD  7,-1(1) 	return to caller
* <- return
* <- End Function
* -> Init Function (sort)
 69:     ST  0,-1(2) 	store return address from ac
* -> Param vet
* <- Param vet
* -> Param
* <- Param
* -> Param
* <- Param
* -> declare var
* <- declare var
* -> declare var
* <- declare var
* -> assign
* -> Id
 70:     LD  0,-3(2) 	load id value
* <- Id
 71:     ST  0,-5(2) 	assign: store value
* <- assign
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
 72:     LD  0,-5(2) 	load id value
* <- Id
 73:     ST  0,-7(2) 	op: push left
* -> Op
* -> Id
 74:     LD  0,-4(2) 	load id value
* <- Id
 75:     ST  0,-8(2) 	op: push left
* -> Const
 76:    LDC  0,1(0) 	load const
* <- Const
 77:     LD  1,-8(2) 	op: load left
 78:    SUB  0,1,0 	op -
* <- Op
 79:     LD  1,-7(2) 	op: load left
 80:    SUB  0,1,0 	op <
 81:    JLT  0,2(7) 	br if true
 82:    LDC  0,0(0) 	false case
 83:    LDA  7,1(7) 	unconditional jmp
 84:    LDC  0,1(0) 	true case
* <- Op
* -> declare var
* <- declare var
* -> assign
* -> Function Call (minloc)
 86:     ST  2,-8(2) 	Guard fp
* -> Id
* -> Vector
 87:     LD  0,-2(2) 	load id value
* <- Id
 88:     ST  0,-10(2) 	Store value of func argument
* -> Id
 89:     LD  0,-5(2) 	load id value
* <- Id
 90:     ST  0,-11(2) 	Store value of func argument
* -> Id
 91:     LD  0,-4(2) 	load id value
* <- Id
 92:     ST  0,-12(2) 	Store value of func argument
 93:    LDA  2,-8(2) 	change fp
 94:    LDC  0,96(0) 	Guard return adress
 95:    LDA  7,-89(7) 	jump to function
* <- Function Call
 96:     ST  0,-6(2) 	assign: store value
* <- assign
* -> assign
* -> Id
* -> Vector
 97:     LD  0,-2(2) 	get the address of the vector
 98:     LD  3,-6(2) 	get the value of the index
 99:    LDC  4,1(0) 	load 1
100:    ADD  3,3,4 	sub 3 by 1
101:    SUB  0,0,3 	get the address
102:     LD  0,0(0) 	get the value of the vector
* <- Id
103:     ST  0,-7(2) 	assign: store value
* <- assign
* -> assign vector
* -> Vector
* -> Id
* -> Vector
104:     LD  0,-2(2) 	get the address of the vector
105:     LD  3,-5(2) 	get the value of the index
106:    LDC  4,1(0) 	load 1
107:    ADD  3,3,4 	sub 3 by 1
108:    SUB  0,0,3 	get the address
109:     LD  0,0(0) 	get the value of the vector
* <- Id
110:     LD  1,-2(2) 	get the address of the vector
111:     LD  3,-6(2) 	get the value of the index
112:    LDC  4,1(0) 	load 1
113:    ADD  3,3,4 	sub 3 by 1
114:    SUB  1,1,3 	get the address
115:     ST  0,0(1) 	get the value of the vector
* -> assign vector
* -> Vector
* -> Id
116:     LD  0,-7(2) 	load id value
* <- Id
117:     LD  1,-2(2) 	get the address of the vector
118:     LD  3,-5(2) 	get the value of the index
119:    LDC  4,1(0) 	load 1
120:    ADD  3,3,4 	sub 3 by 1
121:    SUB  1,1,3 	get the address
122:     ST  0,0(1) 	get the value of the vector
* -> assign
* -> Op
* -> Id
123:     LD  0,-5(2) 	load id value
* <- Id
124:     ST  0,-8(2) 	op: push left
* -> Const
125:    LDC  0,1(0) 	load const
* <- Const
126:     LD  1,-8(2) 	op: load left
127:    ADD  0,1,0 	op +
* <- Op
128:     ST  0,-5(2) 	assign: store value
* <- assign
129:    LDA  7,-58(7) 	jump to savedLoc1
 85:    JEQ  0,44(7) 	repeat: jmp to end
* <- repeat
130:    LDA  1,0(2) 	save current fp into ac1
131:     LD  2,0(2) 	make fp = ofp
132:     LD  7,-1(1) 	return to caller
* <- End Function
* -> Init Function (main)
  6:    LDA  7,126(7) 	jump to main
* -> declare var
* <- declare var
* -> assign
* -> Const
133:    LDC  0,0(0) 	load const
* <- Const
134:     ST  0,-2(2) 	assign: store value
* <- assign
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
135:     LD  0,-2(2) 	load id value
* <- Id
136:     ST  0,-3(2) 	op: push left
* -> Const
137:    LDC  0,10(0) 	load const
* <- Const
138:     LD  1,-3(2) 	op: load left
139:    SUB  0,1,0 	op <
140:    JLT  0,2(7) 	br if true
141:    LDC  0,0(0) 	false case
142:    LDA  7,1(7) 	unconditional jmp
143:    LDC  0,1(0) 	true case
* <- Op
* -> assign vector
* -> Vector
* -> Function Call (input)
145:     IN  0,0,0 	read input
146:    LDC  5,0(0) 	load 0
147:     LD  1,10(5) 	get the address of the vector
148:     LD  3,-2(2) 	get the value of the index
149:    LDC  4,1(0) 	load 1
150:    ADD  3,3,4 	sub 3 by 1
151:    SUB  1,1,3 	get the address
152:     ST  0,0(1) 	get the value of the vector
* -> assign
* -> Op
* -> Id
153:     LD  0,-2(2) 	load id value
* <- Id
154:     ST  0,-3(2) 	op: push left
* -> Const
155:    LDC  0,1(0) 	load const
* <- Const
156:     LD  1,-3(2) 	op: load left
157:    ADD  0,1,0 	op +
* <- Op
158:     ST  0,-2(2) 	assign: store value
* <- assign
159:    LDA  7,-25(7) 	jump to savedLoc1
144:    JEQ  0,15(7) 	repeat: jmp to end
* <- repeat
* -> Function Call (sort)
160:     ST  2,-3(2) 	Guard fp
* -> Id
* -> Vector
161:    LDC  5,0(0) 	load 0
162:     LD  0,10(5) 	load id value
* <- Id
163:     ST  0,-5(2) 	Store value of func argument
* -> Const
164:    LDC  0,0(0) 	load const
* <- Const
165:     ST  0,-6(2) 	Store value of func argument
* -> Const
166:    LDC  0,10(0) 	load const
* <- Const
167:     ST  0,-7(2) 	Store value of func argument
168:    LDA  2,-3(2) 	change fp
169:    LDC  0,171(0) 	Guard return adress
170:    LDA  7,-102(7) 	jump to function
* <- Function Call
* -> assign
* -> Const
171:    LDC  0,0(0) 	load const
* <- Const
172:     ST  0,-2(2) 	assign: store value
* <- assign
* -> while
* repeat: jump after body comes back here
* -> Op
* -> Id
173:     LD  0,-2(2) 	load id value
* <- Id
174:     ST  0,-3(2) 	op: push left
* -> Const
175:    LDC  0,10(0) 	load const
* <- Const
176:     LD  1,-3(2) 	op: load left
177:    SUB  0,1,0 	op <
178:    JLT  0,2(7) 	br if true
179:    LDC  0,0(0) 	false case
180:    LDA  7,1(7) 	unconditional jmp
181:    LDC  0,1(0) 	true case
* <- Op
* -> Function Call (output)
* -> Id
* -> Vector
183:    LDC  5,0(0) 	load 0
184:     LD  0,10(5) 	get the address of the vector
185:     LD  3,-2(2) 	get the value of the index
186:    LDC  4,1(0) 	load 1
187:    ADD  3,3,4 	sub 3 by 1
188:    SUB  0,0,3 	get the address
189:     LD  0,0(0) 	get the value of the vector
* <- Id
190:    OUT  0,0,0 	print value
* -> assign
* -> Op
* -> Id
191:     LD  0,-2(2) 	load id value
* <- Id
192:     ST  0,-3(2) 	op: push left
* -> Const
193:    LDC  0,1(0) 	load const
* <- Const
194:     LD  1,-3(2) 	op: load left
195:    ADD  0,1,0 	op +
* <- Op
196:     ST  0,-2(2) 	assign: store value
* <- assign
197:    LDA  7,-25(7) 	jump to savedLoc1
182:    JEQ  0,15(7) 	repeat: jmp to end
* <- repeat
* <- End Function
* End of execution.
198:   HALT  0,0,0 	
