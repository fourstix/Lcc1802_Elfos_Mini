;nstdlib.inc includes assembly versions of nstdlib routines strcpy, strcmp
;Oct 1 first test seems ok
;20-08-01 moved in dubdabx
 IFNDEF _DUBDABX		;avoid double define
  IFNDEF  LCCNOMATH		;wjr 17-04-09 disable 16 bit math
   align 256			;wjr 20-05-23
_dubdabx:	
;experimental binay-ascii conversion using the double-dabble algorithm
;thanks to Charles Richmond for the suggestion and code
;long interger is passed in rp1p2
;buffer pointer is passed at sp+2+4**+1 for 1806
;a pointer to the 1st non-zero byte in the buffer is passed back in r15
;r8-11 are used as temps
;r8 is the working pointer
;r15.0 is bit count(32) and the return value register
;r9.0 is digit count
;r10 is the number of digits wanted in the result including leading 0's - 0 means no leading 0's
	ld2 r8,'O',sp,(2+4+1); pick up the buffer pointer
	ld2 r10,'O',sp,(2+4+2+1); pick up the number of digits wanted
	cpy2 r15,r8 ;save address for now
	ldi 11	;digit count+1 for trailing 0
	plo r9
$$clrlp:	;clear the passed buffer
	ldi 0	
	str r8	;clear a byte
	inc r8
	dec r9
	glo r9	;check the count
	bnz $$clrlp ;back for more
	cpy2 r8,r15 ;get the address back

	ldi 32	;bit count
	plo r15
;now i'm going to spin off any leading 0's in the binary number
$$cktop:
	ghi rp1p2-1	;get the top bit of the number
	shl		;check for a 1
	bdf $$bitloop	;move on if we have one
	shl4 rp1p2	;shift the input number
	dec r15		;reduce the number of times to shift
	glo r15
	bnz $$cktop	;
	inc r15		;our whole number was 0 but force at least one pass
$$bitloop:
	ldi 10	;digit count
	plo r9
$$dcklp:
	ldn r8 	;pick up a digit
	smi 5	;see if it's greater than 4
	bnf $$dnoadd ;if not, bypass add
	adi 0x08	;add the 5 black and 3 more
	str r8	;put it back
$$dnoadd:
	inc r8
	dec r9	;decrement digit count
	glo r9
	bnz $$dcklp ;and back for next digit
	
	shl4 rp1p2 ;shift the input number
	
	ldi 10	;load the digit count again
	plo r9
;r8 is now just past the units location and ready to walk back
$$dshlp:
	dec r8	;walk back from 0's position
	ldn r8	;get the digit back
	shlc	;continue the shift
	phi r15 ;save it for the carry test
	ani 0x0f ;clear the 10 bit
	str r8	;put the digit back
	ghi r15	;now test for carry
	smi 0x10 ; this will make df 1 if the 10 bit is set
	dec r9	;decrement the digit count
	glo r9
	bnz $$dshlp ;back for more if needed
	
	dec r15
	glo r15
	bnz $$bitloop
	
	cpy2 r15,r8	;save the starting location of the digits
	ldi 10		;digit count again
	plo r9
$$upnxt:
	ldn r8		;get digit
	ori 0x30	;make ascii
	str r8		;put it back
	inc r8		;next digit
	dec r9		;counter
	glo r9
	bnz $$upnxt	;upgrade all 10 spots
	ldaX memaddr,sp,(2+4+2+1+1)	;point to lsb of desired digit count
	sex memaddr	;set up
	ldi 10		;max number of 0's to skip
	sm		;desired number of digits to skip
	sex sp		;reset index register
	plo r9		;number of leading 0's to skip
$$cknext:
	ldn r15		;check digit
	smi 0x30	;for '0'
	bnz $$done
	inc r15		;next digit
	dec r9		;reduce count
	glo r9
	bnz $$cknext
$$done:
	cretn
  ENDIF	
 ENDIF
