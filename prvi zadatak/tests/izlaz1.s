#Symbol table
#ime	sek.	vr.	vid.	rbr.	equ?
UND	UND	0	l	0	0
data	data	0	l	1	0
k	data	0	l	2	0
beki	data	13	l	3	0
ana	data	23	g	4	0
text	text	0	l	5	0
anka	text	0	l	6	0
a	text	27	l	7	0
bla	text	37	l	8	0
anica	anica	0	l	9	0
mika	anica	20	g	10	0


#data
0D 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 

#text
64 00 34 12 68 0F 00 6C 68 0F 00 22 74 00 00 00 
22 00 00 00 00 00 00 00 00 00 00 64 6E FD FF 24 
7C 24 6E FE FF 

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
001D	PCR_16	4
0023	PCR_16	4

