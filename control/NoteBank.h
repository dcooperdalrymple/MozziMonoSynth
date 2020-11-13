/*
 * NoteBank.h
 *
 */

#ifndef NOTE_BANK_H_
#define NOTE_BANK_H_

#define NOTE_BANK_SIZE 32

class NoteBank {
public:

    virtual NoteBank() {
        for (i = 0; i < NOTE_BANK_SIZE; i++) {
            bank[i] = 0;
        }
    }
    virtual ~NoteBank() {}

    bool inBank(byte note) {
        for (i = 0; i < NOTE_BANK_SIZE; i++) {
            if (bank[i] == note) return true;
        }
        return false;
    }
    bool removeNote(byte note) {
        if (!inBank(note)) return false;

        j = k = 0;
        for (i = NOTE_BANK_SIZE; i >= 0; i--) {
            k = bank[i];
            bank[i] = j;
            if (k == note) return true;
            j = k;
        }

        return false;
    }
    bool addNote(byte note) {
        for (i = NOTE_BANK_SIZE; i > 0; i--) {
            bank[i] = bank[i - 1];
        }
        bank[0] = note;
    }

    byte getNote() {
        return bank[0];
    }

    bool hasNote() {
        return bank[0] != 0;
    }

private:

    uint8_t bank[NOTE_BANK_SIZE];
    uint8_t i = 0, j = 0, k = 0;

};

#endif /* NOTE_BANK_H_ */
