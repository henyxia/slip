#ifndef __LIB_THREAD__
#define	__LIB_THREAD__

void	initThreads(int);
int		newThread(void(*) (void*), void*, int);
void	waitForThreads();
void	P(int);
void	V(int);

#endif
