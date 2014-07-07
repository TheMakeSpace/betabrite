#ifndef __SIGN_ECHO__
#define __SIGN_ECHO__
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <errno.h>
#include "rs232.h"

// alpha protocol message control codes
#define SOH 0x01 // start of header
#define STX 0x02 // start of text
#define EOT 0x04 // end of transmission
#define ESC 0x1b // escape

// unit type codes
#define ALL_MSG_CENTERS 0x5a

// special sign addresses
#define BROADCAST_ADDR 0x3030

// command codes
#define CMD_WRITE_TEXT  0x41
#define CMD_READ_TEXT   0x42

// position codes
#define POS_MIDDLE_LINE 0x20
#define POS_TOP_LINE    0x22
#define POS_BOTTOM_LINE 0x26
#define POS_FILL        0x30

// mode code
#define MODE_ROTATE             0x61
#define MODE_HOLD               0x62
#define MODE_FLASH              0x63
#define MODE_ROLL_UP            0x65
#define MODE_ROLL_DOWN          0x66
#define MODE_ROLL_LEFT          0x67
#define MODE_ROLL_RIGHT         0x68
#define MODE_WIPE_UP            0x69
#define MODE_WIPE_DOWN          0x6a
#define MODE_WIPE_LEFT          0x6b
#define MODE_WIPE_RIGHT         0x6c
#define MODE_SCROLL             0x6d
#define MODE_AUTOMODE           0x6f
#define MODE_ROLL_IN            0x70
#define MODE_ROLL_OUT           0x71
#define MODE_WIPE_IN            0x72
#define MODE_WIPE_OUT           0x73
#define MODE_COMPRESSED_ROTATE  0x74
#define MODE_SPECIAL            0x6e

// special specifiers
#define SPECIAL_TWINKLE         0x30
#define SPECIAL_SPARKLE         0x31
#define SPECIAL_SNOW            0x32
#define SPECIAL_INTERLOCK       0x33
#define SPECIAL_SWITCH          0x34
#define SPECIAL_SLIDE           0x35
#define SPECIAL_SPRAY           0x36
#define SPECIAL_STARBURST       0x37
#define SPECIAL_SCRIPT_WELCOME  0x38
#define SPECIAL_SLOT_MACHINE    0x39

// control codes

// speed
#define NO_HOLD 0x09
#define SPEED_1 0x15
#define SPEED_2 0x16
#define SPEED_3 0x17
#define SPEED_4 0x18
#define SPEED_5 0x19

// character set
#define CTRL_CHAR_SET   0x1a
#define CHAR_SET_1      0x31 // five high standard
#define CHAR_SET_3      0x33 // seven high standard
#define CHAR_SET_5      0x35 // seven high fancy
#define CHAR_SET_6      0x36 // ten high standard
#define CHAR_SET_8      0x38 // full height fancy
#define CHAR_SET_9      0x39 // full height standard

// colors
#define CTRL_COLOR      0x1c
#define COLOR_RED       0x31
#define COLOR_GREEN     0x32
#define COLOR_AMBER     0x33
#define COLOR_DIM_RED   0x34
#define COLOR_DIM_GREEN 0x35
#define COLOR_BROWN     0x36
#define COLOR_ORANGE    0x37
#define COLOR_YELLOW    0x38
#define COLOR_RAINBOW1  0x39
#define COLOR_RAINBOW2  0x41
#define COLOR_COLOR_MIX 0x42
#define COLOR_AUTOCOLOR 0x43

typedef struct {
    char    name[128];
    uint8_t value;
} ctrl_code_t;

typedef struct {
    uint8_t start;
    uint8_t type_code;
    uint16_t address;
} alpha_tx_hdr_t;

typedef struct {
    uint8_t start;
    uint8_t cmd;
    uint8_t file_label;
    uint8_t esc;
    uint8_t display_pos;
    uint8_t mode_code;
} alpha_msg_hdr_t;

int get_ctrl_code_list_index(char *name, ctrl_code_t *list);
void print_ctrl_code_list(FILE *fp, const char *prefix, ctrl_code_t *list);

void print_usage(char *cmd_name);
int parse_options(int argc, char **argv);

void init_alpha_tx_hdr(alpha_tx_hdr_t *hdr);
void init_alpha_msg_hdr(alpha_msg_hdr_t *hdr);

int write_msg_to_sign(char *buffer, size_t len);

#endif
