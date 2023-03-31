//
//  ming.cpp
//  ming
//
//  Created by Richard Henry on 24/03/2023.
//

#include "carrier.h"

#include <iostream>
#include <string>
#include <getopt.h>

enum mode { one_shot, interactive, continuous, dump };

std::string carrier_dir = ".ming";

static mode process_options(int argc, char * const argv[]);
static void process_command_line();
static void dump_carrier_messages();


int main(const int argc, char * const argv[]) {
    
    auto mode = process_options(argc, argv);
    
    switch (mode) {
        case dump:
            dump_carrier_messages();
            break;
            
        case interactive:
            process_command_line();
            break;
            
        default:
            std::cout << "mode " << mode << std::endl;
            break;
    }
    
    return 0;
}

static void dump_carrier_messages() {
    
    init_mods(carrier_dir.c_str());
}

static void process_command_line() {
 
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.length() || line.rfind("//", 0) == 0) continue;
        std::cout << "Typed: " << line << std::endl;
    }
}

mode process_options(int argc, char * const argv[]) {
    
    mode mode = one_shot;
    
    for(;;) {
        switch(getopt(argc, argv, "ab:c:dijhv")) {                // note the colon (:) to indicate that 'b' has a parameter and is not a switch
                
            case 'a':
                mode = continuous;
                continue;
                
            case 'b':
                std::cout << "Parameter 'b' specified with the value " << optarg << std::endl;
                continue;
                
            case 'c':
                carrier_dir = optarg;
                continue;
                
            case 'd':
                mode = dump;
                continue;
                
            case 'i':
                mode = interactive;
                continue;
                
            case 'j':
                std::cout << "JSON output mode set" << std::endl;
                continue;
                
            case '?':
            case 'h':
            case 'v':
                std::cout << "🧘🏻‍♀️ ming - MindGuard v" << mg_version << " Psychotronic protection." << std::endl;
                std::cout << "🧘🏻‍♀️ -c Path to carrier directory (" << carrier_dir << ")" << std::endl;
                std::cout << "🧘🏻‍♀️ -i Interactive (command) mode" << std::endl;
                std::cout << "🧘🏻‍♀️ https://zapatopi.net/mindguard " << mg_year << std::endl;
                continue;
                
            default:
                break;
        }
        break;
    }
    
    return mode;
}
