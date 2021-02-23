#Symbol table
#ime	sek.	vr.	vid.	rbr.	equ?
UND	UND	0	l	0	0
print	UND	0	g	1	0
data	data	0	l	2	0
k	data	0	l	3	0
beki	data	13	l	4	0
ana	data	23	g	5	0
text	text	0	l	6	0
anka	text	0	l	7	0
bla	text	9	l	8	0
anica	anica	0	l	9	0
mika	anica	20	g	10	0
duz	data	-40	l	11	1


#data
0D 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 

#text
64 00 D8 FF 22 24 00 00 00 01 

#anica
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 


#.rel.data
#offs.	tip	vr.
0002	ABS_16	5
0000	ABS_16	2

#.rel.text
#offs.	tip	vr.
0007	ABS_16	1

