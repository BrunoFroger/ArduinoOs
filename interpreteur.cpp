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
//      evalueVariable
//
//-----------------------------------
int evalueVariable(taskStruct *task, char *variable){
    t_variable *var;
    char *ptrVar = &(variable[1]);
    for (int i = 0 ; i < NB_VAR_PAR_TACHE ; i++){
        var = &(task->lstVar[i]);
        if (strcmp(ptrVar, var->key) == 0){
            return var->value;
        }
    }
    sprintf(printString, "ERREUR => la variable <%s> n'est pas initialisée\n", variable); Serial.print(printString);
    return 0;
}

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
    //sprintf(printString, "interpreteur => decomposeExpression  => <%s> <%s> <%s> (a coder)\n", operande1, operateur, operande2); Serial.print(printString);
    /*
    if (strcmp(operateur, "<") == 0){
        sprintf(printString, "interpreteur => decomposeExpression  => traitement operation < (a coder)\n"); Serial.print(printString);
    } else if (strcmp(operateur, "<=") == 0){
        sprintf(printString, "interpreteur => decomposeExpression  => traitement operation <= (a coder)\n"); Serial.print(printString);
    } else if (strcmp(operateur, ">") == 0){
        sprintf(printString, "interpreteur => decomposeExpression  => traitement operation > (a coder)\n"); Serial.print(printString);
    } else if (strcmp(operateur, ">=") == 0){
        sprintf(printString, "interpreteur => decomposeExpression  => traitement operation >= (a coder)\n"); Serial.print(printString);
    } else if (strcmp(operateur, "==") == 0){
        sprintf(printString, "interpreteur => decomposeExpression  => traitement operation == (a coder)\n"); Serial.print(printString);
    } else if (strcmp(operateur, "!=") == 0){
        sprintf(printString, "interpreteur => decomposeExpression  => traitement operation != (a coder)\n"); Serial.print(printString);
    } else {
        sprintf(printString, "interpreteur => decomposeExpression  => operation <%s> inconnue \n"); Serial.print(printString);
    }
    */
}

//-----------------------------------
//
//      evalueTest
//
//-----------------------------------
bool evalueTest(taskStruct *task, char *expression){
    char operande1[50]="";
    char operande2[50]="";
    int valeur1, valeur2;
    bool returnValue;
    //sprintf(tmp, "evalueTest => evaluation de %s\n",expression); Serial.print(tmp);
    // decomposition de l'expression
    if (strstr(expression,"==") != nullptr){
        //Serial.println("evalueTest => test == detecte");
        decomposeExpression(task, expression, operande1, operande2, "==");
        if (operande1[0] == '$'){
            //sprintf(printString, "evalueTest => operande1 (%s) est une variable\n",operande1); Serial.print(printString);
            valeur1=evalueVariable(task, operande1);
        } else {
            valeur1 = String(operande1).toInt();
        }
        if (operande2[0] == '$'){
            //sprintf(printString, "evalueTest => operande1 (%s) est une variable\n",operande2); Serial.print(printString);
            valeur2=evalueVariable(task, operande2);
        } else {
            valeur2 = String(operande2).toInt();
        }
        returnValue = (valeur1 == valeur2);
        //sprintf(printString, "evalueTest => resultat expression <%s> => %d\n",expression, returnValue); Serial.print(printString);
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
        //Serial.println("interpreteur => evalueTest => test > detecte");
        decomposeExpression(task, expression, operande1, operande2, ">=");
    } else if (strstr(expression,">=") != nullptr){
        //Serial.println("interpreteur => evalueTest => test >= detecte");
        decomposeExpression(task, expression, operande1, operande2, ">");
    } else {
        sprintf(printString, "interpreteur => evalueTest => expression incomprise => <%s>\n",expression); Serial.print(printString);
    }
    return returnValue;
}

//-----------------------------------
//
//      displayVariables
//
//-----------------------------------
void displayVariables(taskStruct *task){
    Serial.println("affichage des variables");
    for (int i = 0; i < NB_VAR_PAR_TACHE ; i++){
        t_variable *var = &(task->lstVar[i]);
        sprintf(printString,"<%s> : <%d>\n", var->key, var->value); Serial.print(printString);
    }
}

//-----------------------------------
//
//      evalueAffectation
//
//-----------------------------------
int evalueAffectation(taskStruct *task, char *operation){
    //sprintf(printString, "interpreteur => evalueAffectation => evalue <%s>\n", operation); Serial.print(printString);
    char variable[50]="";
    char valeur[50]="";
    t_variable *var;
    strcpy(variable, operation);
    char *ptr=strchr(variable,'=');
    ptr[0]='\0';
    //sprintf(printString, "interpreteur => evalueAffectation => variable <%s>\n", variable); Serial.print(printString);
    ptr=strchr(operation,'=')+1;
    strcpy(valeur,ptr);
    //sprintf(printString, "interpreteur => evalueAffectation => evalue <%s> = <%s>\n", variable, valeur); Serial.print(printString);
    for (int i = 0 ; i < NB_VAR_PAR_TACHE ; i++){
        var = &(task->lstVar[i]);
        if (strcmp(var->key, "") == 0){
            // cette variable est libre on l'initialise
            strcpy(var->key, variable);
            var->value = String(valeur).toInt();
            //sprintf(printString, "interpreteur => evalueAffectation => on affecte <%s> avec <%d>\n", var->key, var->value); Serial.print(printString);
            //displayVariables(task);
            return 0;
        } else {
            if (strcmp(var->key,variable) == 0){
                // la variable existe deja on met a jour la valeur
                var->value = String(valeur).toInt();
                //sprintf(printString, "interpreteur => evalueAffectation => on met a jour <%s> avec <%d>\n", var->key, var->value); Serial.print(printString);
                //displayVariables(task);
                return 0;
            }
        }
    }
    sprintf(printString, "Impossible de stocker la variable <%s> avec <%d> => table des variables pleine (%d)\n", var->key, var->value, NB_VAR_PAR_TACHE); Serial.print(printString);
                //displayVariables(task);
    return -1;
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
    //sprintf(printString,"interpreteur => afficheDonnee => <%s>\n",donnee);Serial.print(printString);
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
    //sprintf(printString, "interpreteur => evalueInstruction => evalue %s\n", instruction); Serial.print(printString);
    // supressrion du ';'
    char lastChar = instruction[strlen(instruction)-1];
    //sprintf(printString, "interpreteur => evalueInstruction => lastChar =%c \n", lastChar); Serial.print(printString);
    if (lastChar == ';'){
        //Serial.println("suppression du ';'");
        instruction[strlen(instruction)-1]='\0';
    }
    if (strncmp(instruction,"echo", 4) == 0){
        //sprintf(tmp,"echo detecté\n");Serial.print(tmp);
        tmp1=&instruction[4];
        //sprintf(printString, "evalueInstruction => valeur a afficher %s\n", tmp1); Serial.print(printString);
        afficheDonnee(task, tmp1);
    } else if (strstr(instruction, "=") != nullptr){
        // evaluation d'une affectation
        evalueAffectation(task, instruction);
    } else {
        // execution d'un autre script
        //sprintf(printString, "interpreteur => evalueInstruction => script %s a lancer\n", instruction); Serial.print(printString);
        sprintf(tmp2, "interpreteur %s", instruction);
        //sprintf(printString, "interpreteur => evalueInstruction => task_add <%s>\n", tmp2); Serial.print(printString);
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
    //sprintf(printString, "---------------------------\n"); Serial.print(printString);
    //sprintf(printString, "interpreteur %s => interpreteLigne => on interprete la ligne numero %d => <%s>\n", task->name, ligne); Serial.print(printString);
    //sprintf(printString, "interpreteur <%s> => interpreteLigne => on interprete la ligne numero %d => <%s>\n", task->name, numLigne, ligne); Serial.print(printString);

    char expression[50];
    // tests syntaxe
    char lastCar=ligne[strlen(ligne)-1];
    if (strncmp(ligne, "if", 2) == 0){
        if (task->context.insideIf || task->context.insideElse){
            sprintf(printString,"ligne %d => ERREUR : if imbrique interdit\n", numLigne);Serial.print(printString);
            return -1;
        }
        task->context.insideIf=true;
        // traitement du debut de if
        //sprintf(printString,"interpreteur => debut de if detecté\n");Serial.print(printString);
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
        //sprintf(printString,"interpreteur => interpreteLigne => expression trouvee <%s>\n", expression);Serial.print(printString);
        task->context.resultTest = evalueTest(task, expression);
    } else if (strncmp(ligne, "else", 4) == 0){
        //sprintf(printString,"interpreteur => else detecté\n");Serial.print(printString);
        task->context.insideElse=true;
        if (!task->context.insideIf){
            sprintf(printString,"ligne %d => ERREUR : else sans if\n", numLigne);Serial.print(printString);
        }
    } else if (strncmp(ligne, "fi", 2) == 0){
        // traitement du fin de if
        //sprintf(printString,"interpreteur => fin de if detecté\n");Serial.print(printString);
        task->context.insideIf=false;
        task->context.insideElse=false;
    } else if (lastCar != ';'){
        sprintf(printString,"ligne %d => ERREUR : Manque ';' en fin de ligne <%c>\n", numLigne, lastCar);Serial.print(printString);
        return -1;
    } else {
        //sprintf(printString,"interpreteur => on evalue l'instruction <%s>\n", ligne);Serial.print(printString);
        if (task->context.insideIf){
            if (task->context.resultTest){
                // test si on est dans le then ou dans le else
                //sprintf(printString,"interpreteur => test = true\n");Serial.print(printString);
                if (!task->context.insideElse){
                    // on est dans le then on execute
                    //sprintf(printString,"interpreteur => on evalue les instructions a l'interieur d'un then\n");Serial.print(printString);
                    evalueInstruction(task, ligne);
                } else {
                    //sprintf(printString,"interpreteur => on evalue pas les instructions a l'interieur d'un else\n");Serial.print(printString);
                }
            } else {
                // test si on est dans le then ou dans le else
                //sprintf(printString,"interpreteur => test = false\n");Serial.print(printString);
                if (task->context.insideElse){
                    // on est dans le then on execute
                    //sprintf(printString,"interpreteur => on evalue les instructions a l'interieur d'un else\n");Serial.print(printString);
                    evalueInstruction(task, ligne);
                } else {
                    //sprintf(printString,"interpreteur => on evalue pas les instructions a l'interieur d'un then\n");Serial.print(printString);
                }

            }
        } else {
            // on est pas dans un if on execute systematiquement
            //sprintf(printString,"interpreteur => on est pas dans un if\n");Serial.print(printString);
            evalueInstruction(task, ligne);
        }
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
    //sprintf(printString, "interpreteur <%s> => analyseLigne => analyse de la ligne numero %d => <%s>\n", task->name, numLigne, ligne); Serial.print(printString);
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
    //sprintf(printString, "interpreteur => analyseLigne => ligne numero %d a interpreter <%s>\n", numLigne, ligne); Serial.print(printString);
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
    //sprintf(printString, "interpreteur => debut du script <%s>\n", task->name); Serial.print(printString);
    File script;
    script = task->context.ptrFile;
    if (script == NULL){
        Serial.println("ERREUR => pointeur de fichier = NULL");
        return;
    }
    char carlu;
    char ligne[100]="";
    int idx=0;
    int erreur = 0;
    bool endOfLine=false;
    while (script.available() && !endOfLine){
        carlu = script.read();
        if (carlu == '\n'){
            // ligne a traiter
            erreur = analyseLigne(task, ligne, task->context.lineNumber++);
            strcpy(ligne,"");
            idx=0;
            if ( erreur != 0){
                task->context.nbErreurs++;
            }
            endOfLine=true;
        } else {
            ligne[idx++]=carlu;
            ligne[idx]='\0';
        }
    }
    if (!script.available()){
        // traitement de la derniere ligne si elle ne se termine pas par un '\n'
        if (strlen(ligne) > 0){
            erreur = analyseLigne(task, ligne, task->context.lineNumber++);
            if ( erreur != 0){
                task->context.nbErreurs++;
            }
            task->status=DEAD;
        }

        if (task->context.insideElse || task->context.insideIf){
            sprintf(printString,"ligne %d => ERREUR : Manque fi pour if\n");Serial.print(printString);
            task->context.nbErreurs++;
        }

        if ( task->context.nbErreurs != 0){
            if (task->context.nbErreurs > 1){
                sprintf(printString, "interpreteur => %d erreurs se sont produites\n", task->context.nbErreurs);
            } else {
                sprintf(printString, "interpreteur => %d erreur s'est produite\n", task->context.nbErreurs);
            }
            Serial.print(printString);
        }
        task->status=DEAD;
        //sprintf(printString, "interpreteur => fin\n"); Serial.print(printString);
    } else {
        //sprintf(printString, "interpreteur => on repasse la main au scheduler avant de traiter la ligne suivante\n"); Serial.print(printString);
    }

}



//-----------------------------------
//
//      interpreteur_init
//
//-----------------------------------
void interpreteur_init(taskStruct *task){
    char *parametres;
    //sprintf(printString, "interpreteur_init => ---------------------------\n"); Serial.print(printString);
    //sprintf(printString, "interpreteur_init => lancement <%s> pid(%d)\n", task->name, task->pid); Serial.print(printString);
    int cdeLength=strlen("interpreteur") + 1;
    if (strlen(task->name) > cdeLength) {
        parametres = &task->name[cdeLength];
        //sprintf(printString, "interpreteur_init => execution commande externe <%s>\n", parametres); Serial.print(printString);
    }
    strcpy(task->context.fileName, parametres);
    char fullName[150];
    strcpy(fullName, task->sdCardParam.currentPathName);
    strcat(fullName,"/");
    strcat(fullName, parametres);
    if (!SD.exists(fullName)){
        sprintf(printString, "ERREUR => impossible d'ouvrir le fichier <%s>\n", task->sdCardParam.currentDirectoryName); Serial.print(printString);
        task->status=DEAD;
        return;
    } 
    task->context.ptrFile=SD.open(fullName);
    // executer interpreteur sur ce fichier
    //strcmp(task->context.fileName,parametres);
    //task->context.ptrFile=entry;
    //sprintf(printString, "interpreteur_init => fichier <%s> trouve => on lance l'interpreteur\n", parametres); Serial.print(printString);
    task->context.lineNumber=0;
    task->context.nbErreurs=0;
    task->context.insideIf=false;
    task->context.insideElse=false;
    task->status = RUN;
}

//-----------------------------------
//
//      interpreteur_exec
//
//-----------------------------------
int interpreteur_exec(taskStruct *task){
    //sprintf(printString, "interpreteur_exec => debut\n"); Serial.print(printString);
    interpreteur(task);
    //task->status=DEAD;
}


//-----------------------------------
//
//      interpreteur_wait
//
//-----------------------------------
void interpreteur_wait(taskStruct *task){
    //task->status = DEAD;
}

//-----------------------------------
//
//      interpreteur_wakeup
//
//-----------------------------------
void interpreteur_wakeup(taskStruct *task){
    //sprintf(printString, "interpreteur_wakeup => pid(%d)\n", task->pid); Serial.print(printString);
    task->status = RUN;
}