/*
 * isha.c
 *
 * A completely insecure and bad hashing algorithm, based loosely on
 * SHA-1 (which is itself no longer considered a good hashing
 * algorithm)
 *
 * Based on code for sha1 processing from Paul E. Jones, available at
 * https://www.packetizer.com/security/sha1/
 *
 * Modified by Suhas Srinivasa Reddy
 * Date 2nd Nov 2023
 *
 */

#include "stdbool.h"
#include "isha.h"
#include "static_profiler.h"

// Do not modify these declarations
uint32_t ISHAProcessMessageBlockEnd, ISHAPadMessageEnd, ISHAResetEnd,
		ISHAResultEnd, ISHAInputEnd;
#define record_pc(x)  asm("mov %0, pc" : "=r"(x))
//----------------------------------------------------------------------

/*
 * circular shift macro
 */
#define ISHACircularShift(bits,word) \
  ((((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32-(bits))))

/*  
 * Processes the next 512 bits of the message stored in the MBlock
 * array.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAProcessMessageBlock(ISHAContext *ctx) {
#ifdef DEBUG
	INCREMENT_STATIC_COUNT(ISHAProcessMessageBlockCount, static_profiling_on);
#endif
	uint32_t temp, * MBlock;
	register uint32_t W, A, B, C, D, E;
	int t;

	A = ctx->MD[0];
	B = ctx->MD[1];
	C = ctx->MD[2];
	D = ctx->MD[3];
	E = ctx->MD[4];

	MBlock = (uint32_t *)ctx->MBlock;

	// Removed a for-loop which was used to fill W array.
	// Instead, a variable W is used which is updated in every iteration

	for (t = 0; t < 16; t++) {
		W = __builtin_bswap32(MBlock[t]);
		temp = ISHACircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W;
		E = ISHACircularShift(25, D);
		D = ISHACircularShift(15, C);
		C = ISHACircularShift(30, B);
		B = ISHACircularShift(10, A);
		A = ISHACircularShift(5, temp);

	}

	ctx->MD[0] = (ctx->MD[0] + A);
	ctx->MD[1] = (ctx->MD[1] + B);
	ctx->MD[2] = (ctx->MD[2] + C);
	ctx->MD[3] = (ctx->MD[3] + D);
	ctx->MD[4] = (ctx->MD[4] + E);

	ctx->MB_Idx = 0;

	// Do not modify this line
	record_pc(ISHAProcessMessageBlockEnd);
}

/*  
 * The message must be padded to an even 512 bits.  The first padding
 * bit must be a '1'.  The last 64 bits represent the length of the
 * original message.  All bits in between should be 0. This function
 * will pad the message according to those rules by filling the MBlock
 * array accordingly. It will also call ISHAProcessMessageBlock()
 * appropriately. When it returns, it can be assumed that the message
 * digest has been computed.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAPadMessage(ISHAContext *ctx) {
#ifdef DEBUG
	INCREMENT_STATIC_COUNT(ISHAPadMessageCount, static_profiling_on);
#endif
	/*
	 *  Check to see if the current message block is too small to hold
	 *  the initial padding bits and length.  If so, we will pad the
	 *  block, process it, and then continue padding into a second
	 *  block.
	 */
	ctx->MBlock[ctx->MB_Idx++] = 0x80;
	if (ctx->MB_Idx > 55) {
		while (ctx->MB_Idx < 64) {
			ctx->MBlock[ctx->MB_Idx++] = 0;
		}

		ISHAProcessMessageBlock(ctx);
	}
	while (ctx->MB_Idx < 56) {
		ctx->MBlock[ctx->MB_Idx++] = 0;
	}

	*(uint32_t*)(ctx->MBlock+56) = __builtin_bswap32(ctx->Length_High);
	*(uint32_t*)(ctx->MBlock+60) = __builtin_bswap32(ctx->Length_Low);

	ISHAProcessMessageBlock(ctx);

	// Do not modify this line
	asm("mov %0, pc" : "=r"(ISHAPadMessageEnd));
}

/*
 * This function computes the final message digest from the
 * accumulated data. It calls ISHAPadMessage() to ensure that the
 * message is properly padded, and then it retrieves the message
 * digest from the context and stores it in the provided output buffer.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 *   digest_out  Pointer to the output buffer where the message digest will be stored
 */
void ISHAResult(ISHAContext *ctx, uint8_t *digest_out) {
#ifdef DEBUG
	INCREMENT_STATIC_COUNT(ISHAResultCount, static_profiling_on);
#endif

	if (ctx->Corrupted) {
		return;
	}

	if (!ctx->Computed) {
		ISHAPadMessage(ctx);
		ctx->Computed = 1;
	}

	// Unrolled the for-loop to avoid for loop overhead used builtin_bswap32 for faster but copy
	*(uint32_t*)(digest_out) = __builtin_bswap32(ctx->MD[0]);
	*(uint32_t*)(digest_out+4) = __builtin_bswap32(ctx->MD[1]);
	*(uint32_t*)(digest_out+8) = __builtin_bswap32(ctx->MD[2]);
	*(uint32_t*)(digest_out+12) = __builtin_bswap32(ctx->MD[3]);
	*(uint32_t*)(digest_out+16) = __builtin_bswap32(ctx->MD[4]);

	// Do not modify this line
	record_pc(ISHAResultEnd);

}

/*
 * This function is used to input data into the ISHA hashing algorithm.
 * It updates the internal state of the algorithm as data is provided
 * incrementally. The input data is processed in chunks of 64 bytes (512 bits).
 *
 * Parameters:
 *   ctx            The ISHAContext (in/out)
 *   message_array  Pointer to the input message data
 *   length         The length of the input message data
 */
void ISHAInput(ISHAContext *ctx, const uint8_t *message_array, size_t length) {
#ifdef DEBUG
	INCREMENT_STATIC_COUNT(ISHAInputCount, static_profiling_on);
#endif
	int i, blockRemaining, bytesToCopy;
	uint8_t* mBlockPtr;                      // Using a pointer variable to reduce de-referencing Mblock array from the contex
	if (!length) {
		return;
	}

	if (ctx->Computed || ctx->Corrupted) {
		ctx->Corrupted = 1;
		return;
	}

	while (length > 0 && !ctx->Corrupted) {
		blockRemaining = 64 - ctx->MB_Idx;
		bytesToCopy = (length < blockRemaining) ? length : blockRemaining;

		// Copy as many bytes as possible in one go
		mBlockPtr = ctx->MBlock + ctx->MB_Idx;

		for (i = 0; i < bytesToCopy; i++) {
		    *(mBlockPtr++) = *(message_array++);        // Using Pointers here increases the time significantly as it doesn't have to access ctx everytime to retrieve Mblock
		}

		// Update ctx->MB_Idx after the loop
		ctx->MB_Idx += bytesToCopy;

		ctx->Length_Low += 8 * bytesToCopy;
		ctx->Length_Low &= 0xFFFFFFFF;

		if (ctx->Length_Low < 8 * bytesToCopy) {        // Increment Length_High if Length_Low overflows
			ctx->Length_High++;
			ctx->Length_High &= 0xFFFFFFFF;

			if (ctx->Length_High == 0) {
				ctx->Corrupted = 1;
			}
		}

		length -= bytesToCopy;

		if (ctx->MB_Idx == 64) {                        // If Message Digest is full call Message Block to derive a key and reset the index
			ISHAProcessMessageBlock(ctx);
		}
	}

	record_pc(ISHAInputEnd);
}

// Do not modify anything below this line

static bool cmp_bin(const uint8_t *b1, const uint8_t *b2, size_t len) {
	for (size_t i = 0; i < len; i++)
		if (b1[i] != b2[i])
			return false;
	return true;
}

void GetFunctionAddress(const char *func_name, uint32_t *start, uint32_t *end) {
	if (cmp_bin((const uint8_t*) func_name,
			(const uint8_t*) "ISHAProcessMessageBlock", 23)) {
		*start = (uint32_t) ISHAProcessMessageBlock;
		*end = ISHAProcessMessageBlockEnd;
	} else if (cmp_bin((const uint8_t*) func_name,
			(const uint8_t*) "ISHAPadMessage", 14)) {
		*start = (uint32_t) ISHAPadMessage;
		*end = ISHAPadMessageEnd;
	} else if (cmp_bin((const uint8_t*) func_name, (const uint8_t*) "ISHAReset",
			9)) {
		*start = (uint32_t) ISHAReset;
		*end = ISHAResetEnd;
	} else if (cmp_bin((const uint8_t*) func_name,
			(const uint8_t*) "ISHAResult", 10)) {
		*start = (uint32_t) ISHAResult;
		*end = ISHAResultEnd;
	} else if (cmp_bin((const uint8_t*) func_name, (const uint8_t*) "ISHAInput",
			9)) {
		*start = (uint32_t) ISHAInput;
		*end = ISHAInputEnd;
	}

	*end += 20;

}
