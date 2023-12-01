#include <stdlib.h>
#include <stdio.h>
#include "communication.h"

#define TRUE 1
#define FALSE 0


unsigned test_left() {
    return get_left_proc_circular(2, 2) == 3 && get_left_proc_circular(3, 2) == 2;
}


unsigned test_right() {
    return get_right_proc_circular(3, 2) == 2 && get_right_proc_circular(2, 2) == 3;
}


unsigned test_above() {
    return get_above_proc_circular(2, 2) == 0 && get_above_proc_circular(0, 2) == 2;
}


unsigned test_below() {
    return get_below_proc_circular(0, 2) == 2 && get_below_proc_circular(2, 2) == 0;
}


unsigned test_upper_left() {
    return get_upper_left_proc_circular(4, 3) == 0 && get_upper_left_proc_circular(0, 3) == 8
            && get_upper_left_proc_circular(3, 3) == 2 && get_upper_left_proc_circular(1, 3) == 6;
}


unsigned test_upper_right() {
    return get_upper_right_proc_circular(4, 3) == 2 && get_upper_right_proc_circular(2, 3) == 6
            && get_upper_right_proc_circular(5, 3) == 0 && get_upper_right_proc_circular(1, 3) == 8;
}


unsigned test_lower_right() {
    return get_lower_right_proc_circular(4, 3) == 8 && get_lower_right_proc_circular(8, 3) == 0
            && get_lower_right_proc_circular(5, 3) == 6 && get_lower_right_proc_circular(7, 3) == 2;
}


unsigned test_lower_left() {
    return get_lower_left_proc_circular(4, 3) == 6 && get_lower_left_proc_circular(6, 3) == 2
            && get_lower_left_proc_circular(3, 3) == 8 && get_lower_left_proc_circular(7, 3) == 0;
}


int main() {
    printf("Left: %d\n", test_left());
    printf("Right: %d\n", test_right());
    printf("Above: %d\n", test_above());
    printf("Below: %d\n", test_below());
    printf("Lower left: %d\n", test_lower_left());
    printf("Lower right: %d\n", test_lower_right());
    printf("Upper left: %d\n", test_upper_left());
    printf("Upper right: %d\n", test_upper_right());
}
