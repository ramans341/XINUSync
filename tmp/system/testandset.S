/* testandset.S - testandset (for x86) */
        .text
		.globl	test_and_set
/*------------------------------------------------------------------------
 * testandset -  X86 testandset; 
 *------------------------------------------------------------------------
 */
test_and_set:
		pushl	%ebp		/* Push ebp onto stack		*/
        movl	%esp,%ebp	/* Record current SP in ebp	*/
        pushl   %ebx		/* Push ebx onto stack		*/
		movl	8(%ebp),%ebx /* Get address of reqd. variable */
		movl	12(%ebp),%eax	/* Get new_value */
		xchg    %eax,(%ebx) /* Exchange old & new guard values */
		popl    %ebx /* Restore ebx */
        popl    %ebp /* Restore ebp */
        ret        /* Return old guard value */
