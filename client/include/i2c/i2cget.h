#ifndef I2C_GET_H
#define I2C_GET_H

#include "i2c/i2c-dev.h"
#include "i2c/i2cbusses.h"
#include "useful/util.h"
#include "i2c/version.h"

#define ASK_BUS "3"
#define ASK_ADDRESS "0x1d"
#define ASK_X "0x29"
#define ASK_Y "0x2B"
#define ASK_Z "0x2D"

int getI2cValue(char* i2cbus_char, char* address_char, char* regist);

#endif
