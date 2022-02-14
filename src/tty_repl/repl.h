#include "Print.h"
#include <stdbool.h>
#include <stddef.h>

#ifndef _REPL_H
#define _REPL_H

#define COUNT_OF(LIST) \
    ((sizeof(LIST) / sizeof(0 [LIST])) / ((size_t)(!(sizeof(LIST) % sizeof(0 [LIST])))))

/**
 * @brief Capacity of the command line buffer in characters
 *
 */
#define CMD_BUF_SIZE (32)

/**
 * @brief Set to 1 enable builtin command, set to 0 to save space
 *
 */
#ifndef BUILTINS
#define BUILTINS (1)
#endif

/**
 * @brief Capacity of the command line buffer in characters
 *
 */
#define PROMPT ("> ")

/**
 * @brief Save space on common strings
 *
 */
static const char NEWLINE[] = "\r\n";

/**
 * @brief Macro to create a command list struct
 *
 */
#define REPL_COMMAND_LIST(COMMANDS)                     \
    {                                                   \
        .Commands = COMMANDS, .len = COUNT_OF(COMMANDS) \
    }

/**
 * @brief Function that processes a received character
 *
 * @param rxChar Character that was received
 */
void REPL_receiveHandler(char rxChar);

/**
 * @brief Type definition for REPL command functions
 *
 */
typedef int (*REPL_commandHandler_t)(size_t argc, char* argv[]);

/**
 * @brief Type definition for REPL commands
 *
 */
typedef struct {
    const char* name;
    const char* help;
    REPL_commandHandler_t handler;
} REPL_Command_t;

/**
 * @brief Type definition for REPL command list
 *
 */
typedef struct {
    const REPL_Command_t* Commands;
    const size_t len;
} REPL_CommandList_t;

#if BUILTINS
/**
 * @brief REPL builtin commands
 *
 */
extern const REPL_CommandList_t BuiltinCommandList;
#endif // BUILTINS

/**
 * @brief System commands
 *
 */
extern const REPL_CommandList_t SystemCommandList;

/**
 * @brief Helper function to check if a character is printable
 *
 * @param c Character to be checkec
 * @return true If character is printable
 * @return false If character is not printable
 */
static inline bool _isPrintable(char c)
{
    return ((' ' <= c) && (c <= '~'));
}

#endif // _REPL_H