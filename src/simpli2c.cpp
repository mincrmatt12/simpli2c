//
// Created by matthew on 24/08/17.
//

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "simpli2c/simpli2c.h"

simpli2c::Device::Device(uint8_t busnum, uint8_t address) {
    std::string pth = "/dev/i2c-" + std::to_string(busnum);
    this->i2cBus = pth;
    this->i2cAddress = address;
    this->i2cDevice = -1;
}

simpli2c::Device::Device(boost::filesystem::path i2cDevice, uint8_t address) {
    this->i2cBus = i2cDevice.string();
    this->i2cAddress = address;
    this->i2cDevice = -1;
}

simpli2c::Device::Device(std::string i2cDevice, uint8_t address) {
    this->i2cBus = i2cDevice;
    this->i2cAddress = address;
    this->i2cDevice = -1;
}

void simpli2c::Device::open_() {
    this->i2cDevice = open(this->i2cBus.c_str(), O_RDWR);
    if (this->i2cDevice < 0) {
        throw std::runtime_error("failed to open i2c device");
    }
    if (ioctl(this->i2cDevice, I2C_SLAVE, this->i2cAddress) < 0) {
        throw std::runtime_error("failed to connect to device. is it plugged in?");
    }
}

bool simpli2c::Device::isOpen() {
    return this->i2cDevice != -1;
}

void simpli2c::Device::writeOne(uint8_t value) {
    if (write(this->i2cDevice, &value, 1) != 1) {
        throw std::runtime_error("failed to write to the i2c device");
    }
}

void simpli2c::Device::writeMany(uint8_t *values, size_t amt) {
    if (write(this->i2cDevice, values, amt) != amt) {
        throw std::runtime_error("failed to write to the i2c device");
    }
}

void simpli2c::Device::writeRegister(uint8_t address, uint8_t value) {
    if (write(this->i2cDevice, new uint8_t[2]{address, value}, 2) != 2) {
        throw std::runtime_error("failed to write to the i2c device");
    }
}

uint8_t simpli2c::Device::readOne() {
    uint8_t buf[1];
    if (read(this->i2cDevice, buf, 1) != 1) {
        throw std::runtime_error("failed to read from the i2c device");
    }
    return buf[0];
}

void simpli2c::Device::readMany(size_t amt, uint8_t *buffer) {
    if (read(this->i2cDevice, buffer, amt) != amt) {
        throw std::runtime_error("failed to read from the i2c device");
    }
}

uint8_t simpli2c::Device::requestOne(uint8_t payload) {
    this->writeOne(payload);
    return this->readOne();
}

uint8_t simpli2c::Device::requestOne(uint8_t *payload, size_t amt) {
    this->writeMany(payload, amt);
    return this->readOne();
}

void simpli2c::Device::requestMany(uint8_t payload, size_t respSize, uint8_t *buffer) {
    this->writeOne(payload);
    this->readMany(respSize, buffer);
}

void simpli2c::Device::requestMany(size_t paySize, uint8_t *payload, size_t respSize, uint8_t *buffer) {
    this->writeMany(payload, paySize);
    this->readMany(respSize, buffer);
}

void simpli2c::Device::close_() {
    close(this->i2cDevice);
    this->i2cDevice = -1;
}
