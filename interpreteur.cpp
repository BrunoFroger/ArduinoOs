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
#include "log.hpp"

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
    log("ERREUR => la variable <%s> n'est pas initialisée\n", variable);
    return 0;
}


//-----------------------------------
//
//      decomposeExpression
//
//-----------------------------------
void decomposeExpression(taskStruct *task, char *expression, char *operande1, char *operande2, char *operateur){
    // suppression des parentheses s'il y en a
    if ((expression[0] == '(') && (expression[strlen(expression)-1] == ')')) {
        strcpy(operande1,&expression[1]);
        operande1[strlen(operande1)-1] = '\0';
        strcpy(expression,operande1);
    }
    strcpy(operande1,expression);
    char *ptr=strchr(operande1,operateur[0]);
    ptr[0]='\0';
    ptr=strchr(expression,operateur[0]);
    ptr+=strlen(operateur);
    strcpy(operande2,ptr);
    //log("decomposeExpression => <%s> decomposé en <%s> <%s> <%s>\n", expression, operande1, operateur, operande2); 
}

//-----------------------------------
//
//      evalueExpression
//
//-----------------------------------
int evalueExpression(taskStruct *task, char *expression, char *resultat){
    char operande1[50];
    char operande2[50];
    char operateur[5];
    char *ptr1, *ptr2, *ptr3;
    nbTabs++;
    //log("evalueExpression => evaluation de <%s>\n", expression);
    // cherche s'il y a des parentheses
    //log("evalueExpression => expression[0]=%c,  expression[strlen(expression)]=%c \n", expression[0], expression[strlen(expression)-1]);
    char debut_exp[50], fin_exp[50], exp_a_evaluer[50];
    ptr1=strrchr(expression, '('); // derniere parenthese ouvrante de l'expression
    if (ptr1 != nullptr){
        int idx_debut = ptr1 - &expression[0];
        //int idx_zone=0;     // index de la chaine de ce qu'il y a dans la parenthese a traiter
        if (idx_debut <= 0){
            strcpy(debut_exp,"");
        } else {
            strncpy(debut_exp, expression, idx_debut);
        }
        //log("evalueExpression => chaine avant sous chaine entre parentheses <%s> \n", debut_exp);
        int j=0;
        exp_a_evaluer[j] = '\0';
        int i = idx_debut + 1;
        while (i <= strlen(expression)){
            if (expression[i] != ')'){
                //Serial.print(expression[i]);
                exp_a_evaluer[j++] = expression[i];
                exp_a_evaluer[j] = '\0';
            } else {
                // on a trouvé la parenthese fermante corrrespondante, c'est la fin de la sous chaine a evaluer
                break;
            }
            i++;
        }
        //log("evalueExpression => sous chaine entre parentheses a evaluer <%s> \n", exp_a_evaluer);
        int tmp_idx = idx_debut + j + 2;
        //log("evalueExpression => tmp_idx =  %d vs strlen(<%s>) = %d\n", tmp_idx, expression, strlen(expression));
        if (tmp_idx <= strlen(expression)){
            char *tmpExp = &expression[tmp_idx];
            //log("evalueExpression => tmpExp =  <%s>\n", tmpExp);
            strcpy(fin_exp,tmpExp);
        } else {
            strcpy(fin_exp,"");
            //strcpy(fin_exp,"");
        }
        //log("evalueExpression => chaine apres sous chaine entre parentheses <%s> \n", fin_exp);
        evalueExpression(task, exp_a_evaluer, resultat);
        strcat(debut_exp, resultat);
        strcat(debut_exp, fin_exp);
        strcpy(expression, debut_exp);
        strcpy(expression, debut_exp);
        //log("evalueExpression => resultat apres evaluation du contenu de la parenthese <%s> \n", expression);
        evalueExpression(task, expression, resultat);

    }
    strcpy(resultat, expression);
    //log("evalueExpression => evaluation de l'expression sans parentheses <%s>\n", resultat);
    // on est en presence d'une expression sans parentheses <operande_1> <operateur_1> <operande_2> <operateur_2> .... <operande_n> 
    // on decompose en plusieurs evaluations en fonction des priorites des operateurs
    // on commence par evaluer les '*'
    if (strchr(resultat, '+') != nullptr){
        // decomposition de chaque cote de cette operateur puis evaluation de ses deux operandes
        decomposeExpression(task, expression, operande1, operande2, "+");
        //log("evalueExpression => evaluation d'une addition <%s>\n", resultat);
        evalueExpression(task, operande1, resultat);
        strcpy(operande1, resultat);
        //log("evalueExpression => evaluation de l'operande 2 <%s>\n", operande2);
        evalueExpression(task, operande2, resultat);
        strcpy(operande2, resultat);
        int op1 = String(operande1).toInt();
        int op2 = String(operande2).toInt();
        strcpy(resultat, String(op1 + op2).c_str());
        // TOT BE TESTED
    } else if (strchr(resultat, '-') != nullptr){
        // decomposition de chaque cote de cette operateur puis evaluation de ses deux operandes
        decomposeExpression(task, expression, operande1, operande2, "-");
        //log("evalueExpression => evaluation de l'operande 1 <%s>\n", operande1);
        evalueExpression(task, operande1, resultat);
        strcpy(operande1, resultat);
        //log("evalueExpression => evaluation de l'operande 2 <%s>\n", operande2);
        evalueExpression(task, operande2, resultat);
        strcpy(operande2, resultat);
        int op1 = String(operande1).toInt();
        int op2 = String(operande2).toInt();
        strcpy(resultat, String(op1 - op2).c_str());
    } else if (strchr(resultat, '*') != nullptr){
        // decomposition de chaque cote de cette operateur puis evaluation de ses deux operandes
        decomposeExpression(task, expression, operande1, operande2, "*");
        //log("evalueExpression => evaluation de l'operande 1 <%s>\n", operande1);
        evalueExpression(task, operande1, resultat);
        strcpy(operande1, resultat);
        //log("evalueExpression => evaluation de l'operande 2 <%s>\n", operande2);
        evalueExpression(task, operande2, resultat);
        strcpy(operande2, resultat);
        int op1 = String(operande1).toInt();
        int op2 = String(operande2).toInt();
        strcpy(resultat, String(op1 * op2).c_str());
    } else if (strchr(resultat, '/') != nullptr){
        // decomposition de chaque cote de cette operateur puis evaluation de ses deux operandes
        decomposeExpression(task, expression, operande1, operande2, "/");
        //log("evalueExpression => evaluation de l'operande 1 <%s>\n", operande1);
        evalueExpression(task, operande1, resultat);
        strcpy(operande1, resultat);
        //log("evalueExpression => evaluation de l'operande 2 <%s>\n", operande2);
        evalueExpression(task, operande2, resultat);
        strcpy(operande2, resultat);
        int op1 = String(operande1).toInt();
        int op2 = String(operande2).toInt();
        strcpy(resultat, String(op1 / op2).c_str());
    } else {
        // pas d'operateur on sort avec le resultat contenant une expression finie
        //log("evalueExpression => fin d'evaluaution  le resultat est <%s>\n", resultat);
        if (resultat[0] == '$'){
            int val = evalueVariable(task, resultat);
            strcpy(resultat, String(val).c_str());
        }
    }
    //delay(10);
    // instructions bidon pour eviter erreur de calcul si on n'affiche pas le log (BUG)
    log("");
    //log("evalueExpression => le resultat de <%s> est <%s>\n", expression, resultat);
    nbTabs--;
    return 0;
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
    //log("evalueTest => evaluation de %s\n",expression);
    // decomposition de l'expression
    if (strstr(expression,"==") != nullptr){
        //log("evalueTest => test == detecte\n");
        decomposeExpression(task, expression, operande1, operande2, "==");
        if (operande1[0] == '$'){
            //log("evalueTest => operande1 (%s) est une variable\n",operande1);
            valeur1=evalueVariable(task, operande1);
        } else {
            valeur1 = String(operande1).toInt();
        }
        if (operande2[0] == '$'){
            //log("evalueTest => operande1 (%s) est une variable\n",operande2);
            valeur2=evalueVariable(task, operande2);
        } else {
            valeur2 = String(operande2).toInt();
        }
        returnValue = (valeur1 == valeur2);
        //log("evalueTest => resultat expression <%s> => %d\n",expression, returnValue);
    } else if (strstr(expression,"!=") != nullptr){
        //log("evalueTest => test != detecte\n");
        decomposeExpression(task, expression, operande1, operande2, "!=");
    } else if (strstr(expression,"<") != nullptr){
        //log("evalueTest => test < detecte\n");
        decomposeExpression(task, expression, operande1, operande2, "<");
    } else if (strstr(expression,"<=") != nullptr){
        //log("evalueTest => test <= detecte\n");
        decomposeExpression(task, expression, operande1, operande2, "<=");
    } else if (strstr(expression,">") != nullptr){
        //log("interpreteur => evalueTest => test > detecte\n");
        decomposeExpression(task, expression, operande1, operande2, ">=");
    } else if (strstr(expression,">=") != nullptr){
        //log("interpreteur => evalueTest => test >= detecte\n");
        decomposeExpression(task, expression, operande1, operande2, ">");
    } else {
        log("interpreteur => evalueTest => expression incomprise => <%s>\n",expression);
    }
    return returnValue;
}

//-----------------------------------
//
//      displayVariable
//
//-----------------------------------
void displayVariable(taskStruct *task, char *key){
    for (int i = 0; i < NB_VAR_PAR_TACHE ; i++){
        t_variable *var = &(task->lstVar[i]);
        if (strcmp(var->key,key) == 0){
            //log("affichage la variable %s => ", var->key);
            log("%d\n", var->value);
            break;
        }
        if (strcmp(var->key,"") == 0){
            break;            
        }
    }
}

//-----------------------------------
//
//      displayVariables
//
//-----------------------------------
void displayVariables(taskStruct *task){
    //log("affichage des variables de %s\n", task->name);
    for (int i = 0; i < NB_VAR_PAR_TACHE ; i++){
        t_variable *var = &(task->lstVar[i]);
        if (strcmp(var->key,"") == 0){
            break;            
        }
        //log("<%s> = <%d>\n", var->key, var->value);
        log("%s = %d\n", var->key, var->value);
    }
}

//-----------------------------------
//
//      evalueAffectation
//
//-----------------------------------
int evalueAffectation(taskStruct *task, char *operation){
    //log("interpreteur => evalueAffectation => evalue <%s>\n", operation);
    char variable[50]="";
    char valeur[50]="";
    char resultat[50];
    t_variable *var;
    strcpy(variable, operation);
    char *ptr=strchr(variable,'=');
    ptr[0]='\0';
    //log("interpreteur => evalueAffectation => variable <%s>\n", variable);
    ptr=strchr(operation,'=')+1;
    strcpy(valeur,ptr);
    //log("interpreteur => evalueAffectation => evalue <%s> = <%s>\n", variable, valeur); 
    evalueExpression(task, valeur, resultat);
    strcpy(valeur,resultat);
    for (int i = 0 ; i < NB_VAR_PAR_TACHE ; i++){
        var = &(task->lstVar[i]);
        if (strcmp(var->key, "") == 0){
            // cette variable est libre on l'initialise
            strcpy(var->key, variable);
            var->value = String(valeur).toInt();
            //log("interpreteur => evalueAffectation => on affecte <%s> avec <%d>\n", var->key, var->value);
            //displayVariables(task);
            return 0;
        } else {
            if (strcmp(var->key,variable) == 0){
                // la variable existe deja on met a jour la valeur
                var->value = String(valeur).toInt();
                //log("interpreteur => evalueAffectation => on met a jour <%s> avec <%d>\n", var->key, var->value); 
                //displayVariables(task);
                return 0;
            }
        }
    }
    log("Impossible de stocker la variable <%s> avec <%d> => table des variables pleine (%d)\n", var->key, var->value, NB_VAR_PAR_TACHE); 
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
    //log("interpreteur => afficheDonnee => <%s>\n",donnee);
    //log("firstChar=<%c>, lastChar=<%c>\n",firstChar, lastChar);
    if (donnee[0] == '"'){
        if (donnee[strlen(donnee)-1] == '"'){
            tmp1=&donnee[1];
            tmp1[strlen(tmp1)-1] = '\0';
            Serial.println(tmp1);
        } else {
            log("ligne %d => ERREUR : manque '\"' en fin de chaine\n");
        } 
    } else if (donnee[0] == '$'){
        tmp1=&donnee[1];
        displayVariable(task, tmp1); // A_TESTER
    } else {
        log("ligne %d => ERREUR : imossible afficher <%s>\n", donnee);
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
    //log("interpreteur => evalueInstruction => evalue %s\n", instruction);
    // supressrion du ';'
    char lastChar = instruction[strlen(instruction)-1];
    //log("interpreteur => evalueInstruction => lastChar =%c \n", lastChar); 
    if (lastChar == ';'){
        //log("suppression du ';\n'");
        instruction[strlen(instruction)-1]='\0';
    }
    if (strncmp(instruction,"echo", 4) == 0){
        //log(,"echo detecté\n");
        tmp1=&instruction[4];
        //log("evalueInstruction => valeur a afficher %s\n", tmp1);
        afficheDonnee(task, tmp1);
    } else if (strncmp(instruction,"vars", 5) == 0){
        //log(tmp,"echo detecté\n");
        displayVariables(task);
    } else if (strstr(instruction, "=") != nullptr){
        // evaluation d'une affectation
        evalueAffectation(task, instruction);
    } else {
        // execution d'un autre script
        //log("interpreteur => evalueInstruction => script %s a lancer\n", instruction); 
        sprintf(tmp2, "interpreteur %s", instruction);
        //log("interpreteur => evalueInstruction => task_add <%s>\n", tmp2); 
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
    //log("---------------------------\n"); 
    //log("interpreteur <%s> => interpreteLigne => on interprete la ligne numero %d => <%s>\n", task->name, numLigne, ligne); 
    log("%s\n",ligne);
    char expression[50];
    // tests syntaxe
    char lastCar=ligne[strlen(ligne)-1];
    if (strncmp(ligne, "if", 2) == 0){
        if (task->context.insideIf || task->context.insideElse){
            log("ligne %d => ERREUR : if imbrique interdit\n", numLigne);
            return -1;
        }
        task->context.insideIf=true;
        // traitement du debut de if
        //log("interpreteur => debut de if detecté\n");
        // recuperation de l'expression de test
        if (ligne[2] != '('){
            log("ligne %d => ERREUR : Manque '(' dans if\n", numLigne);
        }
        int index=0;
        strcpy(expression,"");
        for (int i = 3 ; i < strlen(ligne) ; i++){
            if (ligne[i] != ')'){
                expression[index++] = ligne[i];
                expression[index] = '\0';
            }
        }
        //log("interpreteur => interpreteLigne => expression trouvee <%s>\n", expression);
        task->context.resultTest = evalueTest(task, expression);
    } else if (strncmp(ligne, "else", 4) == 0){
        //log("interpreteur => else detecté\n");
        task->context.insideElse=true;
        if (!task->context.insideIf){
            log("ligne %d => ERREUR : else sans if\n", numLigne);
        }
    } else if (strncmp(ligne, "vars", 4) == 0){
        //log("interpreteur => vars detecté\n");
        displayVariables(task);
    } else if (strncmp(ligne, "fi", 2) == 0){
        // traitement du fin de if
        //log("interpreteur => fin de if detecté\n");
        task->context.insideIf=false;
        task->context.insideElse=false;
    } else if (lastCar != ';'){
        log("ligne %d => ERREUR : Manque ';' en fin de ligne <%c>\n", numLigne, lastCar);
        return -1;
    } else {
        //log("interpreteur => on evalue l'instruction <%s>\n", ligne);
        if (task->context.insideIf){
            if (task->context.resultTest){
                // test si on est dans le then ou dans le else
                //log("interpreteur => test = true\n");
                if (!task->context.insideElse){
                    // on est dans le then on execute
                    //log("interpreteur => on evalue les instructions a l'interieur d'un then\n");
                    evalueInstruction(task, ligne);
                } else {
                    //log("interpreteur => on evalue pas les instructions a l'interieur d'un else\n");
                }
            } else {
                // test si on est dans le then ou dans le else
                //log("interpreteur => test = false\n");
                if (task->context.insideElse){
                    // on est dans le then on execute
                    //log("interpreteur => on evalue les instructions a l'interieur d'un else\n");
                    evalueInstruction(task, ligne);
                } else {
                    //log("interpreteur => on evalue pas les instructions a l'interieur d'un then\n");
                }

            }
        } else {
            // on est pas dans un if on execute systematiquement
            //log("interpreteur => on est pas dans un if\n");
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
    //log("---------------------------\n");
    //log("interpreteur <%s> => analyseLigne => analyse de la ligne numero %d => <%s>\n", task->name, numLigne, ligne); 
    // suppression des commentaires

    index = strcspn(ligne, "#");
    if (index != strlen(ligne)){
        //log("commentaires trouvee en pos %d\n", index); 
        if (index == 0) return 0;
        ligne[index]='\0';
    }
    //log("suppression des commentaires => <%s>\n", ligne); 
    
    // remplacement des tabulations par des espaces
    //log("taille de la ligne %d\n", strlen(ligne)); 
    while (true){
        index = strcspn(ligne, "\t");
        if (index < strlen(ligne)){
            //log("tabulations trouvee en pos %d\n", index);
            ligne[index] = ' ';
        }else {
            break;
        }
    }
    //log("interpreteur => ligne apres suppression des tabulations <%s>\n", ligne);
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
                //log("on garde le caractere <%c> en pos %d\n", ligne[i], index);
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
    //log("interpreteur => ligne apres suppression des blancs <%s>\n", tmp1);
    // suppression des lignes vides
    if (strlen(ligne) == 0) {
        //log("ligne vide => non traitee\n");
        return 0;
    }
    // reste ligne a traiter
    //log("interpreteur => analyseLigne => ligne numero %d a interpreter <%s>\n", numLigne, ligne);
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
    //log("interpreteur => debut du script <%s>\n", task->name);
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
            Serial.println("ligne %d => ERREUR : Manque fi pour if\n");
            task->context.nbErreurs++;
        }

        if ( task->context.nbErreurs != 0){
            if (task->context.nbErreurs > 1){
                log("interpreteur => %d erreurs se sont produites\n", task->context.nbErreurs);
            } else {
                log("interpreteur => %d erreur s'est produite\n", task->context.nbErreurs);
            }
        }
        task->status=DEAD;
        //log("interpreteur => fin\n");
    } else {
        //log("interpreteur => on repasse la main au scheduler avant de traiter la ligne suivante\n");
    }

}



//-----------------------------------
//
//      interpreteur_init
//
//-----------------------------------
void interpreteur_init(taskStruct *task){
    char *parametres;
    //log("interpreteur_init => ---------------------------\n");
    //log("interpreteur_init => lancement <%s> pid(%d)\n", task->name, task->pid);
    int cdeLength=strlen("interpreteur") + 1;
    if (strlen(task->name) > cdeLength) {
        parametres = &task->name[cdeLength];
        //log("interpreteur_init => execution commande externe <%s>\n", parametres);
    }
    strcpy(task->context.fileName, parametres);
    char fullName[150];
    strcpy(fullName, task->sdCardParam.currentPathName);
    strcat(fullName,"/");
    strcat(fullName, parametres);
    if (!SD.exists(fullName)){
        log("ERREUR => impossible d'ouvrir le fichier <%s>\n", task->sdCardParam.currentDirectoryName);
        task->status=DEAD;
        return;
    } 
    task->context.ptrFile=SD.open(fullName);
    // executer interpreteur sur ce fichier
    //strcmp(task->context.fileName,parametres);
    //task->context.ptrFile=entry;
    //log("interpreteur_init => fichier <%s> trouve => on lance l'interpreteur\n", parametres);
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
    //log("interpreteur_exec => debut\n");
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
    //log("interpreteur_wakeup => pid(%d)\n", task->pid);
    task->status = RUN;
}