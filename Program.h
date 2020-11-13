/*
 * Program.h
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <EEPROM.h>

#define PROGRAM_COUNT 8
#define PROGRAM_DATA_SIZE (CONTROLS_NUM_POTS*2)

class Program {
public:

    virtual Program() {
        for (i = 0; i < PROGRAM_DATA_SIZE; i++) {
            data[i] = 0;
        }
    }
    virtual ~Program() {}

    void read(uint8_t index = 0) {
        lastIndex = index;
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            data[i] = (uint16_t)(EEPROM.read((i << 1) + index * PROGRAM_DATA_SIZE) << 8);
            data[i] += (uint16_t)EEPROM.read((i << 1 + 1) + index * PROGRAM_DATA_SIZE);
        }
    }
    void write(uint8_t index = 0) {
        lastIndex = index;
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            EEPROM.write((i << 1) + index * PROGRAM_DATA_SIZE, (uint8_t)(data[i] & 0xFF00 >> 8));
            EEPROM.write((i << 1 + 1) + index * PROGRAM_DATA_SIZE, (uint8_t)(data[i] & 0x00FF));
        }
    }
    void clear(uint8_t index = 0, bool local = true) {
        lastIndex = index;
        for (i = 0; i < PROGRAM_DATA_SIZE; i++) {
            if (!!local) data[i >> 1] = 0;
            EEPROM.write(i + index * PROGRAM_DATA_SIZE, 0);
        }
    }
    void clearAll() {
        for (j = 0; j < PROGRAM_COUNT; j++) {
            clear(j, false);
        }
    }

    uint16_t get(uint8_t key) {
        return data[key];
    }
    void set(uint8_t key, uint16_t value) {
        data[key] = value;
    }

    uint16_t data[CONTROLS_NUM_POTS];

private:

    uint16_t i, j;
    uint8_t lastIndex;

};

#endif /* PROGRAM_H_ */
