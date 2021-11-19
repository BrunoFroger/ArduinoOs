//----------------------------------
//
//      help.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>


#include "log.hpp"

//-----------------------------------
//
//      help
//
//-----------------------------------
void help(void){
    log("task_help => debut\n"); 
    log("liste des commandes : \n");
    log("- monitor\n"); 
    log("- ps\n");
    log("- env\n");
    log("- set\n");
    log("- kill\n");
    log("- nop\n");
    log("- priority\n");
    log("- afficheur\n");
    log("- sdcard\n"); 
    log("      - ls\n");
    log("      - cd ; change de repertoire\n");
    log("      - pwd : repertoire courant\n"); 
    log("      - cat : affiche contenu d'un fichier texte\n"); 
    log("- interpreteur : execute un script ligne a ligne\n"); 
    log("      - if [expression] [instructions] / else [instructions] / fi : instruction de test conditionnel\n");
    log("      - echo\"chaine\" : affiche chaine\n");
    log("      - vars : affiche la liste des variables d'une tache\n"); 
    log("- help\n");
    
}
