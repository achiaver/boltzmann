#ifndef __INOUT_H__
#define __INOUT_H__

struct parameters * parameters_create ();
struct parameters * parameters_input  (char *, char *);
void                parameters_print  (struct parameters *);

#endif /* __INOUT_H__ */
