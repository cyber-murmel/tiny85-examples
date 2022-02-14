#include "repl.h"
#include "platform_print.h"
#include <stdio.h>
#include <string.h>

static char cmdBuf[CMD_BUF_SIZE + 1] = { '\0' }; // space for trailing \0
static size_t cmdBufIdx = 0;

// helper functions
static inline bool _isCmdBufEmpty(void)
{
    return cmdBufIdx <= 0;
}

static inline bool _isCmdBufFull(void)
{
    return cmdBufIdx >= CMD_BUF_SIZE;
}

static inline void _cmdBufClear(void)
{
    cmdBufIdx = 0;
    cmdBuf[cmdBufIdx] = '\0';
}

/**
 * @brief Try to execute a single command
 *
 * @details The command is stored in argv[0]. This function tries to find it in
 * the SystemCommandList and BuiltinCommandList and then call the corresponding
 * function
 *
 * @param argc argument count
 * @param argv argument vector
 * @return -1 If command was not found
 * @return int Else return value of the command
 */
static int _exec(size_t argc, char* argv[])
{
    // try to find argument
    for (size_t i = 0; i < SystemCommandList.len; i++) {
        REPL_Command_t Command = SystemCommandList.Commands[i];
        // compare command names
        if ((strlen(argv[0]) == strlen(Command.name)) && (strncmp(argv[0], Command.name, strlen(Command.name)) == 0)) {
            // call command handler
            Command.handler(argc, argv);
            return 0;
        }
    }
#if BUILTINS
    for (size_t i = 0; i < BuiltinCommandList.len; i++) {
        REPL_Command_t Command = BuiltinCommandList.Commands[i];
        // compare command names
        if ((strlen(argv[0]) == strlen(Command.name)) && (strncmp(argv[0], Command.name, strlen(Command.name)) == 0)) {
            // call command handler
            Command.handler(argc, argv);
            return 0;
        }
    }
#endif // BUILTINS
    return -1;
}

/**
 * @brief Parse a single command line and try to execute it
 *
 * @param line Reference to command line content
 * @param len Length of command
 * @return -1 If command line was empty
 * @return int Else return value of _exec
 */
static int _processCommand(char* line, size_t len)
{
    if (!_isCmdBufEmpty()) {
        int argc = 0;

        // count number of words and insert \0 after each one to create substrings
        bool isArg = false;
        for (size_t idx = 0; idx < len; idx++) {
            if ((line[idx] != ' ') && !isArg) {
                // start of argument
                isArg = true;
            } else if ((line[idx] == ' ') && isArg) {
                // end of argument
                // add zero termination
                line[idx] = '\0';
                argc++;
                isArg = false;
            }
        }
        if (isArg) {
            // line ended with argument
            line[len] = '\0';
            argc++;
        }

        // create and point argv to substrings
        char *argv[argc], *pos = line;
        for (size_t i = 0; i < argc; i++) {
            argv[i] = pos;
            pos += strlen(pos) + 1;
        }

        return _exec(argc, argv);
    }
    return -1;
}

void REPL_receiveHandler(char rxChar)
{
    switch (rxChar) {
    case '\r': {
        // ignore
    } break;
    case '\n': {
        // echo
        platform_print(NEWLINE);
        _processCommand(cmdBuf, cmdBufIdx);
        _cmdBufClear();
        platform_print(F(PROMPT));
    } break;
    case '\b': {
        if (!_isCmdBufEmpty()) {
            cmdBuf[--cmdBufIdx] = '\0';
            platform_print(F("\b \b"));
        }
    } break;
    case '\x03': {
        // "End-of-Text", or Ctrl-C
    } break;
    case '\x04': {
        // "End-of-Transmission", or Ctrl-D
    } break;
    default: {
        if ((!_isCmdBufFull()) && _isPrintable(rxChar)) {
            cmdBuf[cmdBufIdx++] = rxChar;
            cmdBuf[cmdBufIdx] = '\0';
            platform_print(rxChar);
        }
    } break;
    }
}

#if BUILTINS
static inline void print_help(REPL_Command_t Command)
{
    if (Command.help != NULL) {
        // Logf(LogLevel, Info, "%-8s - %s", Command.name, Command.help);
        char helpBuf[12];
        snprintf(helpBuf, sizeof(helpBuf) - 1, "%-8s - ", Command.name);
        platform_print(helpBuf);
        platform_print(Command.help);
        platform_print(Command.help);
        platform_print(NEWLINE);
    }
}

/**
 * @brief Print builtin command help
 *
 * @param argc argument count
 * @param argv argument vector
 * @return int 0
 */
static int help_commandHandler(size_t argc, char* argv[])
{
    // ignore arguments
    (void)argc;
    (void)argv;

    for (size_t i = 0; i < BuiltinCommandList.len; i++) {
        print_help(BuiltinCommandList.Commands[i]);
    }

    for (size_t i = 0; i < SystemCommandList.len; i++) {
        print_help(SystemCommandList.Commands[i]);
    }

    return 0;
}

/**
 * @brief Display a line of text
 *
 * @param argc argument count
 * @param argv argument vector
 * @return int 0
 */
static int echo_commandHandler(size_t argc, char* argv[])
{
    for (size_t i = 1; i < argc; i++) {
        platform_print(argv[i]);
        if (i < (argc - 1)) {
            platform_print(' ');
        }
    }
    platform_print(NEWLINE);
    return 0;
}

const REPL_Command_t BuiltinCommands[] = {
    {
        .name = "?",
        .help = "print builtin command help",
        .handler = &help_commandHandler,
    },
    {
        .name = "echo",
        .help = "display a line of text",
        .handler = &echo_commandHandler,
    },
};

const REPL_CommandList_t BuiltinCommandList = REPL_COMMAND_LIST(BuiltinCommands);

#endif // BUILTINS