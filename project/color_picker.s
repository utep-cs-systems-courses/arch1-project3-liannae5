	.text
	.balign 2
	.extern random
	.global color_picker
color_picker:
	sub #4,r1
	mov r12,0(r1)		;takes secondsW as parameter r12 is low word
	mov r13,2(r1)		;r13 high word
	cmp #5,0(r1)		;if secondsW less than 5 jump to done
	jnc done
	call #random 
	add r12, 0(r1)		;add random value to seconds
	addc #0,2(r1)
	jmp done
done:
	add #128,0(r1) 		;add 128 for added randomness
	addc #0,2(r1)
	mov 0(r1),r12 		;move new color value into r12 and r13
	mov 2(r1),r13
	add #4,r1
	pop r0
