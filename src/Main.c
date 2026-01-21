#include <stdio.h>
#include <stdlib.h>
//#include "./GarbageCollector.h"

void begin();
void end();
void begin(){
    printf("Begin\n");
}
void end(){
    printf("End\n");
}
/*
void Main(){
    void *Ptr = NULL;
    Alloc(&Ptr,10,sizeof(char));
}
*/
int main(){
    atexit(end);
    begin();
    printf("Main\n");
    //int a = 1 / 0;
    //Collector_Main(Main,200);
    return 0;
}