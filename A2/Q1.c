#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

DWORD* Sundaram(DWORD size);

DWORD Split(LPVOID num);

BOOL isPrime(DWORD num);
/*Approach is to get all prime numbers up to n, 
then subtract n with those prime numbers and check if the result is prime, 
if no then keep going, if yes then we have our answer.*/

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
    printf("%d+%d", *result, ourNumber-*result); // print our final answer :)
    return 0; 
}
 
DWORD WINAPI Split(LPVOID num){ // our main function that ties it all together
    DWORD boop = *(DWORD*)num;
    DWORD *list = Sundaram(boop);
    for(int j = 1; j<boop; j++){ 
        if(list[j] == -1){
            continue;
        }
        else{
            int x = boop - list[j];
            if(isPrime(x) == 1){
                return x;
            }
        }
    }
}

DWORD* Sundaram(DWORD size){ // sieve of sundaram finds all the prime numbers before up to n
    DWORD newSize = (size-2)/2;
    DWORD *numbers = malloc(sizeof(DWORD) * newSize); // malloc so we can use this in Split
    for(DWORD i = 1; i<newSize; i++){ // make our list of numbers of 1 to (n-2)/2
        numbers[i] = i;
    }
    for(DWORD i = 1; i<newSize; i++){
        for(DWORD j = i; i+j+2*i*j <= newSize; j++){ // remove the numbers where i+j + 2ij <= (n-2)/2, -1 is convenient because a prime number cannot be negative.
            numbers[i+j+2*i*j] = -1; 
        }
    }
    for(DWORD i = 1; i<newSize; i++){
        if(numbers[i] == -1){
            continue;
        }
        else{
            numbers[i] = numbers[i]*2 + 1; // final part of sieve
        }
    }
    return numbers;
}

BOOL WINAPI isPrime(DWORD num){ // basic prime number check 
    if(num <= 1){
        return 0;
    }
    for(int i = 2; i<=sqrt(num); i++){
        if(num%i == 0){
            return 0;  
        }
    }
    return 1; 
}