		AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
		EXPORT TestStack

TestStack
	PUSH {R4}
	PUSH {R5}
	PUSH {R6}
	PUSH {R7}
	PUSH {R8}
	PUSH {R9}
	PUSH {R10}
	BX      LR                 

    ALIGN
    END