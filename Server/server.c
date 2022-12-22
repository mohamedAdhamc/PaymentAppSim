#include "server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

ST_accountsDB_t accountsDB[255] = {{2000.0, RUNNING, "1234567891234567"}, {100000.0, BLOCKED, "1234567891234568"}
                                    , {100000.0, RUNNING, "1234567891234562"}, {0.0, RUNNING, "1234567891234569"}
                                    , {555.0, RUNNING, "1234567891234561"}};
ST_transaction transactionDB[255] = {0};
int latestTransInd = -1;

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t **accountRefrence){
    for (int i = 0; i < 5; ++i)
        if (atoi(accountsDB[i].primaryAccountNumber) == atoi(cardData->primaryAccountNumber)){
            *accountRefrence = &accountsDB[i];
            return SERVER_OK;
        } 
    accountRefrence = NULL;
    return ACCOUNT_NOT_FOUND;
}

void isValidAccountTest (void) {
    ST_cardData_t* testCard = malloc(sizeof(ST_cardData_t));
    ST_accountsDB_t* testDBref = malloc(sizeof(ST_accountsDB_t));

    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: recieveTransactionData \n");

    strcpy(testCard->primaryAccountNumber, "1234567891234561");
    printf("%s%s%s", "Test Case 1: \n", "Input Data: 1234567891234561 (pan) \n", "Expected Result: 0 (SERVER_OK) \n"); 
    printf("%s: %u  \n\n", "result", isValidAccount(testCard, &testDBref));
    strcpy(testCard->primaryAccountNumber, "1234563891234569");

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 1234563891234569 \n", "Expected Result: 3 (ACCOUNT_NOT_FOUND) \n");
    printf("%s: %u  \n\n", "result", isValidAccount(testCard, &testDBref));
    strcpy(testCard->primaryAccountNumber, "1234527891234569");

    printf("%s%s%s", "Test Case 4: \n", "Input Data: 1234527891234569 \n", "Expected Result: 3 (ACCOUNT_NOT_FOUND) \n");
    printf("%s: %u  \n\n", "result", isValidAccount(testCard, &testDBref));

}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence){
    if (accountRefrence->state == RUNNING)
        return SERVER_OK;
    else
        return BLOCKED_ACCOUNT;
}

void isBlockedAccountTest(void) {
    ST_accountsDB_t* testAccountRef = malloc(sizeof(ST_accountsDB_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: isBlockedAccount \n");
    testAccountRef->state = RUNNING;
    printf("%s%s%s", "Test Case 1: \n", "Input Data: an account with a state = running \n", "Expected Result: 0 (SERVER_OK) \n"); 
    printf("%s: %u  \n\n", "result", isBlockedAccount(testAccountRef));
    testAccountRef->state = BLOCKED;
    printf("%s%s%s", "Test Case 2: \n", "Input Data: an account witha  state = blocked \n", "Expected Result: 5 (BLOCKED_ACCOUNT) \n"); 
    printf("%s: %u  \n\n", "result", isBlockedAccount(testAccountRef));
    free(testAccountRef);
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence){
    if (accountRefrence->balance < termData->transAmount)
        return LOW_BALANCE;
    return SERVER_OK;
}

void isAmountAvailableTest(void) {
    ST_accountsDB_t* testAccountRef = malloc(sizeof(ST_accountsDB_t));
    ST_terminalData_t *termData = malloc(sizeof(ST_terminalData_t));
    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: isBlockedAccount \n");
    testAccountRef->balance = 100;
    termData->transAmount = 200;
    printf("%s%s%s", "Test Case 1: \n", "Input Data: account has balance less than transaction amount \n", "Expected Result: 4 (LOW_BALANCE) \n"); 
    printf("%s: %u  \n\n", "result", isAmountAvailable(termData, testAccountRef));
    testAccountRef->balance = 400;
    printf("%s%s%s", "Test Case 2: \n", "Input Data: account has enough balance \n", "Expected Result: 0 (SERVER_OK) \n"); 
    printf("%s: %u  \n\n", "result", isAmountAvailable(termData, testAccountRef));
    free(testAccountRef);
    free(termData);
}

EN_serverError_t saveTransaction(ST_transaction *transData) {
    ++latestTransInd;
    transData->transactionSequenceNumber = 505 + latestTransInd;
    transactionDB[latestTransInd] = *transData;
    return SERVER_OK;
}

void listSavedTransactions(void){
    for (int i = 0; i<= latestTransInd; ++i){
        printf("%s\n", "#########################");
        printf("%s: %u\n", "Transaction Sequence NUmber", transactionDB[i].transactionSequenceNumber);
        printf("%s: %s\n", "Transaction Date", transactionDB[i].terminalData.transactionDate);
        printf("%s: %f\n", "Transaction Amount", transactionDB[i].terminalData.transAmount);
        printf("%s: %d\n", "Transaction State", (int)transactionDB[i].transState);
        printf("%s: %f\n", "Terminal Max Amount", transactionDB[i].terminalData.maxTransAmount);
        printf("%s: %s\n", "Cardholder Name", transactionDB[i].cardHolderData.cardHolderName);
        printf("%s\n", "#########################");
    }
}

void listSavedTransactionsTest(void){
    //fill up transaction data base with random values
    // transactionDB[0].transactionSequenceNumber
    listSavedTransactions();
}

EN_transState_t recieveTransactionData(ST_transaction *transData){
    ST_accountsDB_t *accountRefrence = malloc(sizeof(ST_accountsDB_t));
    if (isValidAccount(&transData->cardHolderData, &accountRefrence) == ACCOUNT_NOT_FOUND)
        return FRAUD_CARD;
    if (isBlockedAccount(accountRefrence) == BLOCKED_ACCOUNT)
        return DECLINED_STOLEN_CARD;
    if (isAmountAvailable(&transData->terminalData, accountRefrence) == LOW_BALANCE)
        return DECLINED_INSUFFECIENT_FUND;
    if (saveTransaction(transData) == SAVING_FAILED)
        return INTERNAL_SERVER_ERROR;

    accountRefrence->balance -= transData->terminalData.transAmount;
    printf("%s: %f\n", "updated balance", accountRefrence->balance);
    return APPROVED;
}

void recieveTransactionDataTest(void) {
    ST_transaction *transDataTest = malloc(sizeof(ST_transaction));
    // transDataTest->cardHolderData = {"Mohamed Adham Mohamed", "1234567891234567", }

    printf("%s%s", "Tester Name: Mohamed Adham \n", "Function Name: recieveTransactionData \n");

    printf("%s%s%s", "Test Case 1: \n", "Input Data: 25-05-2020 \n", "Expected Result: 1 (WRONG_DATE) \n"); 
    printf("%s: %u  \n\n", "result", recieveTransactionData(transDataTest));

    printf("%s%s%s", "Test Case 2: \n", "Input Data: 25/5/2020 \n", "Expected Result: 1 (WRONG_DATE) \n");
    printf("%s: %u  \n\n", "result", recieveTransactionData(transDataTest));

    printf("%s%s%s", "Test Case 4: \n", "Input Data: 25/05/2020 \n", "Expected Result: 0 (TERMINAL_OK) \n");
    printf("%s: %u  \n\n", "result", recieveTransactionData(transDataTest));

    free(transDataTest);
}

int main(void){
    isAmountAvailableTest();
    return 0;
}