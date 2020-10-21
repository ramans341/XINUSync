/* testandset.S - testandset (for x86) */

/*------------------------------------------------------------------------
 * testandset -  X86 testandset; 
 *------------------------------------------------------------------------
 */
test_and_set:
		pushl	%ebp		/* Push ebp onto stack		*/
        movl	%esp,%ebp	/* Record current SP in ebp	*/
        pushl   %ebx
		movl	8(%ebp),%ebx
		movl	12(%ebp),%eax	
		xchg    %eax,(%ebx)
		popl    %ebx
        popl    %ebp
        ret
