#ifndef _ex22_h
#define _ex22_h

//确保在ex22.c文件中的THE_SIZE变量可以在其他.c文件中可用 
extern int THE_SIZE;

int get_age();
void set_age(int age);

double update_retio(double ratio);

void print_size();

#endif
