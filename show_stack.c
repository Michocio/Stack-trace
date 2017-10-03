/*
	Zadanie 1 Systemy Operacyjne
	MichaĹ JaroĹ
	mj348711

	Uwaga:
		Co prawda program generuje program przy kompilacji,
		zwiÄzany z wypisywaniem uin64_t zamiast long long,
		dla formatu %016llx, zostawiam to jednak z premedytacjÄ,
		nie jest to bĹÄd, a w mojej opini wyglÄda czytelniej niĹź
		np. PRId64.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define CALL_OFFSET 5 // Offset needed to detect call
#define RELATIVE_ADDR_OFFSET 4 // Where is relative address of fun?
#define CALL_CMD 0xe8 // call rel32 opkod

// Pierwszy bajt, gdzie moĹźe znajdowaÄ siÄ kod wykonywalny.
extern uint8_t __executable_start;

// Pierwszy bajt, gdzie juĹź nie moĹźe znajdowaÄ siÄ kod wykonywalny.
extern uint8_t __etext;
extern void *__libc_stack_end;

typedef uint8_t * pointer;

void stack_show()
{
	uint8_t shift = 1;// One byte

	uint8_t stack_top = 0;

	pointer ptr = &stack_top;

	// + 3 Because reading of lbs and mbs
	while((ptr + 3) <= (pointer)__libc_stack_end)
	{// ptr - stack iterator

		// little-endian
		uint32_t * lbs = (uint32_t *)ptr;
		uint32_t * mbs = (uint32_t *)(ptr+3);

		// Make 64-bits number
		uint64_t addr = ((*(mbs))<<16) | *lbs;

		/* Is addr within text segment?
			Sharp inequality, between aadr and etext, because(from man end):
			__etext - This is the first address past the end of the text segment  (the
              program code).
			!first address past!
		*/
		if(addr > CALL_OFFSET &&
			(addr - CALL_OFFSET) >= (uint64_t)&__executable_start &&
				addr < (uint64_t)&__etext)
		{
			if(*(pointer)(addr - CALL_OFFSET) == CALL_CMD)
			{
				// Count address
				uint64_t fun_addr = *(int*)(addr - RELATIVE_ADDR_OFFSET) + *(int*)ptr;
				if(fun_addr >= (uint64_t)&__executable_start && fun_addr < (uint64_t)&__etext)
					printf("%016llx\n", fun_addr);
			}
		}
		ptr += shift;// One byte
	}
	return;
}
