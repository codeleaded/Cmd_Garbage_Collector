#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "./StackVector.h"

typedef size_t Size;

typedef struct {
    void *Memory;
    Vector v;
} Garbage;

typedef struct {
    int MaxBlock;
    Vector v;
} Collector;

Collector *std_Collector;

int PtrExistsGarbage(Garbage *g,void **Ptr){
    for(int i = 0;i<g->v.size;i++){
        if(GetOfVector(&(g->v),i)==(*Ptr)){
            return i;
        }
    }
    return 0;
}

int* PtrExistsCollectorC(Collector *c,void **Ptr){
    int out[2] = { -1,-1 };
    for(int i = 0;i<c->v.size;i++){
        Garbage *g = (Garbage*)GetOfVector(&c->v,i);
        int index = PtrExistsGarbage(g,Ptr);
        if(index){
            out[0] = i;
            out[1] = index;
            return out;
        }
    }
    return out;
}
int* PtrExistsCollector(void **Ptr){
    int out[2] = { -1,-1 };
    for(int i = 0;i<std_Collector->v.size;i++){
        Garbage *g = (Garbage*)GetOfVector(&std_Collector->v,i);
        int index = PtrExistsGarbage(g,Ptr);
        if(index){
            out[0] = i;
            out[1] = index;
            return out;
        }
    }
    return out;
}

Garbage NewGarbage(int l,int s){
    Vector v = NewVector(sizeof(void*));
    void *Ptr = calloc(l,s);
    Garbage g = { Ptr,v };
    return g;
}

void RemovePtrOfCollectorC(Collector *c,int *i){
    Garbage *g = (Garbage*)GetOfVector(&c->v,i[0]);
    RemoveOfVector(&g->v,i[1]);
    if(g->v.size<=0){
        if(g->v.Memory) free(g->Memory);
        FreeVector(&(g->v));
        RemoveOfVector(&c->v,i[0]);
    }
}
void RemovePtrOfCollector(int *i){
    Garbage *g = (Garbage*)GetOfVector(&std_Collector->v,i[0]);
    RemoveOfVector(&g->v,i[1]);
    if(g->v.size<=0){
        if(g->v.Memory) free(g->Memory);
        FreeVector(&(g->v));
        RemoveOfVector(&std_Collector->v,i[0]);
    }
}

void ToNullC(Collector *c,void **Ptr){
    int *out = PtrExistsCollectorC(c,Ptr);
    if(out[0]!=-1){
        RemovePtrOfCollectorC(c,out);
    }else{
        printf("Pointer %x doesn't exist!",Ptr);
        if(*Ptr) free(*Ptr);
    }
    *Ptr = NULL;
}
void ToNull(void **Ptr){
    int *out = PtrExistsCollector(Ptr);
    if(out[0]!=-1){
        RemovePtrOfCollector(out);
    }else{
        printf("Pointer %x doesn't exist!",Ptr);
        if(*Ptr) free(*Ptr);
    }
    *Ptr = NULL;
}

void AllocC(Collector *c,void **Ptr,Size l,Size s){
    int *out = PtrExistsCollectorC(c,Ptr);
    if(out[0]!=-1){
        RemovePtrOfCollectorC(c,out);
    }
    Garbage g = NewGarbage(l,s);
    PushOfVector(&g.v,(char*)(*Ptr));
    *Ptr = g.Memory;
    PushOfVector(&c->v,(char*)(&g));
}
void Alloc(void **Ptr,Size l,Size s){
    int *out = PtrExistsCollector(Ptr);
    if(out[0]!=-1){
        RemovePtrOfCollector(out);
    }
    Garbage g = NewGarbage(l,s);
    PushOfVector(&g.v,(char*)(*Ptr));
    *Ptr = g.Memory;
    PushOfVector(&std_Collector->v,(char*)(&g));
}

void AssignmentC(Collector *c,void **Ptr1,void **Ptr2){
    ToNullC(c,Ptr1);
    int *out = PtrExistsCollectorC(c,Ptr2);
    if(out[0]!=-1){
        Garbage *g = (Garbage*)GetOfVector(&c->v,out[0]);
        PushOfVector(&g->v,(char*)(*Ptr1));
    }else{
        printf("Pointer %x doesn't exist!",*Ptr2);
    }
    *Ptr1 = *Ptr2;
}
void Assignment(void **Ptr1,void **Ptr2){
    ToNull(Ptr1);
    int *out = PtrExistsCollector(Ptr2);
    if(out[0]!=-1){
        Garbage *g = (Garbage*)GetOfVector(&std_Collector->v,out[0]);
        PushOfVector(&g->v,(char*)(*Ptr1));
    }else{
        printf("Pointer %x doesn't exist!",*Ptr2);
    }
    *Ptr1 = *Ptr2;
}

Collector* Init_Collector(int MaxBlock){
    Vector v = NewVector(sizeof(Garbage));
    Collector c = { MaxBlock,v };
    return &c;
}

void Delete_CollectorC(Collector *c){
    for(int i = 0;i<c->v.size;i++){
        Garbage *g = (Garbage*)GetOfVector(&c->v,i);
        if(g->Memory) free(g->Memory);
        FreeVector(&g->v);
    }
    FreeVector(&c->v);
}
void Delete_Collector(){
    for(int i = 0;i<std_Collector->v.size;i++){
        Garbage *g = (Garbage*)GetOfVector(&std_Collector->v,i);
        if(g->Memory) free(g->Memory);
        FreeVector(&g->v);
    }
    FreeVector(&std_Collector->v);
}

void Collector_MainC(Collector *c,void (*Func)(),int MaxBlock){
    c = Init_Collector(MaxBlock);
    Func();
    Delete_CollectorC(c);
}
void Collector_Main(void (*Func)(),int MaxBlock){
    std_Collector = Init_Collector(MaxBlock);
    Func();
    Delete_Collector();
}
