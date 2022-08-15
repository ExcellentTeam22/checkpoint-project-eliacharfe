#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_NAME 31
#define MAX_TEL 15
#define TOTAL_CONTACTS 10

struct Contact{
    char fname[MAX_NAME];
    char lname[MAX_NAME];
    char telep[MAX_TEL];
    char cellp[MAX_TEL];

    struct Contact* next;
};

struct Contact* DB = NULL;

size_t get_input(char * dest, size_t max);
void free_list();
FILE * open_file(char* path, char* mode);
void read_file_insert_to_DB(FILE * file);

void clrscr(void);
void gotoxy(int x, int y);
void insert(void);
void delet(void);
void edit(void);
void search(void);
void searchf(void);
void searchl(void);
void searchp(void);
void searchc(void);
void list(void);
void list2(void);
void sort(void);
void sortf(void);
void sortl(void);
void sortp(void);
void sortc(void);
void help(void);
int last = 0;

//----------------------------
int main()
{
    FILE *fptr = open_file("/home/eliachar/Documents/GitHub/checkpoint-project-eliacharfe/check-point/phonebookFile","r");
    read_file_insert_to_DB(fptr);

    bool isRunning = true;
    char n;

    while(isRunning) {
        clrscr();
        printf("\n|Phone Book12<::>Home|\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Welcome to PHONE BOOK2022!\nYou have inserted ( %d ) contacts.\n\n",last);
        printf("\t[1] |--> Insert\n");
        printf("\t[2] |--> Delete\n");
        printf("\t[3] |--> Edit\n");
        printf("\t[4] |--> Search\n");
        printf("\t[5] |--> Show All\n");
        printf("\t[6] |--> Sort\n");
        printf("\t[7] |--> Help\n");
        printf("\t[8] |--> Exit\n");
        printf("\n\tPlease Enter Your Choice (1-8): ");
        n = (char)getc(stdin);
        getc(stdin);
        switch(n) {
            case '1':
                insert();
                break;
            case '2':
                delet();
                break;
            case '3':
                edit();
                break;
            case '4':
                search();
                break;
            case '5':
                list();
                break;
            case '6':
                sort();
                break;
            case '7':
                help();
                break;
            case '8':
                isRunning = false;
                break;
            default:
                printf("\nThere is no item with symbol \"%c\".Please enter a number between 1-8!\nPress any key to continue...",n);
                getc(stdin);
                getc(stdin);
                break;
        }//End of swicth
    }
    free_list();
    fclose(fptr);
    return EXIT_SUCCESS;
}//End of main function!
//-----------------------------------------

FILE * open_file(char* path, char *mode)
{
    FILE* fptr = fopen(path, mode);
    if(fptr == NULL)
    {
        printf("Error openning file!");
        exit(1);
    }
    return fptr;
}
//------------------------------
void read_file_insert_to_DB(FILE * file)
{
    char * inpfile = NULL;

    while(file){
        struct Contact *temp = NULL;
        temp = (struct Contact *) malloc(sizeof(struct Contact));
        size_t len = 0;
        size_t len_size = 0;
        if((len_size = getline(&inpfile, &len, file)) == EOF)
            break;

        char * token = strtok(inpfile, "\t");

        char* first_name = token;
        strncpy(temp->fname, first_name, MAX_NAME);

        char* last_name = strtok(NULL, "\t");
        strncpy(temp->lname, last_name, MAX_NAME);

        char * tel = strtok(NULL, "\t");
        strncpy(temp->telep, tel, MAX_TEL);

        char * cell = strtok(NULL, "\t");
        strncpy(temp->cellp, cell, MAX_TEL);

        temp->next = DB;
        DB = temp;
        puts(cell);
    }

}
//-----------------------------
void free_list()
{
    struct Contact *temp = DB ;
    while (DB != NULL) {
        temp = DB;
        DB = DB->next ;
        free(temp);
    }
}
//---------------------------
size_t get_input(char *dest, size_t max)
{
    char * temp = NULL;
    size_t len = 0;
    size_t len_size = 0;
    if((len_size = getline(&temp,&len,stdin)) == -1){
        exit(1);
    }

    if (len_size < max){
        temp[len_size - 1] = '\0';
        strncpy(dest, temp, len_size);
    }
    else {
        temp[max - 1] = '\0';
        strncpy(dest, temp, max);
        len_size = max;
    }
    free(temp);
    return len_size;
}
//--------------------------
void print(){
    struct Contact *temp = DB;
    int i = 1;
    while(temp != NULL)
    {
        printf("Contact number: %d is:\n", i);

        printf("First Name: %s\n ",temp->fname);
        printf("Last Name: %s\n ",temp->lname);
        printf("Tel: %s\n ",temp->telep);
        printf("Cell: %s\n ",temp->cellp);
        temp = temp->next;
        i++;
    }
}
//--------------------------------
void insert()
{
    char ans='y';
    clrscr();
    printf("\nPhone Book12<::>Insert Contacts");
    printf("\n--------------------------------------------------------------------------------");

    while(ans == 'y') {
        if (last == TOTAL_CONTACTS)
            last = 0;
        else {
            struct Contact *temp = NULL;
            temp = (struct Contact *) malloc(sizeof(struct Contact));

            printf("\n\nData of Contact %2.2d{\n", last + 1);

            printf("\n\t  1-F.Name: ");
            get_input(temp->fname, MAX_NAME);

            printf("\t  2-L.Name: ");
            get_input(temp->lname, MAX_NAME);

            printf("\t  3-Tele.P: ");
            get_input(temp->telep, MAX_TEL);

            printf("\t  4-Cell.P: ");
            get_input(temp->cellp, MAX_TEL);

            temp->next = DB;
            DB = temp;

            printf("\n|-->Data Recorded!}");
            printf("\n\t\t\tNext Contact?(y/n) Answer:");
            ans = (char) getc(stdin);
            getc(stdin);
            last++;
        }
    }

    printf("\n\nYou have inserted ( %d ) Contact!\nPress a key to return main page & continue program|-->",last);
    getc(stdin);
}
//--------------------------
void delet(void)
{
    char dfname_string[MAX_NAME], dlname_string[MAX_NAME];
    int i = 0;
    bool found = false;
    char ch;
    clrscr();
    printf("\nPhone Book12<::>Delete Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf ("\n::Enter data of the Contact that you want delete it,please:");

    printf("\n\n  ::Enter first name: ");
    get_input(dfname_string, MAX_NAME);

    printf("\n  ::Enter last name: ");
    get_input(dlname_string, MAX_NAME);

    struct Contact* temp = DB;

    if (strcmp (dfname_string, DB->fname) == 0 && strcmp (dlname_string, DB->lname) == 0 ) {
        temp = DB;
        DB = DB->next;
        free(temp);
        return;
    }

    struct Contact *curr  = DB;
    while (curr->next != NULL)
    {
        if (strcmp (dfname_string, curr->next->fname) == 0 && strcmp (dlname_string, curr->next->lname) == 0 ) {
            printf("\nContact was found! Details:");
            printf("\n\nCantact %2.2d{",i+1);
            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
                   curr->next->fname,curr->next->lname,curr->next->telep,curr->next->cellp);
            printf("\n\nAre you sure you want to delete this Contact?(y/n)");
            ch = (char)getc(stdin);
            getc(stdin);
            if (ch == 'y') {
                temp = curr->next;
                curr->next = curr->next->next;
                free(temp);
                last--;
                found = true;
                break;
            }
        }
        else {
            curr = curr->next;
            i++;
        }
    }

    if (!found)
        printf("\t\t\n<<This Contact does not exist in this list or program can not delete it.>>");
    else
        printf("\t\t\n<<Target Contact was successfully deleted from list!>>");
    printf("\n\n\nPress a key to return main page & continue program|-->");
    getc(stdin);
}
//------------------------------------
void edit()
{
    char dfname[MAX_NAME] , dlname[MAX_NAME];
    int i = 0;
    bool found = false;
    char ch;
    clrscr();
    printf("\nPhone Book12<::>Edit Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf ("\n::Enter data of the Contact that you want edit it,please:");

    printf("\n\n  ::Enter first name: ");
    get_input(dfname, MAX_NAME);

    printf("\n  ::Enter last name: ");
    get_input(dlname, MAX_NAME);

    while (DB != NULL){
        puts(DB->fname);
        puts(DB->lname);
        if (strcmp (dfname, DB->fname) == 0 && strcmp (dlname, DB->lname) == 0) {
            printf("\nContact found! Details:");
            printf("\n\nCantact %2.2d{",i+1);
            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
                   DB->fname, DB->lname, DB->telep, DB->cellp);
            printf("\n\nDo you want edit it?(y/n) ");
            ch = (char)getc(stdin);
            getc(stdin);
            if(ch == 'y'){
                printf("\n >|Enter new first name: ");
                get_input(DB->fname, MAX_NAME);

                printf(" >|Enter new last name: ");
                get_input(DB->lname, MAX_NAME);

                printf(" >|Enter new telephone number: ");
                get_input(DB->telep, MAX_TEL);

                printf(" >|Enter new cellphone number: ");
                get_input(DB->cellp, MAX_TEL);

                found = true;
                break;
            }
        }
        DB = DB->next;
    }

    if (!found)
        printf("\t\t\n<<This Contact does not exist or you chose not to Edit it.>>");
    else
        printf("\t\t\n<<Target Contact was successfully updated!>>");
    printf("\n\n\n   ::Press a key to return main page & continue program|-->");
    getc(stdin);
}

void search(void)
{
    char ch;
    clrscr();
    printf("\nPhone Book12<::>$earch Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\nChoose search type,please:\n\n");
    printf("\t[1] |--> Search by first name\n");
    printf("\t[2] |--> Search by last name\n");
    printf("\t[3] |--> Search by phone number\n");
    printf("\t[4] |--> Search by cellphone number\n");
    printf("\t[5] |--> Main Menu\n");
    printf("\n\t::Enter a number (1-5): ");
    ch = (char)getc(stdin);
    getc(stdin);
    printf("\n--------------------------------------------------------------------------------");
    switch(ch) {
        case '1':
            searchf();
            break;
        case '2':
            searchl();
            break;
        case '3':
            searchp();
            break;
        case '4':
            searchc();
            break;
        case '5':
        default:
            return;
    }
}

void searchf(void)
{
    char fname[MAX_NAME];
    int i = 0,find=0;

    printf("Enter a first name to search:");
    get_input(fname, MAX_NAME);

    while (DB != NULL) {
        if(strcmp(fname,DB->fname) == 0) {
            find = 1;
            break;
        }
        DB = DB->next;
    }

    if(find == 0)
        printf("\t\n<<Not Find!There is no Contact with this name in phone book.>>");
    else {
        printf("\t\n<<Target Contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
               DB->fname, DB->lname, DB->telep, DB->cellp);
    }
    printf("\nPress a key to search another Contact.");
    getc(stdin);
    search();
}

void searchl(void)
{
    char lname[MAX_NAME];
    int i = 0;
    bool found = false;

    printf("\n::Enter a last name to search:");
    get_input(lname, MAX_NAME);

    while (DB != NULL) {
        if(strcmp(lname,DB->fname) == 0) {
            found = true;
            break;
        }
        DB = DB->next;
        ++i;
    }

    if(!found)
        printf("\t\n<<Not Find!There is no Contact with this name in phone book.>>");
    else {
        printf("\t\n<<Target Contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
               DB->fname, DB->lname, DB->telep, DB->cellp);
    }
    printf("\nPress a key to search another Contact.");
    getc(stdin);
    search();
}

void searchp(void)
{
    char telep[MAX_TEL];
    int i;
    bool found = false;

    printf("\n::Enter a phone number to search:");
    get_input(telep, MAX_TEL);

    while (DB != NULL) {
        if(strcmp(telep, DB->fname) == 0) {
            found = true;
            break;
        }
        DB = DB->next;
        ++i;
    }

    if(!found)
        printf("\t\n<<Not Find!There is no Contact with this tel number in phone book.>>");
    else {
        printf("\t\n<<Target Contact found! Details:>>");
        printf("\n\nCantact %2.2d{", i + 1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
               DB->fname, DB->lname, DB->telep, DB->cellp);
    }
    printf("\nPress a key to search another Contact.");
    getc(stdin);
    search();
}

void searchc(void)
{
    char cell[MAX_TEL];
    int i = 0;
    bool found = false;

    printf("\n::Enter a cellphone number to search:");
    get_input(cell, MAX_TEL);


    while (DB != NULL) {
        if(strcmp(cell, DB->fname) == 0) {
            found = true;
            break;
        }
        DB = DB->next;
        ++i;
    }

    if(!found)
        printf("\t\n<<Not Find!There is no Contact with this cellphone number in phone book.>>");
    else {
        printf("\t\n<<Target Contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
               DB->fname, DB->lname, DB->telep, DB->cellp);
    }
    printf("\nPress a key to search another Contact.");
    getc(stdin);
    search();
}

void sort(void)
{
    char ch;
    clrscr();
    printf("\nPhone Book12<::>$earch Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\nChoose sort type,please:\n\n");
    printf("\t[1] |--> Sort by first name\n");
    printf("\t[2] |--> Sort by last name\n");
    printf("\t[3] |--> Sort by phone number\n");
    printf("\t[4] |--> Sort by cellphone number\n");
    printf("\t[5] |--> Main Menu\n");
    printf("\n\t::Enter a number (1-5): ");
    ch = getc(stdin);
    getc(stdin);
    printf("\n--------------------------------------------------------------------------------");
    switch(ch){
        case '1':
            sortf();
            break;
        case '2':
            sortl();
            break;
        case '3':
            sortp();
            break;
        case '4':
            sortc();
        case '5':
        default:
            return ;
    }
}

void swap_help(char a[], char b[], size_t size)
{
    char temp[size];
    strncpy(temp, a, size);
    strncpy(a, b, size );
    strncpy(b, temp, size);
}

void swap(struct Contact *a, struct Contact *b)
{
    swap_help(a->fname, b->fname, MAX_NAME);
    swap_help(a->lname, b->lname, MAX_NAME);
    swap_help(a->telep, b->telep, MAX_TEL);
    swap_help(a->cellp, b->cellp, MAX_TEL);
}

void sortf(void)
{
    int swapped, i;
    struct Contact *a;
    struct Contact *b = NULL;

    if (DB == NULL)
        return;

    do {
        swapped = 0;
        a = DB;
        while (a->next != b)
        {
            if(strcmp(a->fname, a->next->fname) > 0) {
                swap(a, a->next);
                swapped = 1;
            }
            a = a->next;
        }
        b = a;
    }
    while (swapped);

    printf("\nplease wait... .Contacts will be sorted by first names.");
    list2();
    printf("\n   ::Press any key to sort Contact by another form... ");
    getc(stdin);
    sort();
}

void sortl(void)
{
    int swapped, i;
    struct Contact *a;
    struct Contact *b = NULL;

    if (DB == NULL)
        return;

    do {
        swapped = 0;
        a = DB;
        while (a->next != b)
        {
            if(strcmp(a->lname, a->next->lname) > 0)
            {
                swap(a, a->next);
                swapped = 1;
            }
            a = a->next;
        }
        b = a;
    }
    while (swapped);

    printf("\nplease wait... .Contacts will be sorted by last names.");
    list2();
    printf("\n   ::Press any key to sort Contact by another form... ");
    getc(stdin);
    sort();
}

void sortp(void)
{
    int swapped, i;
    struct Contact *a;
    struct Contact *b = NULL;

    if (DB == NULL)
        return;

    do {
        swapped = 0;
        a = DB;
        while (a->next != b)
        {
            if(strcmp(a->telep, a->next->telep) > 0)
            {
                swap(a, a->next);
                swapped = 1;
            }
            a = a->next;
        }
        b = a;
    }
    while (swapped);

    printf("\nplease wait... .Contacts will be sorted by telephone numbers.");
    list2();
    printf("\n   ::Press any key to sort Contact by another form... ");
    getc(stdin);
    sort();
}

void sortc(void)
{
    int swapped, i;
    struct Contact *a;
    struct Contact *b = NULL;

    if (DB == NULL)
        return;

    do {
        swapped = 0;
        a = DB;
        while (a->next != b)
        {
            if(strcmp(a->cellp, a->next->cellp) > 0) {
                swap(a, a->next);
                swapped = 1;
            }
            a = a->next;
        }
        b = a;
    }
    while (swapped);

    printf("\nPlease wait... .Contacts will be sort by cellphone numbers.");
    list2();
    printf("\n   ::Press any key to sort Contact by another form... ");
    getc(stdin);
    sort();
}

void list()
{
    struct Contact *temp = DB;
    int i = 1;
    while(temp != NULL)
    {
        printf("Contact number: %d is:\n", i);

        printf("First Name: %s\n ",temp->fname);
        printf("Last Name: %s\n ",temp->lname);
        printf("Tel: %s\n ",temp->telep);
        printf("Cell: %s\n\n ",temp->cellp);
        temp = temp->next;
        i++;
    }

//    i = 0;
//    clrscr();
//    printf("\nPhone Book12<::>All Contacts List");
//    printf("\n--------------------------------------------------------------------------------");
//    gotoxy(1,4);
//    printf("Row");
//    gotoxy(9,4);
//    printf("First Name");
//    gotoxy(27,4);
//    printf("Last Name");
//    gotoxy(44,4);
//    printf("Telephone");
//    gotoxy(60,4);
//    printf("Cellphone");
//    printf("\n--------------------------------------------------------------------------------");
//    for(i = 0; i <= last; i++) {
//        gotoxy(1,i+6);
//        printf("%3.3d",i+1);
//        gotoxy(9,i+6);
//        printf("%s",A[i].fname);
//        gotoxy(28,i+6);
//        printf("%s",A[i].lname);
//        gotoxy(44,i+6);
//        printf("%s",A[i].telep);
//        gotoxy(60,i+6);
//        printf("%s",A[i].cellp);
//    }

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t    **********End Of list!**********");
    printf("\n\n\n   ::Press a key to return main page & continue program.|-->");
    getc(stdin);
}

void list2(void)
{
    struct Contact *temp = DB;
    int i = 1;
    while(temp != NULL)
    {
        printf("Contact number: %d is:\n", i);

        printf("First Name: %s\n ",temp->fname);
        printf("Last Name: %s\n ",temp->lname);
        printf("Tel: %s\n ",temp->telep);
        printf("Cell: %s\n\n ",temp->cellp);
        temp = temp->next;

        i++;
    }

//    int i = 0;
//    printf("\n--------------------------------------------------------------------------------");
//    gotoxy(1,18);
//    printf("Row");
//    gotoxy(9,18);
//    printf("First Name");
//    gotoxy(27,18);
//    printf("Last Name");
//    gotoxy(44,18);
//    printf("Telephone");
//    gotoxy(60,18);
//    printf("Cellphone");
//    printf("\n--------------------------------------------------------------------------------");
//    for(i = 0; i < last; i++) {
//        gotoxy(1,i+21);
//        printf("%3.3d",i+1);
//        gotoxy(9,i+21);
//        printf("%s",A[i].fname);
//        gotoxy(27,i+21);
//        printf("%s",A[i].lname);
//        gotoxy(44,i+21);
//        printf("%s",A[i].telep);
//        gotoxy(60,i+21);
//        printf("%s",A[i].cellp);
//    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t    **********End Of list!**********");
}

void help(void)
{
    clrscr();
    gotoxy(26,1);
    printf("--------------------------");
    gotoxy(26,2);
    printf("|Welcome To Phone Book12!|");
    gotoxy(26,3);
    printf("--------------------------");
    gotoxy(1,5);
    printf("PhoneBook12>>[Help & Instructions] \n");
    printf("--------------------------------------------------------------------------------\n");
    printf("\nAbout Phonebook12\n *Version: 2.5\n *Valence: 50 Contacts(MAX)\n  *By: Morteza Zakeri (2011-12)");
    printf("\n\nThis program allows you to have a list of your contacts.\nIt also allows you to have your listing added or deleted from your list.\nOther features of this program include:\nSearch,edit,sort & list.\n\n   ::Press a key to return main page & continue program.|-->");
    getc(stdin);
}

void clrscr()
{
    system("clear");
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}
