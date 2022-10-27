#ifndef PRINT_SYS_ERR_MSG_H
#define PRINT_SYS_ERR_MSG_H 1

// Print system error message.
void PrintSystemErrorMessage(const char *msg);

// Print system error message. (Can specify errno.)
void PrintSystemErrorMessage2(const char *msg, int no);

// Handle error with sys msg.
void DieWithSystemMessage(const char *msg);

// Handle error with sys msg. (Can specify errno.)
void DieWithSystemMessage2(const char *msg, int no);

#endif  // PRINT_SYS_ERR_MSG_H
