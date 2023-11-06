#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>
#include <Windows.h>
#include <WinUser.h>

// ===========================================================================
// tetris blocks

const int __block_square[4][16] = { {0,0,0,0,
                                     0,1,1,0,
                                     0,1,1,0,
                                     0,0,0,0},
                                    {0,0,0,0,
                                     0,1,1,0,
                                     0,1,1,0,
                                     0,0,0,0},
                                    {0,0,0,0,
                                     0,1,1,0,
                                     0,1,1,0,
                                     0,0,0,0},
                                    {0,0,0,0,
                                     0,1,1,0,
                                     0,1,1,0,
                                     0,0,0,0}};

const int __block_t[4][16] =  {{0,1,0,0,
                                1,1,1,0,
                                0,0,0,0,
                                0,0,0,0},
                               {0,1,0,0,
                                0,1,1,0,
                                0,1,0,0,
                                0,0,0,0},
                               {0,0,0,0,
                                1,1,1,0,
                                0,1,0,0,
                                0,0,0,0},
                               {0,1,0,0,
                                1,1,0,0,
                                0,1,0,0,
                                0,0,0,0}};
                            
const int __block_line[4][16] = {{0,0,1,0,
                                  0,0,1,0,
                                  0,0,1,0,
                                  0,0,1,0},
                                 {0,0,0,0,
                                  1,1,1,1,
                                  0,0,0,0,
                                  0,0,0,0},
                                 {0,0,1,0,
                                  0,0,1,0,
                                  0,0,1,0,
                                  0,0,1,0},
                                 {0,0,0,0,
                                  1,1,1,1,
                                  0,0,0,0,
                                  0,0,0,0}};

const int __block_z_a[4][16] = {{0,1,0,0,
                                 0,1,1,0,
                                 0,0,1,0,
                                 0,0,0,0},
                                {0,0,0,0,
                                 0,1,1,0,
                                 1,1,0,0,
                                 0,0,0,0},
                                {1,0,0,0,
                                 1,1,0,0,
                                 0,1,0,0,
                                 0,0,0,0},
                                {0,1,1,0,
                                 1,1,0,0,
                                 0,0,0,0,
                                 0,0,0,0}};

const int __block_z_b[4][16] = {{0,0,1,0,
                                 0,1,1,0,
                                 0,1,0,0,
                                 0,0,0,0},
                                {1,1,0,0,
                                 0,1,1,0,
                                 0,0,0,0,
                                 0,0,0,0},
                                {0,0,1,0,
                                 0,1,1,0,
                                 0,1,0,0,
                                 0,0,0,0},
                                {1,1,0,0,
                                 0,1,1,0,
                                 0,0,0,0,
                                 0,0,0,0}};

const int __block_l_a[4][16] = {{1,1,0,0,
                                 0,1,0,0,
                                 0,1,0,0,
                                 0,0,0,0},
                                {0,0,0,0,
                                 1,1,1,0,
                                 1,0,0,0,
                                 0,0,0,0},
                                {0,1,0,0,
                                 0,1,0,0,
                                 0,1,1,0,
                                 0,0,0,0},
                                {0,0,1,0,
                                 1,1,1,0,
                                 0,0,0,0,
                                 0,0,0,0}};

const int __block_l_b[4][16] = {{0,1,1,0,
                                 0,1,0,0,
                                 0,1,0,0,
                                 0,0,0,0},
                                {0,0,0,0,
                                 1,1,1,0,
                                 0,0,1,0,
                                 0,0,0,0},
                                {0,1,0,0,
                                 0,1,0,0,
                                 1,1,0,0,
                                 0,0,0,0},
                                {1,0,0,0,
                                 1,1,1,0,
                                 0,0,0,0,
                                 0,0,0,0}};

typedef enum tetramino_t {
    square_tetramino = 0,
    t_tetramino,
    L_left_tetramino,
    L_right_tetramino,
    z_left_tetramino,
    z_right_tetramino,
    line_tetramino
} tetramino_t;

const int *tetramino_ptr_from_id(tetramino_t id, int rotation) {
    switch(id) {
        case (square_tetramino) : { return __block_square[rotation]; }
        case (t_tetramino) : { return __block_t[rotation]; }
        case (L_left_tetramino) : { return __block_l_a[rotation]; }
        case (L_right_tetramino) : { return __block_l_b[rotation]; }
        case (z_left_tetramino) : { return __block_z_a[rotation]; }
        case (z_right_tetramino) : { return __block_z_b[rotation]; }
        case (line_tetramino) : { return __block_line[rotation]; }
        default: { return NULL; }
    }
}

// ===========================================================================
// platform-specific variable definitions

#define GAME_WIDTH 10
#define GAME_HEIGHT 20

uint8_t __board[GAME_WIDTH*GAME_HEIGHT];

uint8_t get_board_at(int row, int col) {
    return __board[((row*GAME_WIDTH)%GAME_HEIGHT) + (col%GAME_WIDTH)];
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
    mingw_gettimeofday(&tv, NULL);

    return (uint64_t)((tv.tv_sec*1000000)+(tv.tv_usec));

}

void timing_delay_us(uint64_t t_us) {
    uint64_t t_cur = timing_get_us_64();
    uint64_t t_loop_exit = t_cur + t_us;

    while ( t_cur < t_loop_exit ) { t_cur = timing_get_us_64(); }
}

// ===========================================================================
// game declarations

typedef struct board_t {

    int width;
    int height;

    uint8_t *raw;

} board_t;

typedef struct {

    // ==================================================
    // timing data

    // frames per second
    int fps;

    // number of frames that occur between blocks moving
    int piece_speed;

    // ==================================================
    // game data
    
    int score;
    int level;  

    board_t board;

    tetramino_t active_piece_type;
    int active_piece_rotation_idx;
    int *active_piece;

    // x/y coordinates of the top-left corner of the piece
    int piece_row;
    int piece_col;

} game_inst_t;

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

void print_piece(int piece[16]) {
    for (int i = 0; i < 4; i++) {
        for ( int j = 0; j < 4; j++ ) {
            printf(piece[i*4+j]? "#" : " ");
        }
        printf("\n");
    }
}

void game_loop(game_inst_t *inst) {

    // check player input

    // TODO:
    // check to see if the desired rotation is legal

    int key_lr_dir = ( io_get_key_state(io_key_type_right) - io_get_key_state(io_key_type_left) );
    int rot_dir_change = io_get_key_state(io_key_type_up);

    // update active piece
    inst->active_piece_rotation_idx += rot_dir_change;
    inst->active_piece = tetramino_ptr_from_id(inst->active_piece_type, inst->active_piece_rotation_idx);


}

// ===========================================================================
// variables

game_inst_t game_inst = {
    .fps = 40,
    .piece_speed = 20,
    .score = 0,
    .level = 0,
    .board = { 
        .width=GAME_WIDTH, 
        .height=GAME_HEIGHT, 
        .raw=__board 
    }
};

// ===========================================================================
// platform-specific execution

int main() {
    


} 