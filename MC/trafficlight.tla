-------------------------MODULE trafficlight-------------------------
EXTENDS Integers
VARIABLES T1, T2, x

TypeOK == x \in 0..20 /\ T1 \in 0..1 /\ T2 \in 0..1

\* T1 traffic light 1 initially red(0)
\* T2 traffic light 2 initially green(1)
\* Green->1 and Red->0
\* x->counter(till 0-20)

Init0 == (T1=0)
Init1 == (T2=1)
Init2 == (x=0)
Init == Init0 /\ Init1 /\ Init2

pi0 == (x<20) /\ (x'=x+1) /\ UNCHANGED<<T1>>
pi1 == (x=20) /\ (x'=0) /\ (T1'=1-T1)

NextT1 == \/ (pi0) \/ (pi1)

si0 == (x<20) /\ (x'=x+1) /\ UNCHANGED<<T2>>
si1 == (x=20) /\ (x'=0) /\ (T2'=1-T2)

NextT2 == \/ (si0) \/ (si1)

Next == \/ ((NextT1) /\ (NextT2))

MEXCLUSION == ~((T1=1) /\ (T2=1)) 

==================================================================