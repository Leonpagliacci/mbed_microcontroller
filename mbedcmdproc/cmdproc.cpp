#include "cmdproc.h"
#include <cstdint>
#include <cstring>

/**************************************************************************/
/*!
    Initialize the command line interface. This initializes things. 
*/
/**************************************************************************/
void commandProcessor::begin(uint8_t prompt_on, uint8_t echo_on)
{
    // init the msg index
    msg_idx = 0;

    // init the command table
    cmd_tbl_list = NULL;

    _PROMPT_ON = prompt_on;
    _ECHO_ON = echo_on;
}

/**************************************************************************/
/*!
    Generate the main command prompt
*/
/**************************************************************************/
void commandProcessor::cmdDisplay()
{
    char buf[50];

    cmdSerial.write("\n", 1);

    strcpy(buf, cmd_banner);
    cmdSerial.write(buf, strlen(buf));
    cmdSerial.write("\n", 1);

    strcpy(buf, cmd_prompt);
    cmdSerial.write(buf, strlen(buf));
}

/**************************************************************************/
/*!
    Parse the command line. This function tokenizes the command input, then
    searches for the command table entry associated with the commmand. Once found,
    it will jump to the corresponding function.
*/
/**************************************************************************/
void commandProcessor::cmdParse()
{
    uint8_t argc, i = 0;
    char *argv[30];
    char buf[50];
    cmd_t *cmd_entry;
    char tmpc;

    //fflush(stdout);
    while(cmdSerial.readable())
        cmdSerial.read(&tmpc, 1); //Does the same thing as fflush

    //Serial.print("Command: ");
    //Serial.println(msg);
    // parse the command line statement and break it up into space-delimited
    // strings. the array of strings will be saved in the argv array.
    argv[i] = strtok(msg, " ");
    do
    {
        argv[++i] = strtok(NULL, " ");
    } while ((i < 30) && (argv[i] != NULL));
    
    // save off the number of arguments for the particular command.
    argc = i;

    // parse the command table for valid command. used argv[0] which is the
    // actual command name typed in at the prompt
    for (cmd_entry = cmd_tbl; cmd_entry != NULL; cmd_entry = cmd_entry->next)
    {
        if (!strcmp(argv[0], cmd_entry->cmd))
        {
            cmd_entry->func(argc, argv);
            if(_PROMPT_ON) cmdDisplay();
            return;
        }
    }

    // command not recognized. print message and re-generate prompt.
    strcpy(buf, cmd_unrecog);
    cmdSerial.write(buf, strlen(buf));
    cmdSerial.write("\n", 1);

    if(_PROMPT_ON) cmdDisplay();
}

/**************************************************************************/
/*!
    This function processes the individual characters typed into the command
    prompt. It saves them off into the message buffer unless its a "backspace"
    or "enter" key. 
*/
/**************************************************************************/
void commandProcessor::cmdHandler()
{
    static char c;

    // Read a single byte.
    cmdSerial.read(&c, 1);
    //cmdSerial.print(c);

    switch (c)
    {
    case '\r':
        // terminate the msg and reset the msg ptr. then send
        // it to the handler for processing.
        msg[msg_idx] = '\0';
        if (_ECHO_ON)
            cmdSerial.write("\r\n", 2);
        msg_idx = 0;
        cmdParse();
        break;
    
    case '\b':
        // backspace 
        if (_ECHO_ON)
            cmdSerial.write(&c, 1);
        if (msg_idx > 0)
        {
            msg_idx--;
        }
        break;
    
    default:
        // normal character entered. add it to the buffer
        if (_ECHO_ON)
        cmdSerial.write(&c, 1);
        msg[msg_idx++] = c;
        break;
    }
}

/**************************************************************************/
/*!
    This function should be set inside the main loop. It needs to be called
    constantly to check if there is any available input at the command prompt.
*/
/**************************************************************************/
void commandProcessor::poll()
{
    while (cmdSerial.readable())
    {
        cmdHandler();
    }
}

/**************************************************************************/
/*!
    Add a command to the command table. The commands should be added in
    at the setup() portion of the sketch. 
*/
/**************************************************************************/
void commandProcessor::add(char *name, void (*func)(int argc, char **argv))
{
    // alloc memory for command struct
    cmd_tbl = (cmd_t *)malloc(sizeof(cmd_t));

    // alloc memory for command name
    char *cmd_name = (char *)malloc(strlen(name)+1);

    // copy command name
    strcpy(cmd_name, name);

    // terminate the command name
    cmd_name[strlen(name)] = '\0';

    // fill out structure
    cmd_tbl->cmd = cmd_name;
    cmd_tbl->func = func;
    cmd_tbl->next = cmd_tbl_list;
    cmd_tbl_list = cmd_tbl;
}

uint32_t commandProcessor::cmdStr2Num(char *str, uint8_t base)
{
    return strtol(str, NULL, base);
}

void commandProcessor::test()
{
    const char testStr[17] = "Test is working!";
    cmdSerial.write(testStr, 17);
    // cmdSerial.flush();
}