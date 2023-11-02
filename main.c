#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>
#include <Windows.h>
#include <WinUser.h>

// ===========================================================================
// tetris blocks

const int block_square[3][3] = {{0,0,0},
                                {1,1,0},
                                {1,1,0}};

const int block_t[3][3]     =  {{0,1,0},
                                {1,1,1},
                                {0,0,0}};

const int block_line[3][3]  =  {{0,1,0},
                                {0,1,0},
                                {0,1,0}};

const int block_z_a[3][3]   =  {{1,0,0},
                                {1,1,0},
                                {0,1,0}};

const int block_z_b[3][3]   =  {{0,0,1},
                                {0,1,1},
                                {0,1,0}};

const int block_l_a[3][3]   =  {{1,1,0},
                                {0,1,0},
                                {0,1,0}};

const int block_l_b[3][3]   =  {{0,1,1},
                                {0,1,0},
                                {0,1,0}};

void mat_3x3_rotate_right(int **in, int **out) {
    out = (int[3][3])  {{in[2][0], in[1][0], in[0][0]},
                        {in[2][1], in[1][1], in[0][1]},
                        {in[2][2], in[1][2], in[0][2]}};
}

// ===========================================================================
// platform-specific io functions 

void io_print(char *str) {
    printf("%s", str);
}

typedef enum io_key_type {
    io_key_type_down = VK_DOWN,
    io_key_type_up = VK_UP,
    io_key_type_left = VK_LEFT,
    io_key_type_right = VK_RIGHT,
    io_key_type_drop = VK_SPACE
} io_key_type_t;

int io_get_key_state(io_key_type_t key) {
    return ((GetKeyState(key) & 0x80) == 0x80);
}


// ===========================================================================
// platform-specific timing functions

uint64_t timing_get_us_64() {
    
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (uint64_t)((tv.tv_sec*1000000)+(tv.tv_usec));

}

void timing_delay_us(uint64_t t_us) {
    uint64_t t_cur = timing_get_us_64();
    uint64_t t_loop_exit = t_cur + t_us;

    while ( t_cur < t_loop_exit ) { t_cur = timing_get_us_64(); }
}

// ===========================================================================
// game functions

int64_t wait_for_next_frame(uint64_t last_frame_start, int game_speed_hz) {
    float frame_time_f = 1.f/(float)game_speed_hz;
    uint64_t frame_time_us = (uint64_t)(frame_time_f*1000000);

    int64_t delay_time_us = frame_time_us - (timing_get_us_64() - last_frame_start);
    if ( delay_time_us > 0 ) {
        timing_delay_us(delay_time_us);
    } 

    return delay_time_us;
}

int main() {

    

} 