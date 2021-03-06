//----------------------------------
//
//      sdcard.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <Arduino.h>
#include <Wire.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>

#include "variables.hpp"
#include "task.hpp"
#include "sdcard.hpp"
#include "tools.hpp"
#include "interpreteur.hpp"
#include "log.hpp"


#if defined(__AVR__)
  #define SD_CS    10
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define SD_CS     53
#elif defined(ESP8266)
  #define SD_CS    D8
#elif ESP32
  //#define SD_CS    5   //SS
  #define SD_CS    14
#endif 
//#define SD_CS    14

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
                    log("parametre <%c> inconnu\n", parametres[i]);
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
    //log("liste des fichiers du repertoire <%s>\n",directoryName);
    //log("avec comme parametres <%s>\n",parametres);
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
    //log("catFile =>  tentative affichage du fichier <%s>\n", filename);
    File directory = SD.open(task->sdCardParam.currentPathName);
    while (true) {
        File entry = directory.openNextFile();
        if (! entry) {
            log("ERREUR => le fichier <%s> est introuvable\n", filename);
            break;
        }
        strcpy(tmp2,entry.name());
        tmp1=&(tmp2[1]);
        //log("catFile =>  test fichier <%s>\n", tmp1);
        if (strcmp(tmp1, filename) == 0) {
            //log("catFile =>  affiche le fichier <%s>\n", entry.name());
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
//      copyFile
//
//-----------------------------------
void copyFile(taskStruct *task, char *parametres) {
    char pathFic1[100];
    char pathFic2[100];
    char carlu;
    char fichierOrigine[50];
    char fichierDestination[50];

    strcpy(pathFic1,parametres);
    strcpy(fichierOrigine, tools_string_cut(pathFic1,' ', 0));
    strcpy(pathFic1, task->sdCardParam.currentPathName);
    strcat(pathFic1, fichierOrigine);
    //strcpy(fichierOrigine,tmp2);
    log("copyFile => tentative copie du fichier <%s> \n", pathFic1);
    if (!SD.exists(pathFic1)){
        // le fichier d'origine n'existe pas
        log("le fichier %s n'existe pas\n", pathFic1);
        log("le fichier %s n'existe pas\n", fichierOrigine);
        return;
    } 
    File ficOrigine = SD.open(pathFic1, FILE_READ);
    File ficDest;
    strcpy(pathFic2,parametres);
    strcpy(fichierDestination, tools_string_cut(pathFic2,' ', 1));
    strcpy(pathFic2, task->sdCardParam.currentPathName);
    strcat(pathFic2, fichierDestination);
    //strcpy(fichierDestination,pathFic2);
    log("copyFile => verification si <%s> existe deja\n", pathFic2);
    if (SD.exists(pathFic2)){
        // le fichier destination existe deja ou porte le nom d'un repertoire
        ficDest = SD.open(pathFic2, FILE_WRITE);
        if (ficDest.isDirectory()){
            log("copyFile => <%s> est un repertoire\n", pathFic2);
            ficDest.close();
            // c'est un repertoire on essaie de copier dans le repertoire avec le nom du fichier d'origine
            strcat(pathFic2,"/");
            strcat(pathFic2,fichierOrigine);
            log("copyFile => verification si <%s> existe deja\n", pathFic2);
            if (SD.exists(pathFic2)){
                // le fichier existe deja dans le nouveau repertoire => KO
                log("le fichier %s existe deja\n", pathFic2);
                log("le fichier %s existe deja\n", fichierDestination);
                return;
            }
        } else {
            // le fichier existe deja dans le repertoire courant => KO
            log("le fichier %s existe deja\n", pathFic2);
            log("le fichier %s existe deja\n", fichierDestination);
            return;
        }
        ficDest.close();
    }  
    // on ouvre le fichier destination
    ficDest = SD.open(pathFic2, FILE_WRITE);
    // on effectue la copie
    log("copyFile => on copie <%s> vers <%s>\n", pathFic1, pathFic2);
    while (ficOrigine.available()){
        carlu = ficOrigine.read();
        ficDest.write(carlu);
        //Serial.print(carlu);
    }
    ficDest.close();
    //Serial.println();
    
    ficOrigine.close();
}

//-----------------------------------
//
//      eraseFile
//
//-----------------------------------
void eraseFile(taskStruct *task, char *parametres) {
    char *tmp1;
    char tmp2[100];
    char carlu;
    char fichierOrigine[50];

    strcpy(tmp2,parametres);
    strcpy(fichierOrigine, tools_string_cut(tmp2,' ', 0));
    strcpy(tmp2, task->sdCardParam.currentPathName);
    //log("copyFile => tentative copie du fichier <%s> du repertoire <%s>\n", fichierOrigine, tmp2);
    strcat(tmp2, fichierOrigine);
    //strcpy(fichierOrigine,tmp2);
    //log("copyFile => tentative copie du fichier <%s> \n", tmp2);
    if (SD.exists(tmp2)){
        if (!SD.remove(tmp2)){
            log("impossible d'effacer le fichier %s\n", fichierOrigine);
        }
    } else {
        log("le fichier %s n'existe pas\n", fichierOrigine);
    }
}

//-----------------------------------
//
//      moveFile
//
//-----------------------------------
void moveFile(taskStruct *task, char *parametres) {
    char *tmp1;
    char tmp2[100];
    char carlu;
    char nomOrigine[50];
    char nomDestination[50];

    strcpy(tmp2,parametres);
    strcpy(nomOrigine, tools_string_cut(tmp2,' ', 0));
    strcpy(tmp2,parametres);
    strcpy(nomDestination, tools_string_cut(tmp2,' ', 1));
    log("moveFile => tentative deplacement fichier <%s> vers le fichier <%s>\n", nomOrigine, nomDestination);
    // TODO
    
    log("moveFile => dev en cours\n");
}

//-----------------------------------
//
//      createDir
//
//-----------------------------------
void createDir(taskStruct *task, char *parametres) {
    char *tmp1;
    char tmp2[100];
    char carlu;
    char nomRepertoire[50];

    strcpy(tmp2,parametres);
    strcpy(nomRepertoire, tools_string_cut(tmp2,' ', 0));
    log("createDir => tentative creation du repertoire <%s>\n", nomRepertoire);
    strcpy(tmp2, task->sdCardParam.currentPathName);
    strcat(tmp2,"/");
    strcat(tmp2, nomRepertoire);
    log("createDir => tentative creation du repertoire <%s>\n", tmp2);
    if (SD.exists(tmp2)){
        log("ERREUR => ce nom existe deja <%s>\n", nomRepertoire);
    } else {
        log("createDir => on cree le repertroire\n");
        SD.mkdir(tmp2);
    }

    // TODO
    log("createDir => dev en cours\n");
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
    //log("sdcard_exec => extractFile Name de (%s)\n", path);
    char *tmp1;
    tmp1 = strchr(path, '/');
    while (tmp1 != nullptr){
        //log("sdcard_exec => extractFile Name tmp1 (%s)\n", tmp1);
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
    //log("sdcard_exec => pid(%d)\n", task->pid);
    if (strlen(task->name) > 7) {
        parametres = &task->name[7];
        //log("sdcard_exec => execution commande externe <%s>\n", parametres);
        if (strncmp(parametres, "ls", 2) == 0){
            if (strlen (parametres) > 3){
                parametres = &parametres[3];
                //log("sdcard_exec => ls de <%s> avec parametres <%s>\n", task->sdCardParam.currentDirectoryName, parametres); 
                printDirectory(task, task->sdCardParam.currentPathName,parametres,0);
            } else {
                //log("sdcard_exec => ls de <%s> sans parametres\n", task->sdCardParam.currentDirectoryName);
                printDirectory(task, task->sdCardParam.currentPathName,"", 0);
            }
            task->status=DEAD;
        } else if (strncmp(parametres, "cd", 2) == 0){
            //Serial.println("commande cd");
            if (strlen (parametres) > 3){
                char tmp1[100];
                parametres = &parametres[3];
                strcpy(tmp1,parametres);
                if (parametres[0] != '-'){
                    tools_string_cut(tmp1, ' ', 0);
                    //log("sdcard_exec => current dir passe de <%s> a <%s>\n", task->sdCardParam.currentDirectoryName, tmp1);
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
            //log("sdcard_exec => pwd = <%s> \n", getPwd(task));
            log("%s\n", getPwd(task));
            task->status=DEAD;
        } else if(strncmp(parametres, "cat", 3) == 0){
            parametres = &parametres[4];
            catFile(task, parametres);
            task->status=DEAD;
        } else if(strncmp(parametres, "cp", 2) == 0){
            parametres = &parametres[3];
            copyFile(task, parametres);
            task->status=DEAD;
        } else if(strncmp(parametres, "rm", 2) == 0){
            parametres = &parametres[3];
            eraseFile(task, parametres);
            task->status=DEAD;
        } else if(strncmp(parametres, "mv", 2) == 0){
            parametres = &parametres[3];
            moveFile(task, parametres);
            task->status=DEAD;
        } else if(strncmp(parametres, "mkdir", 5) == 0){
            parametres = &parametres[6];
            createDir(task, parametres);
            task->status=DEAD;
        } else {
            // test si un script existe avec ce nom
            //log("sdcard_exec => lancement d'un script externe = <%s> \n", parametres);
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
                //log("sdcard_exec => test du fichier <%s> \n", tmp1);
                if (strcmp(parametres,tmp1) == 0){
                    //log("sdcard_exec => execution du script <%s> \n", parametres);
                    sprintf(tmp1,"interpreteur %s", parametres); 
                    fileFound=true;
                    task_add(tmp1,task->priority, task->pid);
                    task->status=WAIT;
                    //interpreteur(task);
                }
            }
            if (!fileFound){
                log("sdcard_exec => le script <%s> n'a pas ??t?? trouv??\n", parametres);
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
    //log("sdcard_wakeup => pid(%d)\n", task->pid);
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
