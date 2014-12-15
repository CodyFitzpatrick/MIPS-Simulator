/* On my honor, I have neither given nor received unauthorized aid on this assignment */
//
//  main.cpp
//  MIPSsim
//
//  Created by Cody Fitzpatrick on 11/24/14.
//  CDA3101 Project #2 (MIPS Simulator)
//  Copyright (c) 2014 Cody Fitzpatrick. All rights reserved.
//

/**
 NOTE TO GRADER!
 THIS PROGRAM REQUIRES C++11 LIBRARY FUNCTIONS, like to_string(), stoi(), etc. as well as definitions like "nullptr".
 C++11 MUST BE SPECIFIED AT COMPILATION!
 */

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, const char * argv[]) {
    
    struct Node {
        int instructionNumber;
        string operation, baseAddress, location, offset, immediate, rt, rd, rs;
        Node* next;
        Node() {
            instructionNumber = 0;
            operation = "";
            baseAddress = "";
            location = "";
            offset = "";
            immediate = "";
            rt = "";
            rd = "";
            rs = "";
            next = nullptr;
        }
        Node(int in, string op, string ba, string lo, string of, string im, string rtr, string rdr, string rsr, Node* ne) {
            instructionNumber = in;
            operation = op;
            baseAddress = ba;
            location = lo;
            offset = of;
            immediate = im;
            rt = rtr;
            rd = rdr;
            rs = rsr;
            next = ne;
        }
        ~Node() {
            free(next);
        }
    };
    
    //string file_name;
    
    //cout << "Welcome to my MIPS simulator. Please enter a filename (including .txt) to load.\n" << endl; // Display introduction
    //cin >> file_name;  // Read in the user-provided file name
    //ifstream File(file_name); // Open the file
    
    ifstream File(argv[1]); // Open the file entered at the command line
    
    //if(!File) { // Check to see if the file exists
    //    cout << "\nThis file does not appear to exist! Please make sure the file is in the correct directory and try again." << endl; // Display warning message
    //}
    
    vector<string> instructions; // A vector to hold all the instructions in the file
    string currentLine; // A string to temporarily hold each line of the file
    
    while(getline(File, currentLine)) { // Read in the file while it still has lines to be read
        instructions.push_back(currentLine.substr(0,32)); // Push the read line onto the end of the vector
    }
    
    int currentInstructionNumber = 128; // The number of the current instruction
    
    string currentLineOrInstruction, opcode, operation, previous_operation, output, decodedOutput = ""; // Initialize variables to hold the current line we are parsing through's contents, the instruction's opcode, the instruction's operation, the previous instruction's operation, the current instruction's dissambler output, and the decoded instructions (to be read later to perform operations), respectively
    
    vector<string> decodedInstructions; // A vector to hold our decoded instructions, to be read later to perform operations
    vector<int> registerValues; // A vector to hold the register values
    
    ofstream disassemblyFile; // A variable to hold the disassembly write-to file
    disassemblyFile.open("disassembly.txt"); // Open the disassembly write-to file
    
    Node* root = new Node();
    Node* currentNode = root;
        
    /*
     The following for-loop does the following:
        1. Reads the instructions (from user specified .txt file), decodes them, and stores the decoded instructions into a vector
        2. Reads the register values (from the user specified .txt file) and stores them into a vector
        3. Writes to/creates the disassembly.txt file
    */
    
    for(vector<string>::iterator iter = instructions.begin(); iter != instructions.end(); iter++) { // Loop through the vector
        
        currentLineOrInstruction = *iter; // Save the current line to our variable
        
        string categoryNumber = currentLineOrInstruction.substr(0,2); // Get the current instruction's category number
        
        if(previous_operation != "BREAK") {
        
            if(categoryNumber == "00") { // The instruction is a category 1
                
                opcode = currentLineOrInstruction.substr(2,4); // Get the current instruction's opcode
                
                if(opcode == "0000") { // J instruction
                    
                    operation = "J";
                    
                    int il = stoi(currentLineOrInstruction.substr(6, 26)); // Get the jump's instruction location
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(il > 0) {remainder = il%10; decimal=decimal+(remainder*base); base=base*2; il=il/10;} // Convert il to decimal format
                    string il_naked = to_string(4*decimal);
                    string il_printable = "#" + il_naked;
                    
                    decodedOutput = "J " + il_printable;
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");

                    Node* tempNode = new Node(currentInstructionNumber, "J", "", il_naked, "", "", "", "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;

                } else if(opcode == "0010") { // BEQ instruction
                    
                    operation = "BEQ";
                    
                    int rs = stoi(currentLineOrInstruction.substr(6,5)); // Get the rs register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(rs > 0) {remainder = rs%10; decimal=decimal+(remainder*base); base=base*2; rs=rs/10;} // Convert rs to decimal format
                    string rs_naked = to_string(decimal);
                    string rs_printable = "R" + rs_naked; // Full, printable rs register name
                    
                    int rt = stoi(currentLineOrInstruction.substr(11,5)); // Get the rt register in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(rt > 0) {remainder = rt%10; decimal=decimal+(remainder*base); base=base*2; rt=rt/10;} // Convert rt to decimal format
                    string rt_naked = to_string(decimal);
                    string rt_printable = "R" + rt_naked; // Full, printable rt register name
                    
                    int of = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(of > 0) {remainder = of%10; decimal=decimal+(remainder*base); base=base*2; of=of/10;} // Convert of to decimal format
                    string of_naked = to_string(4*decimal);
                    string of_printable = "#" + of_naked; // Printable offset value
                    
                    decodedOutput = "BEQ " + rs_printable + ", " + rt_printable + ", " + of_printable;
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    Node* tempNode = new Node(currentInstructionNumber, "BEQ", "", "", of_naked, "", rt_naked, "", rs_naked, nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;

                } else if(opcode == "0100") { // BGTZ instruction
                    
                    operation = "BGTZ";
                    
                    int rs = stoi(currentLineOrInstruction.substr(6,5)); // Get the rs register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(rs > 0) {remainder = rs%10; decimal=decimal+(remainder*base); base=base*2; rs=rs/10;} // Convert rs to decimal format
                    string rs_naked = to_string(decimal);
                    string rs_printable = "R" + rs_naked; // Full, printable rs register name
                    
                    int of = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(of > 0) {remainder = of%10; decimal=decimal+(remainder*base); base=base*2; of=of/10;} // Convert of to decimal format
                    string of_naked = to_string(4*decimal);
                    string of_printable = "#" + of_naked; // Printable offset value
                    
                    decodedOutput = "BGTZ " + rs_printable + ", " + of_printable;
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    Node* tempNode = new Node(currentInstructionNumber, "BGTZ", "", "", of_naked, "", "", "", rs_naked, nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                } else if(opcode == "0101") { // BREAK instruction

                    operation = "BREAK";
                    
                    decodedOutput = "BREAK";
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    Node* tempNode = new Node(currentInstructionNumber, "BREAK", "", "", "", "", "", "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                } else if(opcode == "0110") { // SW instruction

                    operation = "SW";
                    
                    int br = stoi(currentLineOrInstruction.substr(6,5)); // Get the base register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(br > 0) {remainder = br%10; decimal=decimal+(remainder*base); base=base*2; br=br/10;} // Convert br to decimal format
                    string br_naked = to_string(decimal);
                    string br_printable = "R" + br_naked; // Full, printable rs register name
                    
                    int rt = stoi(currentLineOrInstruction.substr(11,5)); // Get the rt register in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(rt > 0) {remainder = rt%10; decimal=decimal+(remainder*base); base=base*2; rt=rt/10;} // Convert rt to decimal format
                    string rt_naked = to_string(decimal);
                    string rt_printable = "R" + rt_naked; // Full, printable rt register name
                    
                    int of = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(of > 0) {remainder = of%10; decimal=decimal+(remainder*base); base=base*2; of=of/10;} // Convert of to decimal format
                    string of_naked = to_string(decimal);
                    string of_printable = of_naked; // Printable offset value
                    
                    decodedOutput = "SW " + rt_printable + ", " + of_printable + "(" + br_printable + ")";
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    Node* tempNode = new Node(currentInstructionNumber, "SW", br_naked, "", of_naked, "", rt_naked, "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                } else if(opcode == "0111") { // LW instruction

                    operation = "LW";
                    
                    int br = stoi(currentLineOrInstruction.substr(6,5)); // Get the base register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(br > 0) {remainder = br%10; decimal=decimal+(remainder*base); base=base*2; br=br/10;} // Convert br to decimal format
                    string br_naked = to_string(decimal);
                    string br_printable = "R" + br_naked; // Full, printable rs register name
                    
                    int rt = stoi(currentLineOrInstruction.substr(11,5)); // Get the rt register in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(rt > 0) {remainder = rt%10; decimal=decimal+(remainder*base); base=base*2; rt=rt/10;} // Convert rt to decimal format
                    string rt_naked = to_string(decimal);
                    string rt_printable = "R" + rt_naked; // Full, printable rt register name
                    
                    int of = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(of > 0) {remainder = of%10; decimal=decimal+(remainder*base); base=base*2; of=of/10;} // Convert of to decimal format
                    string of_naked = to_string(decimal);
                    string of_printable = of_naked; // Printable offset value
                    
                    decodedOutput = "LW " + rt_printable + ", " + of_printable + "(" + br_printable + ")";
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    Node* tempNode = new Node(currentInstructionNumber, "LW", br_naked, "", of_naked, "", rt_naked, "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                }

            } else if(categoryNumber == "01") { // The instruction is a category 2
                
                opcode = currentLineOrInstruction.substr(12,4); // Get the current instruction's opcode
                
                if(opcode == "0000") {
                    operation = "ADD";
                } else if(opcode == "0001") {
                    operation = "SUB";
                } else if(opcode == "0010") {
                    operation = "MUL";
                } else if(opcode == "0011") {
                    operation = "AND";
                } else if(opcode == "0100") {
                    operation = "OR";
                } else if(opcode == "0101") {
                    operation = "XOR";
                } else if(opcode == "0110") {
                    operation = "NOR";
                }
                
                int rs = stoi(currentLineOrInstruction.substr(2,5)); // rs register number in binary format
                int decimal = 0; // Initialize decimal number
                int remainder, base = 1; // Initialize remainder and base
                while(rs > 0) {remainder = rs%10; decimal=decimal+(remainder*base); base=base*2; rs=rs/10;} // Convert rs to decimal format
                string rs_naked = to_string(decimal);
                string rs_printable = "R" + rs_naked; // Full, printable rs register name
                
                int rt = stoi(currentLineOrInstruction.substr(7,5)); // rt register number in binary format
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(rt > 0) {remainder = rt%10; decimal=decimal+(remainder*base); base=base*2; rt=rt/10;} // Convert rt to decimal format
                string rt_naked = to_string(decimal);
                string rt_printable = "R" + rt_naked; // Full, printable rt register name
            
                int rd = stoi(currentLineOrInstruction.substr(16,5)); // rd register number in binary format
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(rd > 0) {remainder = rd%10; decimal=decimal+(remainder*base); base=base*2; rd=rd/10;} // Convert rd to decimal format
                string rd_naked = to_string(decimal);
                string rd_printable = "R" + rd_naked; // Full, printable rd register name
                
                decodedOutput = operation + " " + rd_printable + ", " + rs_printable + ", " + rt_printable;
                output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                
                Node* tempNode = new Node(currentInstructionNumber, operation, "", "", "", "", rt_naked, rd_naked, rs_naked, nullptr);
                if(currentNode != nullptr) {
                    while(currentNode->next != nullptr) {
                        currentNode = currentNode->next;
                    }
                }
                currentNode->next = tempNode;
                currentNode = currentNode->next;
                
            } else if(categoryNumber == "10") { // The instruction is a category 3
                
                opcode = currentLineOrInstruction.substr(12,4); // Get the current instruction's opcode
                
                if(opcode == "0000") {
                    operation = "ADDI";
                } else if(opcode == "0001") {
                    operation = "ANDI";
                } else if(opcode == "0010") {
                    operation = "ORI";
                } else if(opcode == "0011") {
                    operation = "XORI";
                }
                
                int rt = stoi(currentLineOrInstruction.substr(7,5)); // rt register number in binary format
                //cout << "cat3, rt: " << currentLineOrInstruction.substr(7,5) << ". int form: " << stoi(currentLineOrInstruction.substr(7,5)) << endl;
                int decimal = 0; // Initialize decimal number
                int remainder, base = 1; // Initialize remainder and base
                while(rt > 0) {remainder = rt%10; decimal=decimal+(remainder*base); base=base*2; rt=rt/10;} // Convert rt to decimal format
                string rt_naked = to_string(decimal);
                string rt_printable = "R" + rt_naked; // Full, printable rt register name
                
                int rs = stoi(currentLineOrInstruction.substr(2,5)); // rs register number in binary format
                //cout << "cat3, rs: " << currentLineOrInstruction.substr(2,5) << endl;
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(rs > 0) {remainder = rs%10; decimal=decimal+(remainder*base); base=base*2; rs=rs/10;} // Convert rs to decimal format
                string rs_naked = to_string(decimal);
                string rs_printable = "R" + rs_naked; // Full, printable rs register name
                
                int iv = stoi(currentLineOrInstruction.substr(16,16)); // immediate value in binary format
                //cout << "cat3, iv: " << currentLineOrInstruction.substr(16,16) << endl;
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(iv > 0) {remainder = iv%10; decimal=decimal+(remainder*base); base=base*2; iv=iv/10;} // Convert rd to decimal format
                string iv_naked = to_string(decimal);
                string iv_printable = "#" + iv_naked; // Printable immediate value
                
                decodedOutput = operation + " " + rt_printable + ", " + rs_printable + ", " + iv_printable;
                output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                
                Node* tempNode = new Node(currentInstructionNumber, operation, "", "", "", iv_naked, rt_naked, "", rs_naked, nullptr);
                if(currentNode != nullptr) {
                    while(currentNode->next != nullptr) {
                        currentNode = currentNode->next;
                    }
                }
                currentNode->next = tempNode;
                currentNode = currentNode->next;
                
            }

            previous_operation = operation;
            
        } else { // The previous operation was BREAK, so we are now reading in the registers' values
            
            signed int registerValue;
            
            // Convert two's complement string to decimal

            if(currentLineOrInstruction.substr(0,1) == "1") { // If the number is negative
                
                string bvs = currentLineOrInstruction.substr(1,31); // Get the two's complement binary value of the number
                
                // Swap the zeros and ones
                replace(bvs.begin(), bvs.end(), '0', 'x');
                replace(bvs.begin(), bvs.end(), '1', '0');
                replace(bvs.begin(), bvs.end(), 'x', '1');
                
                int bv = stoi(bvs) + 1; // Convert the binary string to an integer (we add one because the two's complement was negative)
                
                int decimal = 0;
                int remainder, base = 1;
                while(bv > 0) {remainder = bv%10; decimal=decimal+(remainder*base); base=base*2; bv=bv/10;} // Convert the binary value to its decimal value
                registerValue = decimal*(-1);
                
            } else { // The number is positive
                
                string bvs = currentLineOrInstruction.substr(1,31); // Get the binary value of the number
                
                int bv = stoi(bvs); // Conver the binary string to an integer
                
                int decimal = 0;
                int remainder, base = 1;
                while(bv > 0) {remainder = bv%10; decimal=decimal+(remainder*base); base=base*2; bv=bv/10;} // Convert the binary value to its decimal value
                
                registerValue = decimal;
                
            }
            
            registerValues.push_back(registerValue); // Store the just-computed register value into the registerValues vector for later access
            
            output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + to_string(registerValue) + "\n");
            
        }
        
        decodedInstructions.push_back(decodedOutput); // Save our decoded instructions to the decodedInstructions vector
        
        currentInstructionNumber = currentInstructionNumber + 4; // Increment the instruction number
        
    }
    
    disassemblyFile << output; // Write to the dissasembly.txt file
    
    ofstream simFile; // A variable to hold the simulation write-to file
    simFile.open("simulation.txt"); // Open the simulation write-to file
    
    // Find which instruction number the register values start (it will be located in the inNu variable after we find it)
    vector<string> decodedInstructionsCopy = decodedInstructions;
    int inNu = 128; // Instruction number of where the register values start
    string previousInstructionString = "";
    for(vector<string>::iterator iter = decodedInstructions.begin(); iter != decodedInstructions.end() && previousInstructionString != "BREAK"; iter++) {
        previousInstructionString = *iter;
        inNu = inNu + 4;
    }
    
    vector<int> modRegisterValues; // Vector of register values whose contents will be modified as we proceed through the instructions
    
    // Fill the modRegistersValues vector with 0's for every register that exists
    for(int i=0; i<=31; i++) {
        modRegisterValues.push_back(0);
    }
    
    // Set the starting node (instruction)
    currentNode = root->next;
    
    /*
     The loop contained in the if-statement below does the following:
        1. Reads each instruction sequentially
        2. Retrieves the necessary data for the specified operation
        3. Performs the specified operation
        4. Saves the operation's results
        5. Writes (prints) the process to simulation.txt
    */
    if(currentNode->next != nullptr) {
        int cycle = 1;
        while(currentNode != nullptr) {
            
            string currentOperation = currentNode->operation; // Get the current instruction's operation
            
            int currentInstructionNumber = currentNode->instructionNumber; // Get the current instruction number
            string output = "Cycle:" + to_string(cycle) + "\t" + to_string(currentInstructionNumber) + "\t"; // Create the output string
            
            if(currentOperation == "J") {
            
                string instructionLocation = currentNode->location;
                int iil = stoi(instructionLocation); // Instruction location in integer form
                
                Node* startingNode = root;
                while(startingNode->next->instructionNumber != iil) { // Find the jump location
                    startingNode = startingNode->next;
                }
                currentNode = startingNode; // Set the current node to the jump location
                
                output.append("J #" + instructionLocation + "\n\n");
                
            } else if(currentOperation == "BEQ") {
                
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rtReg = currentNode->rt;
                int irt = modRegisterValues[stoi(rtReg)]; // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = modRegisterValues[stoi(rsReg)]; // rs register in integer form
                
                int nextInstructionNumber = currentNode->instructionNumber + io; // Get the next instruction number (to be used if rt == rs)
                
                if(irt == irs) {
                    Node* startingNode = root;
                    while(startingNode->next->instructionNumber != nextInstructionNumber) { // Find the instruction at the offsetted location
                        startingNode = startingNode->next;
                    }
                    currentNode = startingNode->next; // Set the current node to the offsetted location
                }
                
                output.append("BEQ R" + rsReg + ", R" + rtReg + ", #" + offset + "\n\n");

            } else if(currentOperation == "BGTZ") {
                
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rsReg = currentNode->rs;
                int irs = modRegisterValues[stoi(rsReg)]; // rs register in integer form
                
                int nextInstructionNumber = currentNode->instructionNumber + io; // Get the next instruction number (to be used if rs > 0)
                
                if(irs > 0) {
                    Node* startingNode = root;
                    while(startingNode->next->instructionNumber != nextInstructionNumber) { // Find the instruction at the offsetted location
                        startingNode = startingNode->next;
                    }
                    currentNode = startingNode->next; // Set the current node to the offsetted location
                }
                
                output.append("BGTZ R" + rsReg + ", #" + offset + "\n\n");
                
            } else if(currentOperation == "BREAK") {
                
                output.append("BREAK\n\n");
                
            } else if(currentOperation == "SW") {
                
                string baseAddress = currentNode->baseAddress;
                int iba = stoi(baseAddress); // base address in integer form
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                
                // Store the value from the local register specified into the memory register specified
                registerValues[(io - inNu + modRegisterValues[iba])/4] = modRegisterValues[irt];
                
                output.append("SW R" + rtReg + ", " + offset + "(R" + baseAddress + ")\n\n");
                
            } else if(currentOperation == "LW") {
                
                string baseAddress = currentNode->baseAddress;
                int iba = stoi(baseAddress); // base address in integer form
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                
                // Store the value from the memory register specified into the local register specified
                modRegisterValues[irt] = registerValues[(io - inNu + modRegisterValues[iba])/4];
                
                output.append("LW R" + rtReg + ", " + offset + "(R" + baseAddress + ")\n\n");
                
            } else if(currentOperation == "ADD") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irs] + modRegisterValues[irt]; // Compute the sum rs + rt
                
                output.append("ADD R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "SUB") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irs] - modRegisterValues[irt]; // Compute the difference rs - rt
                
                output.append("SUB R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "MUL") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]*modRegisterValues[irs]; // Compute the product of rt and rs
                
                output.append("MUL R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "AND") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]&modRegisterValues[irs]; // Compute the bitwise AND of registers rt and rs
                
                output.append("AND R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "OR") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]|modRegisterValues[irs]; // Compute the bitwise OR of registers rt and rs
                
                output.append("OR R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "XOR") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]^modRegisterValues[irs]; // Compute the bitwise XOR of registers rt and rs
                
                output.append("XOR R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "NOR") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = ~(modRegisterValues[irt]|modRegisterValues[irs]); // Compute the bitwise NOR of registers rt and rs
                
                output.append("NOR R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "ADDI") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs] + io;
                
                output.append("ADDI R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            } else if(currentOperation == "ANDI") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs]&io;
                
                output.append("AND R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            } else if(currentOperation == "ORI") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs]|io;
                
                output.append("ORI R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            } else if(currentOperation == "XORI") {
                
                string rtReg = currentNode->rt;
                int irt = stoi(rtReg); // rt register in integer form
                string rsReg = currentNode->rs;
                int irs = stoi(rsReg); // rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs]^io;
                
                output.append("XORI R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            }
            
            // Create the "Registers" section
            string registersSection = "Registers\n";
            registersSection.append("R00:\t" + to_string(modRegisterValues[0]) + "\t" + to_string(modRegisterValues[1]) + "\t");
            registersSection.append(to_string(modRegisterValues[2]) + "\t" + to_string(modRegisterValues[3]) + "\t");
            registersSection.append(to_string(modRegisterValues[4]) + "\t" + to_string(modRegisterValues[5]) + "\t");
            registersSection.append(to_string(modRegisterValues[6]) + "\t" + to_string(modRegisterValues[7]) + "\n");
            registersSection.append("R08:\t" + to_string(modRegisterValues[8]) + "\t" + to_string(modRegisterValues[9]) + "\t");
            registersSection.append(to_string(modRegisterValues[10]) + "\t" + to_string(modRegisterValues[11]) + "\t");
            registersSection.append(to_string(modRegisterValues[12]) + "\t" + to_string(modRegisterValues[13]) + "\t");
            registersSection.append(to_string(modRegisterValues[14]) + "\t" + to_string(modRegisterValues[15]) + "\n");
            registersSection.append("R16:\t" + to_string(modRegisterValues[16]) + "\t" + to_string(modRegisterValues[17]) + "\t");
            registersSection.append(to_string(modRegisterValues[18]) + "\t" + to_string(modRegisterValues[19]) + "\t");\
            registersSection.append(to_string(modRegisterValues[20]) + "\t" + to_string(modRegisterValues[21]) + "\t");
            registersSection.append(to_string(modRegisterValues[22]) + "\t" + to_string(modRegisterValues[23]) + "\n");
            registersSection.append("R24:\t" + to_string(modRegisterValues[24]) + "\t" + to_string(modRegisterValues[25]) + "\t");
            registersSection.append(to_string(modRegisterValues[26]) + "\t" + to_string(modRegisterValues[27]) + "\t");\
            registersSection.append(to_string(modRegisterValues[28]) + "\t" + to_string(modRegisterValues[29]) + "\t");
            registersSection.append(to_string(modRegisterValues[30]) + "\t" + to_string(modRegisterValues[31]) + "\n\n");
            
            // Create the "Data" section
            string dataSection = "Data\n";
            dataSection.append(to_string(inNu) + ":\t" + to_string(registerValues[0]) + "\t" + to_string(registerValues[1]) + "\t");
            dataSection.append(to_string(registerValues[2]) + "\t" + to_string(registerValues[3]) + "\t");
            dataSection.append(to_string(registerValues[4]) + "\t" + to_string(registerValues[5]) + "\t");
            dataSection.append(to_string(registerValues[6]) + "\t" + to_string(registerValues[7]) + "\n");
            dataSection.append(to_string(inNu + 32) + ":\t" + to_string(registerValues[8]) + "\t" + to_string(registerValues[9]) + "\t");
            dataSection.append(to_string(registerValues[10]) + "\t" + to_string(registerValues[11]) + "\t");
            dataSection.append(to_string(registerValues[12]) + "\t" + to_string(registerValues[13]) + "\t");
            if(currentOperation == "BREAK") {
                dataSection.append(to_string(registerValues[14]) + "\t" + to_string(registerValues[15]) + "\n");
            } else {
                dataSection.append(to_string(registerValues[14]) + "\t" + to_string(registerValues[15]) + "\n\n");
            }
            
            // Print the output
            simFile << "--------------------\n";
            simFile << output;
            simFile << registersSection;
            simFile << dataSection;

            currentNode = currentNode->next;
            ++cycle;
        }
    }
    
    return 0;
}
