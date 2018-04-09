//
// Created by matthew on 24/08/17.
//

#ifndef SIMPLI2C_DEVICE_H
#define SIMPLI2C_DEVICE_H

#include <stdint.h>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <array>

namespace simpli2c {

    class Device {
    public:
        Device(uint8_t busnum, uint8_t address);
        Device(boost::filesystem::path i2cDevice, uint8_t address);
        Device(std::string i2cDevice, uint8_t address);
        ~Device() {if (this->isOpen()) this->close_();}

        void open_();
        bool isOpen();

        void writeOne(uint8_t value);
        void writeMany(uint8_t *values, size_t amt);
        void writeRegister(uint8_t address, uint8_t value); // calls write[2]

        uint8_t readOne();
        void readMany(size_t amt, uint8_t *buffer);

        uint8_t requestOne(uint8_t payload);  // useable for reading registers
        uint8_t requestOne(uint8_t *payload, size_t amt);
        void requestMany(uint8_t payload, size_t respSize, uint8_t *buffer);
        void requestMany(size_t paySize, uint8_t *payload, size_t respSize, uint8_t *buffer);

        void close_();

    private:
        int i2cDevice;
        std::string i2cBus;
        uint8_t i2cAddress;
    };

    inline uint8_t * shortBuffer(uint16_t v) {
        return new uint8_t[2] {
                static_cast<uint8_t>(v & 0xFF),
                static_cast<uint8_t>(v >> 8)
        };
    };

    inline uint8_t * longBuffer(uint32_t v) {
        return new uint8_t[4] {
                static_cast<uint8_t>(v & 0xFF),
                static_cast<uint8_t>(v >> 8 & 0xFF),
                static_cast<uint8_t>(v >> 16 & 0xFF),
                static_cast<uint8_t>(v >> 24 & 0xFF),
        };
    };

    inline uint16_t bufferShort(uint8_t *buf) {
        return buf[0] + buf[1] << 8;
    }
    inline uint32_t bufferLong(uint8_t *buf) {
        return buf[0] + buf[1] << 8 + buf[2] << 16 + buf[3] << 24;
    }

}


#endif //SIMPLI2C_DEVICE_H
