/*CSI3131 Assignment 2 Q2 using Windows Threads. 300241608*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

DWORD WINAPI SquareFinder(LPVOID n);

struct Square *globalList; // global variable so everyone can see it

struct Square{
    DWORD x;
    DWORD y; 
};

int main(){
    DWORD ThreadId; 
    DWORD ourNumber;
    DWORD *result;
    printf("Enter a number!\n");
    scanf("%d", &ourNumber);
    if(ourNumber <= 0){
        printf("Number less than or equal to 0 inputted! Exiting!");
        exit(0);
    }
    else{ // create thread, wait then close it.
        HANDLE thread = CreateThread(NULL, 0, SquareFinder, &ourNumber, 0, &ThreadId); 
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }
    printf("Parent thread:\n");
    for(int i = 0; i<ourNumber; i++){ // print our results
        printf("Square %d: (%d,%d)\n", i+1, globalList[i].x, globalList[i].y);
    }
}

DWORD WINAPI SquareFinder(LPVOID n){ // it's a DWORD type and not void because i don't like compiler warnings
    DWORD new  = *(DWORD*) n; // getting the int that's being pointed to by n
    globalList = malloc(new*2*sizeof(int)); // allocate the memory required for the global variable
    globalList[0].x = 0; // the first two squares are always the same...
    globalList[0].y = 1;
    globalList[1].x = 1;
    globalList[1].y = 1;

    for(DWORD i = 2; i < new; i++){ // this is essentially the fibonacci sequence in fancy square form 
        if((i+1)%2 == 1){
            globalList[i].x = 0;
            globalList[i].y = globalList[i-1].x + globalList[i-1].y + globalList[i-2].y;
        }
        if((i+1)%2 == 0){
            globalList[i].x = globalList[i-2].x + globalList[i-2].y;
            globalList[i].y = globalList[i-1].y;
        }
    }
    printf("Child thread:\n" "Length: %d, Width: %d\n", globalList[new-1].y, globalList[new-2].x +globalList[new-2].y);
}



