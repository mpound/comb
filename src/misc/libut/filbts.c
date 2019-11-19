/*
 * Fill a specified range of bits in memory with a given bit.  This routine
 * was designed for the 'use' array but may be useful elsewhere.  Only a
 * FORTRAN calling sequence is available now.  A VAX and FORTRAN like bit
 * numbering scheme is used here and in ibit - the lsb of a word is 1
 * and the msb is 32
 */
void filbts_(int *array, int *ibit, int *first, int *last)
/*array;		 location of the bits to change */
/*ibit;			 ibit's lsb is what goes into array */
/*first, last;		 first and last bits to change */
{
	register int *word1;	/* The first word to be changed */
	register int *word2;	/* The last word to change */
	int bit1;		/* The first bit of word one to change */
	int bit2;		/* The numb of bits of word2 not to change */
	int thebit;		/* The bit to put in (1 or 0) */
	register unsigned int pattern;	/* The bits to change in this word */
				/* Also used as a temporary at first */

	thebit = *ibit & 1;
	pattern = *first - 1;
	bit1 = pattern & 31;
	word1 = array + (pattern >> 5);	/* pattern is a small positive int */
	pattern = *last - 1;
	bit2 = 31 - (pattern & 31);
	word2 = array + (pattern >> 5);
	pattern = -1;
	if(bit1 != 0)
		pattern <<= bit1;
	while(word1 <= word2) {
		if(word1 == word2 && bit2 > 0) {
			pattern <<= bit2;
			pattern >>= bit2;
		}
		if(thebit)
			*word1++ |= pattern;
		else
			*word1++ &= ~pattern;
		pattern = -1;
	}
}
