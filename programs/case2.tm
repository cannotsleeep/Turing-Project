#Q = {0,t,minus,reject0,reject,reject1,f,a,l,s,clear1,plus,plusl,clear0,clear1,accept,r,u,final}
#S = {1}
#G = {1,_,f,a,l,s,e,t,r,u}
#q0 = 0
#B = _
#F = {final}
#N = 2
0 __ t_ r* t
0 *_ *1 ** minus
minus 11 11 rr minus
minus _1 _1 l* reject0
minus 1_ 11 *r plus
minus __ __ l* clear0
reject0 1* 1* r* reject0
reject0 _* _* l* reject1
reject1 1* _* l* reject1
reject1 _* _* r* reject
reject _* f* r* f
f _* a* r* a
a _* l* r* l
l _* s* r* s
s _* e* ** s
s e1 e1 *r s
s e_ e_ *l clear1
plus *_ *1 *l plusl
plusl 11 11 *l plusl
plusl 1_ 1_ *r minus
clear0 1_ __ l* clear0
clear0 __ __ r* accept
accept __ t_ r* t
t __ r_ r* r
r __ u_ r* u
u __ e_ *l clear1
clear1 *1 *_ *l clear1
clear1 *_ *_ ** final