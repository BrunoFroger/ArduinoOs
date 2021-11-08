//----------------------------------
//
//      interpreteur.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"

#define ERR_ERROR   1
#define ERR_WARNING 2
#define ERR_INFO    3


//-----------------------------------
//
//      decomposeExpression
//
//-----------------------------------
void decomposeExpression(taskStruct *task, char *expression, char *operande1, char *operande2, char *operateur){
    strcpy(operande1,expression);
    char *ptr=strchr(operande1,operateur[0]);
    ptr[0]='\0';
    ptr=strchr(expression,operateur[0]);
    ptr+=strlen(operateur);
    strcpy(operande2,ptr);
    sprintf(printString, "interpreteur => decomposeExpression  => <%s> <%s> <%s>\n", operande1, operateur, operande2); Serial.print(printString);
}
//-----------------------------------
//
//      evalueTest
//
//-----------------------------------
bool evalueTest(taskStruct *task, char *expression){
    char operande1[50]="";
    char operande2[50]="";
    //sprintf(tmp, "evalueTest => evaluation de %s\n",expression); Serial.print(tmp);
    // decomposition de l'expression
    if (strstr(expression,"==") != nullptr){
        //Serial.println("evalueTest => test == detecte");
        decomposeExpression(task, expression, operande1, operande2, "==");
    } else if (strstr(expression,"!=") != nullptr){
        //Serial.println("evalueTest => test != detecte");
        decomposeExpression(task, expression, operande1, operande2, "!=");
    } else if (strstr(expression,"<") != nullptr){
        //Serial.println("evalueTest => test < detecte");
        decomposeExpression(task, expression, operande1, operande2, "<");
    } else if (strstr(expression,"<=") != nullptr){
        //Serial.println("evalueTest => test <= detecte");
        decomposeExpression(task, expression, operande1, operande2, "<=");
    } else if (strstr(expression,">") != nullptr){
        Serial.println("interpreteur => evalueTest => test > detecte");
        decomposeExpression(task, expression, operande1, operande2, ">=");
    } else if (strstr(expression,">=") != nullptr){
        Serial.println("interpreteur => evalueTest => test >= detecte");
        decomposeExpression(task, expression, operande1, operande2, ">");
    } else {
        sprintf(printString, "interpreteur => evalueTest => expression incomprise => <%s>\n",expression); Serial.print(printString);
    }
    return false;
}

//-----------------------------------
//
//      evalueOperation
//
//-----------------------------------
void evalueOperation(taskStruct *task, char *operation){
    sprintf(printString, "interpreteur => evalueOperation => evalue %s", operation); Serial.print(printString);
}

//-----------------------------------
//
//      afficheDonnee
//
//-----------------------------------
void afficheDonnee(taskStruct *task, char *donnee){
    char *tmp1;
    char firstChar = donnee[0];
    char lastChar = donnee[strlen(donnee)-1];
    //sprintf(printString,"firstChar=<%c>, lastChar=<%c>\n",firstChar, lastChar);Serial.print(printString);
    if (donnee[0] == '"'){
        if (donnee[strlen(donnee)-1] == '"'){
            tmp1=&donnee[1];
            tmp1[strlen(tmp1)-1] = '\0';
            Serial.println(tmp1);
        } else {
            sprintf(printString,"ligne %d => ERREUR : manque '\"' en fin de chaine\n");Serial.print(printString);
        } 
    } else if (donnee[0] == '$'){
        // TODO traitement d'(une variable)
    } else {
        sprintf(printString,"ligne %d => ERREUR : imossible afficher <%s>\n", donnee);Serial.print(printString);
    }
}
//-----------------------------------
//
//      evalueInstruction
//
//-----------------------------------
void evalueInstruction(taskStruct *task, char *instruction){
    char tmp2[100];
    char *tmp1;
    sprintf(printString, "interpreteur => evalueInstruction => evalue %s\n", instruction); Serial.print(printString);
    // supressrion du ';'
    char lastChar = instruction[strlen(instruction)-1];
    sprintf(printString, "interpreteur => evalueInstruction => lastChar =%c \n", lastChar); Serial.print(printString);
    if (lastChar == ';'){
        //Serial.println("suppression du ';'");
        instruction[strlen(instruction)-1]='\0';
    }
    if (strncmp(instruction,"echo", 4) == 0){
        //sprintf(tmp,"echo detecté\n");Serial.print(tmp);
        tmp1=&instruction[4];
        sprintf(printString, "evalueInstruction => valeur a afficher %s\n", tmp1); Serial.print(printString);
        afficheDonnee(task, tmp1);
    } else {
        // execution d'un autre script
        sprintf(printString, "interpreteur => evalueInstruction => script %s a lancer\n", instruction); Serial.print(printString);
        sprintf(tmp2, "sdcard %s", instruction);
        sprintf(printString, "interpreteur => evalueInstruction => task_add <%s>\n", tmp2); Serial.print(printString);
        task->status=WAIT;
        task_add(tmp2, task->currentPriority,task->pid);
    }
}

//-----------------------------------
//
//      interpreteLigne
//
//-----------------------------------
int interpreteLigne(taskStruct *task, char *ligne, int numLigne){
    sprintf(printString, "---------------------------\n"); Serial.print(printString);
    //sprintf(printString, "interpreteur %s => interpreteLigne => on interprete la ligne numero %d => <%s>\n", task->name, ligne); Serial.print(printString);
    sprintf(printString, "interpreteur => interpreteLigne => on interprete la ligne numero %d => <%s>\n", numLigne, ligne); Serial.print(printString);

    char expression[50];
    // tests syntaxe
    char lastCar=ligne[strlen(ligne)-1];
    if (strncmp(ligne, "if", 2) == 0){
        if (task->insideIf || task->insideElse){
            sprintf(printString,"ligne %d => ERREUR : if imbrique interdit\n", numLigne);Serial.print(printString);
            return -1;
        }
        task->insideIf=true;
        // traitement du debut de if
        sprintf(printString,"interpreteur => debut de if detecté\n");Serial.print(printString);
        // recuperation de l'expression de test
        if (ligne[2] != '('){
            sprintf(printString,"ligne %d => ERREUR : Manque '(' dans if\n", numLigne);Serial.print(printString);
        }
        int index=0;
        strcpy(expression,"");
        for (int i = 3 ; i < strlen(ligne) ; i++){
            if (ligne[i] != ')'){
                expression[index++] = ligne[i];
                expression[index] = '\0';
            }
        }
        sprintf(printString,"interpreteur => interpreteLigne => expression trouvee <%s>\n", expression);Serial.print(printString);
        evalueTest(task, expression);
    } else if (strncmp(ligne, "else", 4) == 0){
        // traitement du fin de if
        sprintf(printString,"interpreteur => else detecté\n");Serial.print(printString);
        if (!task->insideIf){
            sprintf(printString,"ligne %d => ERREUR : else sans if\n", numLigne);Serial.print(printString);
            task->insideElse=true;
        }
    } else if (strncmp(ligne, "fi", 2) == 0){
        // traitement du fin de if
        sprintf(printString,"interpreteur => fin de if detecté\n");Serial.print(printString);
        task->insideIf=false;
        task->insideElse=false;
    } else if (lastCar != ';'){
        sprintf(printString,"ligne %d => ERREUR : Manque ';' en fin de ligne <%c>\n", numLigne, lastCar);Serial.print(printString);
        return -1;
    } else {
        evalueInstruction(task, ligne);
    }
    return 0;
}

//-----------------------------------
//
//      analyseLigne
//
//-----------------------------------
int analyseLigne(taskStruct *task, char *ligne, int numLigne){
    char tmp1[200];
    int index;
    //sprintf(tmp, "---------------------------\n"); Serial.print(tmp);
    sprintf(printString, "interpreteur => analyseLigne <%s> => analyse de la ligne numero %d => <%s>\n", task->name, numLigne, ligne); Serial.print(printString);
    // suppression des commentaires

    index = strcspn(ligne, "#");
    if (index != strlen(ligne)){
        //sprintf(printString, "commentaires trouvee en pos %d\n", index); Serial.print(printString);
        if (index == 0) return 0;
        ligne[index]='\0';
    }
    //sprintf(printString, "suppression des commentaires => <%s>\n", ligne); Serial.print(printString);
    
    // remplacement des tabulations par des espaces
    //sprintf(printString, "taille de la ligne %d\n", strlen(ligne)); Serial.print(printString);
    while (true){
        index = strcspn(ligne, "\t");
        if (index < strlen(ligne)){
            //sprintf(printString, "tabulations trouvee en pos %d\n", index); Serial.print(printString);
            ligne[index] = ' ';
        }else {
            break;
        }
    }
    //sprintf(printString, "interpreteur => ligne apres suppression des tabulations <%s>\n", ligne); Serial.print(printString);
    // supression des blancs inutiles
    strcpy(tmp1,"");
    index=0;
    bool insideGuillemets=false;
    for (int i = 0 ; i < strlen(ligne) ; i++){
        if (!insideGuillemets){
            if (ligne[i] == '"'){
                // on entre dans des guillements on recopie le caractere et on continue
                tmp1[index++] = ligne[i];
                tmp1[index]='\0';
                insideGuillemets=true;
            } else if (ligne[i] != ' '){
                // caractere != ' ' on garde
                //sprintf(printString, "on garde le caractere <%c> en pos %d\n", ligne[i], index); Serial.print(printString);
                tmp1[index++] = ligne[i];
                tmp1[index]='\0';
            }
        } else {
            // on est a l'interieur des guillemets, on recopie sans controles
            tmp1[index++] = ligne[i];
            tmp1[index]='\0';
            //lastChar = ligne[i];
            if (ligne[i] == '"'){
                insideGuillemets=false;
            }
        }
    }
    strcpy(ligne, tmp1);
    //sprintf(printString, "interpreteur => ligne apres suppression des blancs <%s>\n", tmp1); Serial.print(printString);
    // suppression des lignes vides
    if (strlen(ligne) == 0) {
        //Serial.println("ligne vide => non traitee");
        return 0;
    }
    // reste ligne a traiter
    sprintf(printString, "interpreteur => analyseLigne => ligne numero %d a interpreter <%s>\n", numLigne, ligne); Serial.print(printString);
    int resultat = 0;
    //resultat = interpreteLigne(task, ligne, numLigne);
    resultat = interpreteLigne(task, ligne, numLigne);
    return resultat;
}
//-----------------------------------
//
//      interpreteur
//
//-----------------------------------
void interpreteur(taskStruct *task){
    int nbErreurs=0;
    sprintf(printString, "interpreteur => debut du script <%s>\n", task->name); Serial.print(printString);
    if (task->context.ptrFile == NULL){
        sprintf(printString, "interpreteur => impossible de lire le fichier <%s>\n", task->context.fileName); Serial.print(printString);
        return;
    }
    File script;
    script = task->context.ptrFile;
    char carlu;
    char ligne[100]="";
    int idx=0;
    int numLigne=0;
    int erreur = 0;
    task->insideIf=false;
    task->insideElse=false;
    while (script.available()){
        carlu = script.read();
        if (carlu == '\n'){
            // ligne a traiter
            erreur = analyseLigne(task, ligne, numLigne++);
            strcpy(ligne,"");
            idx=0;
            if ( erreur != 0){
                nbErreurs++;
            }
        } else {
            ligne[idx++]=carlu;
            ligne[idx]='\0';
        }
    }
    // traitement de la derniere ligne si elle ne se termine pas par un '\n'
    if (strlen(ligne) > 0){
        erreur = analyseLigne(task, ligne, numLigne++);
        if ( erreur != 0){
            nbErreurs++;
        }
    }

    if (task->insideElse || task->insideIf){
        sprintf(printString,"ligne %d => ERREUR : Manque fi pour if\n");Serial.print(printString);
        nbErreurs++;
    }

    if ( nbErreurs != 0){
        if (nbErreurs > 1){
            sprintf(printString, "interpreteur => %d erreurs se sont produites\n", nbErreurs);
        } else {
            sprintf(printString, "interpreteur => %d erreur s'est produite\n", nbErreurs);
        }
         Serial.print(printString);
    }
    task->status=DEAD;
    sprintf(printString, "interpreteur => fin\n"); Serial.print(printString);
}