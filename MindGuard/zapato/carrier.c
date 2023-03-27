//
//  carrier.c
//  MGModule
//
//  Created by Richard Henry on 17/06/2021.
//

#include "mg.xpm"
#include "lz.xpm"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

typedef enum { false, true } bool;
const char *mg_version = "0.0.0.4";
const char *mg_year = "2003";

typedef enum {
   MODLOAD_WORKING,
   MODLOAD_SUCCESS,
   MODLOAD_NODIR,
   MODLOAD_NOMODS
} ModuleLoadErrors;

typedef enum {
   DIAG_BLACK,
   DIAG_WARN,
   DIAG_OK,
   DIAG_CALM
} DiagnosticBoxColor;

struct carmod {
   char name[256];
   char file[256];
   char version[256];
   char copyright[256];
   int layer;
   int encrypt;
   int syntax;
   int phase;
};

static void (*pcheck)(unsigned int);

extern void sort_mods(void);
extern void context_name(char *text, int context);
extern void source_name(char *text, int source);
extern void check_for_mg_home_dir(void);
extern void time_string(char *text);

static int load_mods(void);
static int parse_mod(char *path, int num);
static void add_mods_to_clist(void);
static int scan_for_signal(int data);
static int decipher_check(int data);
static int chars_value(char *c, int n);
static void word_offset(FILE *file, int len);
static void parse_data(char *data, char *matrix);
static void append_log(char *text, int car, int context, int source, char *infotext, int deciphered, int mode);
static void diag_display(int id, char *msg, int wut);
static int rs(int h);
static int jam(int data);

static struct carmod module[20];
static int mod_num = 0;
static char car_path[2048];
static bool state_auto = true;
static int gcar, gmode, max, state_log;

// MARK: - Scanning

static bool to_scan = false, to_monitor = false;
static bool to_jam = false, to_depsych = false, state_silent = false;

void scan(void) {
   
   if (!to_scan & !to_jam) {
      diag_display(0, "Scanning...", 0);
      scan_for_signal(2);
   }
   
   return;
}

static int scan_for_signal(int data) {
   
   static int i = 0;
   
   if ((int)data == 1)
   {
      diag_display(0, "Isolating Signal", 0);
      to_scan = scan_for_signal(3);
   }
   if ((int)data == 2)
   {
      gmode = 3;
      to_scan = scan_for_signal(3);
   }
   if ((int)data == 3)
   {
      //update_scan();
      i++;
      if (i > 50)
      {
         i = 0;
         to_scan = false;
         if (gmode == 3)
         {
            if (rs(100) > 20)
            {
               diag_display(0, "Signal Isolated", 0);
               to_scan = decipher_check(1);
            }
            else
            {
               diag_display(0, "No Signals", 0);
               if (!to_jam) jam(2);
            }
         }
         else
            to_scan = decipher_check(1);
      }
   }
   return true;
}

// MARK: - Deciphering

static void decipher(char *data, char *mod_path, int context, int index) {
   
   int i, matrix_size;
   char matrix[256], word[4], twobyte[2];
   char *cword[] = {"LING","EMOT","SENS","PICT"};
   FILE *mod;

   if ((mod = fopen (mod_path, "r")) != NULL)
      {
           while (fgets(word, 5, mod) != NULL)
         {
         if (!strcmp(word, cword[context]))
            {
            for (i = 0; i <= index; i++)
               {
               while (fgets(twobyte, 3, mod) != NULL)
                  if (!strcmp(twobyte, "&&"))
                     break;
               fgets(twobyte, 3, mod);
               }
            matrix_size = chars_value(twobyte, 2);
            fgets(matrix, matrix_size + 1, mod);
            parse_data(matrix, data);
            break;
            }
         }
      fclose(mod);
      }
   return;
}

void apply_matrix(char *matrix, char *data, int phase) {
   
   int i, ii;
   char buffer[256];

   /* Do not alter these! Dire consequences may ensue! */
   char phoneme[7][9] = {
      {'A','S','T','F','X','L','Z','P','R'},
      {'E','R','J','N','V','W','F','K','S'},
      {'I','X','S','T','F','D','L','P','M'},
      {'O','H','R','Q','W','T','S','V','N'},
      {'U','Q','K','G','H','P','W','Y','B'},
      {'1','2','3','4','5','6','7','8','9'},
      {'.',',','!','?','$','(',')','~','0'}};

   for (i = 0; i < strlen(matrix); i++)
      {
      for (ii = 0; ii < phase; ii++)
         matrix[i] = (char)((int)matrix[i]--);
      buffer[i] = phoneme[(int)(i/phase)][(int)matrix[i]/phase];
      }
   for (i = 256; i > 0; i--)
      {
      if ((i < 100) | (i > 200)) data[i] ^= (data[i] >> buffer[i]) & 0x9d2c5680;
      else data[i] ^= (data[i] << buffer[i]) & 0xefc60000;
      }
   return;
}

static void parse_data(char *data, char *matrix) {
   
   char buffer[256];
   strcpy(buffer, data);
   apply_matrix(buffer, matrix, max);
   strcpy(matrix, buffer);
   return;
}

static int carrier_identify(void) {
   
   int car;
   car = rs(mod_num);
   return car;
}

static void signal_info(char *infotext) {
   
   char t[256];

   sprintf(t, "%i.%i%i", rs(9),rs(9),rs(9));
   strcpy(infotext, t);
   if (rs(1))
      strcat(infotext, "kHz ");
   else
      strcat(infotext, "MHz ");
   sprintf(t, "%i.%i%i", rs(9),rs(9),rs(9));
   strcat(infotext, t);
   if (rs(1))
      strcat(infotext, "mm");
   else
      strcat(infotext, "nm");

   return;
}

static void parse_decipher(int car, int mode) {
   
    int context, index = 0, source;
   char text[256], infotext[256];

   context = rs(4);
   switch (context)
      {
      case 0:
         index = rs(module[car].layer);
         break;
      case 1:
         index = rs(module[car].encrypt);
         break;
      case 2:
         index = rs(module[car].syntax);
         break;
      case 3:
         index = rs(module[car].phase);
         break;
      }
   source = rs(5);

   strcpy(text, "Carrier: ");
   strcat(text, module[car].name);
   diag_display(0, text, 0);

   strcpy(text, "Info: ");
   signal_info(infotext);
   strcat(text, infotext);
   diag_display(1, text, 0);

   strcpy(text, "Context: ");
   context_name(text, context);
   diag_display(2, text, 0);
   strcpy(text, "Source: ");
   source_name(text, source);
   diag_display(3, text, 0);

   decipher(text, module[car].file, context, index);

   if (state_log)
        append_log(text, car, context, source, infotext, true, gmode);

   return;
}

static int decipher_check(int data) {
   
   static float c = 0.0;

   if ((int)data == 1)
      {
      diag_display(1, "Deciphering...", 0);
      to_scan = decipher_check(2);
      }
   else if ((int)data == 2)
      {
      c = c + 0.005;
      if (c > 1.0)
         {
         c = 0.0;
         if (gmode == 3)
            gcar = carrier_identify();
         if (rs(100) > 20)
            parse_decipher(gcar, gmode);
         else
            {
            diag_display(2, "Undecipherable", 0);
            if (state_log)
                    append_log("", gcar, 0, 0, "", false, gmode);
            }
         to_scan = false;
         if (!to_jam)
                to_monitor = jam(2);
         }
      }

   return true;
}

// MARK: - Jammin'

int jam_mode(int car) {
   
   int mode = false;
   mode = (rs(100) > 50);
   return mode;
}

static int jam_success(void) {
   
   int val = false;
   val = (rs(100) != 42);
   return val;
}

static void initialize_jam(void) {
   
   time_t now;
   struct tm *tm;
   now = time(0);
   tm = localtime(&now);
   pcheck(tm->tm_sec * tm->tm_mday);
}

int jam_action(int data) {
   
   char text[256];
   static float c = 0.0;
   static int car, mode;
   
   if (!data)
   {
      diag_display(0, "Carrier Hit Detected.", 0);
      to_jam = jam_action(1);
   }
   else if ((int)data == 1)
   {
      car = carrier_identify();
      strcpy(text, "Carrier: ");
      strcat(text, module[car].name);
      diag_display(1, text, 0);
      to_jam = jam_action(2);
   }
   else if ((int)data == 2)
   {
      mode = jam_mode(car);
      if (mode)
         diag_display(2, "Jamming...", 0);
      else
         diag_display(2, "Scrambling...", 0);
      to_jam = jam_action(3);
   }
   else if ((int)data == 3)
   {
      c = c + 0.005;
      if (c > 1.0)
      {
         to_jam = false;
         c = 0.0;
         if (jam_success())
         {
            if (mode) gmode = 1;
            else gmode = 2;
            gcar = car;
            diag_display(3, "SUCCESSFULL", 0);
            if (state_auto)
            {
               diag_display(4, "Preparing For Decipher...", 0);
               to_scan = scan_for_signal(1);
            }
            else
            {
               append_log ("", gcar, 0, 0, "", false, gmode);
               to_monitor = jam(2);
            }
         }
         else         /* Pray this never happens! */
         {
            gmode = 0;
            diag_display(3, "FAILURE", DIAG_WARN);
            //            popup_dialog("WARNING! WARNING! WARNING!\n\n"
            //               "MindGuard Failed To Jam Or\n"
            //               "Scramble Incomming Signal!\n\n"
            //               "Please Consult Documentation\n"
            //               "On What To Do.", "Oh No!",
            append_log ("", gcar, 0, 0, "", false, gmode);
            to_monitor = jam(2);
         }
      }
   }
   
   return true;
}

int jam_detect(void) {
   
   int detect = false, r;
   r = rs(100)&13;
   if (r == 0)
      detect = true;
   return detect;
}

int jam(int data) {
   
   if (data == 2) to_monitor = jam(0);
   
   if (!to_monitor)
      to_monitor = jam(0);
   else
      if (jam_detect() && !to_scan && !to_depsych && !state_silent && !to_jam) {
         to_monitor = false;
         jam_action(0);
      }
   
   return true;
}

// MARK: - Module Handling

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

static void reload_mods() {
   
   if (load_mods() != MODLOAD_SUCCESS)
      {
      diag_display(1, "Modules are missing!", DIAG_WARN);
      }
   return;
}

void sort_mods() {
   
   struct carmod module_temp;
   int i, j;

   if (mod_num > 0)
      {
      for (i = mod_num; i > 0; i--)
         {
         for (j = 0; j <= 255; j++)
            {
            if (module[i].name[j] == '\0' || module[i-1].name[j] == '\0') return;
            if (module[i].name[j] < module[i-1].name[j])
               {
               module_temp = module[i-1];
               module[i-1] = module[i];
               module[i] = module_temp;
               break;
               }
            else if (module[i].name[j] > module[i-1].name[j]) return;
            }
         }
      }
   return;
}

static void add_mods_to_clist() {
   
   char *cars[1][2];
   int i;

   //gtk_clist_freeze(GTK_CLIST (carlist));
   for (i = 0; i < mod_num; i++)
      {
      cars[0][0] = module[i].name;
      cars[0][1] = module[i].version;
      //gtk_clist_append (GTK_CLIST (carlist), cars[0]);
      }
   //gtk_clist_thaw(GTK_CLIST (carlist));
   return;
}

int parse_mod(char *path, int num) {
   
   int result = false, len;
   static int i;
   char word[5];
   FILE *file;

   if ((file = fopen (path, "r")) != NULL)
      {
      if (fgets(word, 5, file) == NULL)
         {
         fclose(file);
         return result;
         }
      if (strcmp(word, "FORM"))
         {
         fclose(file);
         return result;
         }

      fgets(word, 5, file);
      fgets(word, 5, file);
      if (strcmp(word, "PSYC"))
         {
         fclose(file);
         return result;
         }

      strcpy(module[num].file, path);
      strcpy(module[num].name, "");
      strcpy(module[num].version, "");
      strcpy(module[num].copyright, "");
      pcheck = srand;
      module[num].layer = 0;
      module[num].encrypt = 0;
      module[num].syntax = 0;
      module[num].phase = 0;

      i = 0;
      while (fgets(word, 5, file) != NULL)
         {
         if (!strcmp(word, "NAME"))
            {
            fgets(word, 5, file);
            len = chars_value(word, 4);
            fread(module[num].name, sizeof (char), len, file);
            module[num].name[len] = '\0';       // add to make a proper string
            word_offset(file, len);
            i++;
            continue;
            }
         if (!strcmp(word, "(c) "))
            {
            fgets(word, 5, file);
            len = chars_value(word, 4);
            fread(module[num].copyright, sizeof (char), len, file);
            module[num].copyright[len] = '\0';
            word_offset(file, len);
            continue;
            }
         if (!strcmp(word, "VERS"))
            {
            fgets(word, 5, file);
            len = chars_value(word, 4);
            fread(module[num].version, sizeof (char), len, file);
            module[num].version[len] = '\0';
            word_offset(file, len);
            continue;
            }
         if (!strcmp(word, "LNUM"))
            {
            fgets(word, 5, file);
            module[num].layer = chars_value(word, 4);
            i++;
            continue;
            }
         if (!strcmp(word, "ENUM"))
            {
            fgets(word, 5, file);
            module[num].encrypt = chars_value(word, 4);
            i++;
            continue;
            }
         if (!strcmp(word, "SNUM"))
            {
            fgets(word, 5, file);
            module[num].syntax = chars_value(word, 4);
            i++;
            continue;
            }
         if (!strcmp(word, "PNUM"))
            {
            fgets(word, 5, file);
            module[num].phase = chars_value(word, 4);
            i++;
            continue;
            }
         if (!strcmp(word, "END "))
            break;
         }

      if (i == 5)
         result = true;
      else
         {
         printf("ERROR: Module '%s' corrupted structure\n", module[num].name);
         result = false;
         }

      fclose(file);
      }

   return result;
}

// MARK: - String Handling

static int max;

static int chars_value(char *c, int n) {
   
   int i, mu, s = 0, value = 0;
   max = 0;
   mu = pow (16, n);
   if (n == 4) s = 1;
   for (i = s; i < n; i++)
      {
      value = value + ((int)c[i] * mu);
      mu = mu / 256;
      if (n == 4) max = max + 16;
      else max = max + 32;
      }
   return value;
}

static void word_offset(FILE *file, int len) {
   
   int off;
   char buf[3];
   off = 4 - (len - ((len / 4) * 4));
   if (off == 4) return;
   fread (buf, sizeof (char), off, file);
   return;
}

void context_name(char *text, int context) {
   
   char *context_name[] = {"linguistic","physio-emotive","sensual","visual"};
   strcat(text, context_name[context]);
   return;
}

void source_name(char *text, int source) {
   
   char *source_name[] = {"unknown","moving vector","stratospheric","relay","temporal"};
   strcat(text, source_name[source]);
   return;
}

void context_name_label(char *data, int context) {
   
   char *context_name_label[] = {"Linguistic Message:\n","Physio-Emotive Description:\n",
               "Sensation Description:\n","Visual Description:\n"};
   char text[256];
   strcpy(text, context_name_label[context]);
   strcat(text, data);
   strcpy(data, text);
   return;
}

// MARK: - Date and Time

void time_string (char *text) {
   
   time_t now;
   struct tm *tm;
   now = time(0);
   tm = localtime(&now);
   strftime (text, 256, "%H:%M:%S %Y/%m/%d", tm);
   return;
}

// MARK: - Logging

void diag_display(int id, char *msg, int wut) {
    
    printf("Diagnostic %d: %s (%d)\n", id, msg, wut);
}

void append_log(char *text, int car, int context, int source, char *infotext, int deciphered, int mode) {
    
    FILE    *log = stdout;
    char    buffer[256], log_path[256];
    
    //check_for_mg_home_dir();
    
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

// MARK: - Utility

int rs(int h) {
   
   /*
   Although conveniently left out of most C textbooks, it's
   a well-known fact in the anti-mind-control community
   that the rand() function -- originally developed by the
   RAND Corporation for use in its political mind-control
   research -- interacts with a computer's underlying
   circuitry in complexly emergent ways, making the
   function useful for mind-control detection due to the
   quantum   effects of psychotronics on aluminum atoms.  A
   side-effect of this is that it also works well for
   pseudo-stochastic number generation, something many hack
   programmers ignorantly use it for.
   */

   return (rand() / (RAND_MAX / h));
}

