/*
    createPDU()
    printPDU()
    PDU format:
    [ 4 byte sequence number in network order] [ 2 byte checksum ] [ 1 byte flag] [ payload (up to 1400 bytes)]
    */
#include "pduFunc.h"

int createPDU(uint8_t * pduBuffer, uint32_t seqNum, uint8_t flag, uint8_t *payload, int payLoadLen){
    /* converts seqnumber to network order. Uses checksum to calculate the crc header value*/
    int pduLength = 0;
    unsigned short crc = 0;
    uint32_t seqNumNW = htonl(seqNum);
    uint16_t zero = 0;

    memcpy(pduBuffer, &seqNumNW, sizeof(uint32_t));
    memcpy(pduBuffer + (sizeof(uint8_t) * 6), &flag, sizeof(uint8_t));
    memcpy(pduBuffer +(sizeof(uint8_t) * 4), &zero, sizeof(uint16_t));
    crc = in_cksum((short unsigned int *)pduBuffer, payLoadLen + 7);
    memcpy(pduBuffer + (sizeof(uint8_t) * 4), &crc, sizeof(uint16_t));
    
    memcpy(pduBuffer + (sizeof(uint8_t) * 7), &payload, payLoadLen);
     pduLength = payLoadLen + 7;

    return pduLength;
}

void printPDU(uint8_t * aPDU, int pduLength){
    /*checking the 7 byte header*/
    if(!in_cksum((short unsigned int *)aPDU, pduLength)){
        /*checksum fails*/
        perror("checksum failed");
        exit(-1);
    }
    else{
        uint32_t seqNum = 0;
        uint8_t flag = 0; 
        char *payload = (char *)malloc(sizeof(uint8_t) * (pduLength - 7));

        memcpy(&seqNum, aPDU, 4);
        memcpy(&flag, aPDU + 6, 1);
        memcpy(payload, aPDU + 7, pduLength - 7);

        seqNum = ntohl(seqNum);
        printf("Sequence: %d, Flag: %d, PayloadLength: %d, Payload: %s.\n", seqNum, flag, pduLength - 7, payload);
        free(payload);
    }
}