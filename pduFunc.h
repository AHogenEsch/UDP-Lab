#include <stdint.h>

int createPDU(uint8_t * pduBuffer, uint32_t seqNum, uint8_t flag, uint8_t *payload, int payLoadLen);
void printPDU(uint8_t * aPDU, int pduLength);