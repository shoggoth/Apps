//
//  carrier.c
//  MGModule
//
//  Created by Richard Henry on 17/06/2021.
//

#include "carrier.h"
#include "mg.xpm"
#include "lz.xpm"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

typedef enum { false, true } bool;

typedef enum
{
    MODLOAD_WORKING,
    MODLOAD_SUCCESS,
    MODLOAD_NODIR,
    MODLOAD_NOMODS
} ModuleLoadErrors;

struct carmod
{
    char file[256];
    char name[256];
    char version[256];
    char copyright[256];
    int layer;
    int encrypt;
    int syntax;
    int phase;
};

extern void scan(void);
extern int parse_mod(char *path, int num);
extern void sort_mods(void);
extern void add_mods_to_clist(void);
extern void context_name(char *text, int context);
extern void source_name(char *text, int source);
extern void diag_display(int id, char *msg, int wut);
extern void check_for_mg_home_dir(void);
extern void time_string(char *text);

static int load_mods(void);
static void append_log(char *text, int car, int context, int source, char *infotext, int deciphered, int mode);

static struct carmod    module[20];
static int              mod_num = 0;
static char             car_path[2048];
static bool             state_auto = true;

void init_mods(const char *mg_path) {
    
    sprintf(car_path, "%s/carriers/", mg_path);

    switch (load_mods()) {
            
        case MODLOAD_NODIR:
            append_log("ERROR:\n"
                       "Couldn't find a carrier module dir\n"
                       "in the default locations. Specify\n"
                       "one using the command line option:\n"
                       "-carrier_path",
                       0, 0, 0, "No Info", 0, 0);
            break;
            
        case MODLOAD_NOMODS:
            append_log("ERROR:\n"
                       "You have no carrier modules or\n"
                       "you installed them improperly!\n"
                       "Please consult the documentation for help.",
                       0, 0, 0, "No Info", 0, 0);
            break;
            
        case MODLOAD_WORKING:
            append_log("ERROR:\n"
                       "Unknown carrier module error\n"
                       "Please consult the documentation for help.",
                       0, 0, 0, "No Info", 0, 0);
            break;
            
        default:
            scan();
    }
}

int load_mods(void) {
    
    int             success = MODLOAD_WORKING, try = 0;
    char            path[256];
    DIR             *dp;
    struct dirent   *dirp;
    
    mod_num = 0;
    
    while (try < 4)
    {
        if ((dp = opendir(car_path)))
        {
            while ((dirp = readdir(dp)) != NULL)
            {
                if ( (strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0) )
                    continue;
                if (strcmp(&dirp->d_name[strlen(dirp->d_name)-8], ".carrier") == 0)
                {
                    strcpy(path, car_path);
                    strcat(path, dirp->d_name);
                    if (parse_mod(path, mod_num))
                    {
                        sort_mods();
                        mod_num++;
                    }
                }
            }
            closedir(dp);
            
            if (mod_num > 0)
            {
                add_mods_to_clist();
                success = MODLOAD_SUCCESS;
                sprintf(path, "%i Modules Installed.", mod_num);
                diag_display(1, path, 0);
                diag_display(3, "Module Path:", 0);
                diag_display(4, car_path, 0);
            }
            else success = MODLOAD_NOMODS;
            
            break;
        }
        else
        {
            switch (try)
            {
                case 0:
                    strcpy(car_path, "./carriers/");
                    break;
                case 1:
                    sprintf(car_path, "%s/carriers/", getenv("HOME"));
                    break;
                case 2:
                    sprintf(car_path, "%s/Mindguard Neo.app/carriers/", getenv("HOME"));
                    break;
                default:
                    break;
            }
            try++;
            if (try > 3)
            {
                success = MODLOAD_NODIR;
                try++;
            }
        }
    }
    
    return success;
}

void append_log(char *text, int car, int context, int source, char *infotext, int deciphered, int mode) {
    
    FILE    *log = stdout;
    char    buffer[256], log_path[256];
    
    check_for_mg_home_dir();
    
    sprintf(log_path, "%s/MindGuard/mindguard.log", getenv("HOME"));
    time_string (buffer);
    fprintf(log, "---------------------------\nTime:     %s\n", buffer);
    strcpy (buffer, module[car].name);
    fprintf(log, "Carrier:  %s\n", buffer);
    if (((mode == 1 || mode == 2) && state_auto) || mode == 3)
    {
        if (deciphered)
        {
            strcpy (buffer, "");
            context_name (buffer, context);
            fprintf(log, "Context:  %s\n", buffer);
            
            fprintf(log, "Info:     %s\n", infotext);
            
            strcpy (buffer, "");
            source_name(buffer, source);
            fprintf(log, "Source:   %s\n", buffer);
            
            fprintf(log, "Contents:\n%s\n", text);
        }
        else
            fprintf(log, "Contents: Undecipherable\n");
    }
    fprintf(log, "Action:   ");
    switch (mode)
    {
        case 0:
            fprintf(log, "FAILURE! DANGER!!\n");
            break;
        case 1:
            fprintf(log, "SUCCESSFULLY JAMMED\n");
            break;
        case 2:
            fprintf(log, "SUCCESSFULLY SCRAMBLED\n");
            break;
        case 3:
            fprintf(log, "SCAN FILTERED\n");
            break;
    }
    return;
}
