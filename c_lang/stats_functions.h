#ifndef __INOUT_H__
#define __INOUT_H__


struct parameters * parameters_input(char *, char *);
void print_parameters (struct parameters *);
void input_parameters (struct parameters *);
void print_network_status (struct network *);

#endif /* __INOUT_H__ */
