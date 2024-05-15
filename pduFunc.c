/*
    createPDU()
    printPDU()
    PDU format:
    [ 4 byte sequence number in network order] [ 2 byte checksum ] [ 1 byte flag] [ payload (up to 1400 bytes)]
    */


int createPDU(uint8_t * pduBuffer, uint32_t seqNum, uint8_t flag, uint8_t *payload, int payLoadLen){
    /* converts seqnumber to network order. Uses checksum to calculate the crc header value*/
    int pduLength = 0;
    unsigned short crc = 0;
    uint32_t seqNumNW = htonl(seqNum);

    memcpy(pduBuffer, seqNumNW, sizeof(uint32_t));
    memcpy(pduBuffer + (sizeof(uint8_t) * 6), flag, sizeof(uint8_t));
    /* do I need to clear the crc bits in the PDU before calling checksum? */
    crc = in_cksum(pduBuffer, 7);
    memcpy(pduBuffer + (sizeof(uint8_t) * 4), crc, sizeof(uint16_t));
    
    memcpy(pduBuffer + (sizeof(uint8_t) * 7), payload, payLoadLen);
     pduLength = payLoadLen + 7;

    return pduLength;
}