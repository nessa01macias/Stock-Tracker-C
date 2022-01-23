/*Write a program that keeps record of stock market trades. Program stores following information
about each trade:
The program must have (at least) following operations:
1. initialize (empty the database)
2. save database to file
3. read database from file
4. buy shares
5. sell shares
6. print a report which prints list of all shares that you still own followed by the total price of
the shares.
7. Print a report of all sold shares. Display profit/loss per sale. Print total profit/losses at the
end of the report.*/

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

#define SIZE 30
#define GENERAL 200
#define EXIT 10
#define INITIALIZER 3
#define CERO 0

            //read all the characters except the comma [^,]
const char* STOCK_FORMAT_IN = "%i, %[^,], %[^,], %i, %f, %[^,], %f, %[^,]"; //aka serialization, to be able to read from the file
const char* STOCK_FORMAT_OUT = "%i, %s, %s, %i, %.2f, %s, %.2f, %s\n"; //aka serialization, to be able to write in the file

struct stock_tracker {
    int identifier;              // stock[i].identifier
    char description[SIZE];      // stock[i].description; [common stock, preferred stock, growth stocks, income stocks, blue-chips stock]
    char name[SIZE];             //stock[i].name
    int shares;                  //stock[i].shares

    bool available;  //available = false = sold ; available = true = currently owned ;

    float purchase_price;       //stock[i].purchase_price
    char purchase_date[SIZE];   //stock[i].purchase_date

    float sell_price;           //stock[i].sell_price
    char sell_date[SIZE];       //stock[i].sell_date
} ;

//simple functions that would be repeated
void toContinue();
void title();
void printbasicColumns();
void deleteSpace(char *word);
bool correctType(char *stock_type);
void displayList(struct stock_tracker *stock, size_t size);


//functions to produce random identifiers for all the trades
bool checkrep (int n, int identifier);
void setIdentifiers(struct stock_tracker *stock, size_t size);


//options (1-9)
void initializeData(struct stock_tracker *stock, size_t size);
void saveData(struct stock_tracker *stock,size_t size);
void readData(struct stock_tracker *stock,size_t size);
void buyShare(struct stock_tracker *stock, size_t size);
void sellShare(struct stock_tracker *stock, size_t size);
void modifyShare(struct stock_tracker *stock,size_t size);
void reportShares(struct stock_tracker *stock, size_t size);
void reportsoldShares(struct stock_tracker *stock, size_t size);


//option 9
void printbyAll(struct stock_tracker *stock, size_t size);
void printbyType(struct stock_tracker *stock, size_t size);
void printbyCompany(struct stock_tracker *stock, size_t size);
void printEverything(struct stock_tracker *stock, size_t size);

int main(){

        int option;
        struct stock_tracker stock[GENERAL];
        setIdentifiers(stock, GENERAL); //every struct of the array of structs will have an unique ID to identify the shares individually
        initializeData(stock,GENERAL); //the first time the program is opened, the data has to be initialized even if the user
         //does not choose the option because otherwise the struct will have random characters from the buffer
        system("COLOR 70");
        title();

        do{
                printf("\n\t\t\t\tMAIN MENU \n");

                printf("\n\t[1] Initialize database.\n");
                printf("\n\t[2] Save database to a file.\n");
                printf("\n\t[3] Read database from a file.\n");
                printf("\n\t[4] Buy shares.\n");
                printf("\n\t[5] Sell shares.\n");
                printf("\n\t[6] Modify information of an specific share.\n");
                printf("\n\t[7] Print a report of current shares owned.\n");
                printf("\n\t[8] Print a report of all sold shares.\n");
                printf("\n\t[9] Visualize stocks.\n");
                printf("\n\t[10] Exit the program.\n\n");

                printf("\n\tIntroduce an option to continue: ");
                fflush(stdin);
                scanf("%i",&option);

                switch(option){
                case 1: initializeData(stock, GENERAL);
                        printf("\n\tThe database has been initialized.\n");
                        toContinue();
                        break;

                case 2: saveData(stock,GENERAL);
                        toContinue();
                        break;

                case 3: readData(stock, GENERAL);
                        toContinue();
                        break;

                case 4: buyShare(stock, GENERAL);
                        toContinue();
                        break;

                case 5: sellShare(stock, GENERAL);
                        toContinue();
                        break;

                case 6: modifyShare(stock, GENERAL);
                        toContinue();
                        break;

                case 7: reportShares(stock, GENERAL);
                        toContinue();
                        break;

                case 8: reportsoldShares(stock, GENERAL);
                        toContinue();
                        break;

                case 9: printbyAll(stock, GENERAL);
                        toContinue();
                        break;

                case 10: printf("\n\tYou have successfully exited the program.\n");
                        break;

                default:
                        printf("\n\tPlease choose an option from above.\n");
                        toContinue();
                }

                system(CLEAR);

        }while(option!=EXIT);
return 0;
}

void toContinue(){
        printf("\n\n\tPress any key to be redirected to the main menu.\n");
        _getch();
}

void printbasicColumns(){
		printf(" ----------------------------------------------------------------------------------------------------------------\n");
		printf(" %5s%19s%19s%15s%20s%20s\n", "ID", "TYPE", "COMPANY", "SHARES", "PURCHASE PRICE", "PURCHASE DATE");
		printf(" -----------------------------------------------------------------------------------------------------------------\n");
}

void title(){
	printf("\n     ======================================================================\n");
	printf("\t\t\t     STOCK TRACKER\n");
	printf("\t   Welcome to the best program to manage your stocks\n");
	printf("\t\t    C programming final project\n");
	printf("     ======================================================================\n");

	putchar('\n');
	for (int i=0; i < 80; i++) {
		putchar('_');
	}
}

bool checkrep (int n, int identifier){
        bool repeated = false;
        for(int i=0;i<GENERAL;i++){
                if (n == identifier){
                        repeated = true;
                }
        }
return repeated;
}

void setIdentifiers(struct stock_tracker *stock, size_t size){
        srand(time(NULL));
        int n;
        int i=0;

        while(i<size){
                do{
                        n = 1000 + rand()%(10000-1);
                } while (checkrep(n,stock[i].identifier)); //calls the function to check if the number has been produced before
                stock[i].identifier = n;
                i++;
        }
}

void deleteSpace (char *word){
        word[strlen(word)-1] = 0;
}

bool correctType (char *stock_type){
        char common[] = "common", preferred[] = "preferred", growth[] = "growth";
        char income[] = "income", bluechips[]= "blue-chips";
        bool incorrect = true;

        //checking if the input is the same as the types
        if((strcmp(stock_type, common) == 0) || (strcmp(stock_type,preferred) == 0) || (strcmp(stock_type, growth) == 0) || (strcmp(stock_type, income) == 0 ) || (strcmp(stock_type, bluechips) == 0)){
                incorrect = false;
        }
return incorrect;
}

void displayList(struct stock_tracker *stock, size_t size){
        char checklist;

        printf("\n\tDo you wish to check all the list before? Y/N: ");
        fflush(stdin);
        scanf("%c", &checklist);
        if (checklist == 'Y' || checklist == 'y'){
            printEverything(stock, GENERAL);
            printf("\n\tPress any key to come back.");
            _getch();
            system(CLEAR);
            title();
        }
}

void initializeData(struct stock_tracker *stock, size_t size){
        //sets all the values to 0
        char cero[]= "0";
        int intCero = 0;
        for(int i=0;i<size;i++){
                strcpy(stock[i].description, cero);
                strcpy(stock[i].name, cero);
                stock[i].shares = intCero;
                stock[i].available = false; //all the shares are categorized as "sold" at the beginning because they have not been bought yet
                stock[i].purchase_price = intCero;
                strcpy(stock[i].purchase_date, cero);
                stock[i].sell_price = intCero;
                strcpy(stock[i].sell_date,cero);
        }
}

void buyShare(struct stock_tracker *stock, size_t size){
        system(CLEAR);
        title();
        int amount, j = 0, i=0;
        bool incorrect = true;
        printf("\n\n\tHow many trades will you enter? ");
        scanf("%i", &amount);
        printf("\tPlease enter the amount %i of trade[s] below:\n", amount);

        while(j < amount && i < size){
                //if(strncasecmp(stock[i].description, "0",2)==0)
                if(strncasecmp(stock[i].description, "0",2)==0) {
                        //  char typestocks[] = {"Common" , "Preferred" , "Growth" , "Income", "Blue-chips"};

                        do{
                                printf("\n\t\tEnter the stock type: ");
                                printf("\n\t\tOptions are: common, preferred, growth, income, blue-chips: ");
                                fflush(stdin);
                                fgets(stock[i].description,SIZE,stdin);
                                deleteSpace(stock[i].description);
                                incorrect = correctType(stock[i].description); //checking that user chooses one of the options
                                if(incorrect){
                                        printf("\n\tPlease type one of the types. ");
                                }
                        } while (incorrect);

                        printf("\t\tEnter the company's name: "); //input can not be checked here because there is no limitation of companies
                        fflush(stdin);
                        fgets(stock[i].name,SIZE,stdin);
                        deleteSpace(stock[i].name);

                        printf("\t\tEnter quantity: ");
                        while (scanf("%i",&stock[i].shares)==0){ //user inputs a int
                                printf("\t\tInvalid input. Please enter quantity: ");
                                scanf("%*s");
                        }

                        stock[i].available = true;

                        printf("\n\t\tEnter total purchase price: "); //user inputs a float
                        while (scanf("%f",&stock[i].purchase_price)==0){
                                printf("\t\tInvalid input. Please enter total purchase price: ");
                                scanf("%*s");
                        }

                        printf("\n\t\tEnter current date (day/month/year) ");
                        fflush(stdin);
                        fgets(stock[i].purchase_date,SIZE,stdin);
                        deleteSpace(stock[i].purchase_date);

                        printf("\n\t\tTrade %i has been saved with ID %i.\n\n", j +1 ,stock[i].identifier);

                        j++;
                }
                i++;
        }
}

void sellShare(struct stock_tracker *stock, size_t size){
        int ID, repeat = 1;
        char answer;
        system(CLEAR);
        title();
        bool found = true;
        do{
            printf("\n\n\tTo sell a share you need to provide the ID number.");
            displayList(stock,size);
            printf("\n\n\tIdentify the sold stock by providing the ID: ");
            scanf("%i",&ID);
            for(int i=0;i<size;i++){
                    if(stock[i].identifier == ID && stock[i].available == true){
                            printf("\n\tEnter the price that the stock was sold at: ");
                            fflush(stdin);
                            scanf("%f", &stock[i].sell_price);
                            printf("\n\tEnter the date it was sold: ");
                            fflush(stdin);
                            fgets(stock[i].sell_date,SIZE,stdin);
                            deleteSpace(stock[i].sell_date);
                            stock[i].available = false;
                            found = false;
                    }
            }

            if(found){
                printf("\n\tThe ID %i does not match to any of the stocks owned.\n", ID);

            }
            printf("\n\tDo you wish to keep entering sold stocks? [Y/N]: ");
            fflush(stdin);
            scanf("%c",&answer);

            if(answer== 'N'||answer =='n'){
                repeat = 0;
            }

        }while(repeat);
}


void modifyShare(struct stock_tracker *stock,size_t size){
        int ID, repeat = 1, option, intCero = 0;
        char answer, status, checklist, cero[]= "0";
        bool incorrect = true;

        system(CLEAR);
        title();
        bool found = true;
        do{
            printf("\n\n\tTo modify a share you need to provide the ID number.");
            displayList(stock,size);
            printf("\n\n\tIdentify the stock to modify by providing the ID: ");
            scanf("%i",&ID);
            for(int i=0;i<size;i++){
                    if(stock[i].identifier == ID){
                            printf("\n\tPlease select one of the options below:\n");
                            printf("\n\t[1]Edit the type\n");
                            printf("\n\t[2]Edit the company's name\n");
                            printf("\n\t[3]Edit the amount of shares\n");
                            printf("\n\t[4]Edit the purchase price\n");
                            printf("\n\t[5]Edit the purchase date\n");
                            printf("\n\t[6]Edit the selling price\n");
                            printf("\n\t[7]Edit the selling date\n");
                            printf("\n\t[8]Delete the share\n");
                            printf("\n\t");
                            fflush(stdin);
                            scanf("%i", &option);
                            switch(option){
                                case 1: printf("\n\tStock's current stock type: %s\n",stock[i].description);

                                        do{
                                            printf("\n\t\tEnter the stock type: ");
                                            printf("\n\t\tOptions are: common, preferred, growth, income, blue-chips: ");
                                            fflush(stdin);
                                            fgets(stock[i].description,SIZE,stdin);
                                            deleteSpace(stock[i].description);
                                            incorrect = correctType(stock[i].description); //checking that user chooses one of the options
                                            if(incorrect){
                                                    printf("\n\tPlease type one of the types. ");
                                            }
                                        } while (incorrect);

                                        break;

                                case 2: printf("\n\tStock's current company's name: %s\n",stock[i].name);
                                        printf("\n\tEnter new company: ");
                                        fflush(stdin);
                                        fgets(stock[i].name,SIZE,stdin);
                                        deleteSpace(stock[i].name);
                                        break;

                                case 3: printf("\n\tStock's current amount of shares: %i\n",stock[i].shares);
                                        printf("\t\tEnter quantity: ");
                                        while (scanf("%i",&stock[i].shares)==0){ //user inputs a int
                                                printf("\t\tInvalid input. Please enter quantity: ");
                                                scanf("%*s");
                                        }
                                        break;

                                case 4: printf("\n\tStock's current purchase price: %f\n",stock[i].purchase_price);
                                        while (scanf("%f",&stock[i].purchase_price)==0){
                                                printf("\t\tInvalid input. Please enter total purchase price: ");
                                                scanf("%*s");
                                        }
                                       break;

                                case 5: printf("\n\tStock's current purchase date: %s\n",stock[i].purchase_date);
                                        printf("\n\tEnter new purchase date: ");
                                        fflush(stdin);
                                        fgets(stock[i].purchase_date,SIZE,stdin);
                                        deleteSpace(stock[i].purchase_date);
                                        break;

                                case 6: printf("\n\tStock's current selling price: %f\n",stock[i].sell_price);
                                         while (scanf("%f",&stock[i].sell_price)==0){
                                            printf("\t\tInvalid input. Please enter total selling price: ");
                                            scanf("%*s");
                                        }
                                        break;

                                case 7: printf("\n\tStock's current selling date: %s\n",stock[i].sell_date);
                                        printf("\n\tEnter new selling date: ");
                                        fflush(stdin);
                                        fgets(stock[i].sell_date,SIZE,stdin);
                                        deleteSpace(stock[i].sell_date);
                                        break;

                                case 8: strcpy(stock[i].description, cero);
                                        strcpy(stock[i].name, cero);
                                        stock[i].shares = intCero;
                                        stock[i].available = false;
                                        stock[i].purchase_price = intCero;
                                        strcpy(stock[i].purchase_date, cero);
                                        stock[i].sell_price = intCero;
                                        strcpy(stock[i].sell_date,cero);
                                        printf("\n\tStock has been deleted.");
                                        break;
                            }
                            found = false;
                    }
            }

            if(found){
                printf("\n\tThe ID %i does not match to any of the stocks owned.\n", ID);

            }
            printf("\n\tDo you wish to keep editing stocks? [Y/N] ");
            fflush(stdin);
            scanf("%c",&answer);

            if(answer== 'N'||answer =='n'){
                repeat = 0;
            }
        }while(repeat);
}


void reportShares(struct stock_tracker *stock, size_t size){
        system(CLEAR);
        title();

        float total_price = 0;

        printf("\n\t\t    ==> LIST OF ALL THE STOCK CURRETLY OWNED <==\n");
        printbasicColumns();

        for(int i=0;i<size;i++){
                if(stock[i].available == true){
                        printf(" %5i%19s%19s%15i%20.2f%20s\n",stock[i].identifier, stock[i].description,
                        stock[i].name,stock[i].shares, stock[i].purchase_price,stock[i].purchase_date);
                        total_price = stock[i].purchase_price + total_price;
                }
        }
        printf(" -----------------------------------------------------------------------------------------------------------------\n");

        if(total_price == 0){
                printf("\t\t\t\t\tNO STOCKS ARE OWNED");
        } else {
                printf("\t\t\t\t\t\t\t\t\tTOTAL PRICE %.2f", total_price);
        }
}


void printEverything(struct stock_tracker *stock, size_t size){
        system(CLEAR);
        title();
        int i = 0;

        printf("\n\t\t\t    ==> LIST OF ALL THE STOCK <==\n");
        printf(" ----------------------------------------------------------------------------------------------------------------\n");
        printf("  %-5s%13s%19s%15s%20s%20s%15s%15s\n", "ID", "TYPE", "COMPANY", "SHARES", "PURCHASE PRICE", "PURCHASE DATE", "SELL PRICE", "SELL DATE");
        printf(" -----------------------------------------------------------------------------------------------------------------\n");

        for(int i=0;i<size; i++){
                if(stock[i].shares != 0){
                    printf("  %-5i%13s%19s%15i%20.2f%20s%15.2f%15s\n",stock[i].identifier, stock[i].description,
                    stock[i].name,stock[i].shares, stock[i].purchase_price,stock[i].purchase_date,
                    stock[i].sell_price, stock[i].sell_date);
                }
        }
}


void printbyType(struct stock_tracker *stock, size_t size){

        char chosen_type[SIZE];
        bool incorrect = true;

        do{
            printf("\n\t\tEnter the stock type ");
            printf("\n\tOptions are: common, preferred, growth, income, blue-chips\n\t");
            fflush(stdin);
            fgets(chosen_type,size,stdin);
            deleteSpace(chosen_type);
            incorrect = correctType(chosen_type);
            if(incorrect){
                printf("\n\tPlease choose one of the the types above. ");
            }
        } while (incorrect);

        printf("\n\t\t    ==> LIST BY TYPE OF STOCKS <==\n");
        printbasicColumns();

        for(int i=0;i<size;i++){
            if(strcmp(chosen_type, stock[i].description) == 0){
                printf(" %5i%19s%19s%15i%20.2f%20s\n",stock[i].identifier, stock[i].description,
                stock[i].name,stock[i].shares, stock[i].purchase_price,stock[i].purchase_date);
            }
        }
}
void printbyCompany(struct stock_tracker *stock, size_t size){
        bool incorrect = true;
        char company_name[SIZE];
        char answer;

        do{
            printf("\n\t\tEnter company's name ");
            fflush(stdin);
            fgets(company_name,SIZE,stdin);
            deleteSpace(company_name);

            printf("\n\t\t    ==> LIST BY TYPE OF COMPANY <==\n");
            printbasicColumns();

            for(int i=0;i<size;i++){
                if(strcmp(stock[i].name, company_name) == 0) {
                    incorrect = false;
                    printf("%5i%15s%19s%15i%20.2f%20s\n",stock[i].identifier, stock[i].description,
                    stock[i].name,stock[i].shares, stock[i].purchase_price,stock[i].purchase_date);
                }
            }

            if(incorrect == true){
                printf("\n\t%s has not been found. Do you wish to keep trying? Y/N : ", company_name);
                fflush(stdin);
                scanf("%c", &answer);
                if(answer == 'N' || answer == 'n'){
                    incorrect = false;
                }
            }
        } while (incorrect);
}

void printbyAll(struct stock_tracker *stock, size_t size){
        int option;
        system(CLEAR);
        title();

        printf("\n\tChoose how do you want to print the information: \n");
        printf("\n\t [1] By type of stock.");
        printf("\n\t [2] By company.");
        printf("\n\t [3] All the information");
        fflush(stdin);
        printf("\n\t");
        scanf("%i", &option);

        if(option == 1) {
            printbyType(stock, size);
        } else if (option == 2){
            printbyCompany(stock, size);
        } else if (option == 3) {
            printEverything(stock,size);
        } else {
            printf("\n\tPlease select an option.");
        }
}

/*Print a report of all sold shares. Display profit/loss per sale. Print total profit/losses at the end of the report*/
void reportsoldShares(struct stock_tracker *stock, size_t size){
        system(CLEAR);
        title();
        float diference [GENERAL];

        float total_purchase = 0, total_selling =0, total_money=0;

        printf("\n\t\t    ==> REPORT OF ALL THE SOLD SHARES <==\n");
		printf(" ----------------------------------------------------------------------------------------------------------------\n");
		printf("%5s%15s%19s%15s%20s%19s%15s\n", "ID", "TYPE", "COMPANY", "SHARES", "PURCHASE PRICE", "SELLING PRICE", "DIFERENCE");
		printf(" -----------------------------------------------------------------------------------------------------------------\n");

        for(int i=0;i<size;i++){
            if(stock[i].available == false && stock[i].sell_price != CERO){
                    total_purchase = total_purchase + stock[i].purchase_price;
                    total_selling = total_selling + stock[i].sell_price;

                    diference[i] = stock[i].sell_price - stock[i].purchase_price;

                    printf("%5i%15s%19s%15i%20.2f%19.2f%15.2f\n",stock[i].identifier, stock[i].description,
                    stock[i].name,stock[i].shares, stock[i].purchase_price,stock[i].sell_price, diference[i]);
            }

        }

        if(total_purchase == 0){
                printf("\t\t\t\t\tNO STOCKS HAVE BEEN SOLD YET");
        } else {
                printf("\t\t\t\t\tTOTAL PURCHASE %.2f\t TOTAL SELLING %.2f", total_purchase, total_selling);
                total_money = total_selling - total_purchase;
                if(total_money<0){
                    printf("\t  TOTAL LOSSES %.2f", total_money);
                } else {
                    printf("\t  TOTAL PROFIT %.2f", total_money);
                }
        }
}

//print in a file a report about all the shares that have been ever owned
void saveData(struct stock_tracker *stock,size_t size){
        FILE *file;
        FILE *filetoread;
        char answer;
        int i = 0;
        float diference [GENERAL], total_purchase = 0, total_selling =0, total_money=0;

        file = fopen("Database.txt", "w+");
        filetoread = fopen("Toread.txt", "w+");

        printf("\n\tDo you wish to save all the information in a text file? Y/N: ");
        fflush(stdin);
        scanf("%c", &answer);
        if(answer == 'N' || answer == 'n'){
            printf("\n\tNothing will be saved.");
        } else if (answer == 'Y' ||answer == 'y'){
            if(file == NULL){
                printf("\n\tFile could not be opened");
            }

            fprintf(file, "\n\t\t    ==> GENERAL REPORT OF ALL THE SHARES <==\n");
            fprintf(file," ----------------------------------------------------------------------------------------------------------------\n");
            fprintf(file, "%5s%15s%19s%15s%20s%20s%15s\n", "ID", "TYPE", "COMPANY", "SHARES", "PURCHASE PRICE", "SELLING PRICE", "DIFERENCE");
            fprintf(file, " ----------------------------------------------------------------------------------------------------------------\n");

            for(int i=0;i<size;i++){
                if(stock[i].shares != CERO){
                            total_purchase = total_purchase + stock[i].purchase_price;
                            if(stock[i].sell_price != CERO){
                                    total_selling = total_selling + stock[i].sell_price;
                                    diference[i] = stock[i].sell_price - stock[i].purchase_price;
                                    total_money = diference[i] + total_money;
                            }
                    fprintf(file, "%5i%15s%19s%15i%20.2f%19.2f%15.2f\n",stock[i].identifier, stock[i].description,
                    stock[i].name,stock[i].shares, stock[i].purchase_price,stock[i].sell_price, diference[i]);

                    fprintf(filetoread, STOCK_FORMAT_OUT,stock[i].identifier, stock[i].description, stock[i].name,stock[i].shares,
                             stock[i].purchase_price, stock[i].purchase_date ,stock[i].sell_price, stock[i].sell_date);
                }
            }
            fprintf(file, "----------------------------------------------------------------------------------------------------------------");
            fprintf(file, "\n\t\t\t\t\t\t\tTOTAL PURCHASE %.2f\t TOTAL SELLING %.2f", total_purchase, total_selling);

            if(total_money<0){
                fprintf(file,"\tTOTAL LOSSES %.2f", total_money);
            } else {
                fprintf(file, "\tTOTAL PROFIT %.2f", total_money);
            }
            fclose(file);
            fclose(filetoread);
            printf("\n\tInformation was successfully written in file named 'Database.txt'.");
        }
}

void readData (struct stock_tracker *stock,size_t size){
        FILE *filetoread;
        filetoread = fopen("Toread.txt", "r");
        int i=0;

        if(filetoread == NULL){
            printf("\n\tFile could not be read because no information has been saved yet.");
        } else {

            char buffer [GENERAL]; //buffer is where the line will be saved temporally
            fgets(buffer, GENERAL, filetoread);
            deleteSpace(buffer); //deleting the last \n from the life
            while(!feof(filetoread)){
                sscanf(buffer, STOCK_FORMAT_IN, &stock[i].identifier, stock[i].description, stock[i].name, &stock[i].shares,
                        &stock[i].purchase_price, stock[i].purchase_date , &stock[i].sell_price, stock[i].sell_date);
                        //the strings do not need & because they are already pointers
                fgets(buffer,GENERAL,filetoread);
                deleteSpace(buffer);
                i++;
            }

            int n=i;
            for(i = 0; i<n; i++){
                    if(strcasecmp(stock[i].sell_date,"0") == 0){
                        stock[i].available = true; //making the stock be "not sold" again
                    }
            }
            printf("\n\tData has been read successfully.");
            fclose(filetoread);
        }

}
