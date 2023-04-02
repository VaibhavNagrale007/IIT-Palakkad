-------------------------MODULE Semaphore-------------------------
EXTENDS Integers
VARIABLES sem, l, m

TypeOK == l \in 0..5 /\ m \in 0..5 /\ sem \in {0,1}

(* 
   sem = 0 => available
   sem = 1 => busy
*)

Init0 == (l=0) /\ (sem=0)
Init1 == (m=0) /\ (sem=0)
Init == Init0 /\ Init1

p01 == (l=0) /\ (l'=1) /\ UNCHANGED<<sem>>
p12 == (l=1) /\ (l'=2) /\ UNCHANGED<<sem>>
p22 == (l=2) /\ (sem=1) /\ UNCHANGED<< l, sem >>
p23 == (l=2) /\ (l'=3) /\ sem = 0 /\ sem'=1
p34 == (l=3) /\ (l'=4) /\ UNCHANGED<<sem>>
p45 == (l=4) /\ (l'=5) /\ sem' = 0
p50 == (l=5) /\ (l'=0) /\ UNCHANGED<<sem>>

Nextp == \/ p01 \/ p12 \/ p22 \/ p23 \/ p34 \/ p45 \/ p50 \/ UNCHANGED<< l, sem >>

s01 == (m=0) /\ (m'=1) /\ UNCHANGED<<sem>>
s12 == (m=1) /\ (m'=2) /\ UNCHANGED<<sem>>
s22 == (m=2) /\ (sem=1) /\ UNCHANGED<< m, sem >>
s23 == (m=2) /\ (m'=3) /\ sem = 0 /\ sem'=1
s34 == (m=3) /\ (m'=4) /\ UNCHANGED<<sem>>
s45 == (m=4) /\ (m'=5) /\ sem' = 0
s50 == (m=5) /\ (m'=0) /\ UNCHANGED<<sem>>

Nexts == \/ s01 \/ s12 \/ s22 \/ s23 \/ s34 \/ s45 \/ s50 \/ UNCHANGED<< m, sem >>

Next == \/ (Nextp /\ UNCHANGED<<m>>) \/ (Nexts /\ UNCHANGED<<l>>)

vars == << sem, l, m >>

FairnessW == WF_l(p01)
            /\ WF_l(p23)
            /\ WF_l(p34)
            /\ WF_l(p45)
            /\ WF_l(p50)
            /\ WF_m(s01)
            /\ WF_m(s23)
            /\ WF_m(s34)
            /\ WF_m(s45)
            /\ WF_m(s50) 

FairSpecW == Init /\ [][Next]_vars /\ FairnessW

FairnessS == WF_l(p01)
            /\ SF_l(p23)
            /\ WF_l(p34)
            /\ WF_l(p45)
            /\ WF_l(p50)
            /\ WF_m(s01)
            /\ SF_m(s23)
            /\ WF_m(s34)
            /\ WF_m(s45)
            /\ WF_m(s50) 

FairSpecS == Init /\ [][Next]_vars /\ FairnessS

MEXCLUSION == ~((l=3)/\(m=3))

BOUNDEDWAIT1 == []((l=2) => <>(l=3))
BOUNDEDWAIT2 == []((m=2) => <>(m=3))

==================================================================
(*
    + FairnessW   => It should be countinuously enabled (eg. ||||||||... )
    + FairnessS   => It must be at least once enabled and can be enabled in intervals (eg. ||||  ||||...  ||||...  ...)
    + FairSpec    => For checing fairness that's it
    + MEXCLUSION  => (It's invariant) Condition that both must not be in same state (critical state)
    + BOUNDEDWAIT => (It's property '/\ of both of them') Property that if it comes to l=2 then it must go to l=3
                     (for FairSpecS no error comes as 2-3 while loop stops them thus enabled in interval but 
                      for FairSpecW error comes as 2-3 while loop causes it to wait and not countinuously enabled it's correct)
*)