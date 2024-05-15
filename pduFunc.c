/*
    createPDU()
    printPDU()
    PDU format:
    [ 4 byte sequence number in network order] [ 2 byte checksum ] [ 1 byte flag] [ payload (up to 1400 bytes)]
    */
#include "pduFunc.h"
#define MAXBUF 1400


int createPDU(uint8_t * pduBuffer, uint32_t seqNum, uint8_t flag, uint8_t *payload, int payLoadLen){
    /* converts seqnumber to network order. Uses checksum to calculate the crc header value*/
    int pduLength = 0;
    uint16_t crc = 0;
    uint32_t seqNumNW = htonl(seqNum);

    /*putting the header in the PDU*/
    memcpy(pduBuffer, &seqNumNW, sizeof(uint32_t));
    /* zeroing out checksum bits*/
    memcpy(pduBuffer +(sizeof(uint8_t) * 4), &crc, sizeof(uint16_t));
    memcpy(pduBuffer + (sizeof(uint8_t) * 6), &flag, sizeof(uint8_t));
    
    /*Putting the payload into the PDU*/
    memcpy(pduBuffer + (sizeof(uint8_t) * 7), payload, payLoadLen);
    
    pduLength = payLoadLen + 7;  
    
    /*running checksum on the entire PDU*/
    crc = (uint16_t)in_cksum((short unsigned int *)pduBuffer, pduLength);
    /*printf("Checksum: %d\n", crc);*/
    memcpy(pduBuffer + (sizeof(uint8_t) * 4), &crc, sizeof(uint16_t));

    return pduLength;
}

void printPDU(uint8_t * aPDU, int pduLength){
    /*checking the 7 byte header*/
    short unsigned int crc = in_cksum((short unsigned int *)aPDU, pduLength);
    if(!(crc == 0)){
        /*checksum fails*/
        perror("checksum failed");
        printf("Checksum failed, was : %d\n", crc);
        exit(-1);
    }
    else{
        uint32_t seqNum = 0;
        uint8_t flag = 0; 
        int payLoadLength = pduLength - 7;
        uint8_t payload[MAXBUF];

        memcpy(&seqNum, aPDU, 4);
        memcpy(&flag, aPDU + 6, 1);
        memcpy(&payload, aPDU + 7, pduLength - 7);

        seqNum = ntohl(seqNum);
        printf("Sequence: %d, Flag: %d, PayloadLength: %d, Payload: %s\n", seqNum, flag, payLoadLength, payload);
    }
}