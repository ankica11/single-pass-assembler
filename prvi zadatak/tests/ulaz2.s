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
   		mov $1, %r2
   		mov mika, %r5
   		cmp %r2, %r5
   		jeq anka
   		jmp *anka(%pc)
   		
   		
   		
   
   bla:
   	.byte 1
   
   
  .section anica
  .skip 20
   mika:
  
 
   
  .end
  
  
  
   
   
  
	
