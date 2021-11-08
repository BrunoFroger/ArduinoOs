//----------------------------------
//
//      afficheur.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"
#include "sdcard.hpp"

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_HX8357.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 32
//Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS   15
#define TFT_DC   33
#define SD_CS    14

#define TFT_RST -1

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

#define _WHITE HX8357_WHITE
#define _BLACK HX8357_BLACK
#define _BLUE HX8357_BLUE
#define _GREEN HX8357_GREEN
#define _RED HX8357_RED
#define _YELLOW HX8357_YELLOW

// orientation de l'ecran
// 2  portrait (prise en bas)
// 0  portrait (prise en haut)
// 1  paysage (prise a gauche)
// 3  paysage (prise a gauche)
#define SCREEN_ORIENTATION 1

#define TEXT_ORIGIN 30 
#define LINE_SIZE   20
#define DECALAGE_COLONNE    200
#define DECALAGE_VALUE  150

#define AFFICHEUR_ENV   1
#define AFFICHEUR_PS    2
int afficheur_mode=AFFICHEUR_ENV;

int old_newPid=0;
int old_run_process=0;
int old_standardPriority=0;
int old_currentPriority=0;
char old_currentDirectory[50]="";

//-----------------------------------
//
//      setTextPos
//
//-----------------------------------
int setTextPos(int lig, int col){
    int ligne=TEXT_ORIGIN+(lig*LINE_SIZE);
    int colonne= col * DECALAGE_COLONNE;
    tft.setCursor(colonne,ligne);
}

//-----------------------------------
//
//      setValuePos
//
//-----------------------------------
int setValuePos(int lig, int col){
    int ligne = TEXT_ORIGIN + (lig * LINE_SIZE);
    int colonne= col * DECALAGE_COLONNE + DECALAGE_VALUE;
    tft.setCursor(colonne,ligne);
}

//-----------------------------------
//
//      affiche_titre
//
//-----------------------------------
void affiche_titre(void){
    sprintf(printString, "affiche_titre => debut\n"); Serial.print(printString);
    
    tft.setTextSize(3); 
    // affiche nouveau titre
    tft.setCursor(150,0);
    tft.setTextColor(_RED);
    tft.println("Arduino OS");
    /*
    switch(afficheur_mode){
        case AFFICHEUR_ENV:
            // affiche les textes des valeurs a afficher
            tft.setTextSize(2); 
            tft.setTextColor(_WHITE);
            setTextPos(0,0);
            tft.println("next pid  : ");
            setTextPos(1,0);
            tft.println("run queue : ");
            setTextPos(2,0);
            tft.println("std prio  : ");
            setTextPos(3,0);
            tft.println("curr prio : ");
            setTextPos(4,0);
            tft.println("curr dir  : ");
            break;
        case AFFICHEUR_PS:
            // affiche la liste des process
            tft.setTextSize(2); 
            tft.setTextColor(_WHITE);
            setTextPos(0,0);
            sprintf(printString, "+------+-------\n"); Serial.print(printString);
            sprintf(printString, "|    nom   | pid | statu |\n"); Serial.print(printString);
            sprintf(printString, "+------+-------\n"); Serial.print(printString);
            break;
    }*/
    sprintf(printString, "affiche_titre => fin\n"); Serial.print(printString);
}

//-----------------------------------
//
//      afficheur_init
//
//-----------------------------------
void afficheur_init(taskStruct *task){

    Serial.println("+----------------------------------------+");
    Serial.println("|                                        |");
    Serial.println("|           Init display                 |");
    Serial.println("|                                        |");
    Serial.println("+----------------------------------------+");

    tft.begin();

    // read diagnostics (optional but can help debug problems)

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDCOLMOD);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDDIM);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDDSDR);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
    
    Serial.println(F("Benchmark                Time (microseconds)"));
    
    tft.setRotation(SCREEN_ORIENTATION);

    // efface l'ecran
    tft.fillScreen(HX8357_BLACK);

    tft.setTextSize(3); 
    // affiche nouveau titre
    tft.setCursor(150,0);
    tft.setTextColor(_RED);
    tft.println("Arduino OS");

    //affiche_titre();


    task->status = RUN;
    Serial.println("Init display => fin");
    delay(1000);
}

//-----------------------------------
//
//      affiche_valeur_int
//
//-----------------------------------
void affiche_valeur_int(int ligne, int colonne, int old_value, int new_value){
    setValuePos(ligne,colonne);
    tft.setTextColor(_BLACK);
    tft.println(old_value);
    setValuePos(ligne,colonne);
    tft.setTextColor(_WHITE);
    tft.println(new_value);
}

//-----------------------------------
//
//      affiche_valeur_chaine
//
//-----------------------------------
void affiche_valeur_chaine(int ligne, int colonne, char *old_value, char *new_value){
    setValuePos(ligne,colonne);
    tft.setTextColor(_BLACK);
    tft.println(old_value);
    setValuePos(ligne,colonne);
    tft.setTextColor(_WHITE);
    tft.println(new_value);
}

//-----------------------------------
//
//      afficheur_exec
//
//-----------------------------------
int afficheur_exec(taskStruct *task){
    //sprintf(printString, "afficheur_exec => pid(%d)\n", task->pid); Serial.print(printString);

    switch(afficheur_mode){
        case AFFICHEUR_ENV:
            if (old_newPid != newPid){
                affiche_valeur_int(0,0,old_newPid,newPid);
                old_newPid=newPid;
            }
            if (old_run_process != run_process){
                affiche_valeur_int(1,0,old_run_process,run_process);
                old_run_process=run_process;
            }
            if (old_standardPriority != standardPriority){
                affiche_valeur_int(2,0,old_standardPriority,standardPriority);
                old_standardPriority=standardPriority;
            }
            if (old_currentPriority != currentPriority){
                affiche_valeur_int(3,0,old_currentPriority,currentPriority);
                old_currentPriority=currentPriority;
            }
            if (strcmp(old_currentDirectory,sdcard_pwd(task)) != 0){
                affiche_valeur_chaine(4,0,old_currentDirectory,sdcard_pwd(task));
                strcpy(old_currentDirectory, sdcard_pwd(task));
            }
            break;
        case AFFICHEUR_PS:
            for (int i =0 ; i < NB_TASKS ; i++){
                if (taskTbl[i].status != FREE){
                    setTextPos(i + 1,0);
                    sprintf(printString,"| %20s "); Serial.print(printString);
                }
            }
            break;
    }

}


//-----------------------------------
//
//      afficheur_wait
//
//-----------------------------------
void afficheur_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      afficheur_wakeup
//
//-----------------------------------
void afficheur_wakeup(taskStruct *task){
    task->status = RUN;
}