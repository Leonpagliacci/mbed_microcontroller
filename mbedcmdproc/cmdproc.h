#ifndef CMDPROC_H_
#define CMDPROC_H_

#include "BufferedSerial.h"
#include "mbed.h"
#include <cstdint>

#define MAX_MSG_SIZE    60
#define PROMPT_ON 1
#define PROMPT_OFF 0
#define ECHO_ON 1
#define ECHO_OFF 0

static const char cmd_banner[] = "*************** CMD *******************";
static const char cmd_prompt[] = "CMD >> ";
static const char cmd_unrecog[] = "CMD: Command not recognized.";

typedef struct _cmd_t
{
    char *cmd;
    void (*func)(int argc, char **argv);
    struct _cmd_t *next;
} cmd_t;

class commandProcessor
{
    public:
    commandProcessor(BufferedSerial &s):cmdSerial(s){}
    void begin(uint8_t prompt_on=PROMPT_ON, uint8_t echo_on=ECHO_ON);
    void add(char *name, void (*func)(int argc, char **argv));
    void poll();
    void test();
    private:
    void cmdDisplay();
    void cmdParse();
    void cmdHandler();
    uint32_t cmdStr2Num(char *str, uint8_t base);
    // Stream class to write and read to
    mbed::BufferedSerial &cmdSerial;
    // command line message buffer and pointer
    char msg[MAX_MSG_SIZE];
    uint8_t msg_idx;
    // linked list for command table
    cmd_t *cmd_tbl_list, *cmd_tbl;
    //Should there be a prompt?
    uint8_t _PROMPT_ON;
    uint8_t _ECHO_ON;
};


#endif