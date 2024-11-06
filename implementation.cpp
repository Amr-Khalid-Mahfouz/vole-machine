#include "header.h"    

    // constructor    
    Memory::Memory(){        
        for(int i = 0 ; i < 16; i++){
            for(int j = 0 ; j < 16; j++){
                mem[i][j] = "00";}
        }
    };

    // get the value in a memory cell
    string Memory::get_cell(int row, int column){
        return mem[row][column];}

    // change the value in a memory cell
    void Memory::set_cell(string val, int row, int column){
        mem[row][column][0] = val[0];
        mem[row][column][1] = val[1];}


    // constructor
    Register::Register(){
        for(int i = 0 ; i<16 ; i++){
            reg[i] = "00";}
    }

    // get the value in a register cell
    string Register::get_cell(int cell){
        return reg[cell];}

    // change the value in a register cell
    void Register::set_cell(string val, int cell){
        reg[cell][0] = val[0];
        reg[cell][1] = val[1];}


    // function to convert hexadecimals into decimals :
    int Translate_strings::hex_to_dec(string hexNum){

        char* end;
        long decimalValue = std::strtol(hexNum.c_str(), &end, 16);

        return decimalValue;}

    // function to convert decimals into hexadecimals : 
    string Translate_strings::dec_to_hex(int decimal){   
        stringstream ss;
        bitset <8> bit_num(decimal);
        // Convert to hexadecimal and print
        unsigned num = bit_num.to_ulong();
        ss << hex << num << endl;
        string hexa;
        ss >> hexa;
        for(char &c : hexa){
            c = toupper(c);}
        return hexa;}

    // function to convert hexadecimals into float integers
    float Translate_strings::hex_to_float(string hex_val){
        int num = hex_to_dec(hex_val);
        bitset <8> bit_num(num);
        
        // split the number into 3 parts
        string s_num = bit_num.to_string();
        string mantissa = s_num.substr(4,4);
        string exponent = s_num.substr(1,3);
        
        // check sign
        bool is_neg = bit_num[7];
        int power = 0;
        int exp = 0;
        for(int i = exponent.length()-1 ; i>=0 ; i--){
            int digit = exponent[i] - '0';
            exp += pow(2, power) * digit;
            power ++;}

        exp = exp - 4;
        float ans = 0;
        power = -1;
        for(int i = 0; i < mantissa.length() ; i++){
            int digit = mantissa[i] - '0';
            ans += pow(2, power) * digit;
            power--;}
            
        ans = ans * pow(2, exp);
        if(is_neg){ans = ans * -1;}
        return ans;}

    
    // function to get the signed number from it's binary representation 
    int instructions::from_unsigned_to_signed(bitset <8> num){
        unsigned U_num = num.to_ulong();

        // check if the number is negative by checking the most significant bit 
        bool is_negative = num[num.size()-1];
        
        int signed_num;
        if(is_negative){signed_num = U_num - (1 << num.size());}
        else{signed_num = U_num;}

        return signed_num;
    }

    instructions::instructions(){
        // make a map with each possible float number "wow, how dynamic" 
        for(int i = 0 ; i < 256 ; i++){
            string hex_value = converter.dec_to_hex(i);
            float_to_int[converter.hex_to_float(hex_value)] = i;}
    }

    // 1RXY : load value in memory slot XY in register R
    void instructions::load(Register &reg, string reg_cell, Memory &mem, string hex_location){
        int row = converter.hex_to_dec(hex_location.substr(0, 1));
        int column = converter.hex_to_dec(hex_location.substr(1, 1));
        int reg_c = converter.hex_to_dec(reg_cell);
        reg.set_cell(mem.get_cell(row, column), reg_c);
    }
    
    // 2RXY : load value XY in register R
    void instructions::load(Register &reg, string reg_cell, string hex_value){
        int reg_c = converter.hex_to_dec(reg_cell);
        reg.set_cell(hex_value, reg_c);
    }
    
    // 3RXY : store value in register R in memory cell XY
    void instructions::store(Register &reg, string reg_cell , Memory &mem, string hex_location){
        int row = converter.hex_to_dec(hex_location.substr(0, 1));
        int column = converter.hex_to_dec(hex_location.substr(1, 1));        
        int reg_c = converter.hex_to_dec(reg_cell);
        string value = reg.get_cell(reg_c);
        mem.set_cell(value, row, column);
        
        // (specific case) 3R00 :  
        if(hex_location == "00"){cout << mem.get_cell(row, column) << " is in memory cell 00\n";}
    }    

    // 40RS : store value in register R to register s
    void instructions::store(Register &reg, string hex_location_from, string hex_location_to){
        int from = converter.hex_to_dec(hex_location_from);
        int to = converter.hex_to_dec(hex_location_to);
        reg.set_cell(reg.get_cell(from), to);
        reg.set_cell("00", from);
    }

    // 5RST : register R = register S + register T
    void instructions::add(Register &reg, string hex_reg1, string hex_reg2 , string hex_sum_reg){
        // change addresses into ints
        int reg1 = converter.hex_to_dec(hex_reg1);
        int reg2 = converter.hex_to_dec(hex_reg2);
        int sum_reg = converter.hex_to_dec(hex_sum_reg);
        
        // get the values in both registers in hexa
        string val1 = reg.get_cell(reg1);
        string val2 = reg.get_cell(reg2);
        
        // convert them into binary
        bitset <8> b_num1(converter.hex_to_dec(val1));
        bitset <8> b_num2(converter.hex_to_dec(val2));
        int num1 = from_unsigned_to_signed(b_num1); 
        int num2 = from_unsigned_to_signed(b_num2); 

        // add both 
        int sum = num1 + num2;

        string s_sum = converter.dec_to_hex(sum);

        // store the new value in the third register
        reg.set_cell(s_sum, sum_reg);
    }
    
    // 6RST : register R = register S + register T, but float
    void instructions::float_add(Register &reg, string hex_reg1, string hex_reg2 , string hex_sum_reg){
        // change addresses into ints
        int reg1 = converter.hex_to_dec(hex_reg1);
        int reg2 = converter.hex_to_dec(hex_reg2);
        int sum_reg = converter.hex_to_dec(hex_sum_reg);

        // get the values in both registers in hexa
        string val1 = reg.get_cell(reg1);
        string val2 = reg.get_cell(reg2);
        float num1 = converter.hex_to_float(val1);
        float num2 = converter.hex_to_float(val2);

        // add
        float sum = num2 + num1;
        string s = converter.dec_to_hex(float_to_int[sum]);

        reg.set_cell(s, sum_reg);
    }


    // constructor
    Machine::Machine(): mem(), reg(){};
    
    void Machine::run_instructions(){
        int count = 0;
        string sub = "";
        cout << endl;
        
        for (int i = 0; i < 16; i++){ // leave the first row since it won'converter contain any instructions 
            for (int j = 0; j < 16; j++) { // Start from j = 0 to include all columns
                sub.append(mem.get_cell(i, j));
                count++;
                if (count % 2 == 0){
                    if (sub[0] == '1'){cpu.load(reg, sub.substr(1,1), mem, sub.substr(2,2));}
                    else if (sub[0] == '2'){cpu.load(reg,sub.substr(1,1),sub.substr(2,2));}
                    else if (sub[0] == '3'){cpu.store(reg, sub.substr(1,1), mem, sub.substr(2,2));}
                    else if (sub[0] == '4'){cpu.store(reg, sub.substr(2,1), sub.substr(3,1));}
                    else if (sub[0] == '5'){cpu.add(reg, sub.substr(2,1), sub.substr(3,1), sub.substr(1,1));}
                    
                    // A problem we couldn't fix is which value should we print to the register/memory exactly
                    // since a single float number can have multiple hex/ binary representation in the algorithm the doctor told us to use "I think"
                    // for example : -0.25 can be B8, E1, D2 or even c4 so please double check thank you :)
                    else if (sub[0] == '6'){cpu.float_add(reg, sub.substr(2,1), sub.substr(3,1), sub.substr(1,1));}
                    else if (sub[0] == 'B'){
                        if(converter.hex_to_dec(reg.get_cell(converter.hex_to_dec(sub.substr(1,1)))) == converter.hex_to_dec(reg.get_cell(0))){
                            // new row 'i' and column 'j' to go to
                            int row = converter.hex_to_dec(sub.substr(2,1));
                            int column = converter.hex_to_dec(sub.substr(3,1))-1;
                            sub = "";
                            count = 0;
                            i = row; j = column;
                            continue;}
                    }
                    else if (sub[0] == 'C' || sub[0] == 'c'){
                        if(sub == "c000" || sub == "C000"){
                            cout << "program executed\n";
                            return;}}
                    sub="";}
                }
                count = 0;}
    }


    // a function to organize the output of each memory cell visually 
    void Ui::check_cells(Memory &mem, int row, int column){
        string val = mem.get_cell(row, column);
        int num = converter.hex_to_dec(val);
        // if it consists of 1 character
        if(num <= 15 && val[0] != '0'){
            char corrected_val[2] = {'0', val[0]};
            mem.set_cell(corrected_val, row, column);}
    }

    // a function to organize the output of each register cell visually 
    void Ui::check_cells(Register &reg, int cell){
        string val = reg.get_cell(cell);
        int num = converter.hex_to_dec(val);
        // if it consists of 1 character
        if(num <= 15 && val[0] != '0'){
            char corrected_val[2] = {'0', val[0]};
            reg.set_cell(corrected_val, cell);}
    }

    // a function to display the content of each memory or register cell
    void Ui::output(Machine &m){
        string hexa[] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
        
        cout << "Memory cells : \n";

        for(int x = 0 ; x<16 ; x++){cout  << "  " << hexa[x];}
        cout << endl;

        for(int i = 0 ; i < 16 ; i ++){
            cout << hexa[i] << " ";
            for(int j = 0 ; j < 16 ; j ++){
                check_cells(m.mem, i, j);
                cout << m.mem.get_cell(i, j) << " ";}
            cout << endl;}

        cout << "\nRegisters : \n";
        for(int x = 0 ; x<16 ; x++){
            check_cells(m.reg, x);
            cout << hexa[x] << "  " ;}

        cout << endl;

        for(int i = 0 ; i < 16 ; i++){
            cout << m.reg.get_cell(i) << " ";}
        cout << endl << endl;}

    // fetch instructions from file 
    void Ui::load_instructions(string file_name, Machine &m){
        fstream all_instructions;
        all_instructions.open(file_name, ios::in);

        // the string that will hold all the instructions
        string loader = "";
        
        string str; 
        // read all the instructions
        while(!all_instructions.eof()){
            all_instructions >> str;
            loader = loader + str;}

        all_instructions.close();

        // insert each 2 hexa_decimals into a memory cell
        int loader_pos = 0;
        
        for(int i = 1 ; i < 16 ; i ++){
            for(int j = 0 ; j < 16 ; j++){
                char insert[2];
                loader.copy(insert, 2, loader_pos);
                // machine . memory . set_cell()
                m.mem.set_cell(insert, i, j);
                loader_pos = loader_pos +2;
                
                if(loader_pos >= loader.length()){
                    break;}
                }
            if(loader_pos >= loader.length()){
                break;}
            }    
    }
