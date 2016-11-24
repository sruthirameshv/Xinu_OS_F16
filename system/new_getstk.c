#include <xinu.h>

/*------------------------------------------------------------------------
 *  new_getstk  -  Allocate stack memory, returning highest word address using Best FIT method
 *------------------------------------------------------------------------
 */
char  	*new_getstk(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr;	/* Walk through memory list	*/
	struct	memblk	*best_fits, *best_fitsprev; /* Record block that fits	*/
	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use mblock multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	best_fits = prev;
	best_fitsprev = NULL;  /* Just to avoid a compiler warning */
	while (curr != NULL) {			/* Scan entire list	*/
		if ((curr->mlength >= nbytes)&&(curr->mlength>best_fits->mlength)) {	/* Record block address	*/
			best_fits = curr;		/*   when request fits	*/
			best_fitsprev = prev;
		}
		prev = curr;
		curr = curr->mnext;
	}

	if (best_fits == NULL) {			/* No block was found	*/
		restore(mask);
		return (char *)SYSERR;
	}
	if (nbytes == best_fits->mlength) {		/* Block is exact match	*/
		best_fitsprev->mnext = best_fits->mnext;
	} else {				/* Remove top section	*/
		best_fits->mlength -= nbytes;
		best_fits = (struct memblk *)((uint32)best_fits + best_fits->mlength);
	}
	memlist.mlength -= nbytes;
	restore(mask);
	char* rtn=(char *)((uint32) best_fits + nbytes - sizeof(uint32));
	int size=nbytes;
	while(size>0)
	{
	*rtn='#';
	*--rtn;
	--size;
	}
	return (char *)((uint32) best_fits + nbytes - sizeof(uint32));
}
