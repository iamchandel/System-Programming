MACRO
INCR &X, &Y,&REG=AREG
MOVER &REG,&X
ADD &REG,&Y
MOVEM &REG,&X
MEND
MACRO
DECR &A,&B,&REG=BREG
MOVER &REG,&A
SUB &REG,&B
MOVEM &REG,&A
MEND
START 100
READ N1
READ N2
INCR N1,N2,REG=CREG
DECR N1,N2
STOP
N1 DS 1
N2 DS 1
END
