#ifndef vole_h
#define vole_h

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <bitset>
#include <map>

using namespace std;

class Memory{
    string mem[16][16];

    public:
    Memory();
    string get_cell(int row, int column);
    void set_cell(string vla, int row, int column);
};

class Register{
    string reg[16];

    public:
    Register();
    string get_cell(int cell);
    void set_cell(string val, int cell);
};


class Translate_strings{
    public:
    int hex_to_dec(string hexNum);
    string dec_to_hex(int decimal);   
    float hex_to_float(string hex_val);
};

class instructions{
    Translate_strings converter;
    map <float, int> float_to_int;
    int from_unsigned_to_signed(bitset <8> num);

    public:
    instructions();
    void load(Register &reg, string reg_cell, Memory &mem, string hex_location);
    void load(Register &reg, string reg_cell, string hex_value);
    void store(Register &reg, string reg_cell , Memory &mem, string hex_location);
    void store(Register &reg, string hex_location_from, string hex_location_to);
    void add(Register &reg, string hex_reg1, string hex_reg2 , string hex_sum_reg);
    void float_add(Register &reg, string hex_reg1, string hex_reg2 , string hex_sum_reg);
};

class Machine{
    public:
    Memory mem;
    Register reg;
    instructions cpu;
    Translate_strings converter;    

    Machine();
    void run_instructions();
};

class Ui{
    string file_name;
    Translate_strings converter;

    void check_cells(Memory &mem, int row, int column);
    void check_cells(Register &reg, int cell);

    public:
    void output(Machine &m);
    void load_instructions(string file_name, Machine &m);
};

#endif
