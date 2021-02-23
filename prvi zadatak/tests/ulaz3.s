 .global ana, mika
 .extern print
  .section data
  		.equ duz, beki - ana - 8 - 22
  
  k: .word beki
  .byte ana
  .skip 10
  beki:
      .skip 10
  ana:
  
  
  .section text
  
  
   anka: 
   		mov $duz, %r1
   		call print
   		
   		
   		
   		
   
   bla:
   	.byte 1
   
   
  .section anica
  .skip 20
   mika:
  
 
   
  .end
  
  
  
   
   
  
	
