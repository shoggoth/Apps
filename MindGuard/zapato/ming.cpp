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

std::string carrier_dir = ".ming";

static void process_options(int argc, char * const argv[]);
static void start();

int main(const int argc, char * const argv[]) {
    
    process_options(argc, argv);
    
    return 0;
}

void start() {
    
    init_mods(carrier_dir.c_str());
}

void process() {
 
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.length() || line.rfind("//", 0) == 0) continue;
        std::cout << "Typed: " << line << std::endl;
    }
}

void process_options(int argc, char * const argv[]) {
    
    for(;;) {
        switch(getopt(argc, argv, "ab:c:dijhv")) {                // note the colon (:) to indicate that 'b' has a parameter and is not a switch
                
            case 'a':
                start();
                continue;
                
            case 'b':
                std::cout << "Parameter 'b' specified with the value " << optarg << std::endl;
                continue;
                
            case 'c':
                carrier_dir = optarg;
                continue;
                
            case 'd':
                std::cout << "Dump all modules mode set" << std::endl;
                continue;
                
            case 'i':
                std::cout << "Interactive mode set" << std::endl;
                process();
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
                break;
                
            default:
                break;
        }
        
        break;
    }
}
