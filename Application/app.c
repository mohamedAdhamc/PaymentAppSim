#include "app.h"
#include <stdio.h>

//declaring the diffrent main structs
ST_cardData_t cardData;
ST_terminalData_t termData;
ST_transaction transData;

void appStart(void){
    //card modue
    getCardHolderName(&cardData);
    getCardExpiryDate(&cardData);
    getCardPAN(&cardData);

    //terminal module
    getTransactionDate(&termData);
    if(isCardExpired(&cardData, &termData) != TERMINAL_OK){
        printf("%s", "Declined Expire Card");
        return;
    }
    getTransactionAmount(&termData);
    setMaxAmount(&termData, 15000);
    if (isBelowMaxAmount(&termData) != TERMINAL_OK){
        printf("%s", "Declined Amount Exceeding Limit");
        return;
    }

    // init transData
    transData.cardHolderData = cardData;
    transData.terminalData = termData;

    //server module
    EN_transState_t serverState =  recieveTransactionData(&transData);
    switch (serverState) {
        case FRAUD_CARD:
                        // printf("%s", "Declined Fraud Card");
                        return;            
        case DECLINED_STOLEN_CARD:
                        printf("%s", "Declined Stolen Card");
                        return;            
        case DECLINED_INSUFFECIENT_FUND:
                        printf("%s", "Declined insufficient Funds");
                        return;            
        case INTERNAL_SERVER_ERROR:
                        printf("%s", "Declined Server Error");
                        return;            
    }
}


int main () {
    while (1){
    appStart();
    printf("\nListing saved transaction...\n");
    listSavedTransactions();
    //clear input buffer
    while ((getchar()) != '\n');
    printf("\n%s", "Program has terminated. Would you like to make another transaction (y(1)/n(0))?\n");
    int con = 1;
    scanf("%d", &con);
    //clear input buffer
    while ((getchar()) != '\n');
    if(con != 1)
        break;
    }
    return 0;
}