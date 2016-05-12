    ;;                 Does stuff
	.ORIG   x3000
	AND     R2,R2,#0
	LD      R3,PTR
	TRAP    x23
	LDR     R1,R3,#0
    ;;                  test for EOF
TEST
    ADD     R4,R1,#-4
	BRz     OUTPUT
    ;;                  test
	NOT     R1,R1
	ADD     R1,R1,R0
    NOT     R1,R1
    BRnp    GETCHAR
    ADD     R2,R2,#1
    ;;                  Get next char from file
GETCHAR
    ADD     R3,R3,#1
    LDR     R1,R3,#0
    BRnzp   TEST
    ;;                  Output the count
OUTPUT
    LD      R0,ASCII
    ADD     R0,R0,R2
    TRAP    x21
    TRAP    x25
    ;;                  Storage
ASCII
    .FILL   x0030
PTR
    .FILL   x3014
    .STRINGZ "Testing 123"
    .END
