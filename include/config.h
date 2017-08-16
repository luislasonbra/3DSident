#ifndef CONFIG_H
#define CONFIG_H

#include <3ds.h>

const wchar_t * getUsername(void);
char * getBirthday(void);
char * getEulaVersion(void);
char * getParentalPin(void);
char * getParentalEmail(void);
char * getParentalSecretAnswer(void);
bool isDebugModeEnabled(void);

#endif