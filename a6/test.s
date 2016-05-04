.pos 0x100
				ld $0, r0			# r0 = 0
				ld $1, r1			# r1 = 1
				add r0, r1			# r1 = 0 + 1
				br branch1			# goto branch1
branch1:		beq r1, branch2		# goto branch2
				bgt r1, branch3		# goto branch3
branch2:		nop					# do nothing
branch3:		ld $2, r0			# r0 = 2
				add r0, r1			# r1 = 1 + 2
				j branch4			# goto branch4
branch4:		ld $0x200, r0		# r0 = 0x200
				j (r0)				# goto 0x200
.pos 0x200
				gpc $6, r6			# r6 = pc + 6
				halt