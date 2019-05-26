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

#include <Wire.h>
#include "MCP23008.h"

MCP23008::MCP23008(uint8_t addr, bool verify)
{
    _addr = addr | MCP23008_ADDRESS;
    _verify = verify;
}

bool MCP23008::begin(void)
{
    Wire.begin();

    // Setup the expander to all inputs and
    // zero all other registers
    //
    // Always verify!
    // This tells us if we talk to the expander

    bool _old_verify = _verify;
    _verify = true;

    uint8_t _rply = true;
    _rply &= write8_verify(MCP23008_IODIR,   0xFF);
    _rply &= write8_verify(MCP23008_IPOL,    0x00);
    _rply &= write8_verify(MCP23008_GPINTEN, 0x00);
    _rply &= write8_verify(MCP23008_DEFVAL,  0x00);
    _rply &= write8_verify(MCP23008_INTCON,  0x00);
    _rply &= write8_verify(MCP23008_IOCON,   0x00);
    _rply &= write8_verify(MCP23008_GPPU,    0x00);
    _rply &= write8_verify(MCP23008_GPIO,    0x00);

    _verify = _old_verify;

    return _rply;
}

bool MCP23008::pinMode(uint8_t pin, uint8_t mode)
{
    if(pin > 7)
    {
         return false;
    }

    uint8_t iodir = read8(MCP23008_IODIR);
    uint8_t pullup = read8(MCP23008_GPPU);
    
    uint8_t pmask = 1 << pin;

    if(mode == MCP23008_INPUT_PULLUP) {
        iodir |= pmask;
        pullup |= pmask;
    } else if(mode == MCP23008_INPUT) {
        iodir |= pmask;
        pullup &= ~pmask;
    } else if(mode == MCP23008_OUTPUT) {
        iodir &= ~pmask;
        pullup &= ~pmask;
    }
  
    bool _rply = true;
    _rply &= write8_verify(MCP23008_IODIR, iodir);
    _rply &= write8_verify(MCP23008_GPPU, pullup);

    return _rply;
}

bool MCP23008::setIODir(uint8_t iodir, bool pullup)
{
    bool _rply = true;
    _rply &= write8_verify(MCP23008_IODIR, iodir);
    if(pullup)
    {
        _rply &= write8_verify(MCP23008_GPPU, iodir);
    }

    return _rply;
}

bool MCP23008::setPullup(uint8_t pullup)
{
    return write8_verify(MCP23008_GPPU, pullup);
}

bool MCP23008::setPullup(uint8_t pin, bool pullup)
{
    if(pin > 7)
    {
        return false;
    }

    uint8_t _pullup = read8(MCP23008_GPPU);
    setbit(pin, pullup, &_pullup);

    return write8_verify(MCP23008_GPPU, pullup);
}

uint8_t MCP23008::getPullup(void)
{
    return read8(MCP23008_GPPU);
} 

uint8_t MCP23008::getPullup(uint8_t pin)
{
    if(pin > 7)
    {
        return 0;
    }
    uint8_t pullup = read8(MCP23008_GPPU);
    return (pullup >> pin) & 0x1;
}

uint8_t MCP23008::readGPIO(void)
{
    return read8(MCP23008_GPIO);
}

bool MCP23008::writeGPIO(uint8_t val)
{
    return write8_verify(MCP23008_GPIO, val);
}

bool MCP23008::digitalWrite(uint8_t pin, uint8_t val)
{
    if(pin > 7)
    {
        return false;
    }

    uint8_t gpio = read8(MCP23008_GPIO);
    setbit(pin, val, &gpio);

    return write8_verify(MCP23008_GPIO, gpio);
}

uint8_t MCP23008::digitalRead(uint8_t pin)
{
    if(pin > 7)
    {
        return 0;
    }

    uint8_t gpio = read8(MCP23008_GPIO);
    return (gpio >> pin) & 0x1;
}

uint8_t MCP23008::read8(uint8_t addr)
{
  Wire.beginTransmission(_addr);
  Wire.write(addr);	
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);

  return Wire.read();
}

void MCP23008::write8(uint8_t addr, uint8_t data)
{
    Wire.beginTransmission(_addr);
    Wire.write(addr);
    Wire.write(data);
    Wire.endTransmission();
}

bool MCP23008::write8_verify(uint8_t addr, uint8_t data)
{
    write8(addr, data);

    if(!_verify)
    {
        // Fake it
        return true;
    }

    uint8_t _data;
    _data = read8(addr);
    if(_data == data)
    {
        return true;
    }

    return false;
}

void MCP23008::setbit(uint8_t bit, uint8_t toset, uint8_t *val)
{
    if(toset)
    {
        *val |= (1 << bit); 
    } else {
        *val &= ~(1 << bit);
    }
}
