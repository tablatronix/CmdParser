[![Build Status](https://travis-ci.org/pvizeli/CmdParser.svg?branch=master)](https://travis-ci.org/pvizeli/CmdParser)

# CmdParser
A simple and powerful cmd parser with a small memory footprint and very fast algorithm.

This library is for handling commands over serial and short data transfers.

If you need to transfer complex data, use JSON. You can combine these 2 techniques.

Functions with "static" names is possible by handling them with PROGMEM,
add ```_P``` to end of function name for CmdParser and CmdCallback object.

## Parser Object

The function ```parseCmd``` will clear the buffer! Only the parser object can
handle the new buffer...

By default it uses a static parameter list. With the KeyValue option you can change
to dynamic key=value parser handling.

```c++
#include <CmdParser.hpp>

CmdParser myParser;

myParser.parseCmd(myBuffer); // CmdBuffer Object
myParser.parseCmd(byteBuffer, size); // Binary buffer
myParser.parseCmd(cString); // C string buffer

```

### Options
- ```setOptIgnoreQuote``` (default off) supports strings with enclosing quotes eg. "my value"
- ```setOptSeperator``` (default ' ') specify character for cmd seperator
- ```setOptKeyValue``` (default off) Support dynamic key=value feature


## Buffer Object

```c++
#include <CmdBuffer.hpp>

CmdBuffer<32> myBuffer;

// Reading Data
myBuffer.readFromSerial(&Serial, numTimeout);
```

### Options
- ```setEndChar``` (default '\n') set character for stop reading. Normal is a line end.

## Callback Object

```c++
#include <CmdCallback.hpp>

CmdCallback<5> myCallback; // Object for handling 5 functions in SRAM
CmdCallback_P<5> myCallbackP; // Object for handling 5 functions in PROGMEM

// add function
myCallbackP.addCmd(PSTR("SET"), &myFunctForSet);

// Automatic proccessing
myCallbackP.loopCmdProcessing(&myParser, &myBuffer, &Serial);

// Manual
myCallbackP.processCmd(cstrCmd);
```
