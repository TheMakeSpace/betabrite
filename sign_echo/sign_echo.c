#include "sign_echo.h"

#define BUFFER_SIZE 512
#define NUM_PORTS 30

// lists of control codes and their names.
// these must be terminated by a control code
// with "" as the name and 0x00 as the value.
static ctrl_code_t speeds[] = {
    {"no_hold", NO_HOLD},
    {"speed_1", SPEED_1}, 
    {"speed_2", SPEED_2}, 
    {"speed_3", SPEED_3}, 
    {"speed_4", SPEED_4}, 
    {"speed_5", SPEED_5},
    {"",        0x00}
};

static ctrl_code_t colors[] = {
    {"red",         COLOR_RED},
    {"green",       COLOR_GREEN},
    {"amber",       COLOR_AMBER},
    {"dim_red",     COLOR_DIM_RED},
    {"dim_green",   COLOR_DIM_GREEN},
    {"brown",       COLOR_BROWN},
    {"orange",      COLOR_ORANGE},
    {"yellow",      COLOR_YELLOW},
    {"rainbow_1",   COLOR_RAINBOW1},
    {"rainbow_2",   COLOR_RAINBOW2},
    {"color_mix",   COLOR_COLOR_MIX},
    {"autocolor",   COLOR_AUTOCOLOR},
    {"",            0x00}
};

static ctrl_code_t positions[] = {
    {"middle",  POS_MIDDLE_LINE},
    {"top",     POS_TOP_LINE},
    {"bottom",  POS_BOTTOM_LINE},
    {"fill",    POS_FILL},
    {"",        0x00}
};

static ctrl_code_t modes[] = {
    {"rotate",              MODE_ROTATE},
    {"hold",                MODE_HOLD},
    {"flash",               MODE_FLASH},
    {"roll_up",             MODE_ROLL_UP},
    {"roll_down",           MODE_ROLL_DOWN},
    {"roll_left",           MODE_ROLL_LEFT},
    {"roll_right",          MODE_ROLL_RIGHT},
    {"wipe_up",             MODE_WIPE_UP},
    {"wipe_down",           MODE_WIPE_DOWN},
    {"wipe_left",           MODE_WIPE_LEFT},
    {"wipe_right",          MODE_WIPE_RIGHT},
    {"scroll",              MODE_SCROLL},
    {"automode",            MODE_AUTOMODE},
    {"roll_in",             MODE_ROLL_IN},
    {"roll_out",            MODE_ROLL_OUT},
    {"wipe_in",             MODE_WIPE_IN},
    {"wipe_out",            MODE_WIPE_OUT},
    {"compressed_rotate",   MODE_COMPRESSED_ROTATE},
    {"special",             MODE_SPECIAL},
    {"",                    0x00}
};

static ctrl_code_t special_modes[] = {
    {"none",            0x00},
    {"twinkle",         SPECIAL_TWINKLE},
    {"sparkle",         SPECIAL_SPARKLE},
    {"snow",            SPECIAL_SNOW},
    {"interlock",       SPECIAL_INTERLOCK},
    {"switch",          SPECIAL_SWITCH},
    {"slide",           SPECIAL_SLIDE},
    {"spray",           SPECIAL_SPRAY},
    {"starburst",       SPECIAL_STARBURST},
    {"script_welcome",  SPECIAL_SCRIPT_WELCOME},
    {"slot_machine",    SPECIAL_SLOT_MACHINE},
    {"",                0x00}
};

// command line options, set to defaults
static int port = 0;
static int baud = 9600;
static int msg_speed = 0;
static int msg_color = 0;
static int msg_pos = 0;
static int msg_mode = 1;
static int msg_special_mode = 0;

int get_ctrl_code_list_index(char *name, ctrl_code_t *list)
{
    int index = 0;

    while (list && !(strcmp(list->name,"") == 0 && list->value == 0x00)) {
        if (strcmp(list->name, name) == 0)
            return index;

        index++;
        list++;
    }

    return -1;
}

void print_ctrl_code_list(FILE *fp, const char *prefix, ctrl_code_t *list)
{
    while (list && !(strcmp(list->name,"") == 0 && list->value == 0x00)) {
        fprintf(fp, "%s%s\n", prefix, list->name);
        list++;
    }
}

void print_usage(char *cmd_name)
{
    fprintf(stdout, "%s [OPTION...]\n", cmd_name);
    fprintf(stdout, "    -h, --help\n");
    fprintf(stdout, "    --port=PORT (0 - 29)\n");
    fprintf(stdout, "    --baud=BAUD\n");
    fprintf(stdout, "        1200, 2400, 4800, 9600\n");
    fprintf(stdout, "    -p, --position\n");
    print_ctrl_code_list(stdout, "        ", positions);
    fprintf(stdout, "    -s, --speed=SPEED\n");
    print_ctrl_code_list(stdout, "        ", speeds);
    fprintf(stdout, "    -c, --color=COLOR\n");
    print_ctrl_code_list(stdout, "        ", colors);
    fprintf(stdout, "    -m, --mode=MODE\n");
    print_ctrl_code_list(stdout, "        ", modes);
    fprintf(stdout, "    --special=SPECIAL\n");
    fprintf(stdout, "    value other than none must be specified if mode is set to special\n");
    print_ctrl_code_list(stdout, "        ", special_modes);
};

int parse_options(int argc, char **argv)
{
    int ret;
    int option_index;
    static struct option long_options[] = {
        {"speed",       required_argument,  0, 's'},
        {"port",        required_argument,  0, 0},
        {"baud",        required_argument,  0, 0},
        {"color",       required_argument,  0, 'c'},
        {"help",        no_argument,        0, 'h'},
        {"position",    required_argument,  0, 'p'},
        {"mode",        required_argument,  0, 'm'},
        {"special",     required_argument,  0, 0},
        {0, 0, 0, 0}
    };

    while (1) {
        if ((ret = getopt_long(argc, argv, "hs:c:p:m:", long_options, &option_index)) == -1)
            break;

        switch (ret) {
            case 0:
                if (strcmp(long_options[option_index].name,"port") == 0) {
                    port = atoi(optarg);
                    if (port < 0 || port >= NUM_PORTS) {
                        fprintf(stderr, "error: invalid port\n");
                        return -1;
                    }
                } else if (strcmp(long_options[option_index].name,"baud") == 0) {
                    baud = atoi(optarg);
                    if (baud != 1200 &&
                            baud != 2400 &&
                            baud != 4800 &&
                            baud != 9600) {
                        fprintf(stderr, "error: invalid baudrate\n");
                        return -1;
                    }
                } else if (strcmp(long_options[option_index].name,"special") == 0) {
                    msg_special_mode = get_ctrl_code_list_index(optarg, special_modes);
                    if (msg_special_mode < 0) {
                        fprintf(stderr, "error: invalid special mode\n");
                        return -1;
                    }
                    break;
                } else {
                    exit(-1);
                }
                break;
            case 'p':
                msg_pos = get_ctrl_code_list_index(optarg, positions);
                if (msg_pos < 0) {
                    fprintf(stderr, "error: invalid message position\n");
                    return -1;
                }
                break;
            case 's':
                msg_speed = get_ctrl_code_list_index(optarg, speeds);
                if (msg_speed < 0) {
                    fprintf(stderr, "error: invalid message speed\n");
                    return -1;
                }
                break;
            case 'c':
                msg_color = get_ctrl_code_list_index(optarg, colors);
                if (msg_color < 0) {
                    fprintf(stderr, "error: invalid color\n");
                    return -1;
                }
                break;
            case 'm':
                msg_mode = get_ctrl_code_list_index(optarg, modes);
                if (msg_mode < 0) {
                    fprintf(stderr, "error: invalid mode\n");
                    return -1;
                }
                break;
            case 'h':
                print_usage(argv[0]);
                exit(0);
                break;
            case '?': // unrecognized option, getopts_long will already have printed an error
                exit(-1);
                break;
            default:
                break;
        };
    }

    if (modes[msg_mode].value == MODE_SPECIAL &&
            special_modes[msg_special_mode].value == 0x00) {
        fprintf(stderr, "error: special must be a value other than none when mode is set to special\n");
        return -1;
    }

    if (special_modes[msg_special_mode].value != 0x00 &&
            modes[msg_mode].value != MODE_SPECIAL) {
        fprintf(stderr, "error: special cannot be set to a value other than none when mode is not set to special\n");
        return -1;
    }

    return 0;
}

void init_alpha_tx_hdr(alpha_tx_hdr_t *hdr)
{
    hdr->start = SOH;
    hdr->type_code = ALL_MSG_CENTERS;
    hdr->address = BROADCAST_ADDR;
}

void init_alpha_msg_hdr(alpha_msg_hdr_t *hdr)
{
    hdr->start = STX;
    hdr->cmd = CMD_WRITE_TEXT;
    hdr->file_label = 0x41; // file 'A'
    hdr->esc = ESC;
    hdr->display_pos = positions[msg_pos].value;
    hdr->mode_code = modes[msg_mode].value;
}

int write_msg_to_sign(char *buffer, size_t len)
{
    alpha_tx_hdr_t tx_hdr;
    alpha_msg_hdr_t msg_hdr;
    uint32_t msg_begin = 0x00000000U;

    init_alpha_tx_hdr(&tx_hdr);
    init_alpha_msg_hdr(&msg_hdr);

    // send null bytes to indicate new message
    RS232_SendBuf(port, (unsigned char *)&msg_begin, sizeof(msg_begin));
    RS232_SendBuf(port, (unsigned char *)&msg_begin, sizeof(msg_begin));

    // send transmission and message headers
    RS232_SendBuf(port, (unsigned char *)&tx_hdr, sizeof(tx_hdr));
    RS232_SendBuf(port, (unsigned char *)&msg_hdr, sizeof(msg_hdr));

    if (modes[msg_mode].value == MODE_SPECIAL) {
        RS232_SendByte(port, special_modes[msg_special_mode].value);
    }

    RS232_SendByte(port, speeds[msg_speed].value);
    RS232_SendByte(port, CTRL_COLOR);
    RS232_SendByte(port, colors[msg_color].value);

    // send text
    RS232_SendBuf(port, (unsigned char *)buffer, len);

    // send end of trasmission byte
    RS232_SendByte(port,EOT);

    return 0;
}

int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];

    if (parse_options(argc, argv)) {
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL && !feof(stdin)) {
        fprintf(stderr, "error: unable to get input : %s\n", strerror(errno));
        return -1;
    }

    fprintf(stderr, "got message: %s\n", buffer);

    RS232_OpenComport(port, baud);

    write_msg_to_sign(buffer, strlen(buffer));

    RS232_CloseComport(port);

    return 0;
}

#undef BUFFER_SIZE
#undef NUM_PORTS
