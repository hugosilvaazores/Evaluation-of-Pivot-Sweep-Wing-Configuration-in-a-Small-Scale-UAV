#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include "pico/stdlib.h"
//#include "hardware/i2c.h"

static const float EARTH_GRAVITY = 9.80665;

struct gyro {
    uint8_t scl; // serial clock line
    uint8_t sda; // serial data line 
    uint8_t xda; // aux data 
    uint8_t xcl; // aux clock 
    uint8_t i2c_addr;
    uint8_t interrupt; // interrupt digital output
};

int create_gyro(uint8_t scl, uint8_t sda, uint8_t xda, uint8_t xcl, uint8_t ado, uint8_t interrupt, struct gyro* gyro) {
    if (!gyro) {
        fprintf(stderr, "Critical error: Failed to allocate memory for gyro object.\n"); 
        return -1;
    }
    gyro->scl = scl;
    gyro->sda = sda;
    gyro->xda = xda;
    gyro->xcl = xcl;
    gyro->interrupt = interrupt;
    if (ado) {
        gyro->i2c_addr = 0x69;
    } else {
        gyro->i2c_addr = 0x68;
    }
    return 0;
}

int reg_write(i2c_inst_t* i2c, const uint addr, const uint8_t reg, uint8_t* buf, const uint8_t nbytes);

int reg_read(i2c_inst_t* i2c, const uint addr, const uint8_t reg, uint8_t* buf, const uint8_t nbytes);

// Write bytes to specified address
int reg_write(i2c_inst_t* i2c, const uint addr, const uint8_t reg, uint8_t* buf, const uint8_t nbytes) {
    int num_bytes_read = 0;
    uint8_t msg[nbytes + 1];

    if (nbytes < 1) {
        return 0;
    }

    msg[0] = reg;
    for (int i = 0; i < nbytes; i++) {
        msg[i+1] = buf[i];
    }

    i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);

    return num_bytes_read;
}
