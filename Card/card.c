#include"card.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
    printf( "%s","Please Enter The Card Holder Name: ");
    uint8_t name_input[25];
    //init with spaces
    for (int i = 0; i< 25; ++i)
        name_input[i] = ' ';
    fgets(name_input, sizeof(name_input), stdin);

    //test if input is null
    short int isNull = 1;
    for(int i = 0; i<25; ++i)
        if (name_input[i] != ' '){
            isNull = 0;
            break;
        }
    //test if input is less than 20 characters
    short int isLessThanTwenty = 1;
     for(int i = 19; i<25; ++i)
        if (name_input[i] != ' '){
            isLessThanTwenty = 0;
            break;
        }
    short int isMoreThanTwentyFour = name_input[24] != ' ' ? 1 : 0;

    if (isMoreThanTwentyFour)
        while ((getchar()) != '\n');


    if (isNull || isLessThanTwenty || isMoreThanTwentyFour)
        return WRONG_NAME ;
    strcpy(cardData->cardHolderName, name_input);
    return CARD_OK;
}

void getCardHolderNameTest(void) {
    ST_cardData_t* testCard = malloc(sizeof(ST_cardData_t));

    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: getCardHolderName \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: Mohamed Adham Mohamed Nagiub (28) \n", "Expected Result: 1 (WRONG_NAME) \n");
    printf("%s: %u  \n\n", "result", getCardHolderName(testCard));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: Mohamed Adham Mohamed (21) \n", "Expected Result: 0 (CARD_OK) \n");
    printf("%s: %u  \n\n", "result", getCardHolderName(testCard));

    printf("%s%s%s", "Test Case 3: \n", "Input Data: Mohamed Adham (13) \n", "Expected Result: 1 (WRONG_NAME) \n");
    printf("%s: %u  \n\n", "result", getCardHolderName(testCard));

    printf("%s%s%s", "Test Case 3: \n", "Input Data: (0) \n", "Expected Result: 1 (WRONG_NAME) \n");
    printf("%s: %u  \n\n", "result", getCardHolderName(testCard));

}

EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
    printf( "%s","Please Enter The Card Holder PAN: ");
    uint8_t pan_input[20];
    //init with spaces
    for (int i = 0; i< 20; ++i)
        pan_input[i] = ' ';
    fgets(pan_input, sizeof(pan_input), stdin);

    //test if input is null
    short int isNull = 1;
    for(int i = 0; i<20; ++i)
        if (pan_input[i] != ' '){
            isNull = 0;
            break;
        }
    //test if input is less than 16 characters
    short int isLessThanSixteen = 1;
     for(int i = 15; i<20; ++i) //start from the element number 16 and check if you find any not null then it is not less than 16
        if (pan_input[i] != ' ' && pan_input[i] != '\n' && pan_input[i] != '\0'){
            isLessThanSixteen = 0;
            break;
        }
    short int isMoreThanNineteen = pan_input[19] != ' ' ? 1 : 0;

    if (isMoreThanNineteen)
        while ((getchar()) != '\n');

    if (isNull || isLessThanSixteen || isMoreThanNineteen)
        return WRONG_PAN ;
    strcpy(cardData->primaryAccountNumber, pan_input);
    return CARD_OK;
}

void getCardPANTest(void) {
    ST_cardData_t* testCard = malloc(sizeof(ST_cardData_t));

    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: getCardPAN \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 12345678912345 (14) \n", "Expected Result: 3 (WRONG_PAN) \n"); 
    printf("%s: %u  \n\n", "result", getCardPAN(testCard));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 123456789123456789123456789 (27) \n", "Expected Result: 3 (WRONG_PAN) \n");
    printf("%s: %u  \n\n", "result", getCardPAN(testCard));

    printf("%s%s%s", "Test Case 3: \n", "Input Data: (0) \n", "Expected Result: 3 (WRONG_PAN) \n");
    printf("%s: %u  \n\n", "result", getCardPAN(testCard));

    printf("%s%s%s", "Test Case 4: \n", "Input Data: 12345678912345678 (17) \n", "Expected Result: 0 (CARD_OK) \n");
    printf("%s: %u  \n\n", "result", getCardPAN(testCard));

}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf( "%s","Please Enter The Card Expiration Date: ");
    uint8_t expirationInput[6];
    //init with spaces
    for (int i = 0; i<6 ; ++i)
        expirationInput[i] = '&';
    scanf("%6s", expirationInput);
    //check for wrong format
    if (expirationInput[2] != '/')
        return WRONG_EXP_DATE;
    //check if less than 5 chars -- i.e. fifth char is null or nothing
    if (expirationInput[4] == '%')
        return WRONG_EXP_DATE;
    //check if more than 5 chars
    if (expirationInput[5] != '%' && expirationInput[5] != 0){
        printf("%c", expirationInput[5]);
        while ((getchar()) != '\n'); //clear the buffer
        return WRONG_EXP_DATE;
    }
    while ((getchar()) != '\n'); //clear the buffer
    strcpy(cardData->cardExpirationDate, expirationInput);
    return CARD_OK;
}

void getCardExpiryDateTest(void) {
    ST_cardData_t* testCard = malloc(sizeof(ST_cardData_t));

    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: getCardPAN \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 05-25 (5) \n", "Expected Result: 2 (WRONG_EXP_DATE) \n"); 
    printf("%s: %u  \n\n", "result", getCardExpiryDate(testCard));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 5/25 (4) \n", "Expected Result: 2 (WRONG_EXP_DATE) \n");
    printf("%s: %u  \n\n", "result", getCardExpiryDate(testCard));

    printf("%s%s%s", "Test Case 3: \n", "Input Data: 05/2020 (7) \n", "Expected Result: 2 (WRONG_EXP_DATE) \n");
    printf("%s: %u  \n\n", "result", getCardExpiryDate(testCard));

    printf("%s%s%s", "Test Case 4: \n", "Input Data: 05/25 (5) \n", "Expected Result: 0 (CARD_OK) \n");
    printf("%s: %u  \n\n", "result", getCardExpiryDate(testCard));

    free(testCard);
}


// int main (void){
//     getCardExpiryDateTest();
// }