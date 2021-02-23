 .global ana, mika
  .section data
  
  
  k: .word beki
  .byte ana
  .skip 10
  beki:
      .skip 10
  ana:
  
  
  .section text
  
  
   anka: 
   
   		mov $0x1234, 15(%r4)
   		add 15(%r4), %r1
   		sub $anka, %r1
   	.skip 10
   a: mov ana(%pc), %r2
      mul %r2, ana(%pc)
   
   bla:
   
   
  .section anica
  .skip 20
   mika:
  
 
   
  .end
  
  
  
   
   
  
	
