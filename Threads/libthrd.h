#ifndef __LIB_THREAD__
#define	__LIB_THREAD__

int		newThread(void(*) (void*), void*, int);
void	waitForThreads();
void	P(int);
void	V(int);

#endif
