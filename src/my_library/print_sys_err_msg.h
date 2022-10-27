#ifndef DIE_WITH_MESSAGE_H
#define DIE_WITH_MESSAGE_H 1

// Handle error with sys msg.
void DieWithSystemMessage(const char *msg);

// Handle error with sys msg. (Can specify errno.)
void DieWithSystemMessage2(const char *msg, int no);

#endif  // DIE_WITH_MESSAGE_H
