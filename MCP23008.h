// MIT License
// 
// Copyright (c) 2019 Stuart Wilkins
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 

#ifndef _MCP23008_H_
#define _MCP23008_H_

#define MCP23008_ADDRESS         0x20

// registers
#define MCP23008_IODIR           0x00
#define MCP23008_IPOL            0x01
#define MCP23008_GPINTEN         0x02
#define MCP23008_DEFVAL          0x03
#define MCP23008_INTCON          0x04
#define MCP23008_IOCON           0x05
#define MCP23008_GPPU            0x06
#define MCP23008_INTF            0x07
#define MCP23008_INTCAP          0x08
#define MCP23008_GPIO            0x09
#define MCP23008_OLAT            0x0A

// Definitions for pin modes
#define MCP23008_OUTPUT          0
#define MCP23008_INPUT           1
#define MCP23008_INPUT_PULLUP    2

class MCP23008 {
    private:
        uint8_t _addr;
        bool _verify;
        uint8_t read8(uint8_t addr);
        void write8(uint8_t addr, uint8_t data);
        bool write8_verify(uint8_t addr, uint8_t data);
        void setbit(uint8_t bit, uint8_t toset, uint8_t *val);
    public:
        MCP23008(uint8_t addr, bool verify = true);
        bool begin(void);

        bool pinMode(uint8_t p, uint8_t d);
        bool setIODir(uint8_t mask, bool pullup = true);
        bool setPullup(uint8_t pullup);
        bool setPullup(uint8_t pin, bool pullup);
        uint8_t getPullup(void);
        uint8_t getPullup(uint8_t pin);
        bool digitalWrite(uint8_t pin, uint8_t val);
        uint8_t digitalRead(uint8_t p);
        bool pullUp(uint8_t p, uint8_t d);
        uint8_t readGPIO(void);
        bool writeGPIO(uint8_t val);
};

#endif
