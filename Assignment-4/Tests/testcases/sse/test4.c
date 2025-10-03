#include "stdbool.h"
//ADDR, LOAD, STORE, CAST(COPY)
extern void svf_assert(bool);

int main(){
    int x = 1, y = 1;
    int a = 1, b = 2;
    if (a > b) {
        y++;
    } else {
        x++;
        svf_assert (x == 2);
    }
    return 0;
}