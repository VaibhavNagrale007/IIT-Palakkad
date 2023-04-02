-------------------------MODULE Peterson-------------------------
EXTENDS Integers
VARIABLES turn, Interested0, Interested1, l, m

TypeOK == l \in 0..7 /\ m \in 0..7 /\ turn \in 0..1 /\ Interested0 \in {TRUE,FALSE} /\ Interested1 \in {TRUE,FALSE}

Init0 == (l=0) /\ (Interested0=FALSE)
Init1 == (m=0) /\ (Interested1=FALSE)
Init2 == (turn=0)
Init == Init0 /\ Init1 /\ Init2

pi0 == (l=0) /\ (l'=1) /\ UNCHANGED<<turn, Interested0, Interested1>>
pi1 == (l=1) /\ (l'=2) /\ UNCHANGED<<turn, Interested0, Interested1>>
pi2 == (l=2) /\ (l'=3) /\ (Interested0'=TRUE) /\ UNCHANGED<<turn, Interested1>>
pi3 == (l=3) /\ (l'=4) /\ (turn'=1) /\ UNCHANGED<<Interested0, Interested1>>
pi4 == (l=4) /\ (turn=1) /\ (Interested1=TRUE) /\ UNCHANGED<<turn, Interested0, Interested1, l>>
pi5 == (l=4) /\ (l'=5) /\ ((turn=0) \/ (Interested1=FALSE)) /\ UNCHANGED<<turn, Interested0, Interested1>>
pi6 == (l=5) /\ (l'=6) /\ UNCHANGED<<turn, Interested0, Interested1>>
pi7 == (l=6) /\ (l'=7) /\ (Interested0'=FALSE) /\ UNCHANGED<<turn, Interested1>>
pi8 == (l=7) /\ (l'=0) /\ UNCHANGED<<turn, Interested0, Interested1>>

Nextpi == \/ pi0 \/ pi1 \/ pi2 \/ pi3 \/ pi4 \/ pi5 \/ pi6 \/ pi7 \/ pi8

si0 == (m=0) /\ (m'=1) /\ UNCHANGED<<turn, Interested0, Interested1>>
si1 == (m=1) /\ (m'=2) /\ UNCHANGED<<turn, Interested0, Interested1>>
si2 == (m=2) /\ (m'=3) /\ (Interested1'=TRUE) /\ UNCHANGED<<turn, Interested0>>
si3 == (m=3) /\ (m'=4) /\ (turn'=0) /\ UNCHANGED<<Interested0, Interested1>>
si4 == (m=4) /\ (turn=0) /\ (Interested0=TRUE) /\ UNCHANGED<<turn, Interested0, Interested1, m>>
si5 == (m=4) /\ (m'=5) /\ ((turn=1) \/ (Interested0=FALSE)) /\ UNCHANGED<<turn, Interested0, Interested1>>
si6 == (m=5) /\ (m'=6) /\ UNCHANGED<<turn, Interested0, Interested1>>
si7 == (m=6) /\ (m'=7) /\ (Interested1'=FALSE) /\ UNCHANGED<<turn, Interested0>>
si8 == (m=7) /\ (m'=0) /\ UNCHANGED<<turn, Interested0, Interested1>>

Nextsi == \/ si0 \/ si1 \/ si2 \/ si3 \/ si4 \/ si5 \/ si6 \/ si7 \/ si8

Next == \/ (Nextpi /\ UNCHANGED<<m>>) \/ (Nextsi /\ UNCHANGED<<l>>)

vars == << turn, Interested0, Interested1, l, m >>

Fairness == WF_l(pi0)
            /\ WF_l(pi2)
            /\ WF_l(pi3)
            /\ WF_l(pi5)
            /\ WF_l(pi6)
            /\ WF_l(pi7)
            /\ WF_l(pi8)
            /\ WF_m(si0)
            /\ WF_m(si2)
            /\ WF_m(si3)
            /\ WF_m(si5)
            /\ WF_m(si6)
            /\ WF_m(si7)
            /\ WF_m(si8) 

FairSpec == Init /\ [][Next]_vars /\ Fairness

MEXCLUSION == ~((l=5) /\ (m=5)) 

BOUNDEDWAIT1 == []((l=3) => <>(l=5))
BOUNDEDWAIT2 == []((m=3) => <>(m=5))

(*
Jpi1 == l # 0
Jpi2 == l # 2
Jpi3 == l # 3
Jpi4 == ~( (l=4) /\ ( (turn=0) \/ (Interested1=FALSE) ) )
Jpi5 == l # 5
Jpi6 == l # 6
Jpi7 == l # 7
Jpi == (<> Jpi1) /\ (<> Jpi2) /\ (<> Jpi3) /\ (<> Jpi4) /\ (<> Jpi5) /\ (<> Jpi6) /\ (<> Jpi7)

Jsi1 == m # 0
Jsi2 == m # 2
Jsi3 == m # 3
Jsi4 == ~( (m=4) /\ ( (turn=1) \/ (Interested0=FALSE) ) )
Jsi5 == m # 5
Jsi6 == m # 6
Jsi7 == m # 7
Jsi == (<> Jsi1) /\ (<> Jsi2) /\ (<> Jsi3) /\ (<> Jsi4) /\ (<> Jsi5) /\ (<> Jsi6) /\ (<> Jsi7)
*)

==================================================================
