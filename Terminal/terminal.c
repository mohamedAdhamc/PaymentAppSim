#include <stdio.h>
#include "terminal.h"
#include <stdlib.h>
#include <string.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
    uint8_t transactionDateInput[11];
    for (int i = 0; i<11; ++i)
        transactionDateInput[i] = ' ';

    printf("%s", "Enter Transaction Date: ");
    scanf("%s", transactionDateInput);
    //check if wrong format
    if (transactionDateInput[2] != '/' || transactionDateInput[5] != '/')
        return WRONG_DATE;
    //check if less than 10 chars
    if (transactionDateInput[9] == ' ')
        return WRONG_DATE;
    //if more than 10 clear the input buffer
    if (transactionDateInput[10] != ' ' && transactionDateInput[10] != '\0'){
        while ((getchar()) != '\n');
        return WRONG_DATE;
    }
    //TO DO check if there are non numbers
    for (int i = 0; i<11; ++i)
        termData->transactionDate[i] = transactionDateInput[i];
    return TERMINAL_OK;
}

void getTransactionDateTest(void) {
    ST_terminalData_t* testTermData = malloc(sizeof(ST_terminalData_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: getCardPAN \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 25-05-2020 \n", "Expected Result: 1 (WRONG_DATE) \n"); 
    printf("%s: %u  \n\n", "result", getTransactionDate(testTermData));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 25/5/2020 \n", "Expected Result: 1 (WRONG_DATE) \n");
    printf("%s: %u  \n\n", "result", getTransactionDate(testTermData));

    printf("%s%s%s", "Test Case 4: \n", "Input Data: 25/05/2020 \n", "Expected Result: 0 (TERMINAL_OK) \n");
    printf("%s: %u  \n\n", "result", getTransactionDate(testTermData));

    free(testTermData);
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData) {
    // if card expiry date is before transaction date return EXPIRED_CARD
    // card datas date MM/YY and term datas date is DD/MM/YYYY
    // Assuming cards expiry date is last day of the month written -- only have to compare years and months
    char yearCard [] = {cardData->cardExpirationDate[3], cardData->cardExpirationDate[4], '\0'};
    char yearTerm [] = {termData->transactionDate[8], termData->transactionDate[9], '\0'};
    if (atoi(yearCard) < atoi(yearTerm))
        return EXPIRED_CARD;
    else if (atoi(yearCard) == atoi(yearTerm)){
    char monthCard [] = {cardData->cardExpirationDate[0], cardData->cardExpirationDate[1], '\0'};
    char monthTerm [] = {termData->transactionDate[3], termData->transactionDate[4], '\0'};
    if (atoi(monthCard) < atoi(monthTerm))
        return EXPIRED_CARD;
    }
    return TERMINAL_OK;
}

void isCardExpriedTest(void){
    ST_terminalData_t* testTermData = malloc(sizeof(ST_terminalData_t));
    ST_cardData_t* testCardData = malloc(sizeof(ST_cardData_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: isCardExpired \n");

    strcpy(testTermData->transactionDate, "25/05/2020");
    strcpy(testCardData->cardExpirationDate, "05/20");
    printf("%s%s%s", "Test Case 1: \n", "Input Data: terminal = 25/05/2020  card = 05/20 \n", "Expected Result: 0 (TERMINAL_OK) \n"); 
    printf("%s: %u  \n\n", "result", isCardExpired(testCardData, testTermData));

    strcpy(testCardData->cardExpirationDate, "06/20");
    printf("%s%s%s", "Test Case 2: \n", "Input Data: terminal = 25/05/2020  card = 06/20 \n", "Expected Result: 0 (TERMINAL_OK) \n");
    printf("%s: %u  \n\n", "result", isCardExpired(testCardData, testTermData));

    strcpy(testCardData->cardExpirationDate, "04/20");
    printf("%s%s%s", "Test Case 4: \n", "Input Data: terminal = 25/05/2020  card = 04/20 \n", "Expected Result: 2 (EXPIRED_CARD) \n");
    printf("%s: %u  \n\n", "result", isCardExpired(testCardData, testTermData));

    free(testTermData);
    free(testCardData);
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
    printf("%s", "Enter Transaction Amount: ");
    float transActionAmount = 0;
    scanf("%f", &transActionAmount);
    if (transActionAmount <= 0.0)
        return INVALID_AMOUNT;
    termData->transAmount = transActionAmount;
    return TERMINAL_OK;
}

void getTransactionAmountTest(void) {
    ST_terminalData_t* testTermData = malloc(sizeof(ST_terminalData_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: getTransactionAmount \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 300 \n", "Expected Result: 0 (TERMINAL_OK) \n"); 
    printf("%s: %u  \n\n", "result", getTransactionAmount(testTermData));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 0 \n", "Expected Result: 4 (INVALID_AMOUNT) \n");
    printf("%s: %u  \n\n", "result", getTransactionAmount(testTermData));

    printf("%s%s%s", "Test Case 4: \n", "Input Data: -200 \n", "Expected Result: 4 (INVALID_AMOUNT) \n");
    printf("%s: %u  \n\n", "result", getTransactionAmount(testTermData));

    free(testTermData);
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
    if (termData->transAmount > termData->maxTransAmount)
        return EXCEED_MAX_AMOUNT;
    return TERMINAL_OK;
}

void isBelowMaxAmountTest(void) {
    ST_terminalData_t* testTermData = malloc(sizeof(ST_terminalData_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: isBelowMaxAmount---- assume max amount of 500 \n");
    testTermData->maxTransAmount = 500;
    testTermData->transAmount = 300;

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 300 \n", "Expected Result: 0 (TERMINAL_OK) \n"); 
    printf("%s: %u  \n\n", "result", isBelowMaxAmount(testTermData));
    testTermData->transAmount = 500;

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 500 \n", "Expected Result: 0 (TERMINAL_OK) \n");
    printf("%s: %u  \n\n", "result", isBelowMaxAmount(testTermData));
    testTermData->transAmount = 501;

    printf("%s%s%s", "Test Case 4: \n", "Input Data: 501 \n", "Expected Result: 5 (EXCEED_MAX_AMOUNT) \n");
    printf("%s: %u  \n\n", "result", isBelowMaxAmount(testTermData));

    free(testTermData);
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount){
    if (maxAmount <= 0)
        return INVALID_MAX_AMOUNT;
    termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;
}

void setMaxAmountTest(void) {
    ST_terminalData_t* testTermData = malloc(sizeof(ST_terminalData_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: getTransactionAmount \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 300 \n", "Expected Result: 0 (TERMINAL_OK) \n"); 
    printf("%s: %u  \n\n", "result", setMaxAmount(testTermData, 500));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 0 \n", "Expected Result: 6 (INVALID_MAX_AMOUNT) \n");
    printf("%s: %u  \n\n", "result", setMaxAmount(testTermData, 0));

    printf("%s%s%s", "Test Case 4: \n", "Input Data: -200 \n", "Expected Result: 6 (INVALID_MAX_AMOUNT) \n");
    printf("%s: %u  \n\n", "result", setMaxAmount(testTermData, -200));

    free(testTermData);
}


// int main () {

// }