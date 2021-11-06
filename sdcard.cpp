//----------------------------------
//
//      sdcard.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include "sdcard.hpp"
#include "tools.hpp"
#include "interpreteur.hpp"


#define SD_CS    14

File currentDirectory;
char currentDirectoryName[50];
char currentPathName[100];
File sdcardDataFile;
char sdcardValue[50];
bool sdcard_initialzed=false;

struct {
    bool recursif;
} param;

//-----------------------------------
//
//      sdcard_init
//
//-----------------------------------
void sdcard_init(taskStruct *task){
    if (!sdcard_initialzed) {
        Serial.println("+----------------------------------------+");
        Serial.println("|                                        |");
        Serial.println("|           Init SD Card                 |");
        Serial.println("|                                        |");
        Serial.println("+----------------------------------------+");
        if(!SD.begin(SD_CS)){
            Serial.println("Card Mount Failed");
        } else {
            Serial.println("SD Card mounted with success");
        }
        strcpy(currentDirectoryName,"/");
        strcpy(currentPathName,"/");
        sdcard_initialzed=true;
    }
    task->status = RUN;
}

void printDirectory(File dir, int numTabs);
//-----------------------------------
//
//      printDirectoryRecursive
//
//-----------------------------------
void printDirectoryRecursive(File dir, int numTabs) {
    while (true) {
        File entry =  dir.openNextFile();
        if (! entry) {
            // no more files
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

//-----------------------------------
//
//      reset_parametres
//
//-----------------------------------
void reset_parametres(){
    param.recursif=false;
}

//-----------------------------------
//
//      analyse_parametres
//
//-----------------------------------
void analyse_parametres(char *parametres){
    if (parametres[0] == '-'){
        for (int i = 1 ; i < strlen(parametres); i++){
            switch (parametres[i]){
                case 'r':
                    param.recursif=true;
                    break;
                default:
                    sprintf(printString, "parametre <%c> inconnu\n", parametres[i]);Serial.print(printString);
            }
        }
    }
}

//-----------------------------------
//
//      printDirectory
//
//-----------------------------------
void printDirectory(char *directoryName, char * parametres, int numTabs) {
    const char *tmp1;
    reset_parametres();
    sprintf(printString, "liste des fichiers du repertoire <%s>\n",directoryName);Serial.print(printString);
    sprintf(printString, "avec comme parametres <%s>\n",parametres);Serial.print(printString);
    analyse_parametres(parametres);
    File directory = SD.open(directoryName);
    while (true) {
        File entry =  directory.openNextFile();
        if (! entry) {
            // no more files
            break;
        }
        tmp1 = &(entry.name()[1]);
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(tmp1);
        if ( !entry.isDirectory()) {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.print(entry.size(), DEC);
        }
        Serial.println();
        entry.close();
    }
}

//-----------------------------------
//
//      getPwd
//
//-----------------------------------
char *getPwd(void){
    return currentPathName;
}


//-----------------------------------
//
//      extractFileName
//
//-----------------------------------
void extractFileName(char *path){
    //sprintf(printString, "sdcard_exec => extractFile Name de (%s)\n", path); Serial.print(printString);
    char *tmp1;
    tmp1 = strchr(path, '/');
    while (tmp1 != nullptr){
        //sprintf(printString, "sdcard_exec => extractFile Name tmp1 (%s)\n", tmp1); Serial.print(printString);
        strcpy(path, &tmp1[1]);
        tmp1 = strchr(path, '/');
    }
}
//-----------------------------------
//
//      sdcard_exec
//
//-----------------------------------
int sdcard_exec(taskStruct *task){
    char *parametres;
    //sprintf(printString, "sdcard_exec => pid(%d)\n", task->pid); Serial.print(printString);
    if (strlen(task->name) > 7) {
        parametres = &task->name[7];
        //sprintf(printString, "sdcard_exec => execution commande externe <%s>\n", parametres); Serial.print(printString);
        if (strncmp(parametres, "ls", 2) == 0){
            if (strlen (parametres) > 3){
                parametres = &parametres[3];
                sprintf(printString, "sdcard_exec => ls de <%s> avec parametres <%s>\n", currentDirectoryName, parametres); Serial.print(printString);
                printDirectory(currentPathName,parametres,0);
            } else {
                sprintf(printString, "sdcard_exec => ls de <%s> sans parametres\n", currentDirectoryName); Serial.print(printString);
                printDirectory(currentPathName,"",0);
            }
            task->status=DEAD;
        } else if (strncmp(parametres, "cd", 2) == 0){
            Serial.println("commande cd");
            if (strlen (parametres) > 3){
                char tmp1[100];
                parametres = &parametres[3];
                strcpy(tmp1,parametres);
                if (parametres[0] != '-'){
                    tools_string_cut(tmp1, ' ', 0);
                    sprintf(printString, "sdcard_exec => current dir passe de <%s> a <%s>\n", currentDirectoryName, tmp1); Serial.print(printString);
                    strcpy(currentDirectoryName,tmp1);
                    if (currentDirectoryName[0] == '/'){
                        strcpy(currentPathName, currentDirectoryName);
                    } else {
                        if (strlen(currentPathName) > 1){
                            strcat(currentPathName, "/");
                        }
                        strcat(currentPathName, currentDirectoryName);
                    }
                }
            }
            task->status=DEAD;
        } else if(strncmp(parametres, "pwd", 3) == 0){
            char tmp1[100];
            sprintf(printString, "sdcard_exec => pwd = <%s> \n", getPwd()); Serial.print(printString);
            task->status=DEAD;
        } else {
            // test si un script existe avec ce nom
            //sprintf(printString, "sdcard_exec => lancement d'un script externe = <%s> \n", parametres); Serial.print(printString);
            File directory = SD.open(currentDirectoryName);
            bool fileFound=false;
            while (true) {
                File entry =  directory.openNextFile();
                if (! entry) {
                    // no more files
                    break;
                }
                char tmp1[50];
                strcpy(tmp1, entry.name());
                extractFileName(tmp1);
                //sprintf(printString, "sdcard_exec => test du fichier <%s> \n", tmp1); Serial.print(printString);
                if (strcmp(parametres,tmp1) == 0){
                    sprintf(printString, "sdcard_exec => execution du script <%s> \n", parametres); Serial.print(printString);
                    fileFound=true;
                    // executer interpreteur sur e fichier
                    t_context newContext;
                    strcmp(newContext.fileName,parametres);
                    newContext.ptrFile=entry;
                    newContext.lineNumber=0;
                    interpreteur(task, &newContext);
                }
            }
            if (!fileFound){
                sprintf(printString, "sdcard_exec => le script <%s> n'a pas été trouvé\n", parametres); Serial.print(printString);
                task->status=DEAD;
            }
        }
    } else {
        task->status=DEAD;
    }
}


//-----------------------------------
//
//      sdcard_wait
//
//-----------------------------------
void sdcard_wait(taskStruct *task){
    //task->status = DEAD;
}

//-----------------------------------
//
//      sdcard_wakeup
//
//-----------------------------------
void sdcard_wakeup(taskStruct *task){
    sprintf(printString, "sdcard_wakeup => pid(%d)\n", task->pid); Serial.print(printString);
    task->status = RUN;
}


//-----------------------------------
//
//      sdcard_pwd
//
//-----------------------------------
char *sdcard_pwd(void){
    return currentDirectoryName;
}

//-----------------------------------
//
//      is_sdcard_initialized
//
//-----------------------------------
bool is_sdcard_initialized(void){
    return sdcard_initialzed;
}
