//
// Created by tomer on 27/10/2020.
//

#ifndef EEPROG_EEPROG_H
#define EEPROG_EEPROG_H

class EEProg {
public:
    EEProg();
    ~EEProg();

private:

    static constexpr int PAGE_SIZE = 4*1024;
    static constexpr int BLOCK_SIZE = 4*1024;
};

#endif //EEPROG_EEPROG_H
