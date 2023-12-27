/*
 * ISHAResets.s
 *
 *
 * Authored by Suhas Srinivasa Reddy
 * Date 2nd Nov 2023
 */

.syntax unified
.thumb

.align 2
.global ISHAReset
.global ISHAResetEnd

/*
 * Resets the ISHA hashing algorithm context to its initial state.
 *
 * This function is used to reset the internal state of the ISHA hashing algorithm.
 * It sets all the context variables to their initial values, clears any computed
 * digest, and marks the context as uncorrupted and not computed. This allows the
 * algorithm to be used for a new hashing operation after a previous one has been
 * completed.
 *
 */

ISHAReset:
	PUSH {r4, lr}

	// Reset MD array each element in the array is 4 Bytes hence, offset is increased by 4
    LDR r1, =0x67452301
    STR r1, [r0, #0]          // Resetting MD[0]
    LDR r1, =0xEFCDAB89
    STR r1, [r0, #4]          // Resetting MD[1]
    LDR r1, =0x98BADCFE
    STR r1, [r0, #8]          // Resetting MD[2]
    LDR r1, =0x10325476
    STR r1, [r0, #12]         // Resetting MD[3]
    LDR r1, =0xC3D2E1F0
    STR r1, [r0, #16]         // Resetting MD[14

    // Reset Length_Low and Length_High by setting it to 0
    LDR r1, =0
    STR r1, [r0, #20]         // Setting Length_Low = 0
    STR r1, [r0, #24]         // Setting Length_High = 0

    // Reset MB Idx, computer, corrupted.
    // Offset for MBlock starts from 92 as MBlock array occupies Bytes starting from 28 to 91
    STR r1, [r0, #92]         // Setting MB_Idx index to 0
    STR r1, [r0, #96]         // Setting Computed index to 0
    STR r1, [r0, #100]        // Setting Corrupted index to 0

    // Store Program Counter at the end of the function to ISHAResetEnd
	LDR r1, =ISHAResetEnd
	MOV r2, pc
	STR r2, [r1, #0]          // Setting current PC to ISHAReset

    POP {r4, pc}

