/*CSI3131 Assignment 2 Q1 using Windows Threads. 300241608*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

DWORD Split(LPVOID num);

BOOL isPrime(DWORD num);

/*Approach is to check all numbers up to n/2, then substract the prime ones from n and check if the result is prime*/

int main(){ 
    DWORD ThreadId;
    DWORD ourNumber;
    DWORD *result;

    printf("Enter a number!\n");
    scanf("%d", &ourNumber); //take input 

    if(ourNumber%2 != 0 || ourNumber < 0){ 
        printf("Non-even or negative number inputted! Bye!");
        exit(0);
    }
    else{
        HANDLE thread = CreateThread(NULL, 0, Split, &ourNumber, 0, &ThreadId); // create thread
        WaitForSingleObject(thread, INFINITE); // wait for thread to finish
        GetExitCodeThread(thread, (LPDWORD) result); // return value will be our answer
        CloseHandle(thread); // close thread
    }
    printf("Finished! Thread ID was %d", ThreadId);
    return 0; 
}
 
DWORD WINAPI Split(LPVOID num){  
    DWORD boop = *(DWORD*)num;
    DWORD* list = malloc(sizeof(DWORD)*((boop/2) + 1));
    for(int i = 1; i<(boop/2)+1; i++){
        list[i] = i; 
    }
    for(int i = 3; i<(boop/2)+1; i++){ // starting from 3 cause an even number - 2 will still be even 
        if(!isPrime(list[i])){
            continue; 
        }
        if(isPrime(boop-list[i])){
            printf("%d+%d\n", list[i], boop-list[i]);
        }
    }
    return 0; 
}


BOOL WINAPI isPrime(DWORD num){ // basic prime number check 
    if(num <= 1){
        return 0;
    }
    for(int i = 2; i<=num/2; i++){
        if(num%i == 0){
            return 0;  
        }
    }
    return 1; 
}




