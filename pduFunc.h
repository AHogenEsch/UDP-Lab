#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "cpe464.h"

int createPDU(uint8_t * pduBuffer, uint32_t seqNum, uint8_t flag, uint8_t *payload, int payLoadLen);
void printPDU(uint8_t * aPDU, int pduLength);