#ifndef __APISTATUS_H__
#define __APISTATUS_H__

typedef char* Status;

extern char OK[];
extern char NOT_IMPLEMENTED[];
extern char INVALID_PARAMETER[];
extern char INVALID_ID[];
extern char INVALID_REFERENCE[];
extern char OUT_OF_MEMORY[];
extern char BUFFER_OVERFLOW[];
extern char WRITE_ERROR[];

int min(int a, int b);
int max(int a, int b);

int minf(float a, float b);
int maxf(float a, float b);

#endif /* __APISTATUS_H__ */
