-------------------------MODULE MeasuringJar-------------------------
EXTENDS Integers
VARIABLES small, big

TypeOK == small \in 0..3 /\ big \in 0..5

Init == (small = 0) /\ (big = 0)

EmptySmall == (small' = 0) /\ (big' = big)

EmptyBig == (small' = small) /\ (big' = 0)

FillSmall == (small' = 3) /\ (big' = big)

FillBig == (small' = small) /\ (big' = 5)

PourSmalltoBig == IF (small+big<=5) THEN 
                    (small' = 0) /\ (big' = big+small)
                 ELSE 
                    (small' = (big+small-5)) /\ (big' = 5)

PourBigtoSmall == IF (small+big<=3) THEN 
                    (small' = small+big) /\ (big' = 0)
                 ELSE 
                    (small' = 3) /\ (big' = (big+small-3))

Next == \/ EmptySmall
        \/ EmptyBig
        \/ FillSmall
        \/ FillBig
        \/ PourSmalltoBig
        \/ PourBigtoSmall
        
==================================================================
