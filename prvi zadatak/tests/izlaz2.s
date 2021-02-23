#Symbol table
#ime	sek.	vr.	vid.	rbr.	equ?
UND	UND	0	l	0	0
data	data	0	l	1	0
k	data	0	l	2	0
beki	data	13	l	3	0
ana	data	23	g	4	0
text	text	0	l	5	0
anka	text	0	l	6	0
bla	text	20	l	7	0
anica	anica	0	l	8	0
mika	anica	20	g	9	0


#data
0D 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 

#text
64 00 01 24 64 80 00 00 2A 8C 24 2A 34 00 00 00 
2C 6E FE FF 01 

#anica
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 


#.rel.data
#offs.	tip	vr.
0002	ABS_16	4
0000	ABS_16	1

#.rel.text
#offs.	tip	vr.
000E	ABS_16	5
0006	ABS_16	9

