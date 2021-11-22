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
    log("- set var valeur : memorise valeur dans la variable var\n");
    log("- kill pid : tue la tache pid\n");
    log("- nop : tache qui ne fait rien (boucle infinie)\n");
    log("- priority p : change priorite courante avec la valeur p\n");
    log("- afficheur\n");
    log("- sdcard\n"); 
    log("      - ls\n");
    log("      - cd dir ; change de repertoire vers dir\n");
    log("      - pwd : repertoire courant\n"); 
    log("      - cat fic : affiche contenu d'un fichier texte fic\n"); 
    log("      - cp fic1 fic2 : affiche contenu d'un fichier texte\n"); 
    log("      - rm fichier :efface le fichier\n"); 
    log("- interpreteur : execute un script ligne a ligne\n"); 
    log("      - if [expression] [instructions] / else [instructions] / fi : instruction de test conditionnel\n");
    log("      - echo [\"chaine\" | $var] : affiche une chaine ou le contenue d'une variable\n");
    log("      - vars : affiche la liste des variables d'une tache\n"); 
    log("- help : affiche cet ecran\n");
    
}
