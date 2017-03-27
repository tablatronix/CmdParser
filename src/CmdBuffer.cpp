/* Copyright 2016 Pascal Vizeli <pvizeli@syshack.ch>
 * BSD License
 *
 * https://github.com/pvizeli/CmdParser
 */

#include "CmdBuffer.h"

bool CmdBufferObject::readFromSerial(Stream *serial, uint32_t timeOut)
{
    uint32_t isTimeOut;
    uint32_t startTime;
    size_t   readPtr;
    uint8_t  readChar;
    uint8_t *buffer   = this->getBuffer();
    bool     over     = false;

    // UART initialize?
    if (serial == NULL) {
        return false;
    }

    ////
    // init buffers
    this->clear();

    // init Counter
    readPtr ^= readPtr;

    ////
    // Calc Timeout
    if (timeOut != 0) {
        startTime = millis();
        isTimeOut = startTime + timeOut;

        // overloaded
        if (isTimeOut < startTime) {
            over = true;
        } else {
            over = false;
        }
    }

    ////
    // process serial reading
    do {

        // if data in serial input buffer
        while (serial->available()) {

            // is buffer full?
            if (readPtr >= this->getBufferSize()) {
                return false;
            }

            // read into buffer
            readChar = serial->read();

            // is that the end of command
            if (m_endChar == CMDBUFFER_CHAR_CRLF && 
                    (readChar == CMDBUFFER_CHAR_CR || readChar == CMDBUFFER_CHAR_LF))
                    return true;
            else if(readChar == m_endChar) return true;

            // is a printable character
            if (readChar > CMDBUFFER_CHAR_PRINTABLE) {
                buffer[readPtr++] = readChar;
            }
        }

        // Timeout is active?
        if (timeOut != 0) {
            // calc diff timeout
            if (over) {
                if (startTime > millis()) {
                    over = false;
                }
            }

            // timeout is receive
            if (isTimeOut <= millis() && !over) {
                return false;
            }
        }
        yield();
    } while (true); // timeout

    return false;
}

bool CmdBufferObject::readFromSerialnonblocking(Stream *serial, uint32_t timeOut)
{
    uint32_t isTimeOut;
    uint32_t startTime = this->getBufferStartTime();
    size_t   readPtr = this->getBufferPtr();  
    uint8_t  readChar;
    uint8_t *buffer   = this->getBuffer();
    bool     over     = false;

    // UART initialize?
    if (serial == NULL) {
        return false;
    }

    ////
    // Calc Timeout
    over = (timeOut > 0 && startTime > 0 && (millis()-startTime > timeOut));

    // if timeout reached, clear buffer of old data
    if(over){
        Serial.println("BUFFER TIMEOUT, old data removed from buffer");        
        this->clear();
        readPtr = this->getBufferPtr();
    }

    // if data in serial input buffer
    while (serial->available()) {
        // is buffer full?
        if (readPtr >= this->getBufferSize()) {
            this->clear();
            Serial.println("BUFFER FULL, emptying buffer");
            return false;
        }

        setBufferStartTime(millis());

        // read into buffer
        readChar = serial->read();
        Serial.println(readChar,HEX);

        // is that the end of command
        if (m_endChar == CMDBUFFER_CHAR_CRLF && 
                (readChar == CMDBUFFER_CHAR_CR || readChar == CMDBUFFER_CHAR_LF))
                return true;
        else if(readChar == m_endChar) return true;

        // is a printable character
        if (readChar > CMDBUFFER_CHAR_PRINTABLE) {
            buffer[readPtr++] = readChar;
            this->setBufferPtr(readPtr);
        }
   }
    return false;
}
