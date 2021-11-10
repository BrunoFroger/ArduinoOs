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

bool sdcard_initialzed=false;

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
        strcpy(task->sdCardParam.currentDirectoryName,"/");
        strcpy(task->sdCardParam.currentPathName,"/");
        sdcard_initialzed=true;
    }
    task->status = RUN;
}

void printDirectory(taskStruct *task, File dir, char * parametres, int numTabs);
//-----------------------------------
//
//      printDirectoryRecursive
//
//-----------------------------------
void printDirectoryRecursive(taskStruct *task, File dir, char * parametres, int numTabs) {
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
            printDirectory(task, entry, parametres, numTabs + 1);
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
void reset_parametres(taskStruct *task){
    task->sdCardParam.recursif=false;
}

//-----------------------------------
//
//      analyse_parametres
//
//-----------------------------------
void analyse_parametres(taskStruct *task, char *parametres){
    if (parametres[0] == '-'){
        for (int i = 1 ; i < strlen(parametres); i++){
            switch (parametres[i]){
                case 'r':
                    task->sdCardParam.recursif=true;
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
void printDirectory(taskStruct *task, char *directoryName, char * parametres, int numTabs) {
    const char *tmp1;
    reset_parametres(task);
    //sprintf(printString, "liste des fichiers du repertoire <%s>\n",directoryName);Serial.print(printString);
    //sprintf(printString, "avec comme parametres <%s>\n",parametres);Serial.print(printString);
    analyse_parametres(task, parametres);
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
//      catFile
//
//-----------------------------------
void catFile(taskStruct *task, char * filename) {
    char *tmp1;
    char tmp2[100];
    char carlu;
    //sprintf(printString,"catFile =>  tentative affichage du fichier <%s>\n", filename);Serial.print(printString);
    File directory = SD.open(task->sdCardParam.currentPathName);
    while (true) {
        File entry = directory.openNextFile();
        if (! entry) {
            sprintf(printString,"ERREUR => le fichier <%s> est introuvable\n", filename);Serial.print(printString);
            break;
        }
        strcpy(tmp2,entry.name());
        tmp1=&(tmp2[1]);
        //sprintf(printString,"catFile =>  test fichier <%s>\n", tmp1);Serial.print(printString);
        if (strcmp(tmp1, filename) == 0) {
            //sprintf(printString,"catFile =>  affiche le fichier <%s>\n", entry.name());Serial.print(printString);
            while (entry.available()){
                carlu = entry.read();
                Serial.print(carlu);
            }
            Serial.println();
            return;
        }
        entry.close();
    }
}

//-----------------------------------
//
//      getPwd
//
//-----------------------------------
char *getPwd(taskStruct *task){
    return task->sdCardParam.currentPathName;
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
                //sprintf(printString, "sdcard_exec => ls de <%s> avec parametres <%s>\n", task->sdCardParam.currentDirectoryName, parametres); Serial.print(printString);
                printDirectory(task, task->sdCardParam.currentPathName,parametres,0);
            } else {
                //sprintf(printString, "sdcard_exec => ls de <%s> sans parametres\n", task->sdCardParam.currentDirectoryName); Serial.print(printString);
                printDirectory(task, task->sdCardParam.currentPathName,"", 0);
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
                    //sprintf(printString, "sdcard_exec => current dir passe de <%s> a <%s>\n", task->sdCardParam.currentDirectoryName, tmp1); Serial.print(printString);
                    strcpy(task->sdCardParam.currentDirectoryName,tmp1);
                    if (task->sdCardParam.currentDirectoryName[0] == '/'){
                        strcpy(task->sdCardParam.currentPathName, task->sdCardParam.currentDirectoryName);
                    } else {
                        if (strlen(task->sdCardParam.currentPathName) > 1){
                            strcat(task->sdCardParam.currentPathName, "/");
                        }
                        strcat(task->sdCardParam.currentPathName, task->sdCardParam.currentDirectoryName);
                    }
                }
            }
            task->status=DEAD;
        } else if(strncmp(parametres, "pwd", 3) == 0){
            char tmp1[100];
            //sprintf(printString, "sdcard_exec => pwd = <%s> \n", getPwd(task)); Serial.print(printString);
            task->status=DEAD;
        } else if(strncmp(parametres, "cat", 3) == 0){
            char tmp1[100];
            parametres = &parametres[4];
            catFile(task, parametres);
            task->status=DEAD;
        } else {
            // test si un script existe avec ce nom
            //sprintf(printString, "sdcard_exec => lancement d'un script externe = <%s> \n", parametres); Serial.print(printString);
            File directory = SD.open(task->sdCardParam.currentDirectoryName);
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
                    //sprintf(printString, "sdcard_exec => execution du script <%s> \n", parametres); Serial.print(printString);
                    sprintf(tmp1,"interpreteur %s", parametres); Serial.print(printString);
                    fileFound=true;
                    task_add(tmp1,task->priority, task->pid);
                    task->status=WAIT;
                    //interpreteur(task);
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
    //task->status = RUN;
}

//-----------------------------------
//
//      sdcard_wakeup
//
//-----------------------------------
void sdcard_wakeup(taskStruct *task){
    //sprintf(printString, "sdcard_wakeup => pid(%d)\n", task->pid); Serial.print(printString);
    task->status = DEAD;
}


//-----------------------------------
//
//      sdcard_pwd
//
//-----------------------------------
char *sdcard_pwd(taskStruct *task){
    return getPwd(task);
}

//-----------------------------------
//
//      is_sdcard_initialized
//
//-----------------------------------
bool is_sdcard_initialized(void){
    return sdcard_initialzed;
}
