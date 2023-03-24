//
//  main.cpp
//  ming
//
//  Created by Richard Henry on 24/03/2023.
//

extern void process_options(int argc, char * const argv[]);
extern void start();

int main(const int argc, char * const argv[]) {
   
   process_options(argc, argv);
   start();

   return 0;
}
