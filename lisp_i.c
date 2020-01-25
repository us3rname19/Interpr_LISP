#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define print "print"
#define cond "cond"
#define cons "cons"
#define plus "+"
#define mult "*"
#define mins "-"
#define car "car"
#define cdr "cdr"


struct charval { char str[255]; }; // ���������� ��������� ��� ������ �������\����������
struct charval stack[255];         // ���������� ������� ��������


//���������� ��������� ���������� ������� - ����������
struct form {
    struct charval S;        // ��������� - �������
    struct charval val[255]; // ������ ��� ����������
}form;


//���������� ��������� ������� ������� ������� - ����������
struct doing {
    struct charval S;      // ��������� - �������
    struct form D[255];    // ��������� ��������� ������� - ����������
}doing;


//���������� ��������� ����
struct spis {
    struct doing In;  // ��������� � �������
    struct spis* v1;  // v1 � ��������� �� ���������� ���������
    struct spis* v2;  // v2 � ��������� �� ����������� ���������
} spis;
struct spis* head, * tail;  // ��������� �� ������ � ����� ������


void interp(); // ������������� � �������
void load();   // ������������� ���� �� �����


// ����
int main() {

    char c;

    while (1) {
        puts("\n\n____________________________________");
        puts("1-Lisp interpreter in console (read)");
        puts("2-Download code from file");
        puts("0-Exit\n\n");
        c = getch();
        switch (c) {
        case '1':interp(); break;
        case '2':load(); break;
        case '0':exit(0);
        default: puts("\n<Error, try again>\n");
        }
    }
}


// ����� ���������� 
void vals(int i, struct doing* d) {

    int w = 0;
    char out[255] = { 0 };
    char sep[5] = " ";
    int c = 0;

    while (1) {
        if (strcmp(d->D[0].val[0].str, "\0") == 0) { c = 1; }
        if (strcmp(d->D[w].val[i].str, "\0") == 0) { break; }
        else {
            while (1) {
                if (strcmp(d->D[w].val[i].str, "\0") == 0) { break; }
                else {
                    strcat(out, d->D[w].val[i].str);
                    strcat(out, sep);
                }
                i++;
            }
        }
        w++;
    }
    if (!c) printf("<Output : %s\n", out);
    else printf("<Output : NIL\n");
}


// ������� ���������� ����
void eval(struct doing* d) {

    struct doing* d_new;
 
    // car --- ������ ������� � ������. 
    if (strcmp(car, d->S.str) == 0) {
        if (strcmp(d->D[0].val[0].str, "\0") ==0) { printf("<Output : NIL\n"); }
        else printf("<Output : %s\n", d->D[0].val[0].str);
    }

    // cdr --- ���������� ����� ������ ����� ������� ��������
    else if (strcmp(cdr, d->S.str) == 0) {
        vals(1, d);
    }

    // print --- ������ �� �������
    else if (strcmp(print, d->S.str) == 0) {
        vals(0, d);
    }

    // cons --- �������� ����� ������� � ������ ������
    else if (strcmp(cons, d->S.str) == 0) {
        vals(0, d);
    }

    // cond --- ��������������� ��� ��������� � ������� ����������, 
    // � ���� ���� ���� �� ��� ��������, �� cond ���������� ���������, 
    // ��������� � ���� ����������. ���� �� ���� �������� �� ��� ��������, 
    // �� ��� ������ NIL.
    else if (strcmp(cond, d->S.str) == 0) {
        
        int w = 0;
        int c = 0;
        
        while (1){
            if (strcmp(d->D[w].S.str, "\0") == 0 || c) { break; }
            else {
                d_new = (struct doing*)malloc(1 * sizeof(doing));
                if (strcmp(d->D[w].val[0].str, "\0") == 0 ) { w++;}
                else {
                    c = 1;
                    strcpy(d_new->S.str,d->D[w].S.str);
                    *d_new->D = d->D[w];
                    eval(d_new);
                }
            }
        }
        if (!c) { printf("<Output : NIL\n"); }
    }

    // "+" --- ����� �������� 
    else if (strcmp(plus, d->S.str) == 0) {
        
        int sum = 0;
        int i = 0;
        
        while (1) {
            if (strcmp(d->D[0].val[i].str, "\0") == 0) { break; }
            sum += atoi(d->D[0].val[i].str);
            i++;
        }
        printf("<Output : %d\n", sum);
    }

    // "*" --- ������������ �������� 
    else if (strcmp(mult, d->S.str) == 0) {

        int i = 0;
        int mul = atoi(d->D[0].val[i].str); i++;
        
        while (1){
            if (strcmp(d->D[0].val[i].str, "\0") == 0) { break; }
            mul *= atoi(d->D[0].val[i].str); i++;
        }
        printf("<Output : %d\n", mul);
    }

    // "-" --- �������� �������� 
    else if (strcmp(mins, d->S.str) == 0) {

        int i = 0;
        int minus = atoi(d->D[0].val[i].str); i++;

        while (1) {
            if (strcmp(d->D[0].val[i].str, "\0") == 0) { break; }
            minus -= atoi(d->D[0].val[i].str); i++;
        }
        printf("<Output : %d\n", minus);
    }

    else puts("\n<Unknown command!>\n");
}


// ������� ����������� ������ �����
void step(char* str) {

    char atom_or_com[50];          // ������ ��� ������ ����������\�������
    struct spis* p, * pred;        // ��������� ��� �������� ������
    struct doing* d_new;           // ��������� �������������� ������
    struct charval* S;             // ������ ��� �������� ����������
    int i = 0;                     // ������� ������� ������
    int s = 0;                     // ������� ����� ���������� 
    int t = 0;                     // ������� ������� atom_or_com
    int b = 0;                     // ������� ������
    pred = NULL;


    if (strlen(str) == 0) { printf("\n<Data not found!>\n"); }  // �������� �� ���� ������
    else {
        if (str[0] != '(' || str[strlen(str) - 1] != ')') {  // �������� ���������� LISP - "()"
            puts("\n<Syntax ERROR>\n<Try again...>");
            main();
        }
        else {
            b++;
            puts("\n<Transliting code...>");
            S = (struct charval*)malloc(1 * sizeof(stack));  // ��������� ������ ��� ������� ����������
            while (str[i] != 0) {
                i++;
                char c = str[i];
                if (c != ' ' && c != ',' && c != '(' && c != ')'){ atom_or_com[t] = c; t++; }
                if (c == '(') {b++;}
                if (c == ')') {b--;}
                if (c == ' ' || c == ',' || c == '(' || c == ')') {
                    atom_or_com[t] = '\0';
                    if (strlen(atom_or_com) > 0 ) { 
                        strcpy(S[s].str, atom_or_com); // ���������� ����� �����������
                        s++;
                        t = 0;
                    }  
                    strcpy(atom_or_com, "\0");
                }
                if (b == 0 && str[i] == 0) { // ���� ����� ������ � ���-�� "(" == ")"
                    do { // ���������� ������ ����/��������� �������
                        p = (struct spis*)malloc(1 * sizeof(spis));  // ��������� ������ ��� ���������
                        strcpy(p->In.S.str, S[0].str); 
                        int q = 0;
                        // ���������� ���� ��� --- cond
                        if (strcmp(cond, p->In.S.str) == 0) {
                            int w = -1;
                            int j = 1;
                            while (1) {
                                if (strcmp(S[j].str, "\0") == 0) { break; }
                                else {
                                    if (strcmp(S[j].str, car) == 0 
                                        || strcmp(S[j].str, cdr) == 0
                                        || strcmp(S[j].str, print) == 0 
                                        || strcmp(S[j].str, mult) == 0
                                        || strcmp(S[j].str, plus) == 0 
                                        || strcmp(S[j].str, cons) == 0
                                        || strcmp(S[j].str, mins) == 0) {
                                        w++;
                                        strcpy(p->In.D[w].S.str, S[j].str);
                                    }
                                    else {
                                        strcpy(p->In.D[w].val[q].str, S[j].str);
                                        q++;
                                    }
                                    j++;
                                }
                            }
                        }
                        // ���������� ���� ��� ���� ��������� ������� LISP
                        else {
                            for (int j = 1; j < strlen(str) - 1; j++) {
                                if (strcmp(S[j].str, "(") == 0 || strcmp(S[j].str, ")") == 0) { j++; }
                                strcpy(p->In.D[0].val[q].str, S[j].str);
                                q++;
                            }
                        }
                        // ������������ ������ ���������
                        p->v1 = pred;  
                        if (pred != NULL) pred->v2 = p;
                        else head = p;
                        pred = p;
                        break;
                    } while (getch() != 27);
                             tail = p;
                             tail->v2 = NULL;
                }
                // ���� ���������� ������ ��������� �� ������ � ������� � ������� ����
                if (str[i] == 0 && b != 0) {
                    puts("\n<Syntax ERROR>\n<Missing ')' or '('>\n");
                    main();
                }
            }
        }
        while (p != NULL) {
            d_new = (struct doing*)malloc(1 * sizeof(doing)); // ��������� ������ ��� ��������� �������������� ������
            *d_new = p->In;
            eval(d_new); // ����� ������� ����������� ���� LISP
            p = p->v2;
        }
    }
}


// ������� ����� ���� � ������� 
void interp() {

    char str[255];

    puts("\n<Input lisp code : ");
    gets(str);
    step(str); // ����� ������� ����������� ������
}
   

// ������� ��������� ������ �� �����
void load() {

    FILE* tf;
    char str[255] = "\0";
    char* estr;
    tf = fopen("lisp.txt", "r"); // �������� ����� ��� ������

    if (tf == NULL) { printf("\nData not found!\n"); }  // �������� �� ������������� ����� ��� ������ ������
    else {
        printf("\n<Reading code from file...>\n"); 
        while (getc(tf) != '\n') {}
        while (1) {
            // ���������� ������������ �����
            estr = fgets(str, sizeof(str), tf);
            if (estr == NULL){
                if (feof(tf) != 0) {
                    puts("\n<!Done.>");
                    break;
                }
                else{
                    printf("\n<Reading file ERROR!>\n");
                    break;
                }
            }
            int i = 0;
            if (str[0] == '(') {
                str[strlen(str)-1] = '\0';
                printf("\nCode : %s", str);
                step(str);  // ����� ������� ����������� ������
            }
        }
    }
    fclose(tf);
}