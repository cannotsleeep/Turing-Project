#Q = {0,2,_0,_1,_0_,_1_,accept}
#S = {0,1}
#G = {0,1,_}
#q0 = 0
#B = _
#F = {accept}
#N = 2
0 __ __ ** accept
0 0_ __ r* _0
0 1_ __ r* _1
_0 0_ 0_ r* _0
_0 1_ 0_ r* _1
_1 0_ 1_ r* _0
_1 1_ 1_ r* _1
_0 __ 0_ ** 2
_1 __ 1_ ** 2
2 0_ __ l* _0_
2 1_ __ l* _1_
_0_ *_ *_ l* _0_
_1_ *_ *_ l* _1_
_0_ __ 0_ ** accept
_1_ __ 1_ ** accept