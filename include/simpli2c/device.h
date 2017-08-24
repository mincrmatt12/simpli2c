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

        void write(uint8_t value);
        void write(uint8_t *values, size_t amt);

        template<int N>
        void write(std::array<uint8_t, N> values) {

        }

        uint8_t readOne();

        template<int N>
        std::array<uint8_t, N> read();

    };

}


#endif //SIMPLI2C_DEVICE_H
