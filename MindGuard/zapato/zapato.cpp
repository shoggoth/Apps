//
//  zapato.cpp
//  ming
//
//  Created by Richard Henry on 24/03/2023.
//

#include "carrier.h"

#include <iostream>
#include <string>
#include <getopt.h>

std::string carrier_dir = ".ming/carriers";

void start() {
   
   init_mods(carrier_dir.c_str());
}

void process_options(int argc, char * const argv[]) {
   
   for(;;) {
      switch(getopt(argc, argv, "ab:c:h")) {                // note the colon (:) to indicate that 'b' has a parameter and is not a switch
            
         case 'a':
            std::cout << "Switch 'a' specified" << std::endl;
            continue;
            
         case 'b':
            std::cout << "Parameter 'b' specified with the value " << optarg << std::endl;
            continue;
            
         case 'c':
            carrier_dir = optarg;
            continue;

         case '?':
         case 'h':
            std::cout << "🧘🏻‍♀️ ming - MindGuard v TBD Psychotronic protection." << std::endl;
            std::cout << "🧘🏻‍♀️ -c Path to carrier directory (" << carrier_dir << ")" << std::endl;
            std::cout << "🧘🏻‍♀️ https://zapatopi.net/mindguard" << std::endl;
            break;
            
         default:
            break;
      }
      
      break;
   }
}
