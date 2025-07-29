#pragma once
#define StartSize 5
#define ExpandSize 5
#define NULL_PTR (void*)NULL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef struct Vector
{
    u32 size;
    size_t ELEMENT_SIZE;
    u32 Memory_SIZE;
    void *Memory;
} Vector;

Vector NewVector(size_t ELEMENT_SIZE){
    Vector v;
    v.size = 0U;
    v.ELEMENT_SIZE = ELEMENT_SIZE;
    v.Memory_SIZE = StartSize;
    v.Memory = (void*)calloc(v.Memory_SIZE,v.ELEMENT_SIZE);
    printf("init -> Vector.\n");
    printf("Size -> Vector: %i.\n",(int)v.Memory_SIZE);
    return v;
}

void ExpandVector(Vector *v){
    if(v->size>=v->Memory_SIZE){
        printf("Expand Vector.\n");
        u32 NewSize = v->Memory_SIZE+ExpandSize;
        char* NewMemory = (char*)calloc(NewSize,v->ELEMENT_SIZE);
        for(int i = 0;i<v->size;i++){
            if(i<NewSize&&i<v->Memory_SIZE){
                printf("Copied:\n");
                for(int j = 0;j<v->ELEMENT_SIZE;j++){
                    NewMemory[i*v->ELEMENT_SIZE+j] = ((char*)(v->Memory))[i*v->ELEMENT_SIZE+j];
                }
                //Dont Free Pointer -> Copied it
            }else{
                printf("Copy Error:\n");
                printf("Can't Get At Index: %i\n",i);
            }
        }
        //Just Free Pointer -> Copied Pointers not Values
        if(v->Memory) free(v->Memory);
        v->Memory = NULL;
        v->Memory = NewMemory;
        v->Memory_SIZE = NewSize;
    }else{
        printf("Don't Expand Vector: \n");
    } 
}

void CompressVector(Vector *v){
    if(v->size<=(v->Memory_SIZE-ExpandSize)){
        u32 NewSize = v->Memory_SIZE - ExpandSize;
        char* NewMemory = (char*)calloc(v->size,v->ELEMENT_SIZE);
        for(int i = 0;i<v->size;i++){
            printf("Copied:\n");
            for(int j = 0;j<v->ELEMENT_SIZE;j++){
                NewMemory[i*v->ELEMENT_SIZE+j] = ((char*)(v->Memory))[i*v->ELEMENT_SIZE+j];
            }
        }
        if(v->Memory) free(v->Memory);
        v->Memory = NULL;
        v->Memory = NewMemory;
        v->Memory_SIZE = v->Memory_SIZE - ExpandSize;
        printf("Resized Vector: \n");
    }else{
        printf("Don't Expand Vector: \n");
    } 
}

void PushOfVector(Vector *v,char* Item){
    printf("size: %i.\n",(int)v->size);
    printf("Size: %i.\n",(int)v->Memory_SIZE);
    ExpandVector(v);
    if(v->size<v->Memory_SIZE){
        printf("Add Item.\n");
        for(int i = 0;i<v->ELEMENT_SIZE;i++){
            ((char*)v->Memory)[v->size*v->ELEMENT_SIZE+i] = Item[i];
        }
        v->size++;
    }else{
        printf("Still Not Able to Add.\n");
    }
}

void AddOfVector(Vector *v,char* Item,int Index){
    printf("size: %i.\n",(int)v->size);
    printf("Size: %i.\n",(int)v->Memory_SIZE);
    ExpandVector(v);
    if(v->size<v->Memory_SIZE&&Index<=v->size){
        printf("Add Item.\n");
        for(int i = v->size-1;i>=Index;i--){
            printf("Set %i to %i.\n",i+1,i);
            for(int i = 0;i<v->ELEMENT_SIZE;i++){
                ((char*)v->Memory)[i+1] = ((char*)v->Memory)[i];
            }
        }
        printf("Set Inserted Element.\n");
        for(int i = 0;i<v->ELEMENT_SIZE;i++){
            ((char*)v->Memory)[v->size*v->ELEMENT_SIZE+i] = Item[i];
        }
        v->size++;
    }else{
        printf("Still Not Able to Add.\n");
    }
}

void PopTopOfVector(Vector *v){
    if(v->size>0){
        v->size--;
        printf("Poped Top: \n");
        CompressVector(v);
    }else{
        printf("Can't Pop Top: \n");
    }
}

void RemoveOfVector(Vector *v,u32 Index){
    if(Index>=0&&Index<v->size){
        v->size--;
        printf("Removed At Index: %i\n",Index);
        for(int i = Index;i<v->size;i++){
            printf("Copy %i to %i:\n",i+1,i);
            for(int i = v->size-1;i>=Index;i--){
                printf("Set %i to %i.\n",i+1,i);
                for(int i = 0;i<v->ELEMENT_SIZE;i++){
                    ((char*)v->Memory)[i] = ((char*)v->Memory)[i+1];
                }
            }
        }
        CompressVector(v);
    }else{
        printf("Can't Remove At Index: %i\n",Index);
    }
}

void ClearOfVector(Vector *v){
    int Elements = v->size;
    printf("Clear Vector: \n");
    for(int i = Elements;i>0;i--){
        PopTopOfVector(v);
    }
}

void *GetOfVector(Vector *v,int Index){
    if(Index>=0&&Index<v->size){
        return &(((char*)v->Memory)[Index*v->ELEMENT_SIZE]);
    }else{
        printf("Can't Get At Index: %i\n",Index);
    }
    return NULL;
}

void SetOfVector(Vector *v,char *Item,int Index){
    if(Index>=0&&Index<v->size){
        for(int i = 0;i<v->ELEMENT_SIZE;i++){
            ((char*)v->Memory)[Index*v->ELEMENT_SIZE+i] = Item[i];
        }
    }else{
        printf("Can't Set At Index: %i\n",Index);
    }
}

void PrintVector(Vector *v){
    printf("Elements:%i\n",(int)v->size);
    printf("Element Size:%i\n",(int)v->ELEMENT_SIZE);
    for(int i = 0;i<v->size;i++){
        for(int j = 0;j<0;j++){
            printf("%c ",((char*)v->Memory)[i*v->ELEMENT_SIZE+j]);
        }
        printf("Element %i: \n",i);
    }
}

void FreeVector(Vector *v){
    if(v->Memory) free(v->Memory);
    v->Memory = NULL;
    v->ELEMENT_SIZE = (size_t)0ULL;
    v->size = 0U;
    printf("Freed Hole Vector.\n");
}
