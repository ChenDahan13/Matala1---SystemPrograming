#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

void deletePelementArr(PAdptArray, PElement*, int);


typedef struct AdptArray_ {

    int size; // Size ot the array
    PElement* pElemArr; // Kind of object
    void(*DEL_FUNC)(PElement); // Pointers to the functions
    PElement(*COPY_FUNC)(PElement);
    void(*PRINT_FUNC)(PElement);

} AdptArray, *PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC del, PRINT_FUNC prt){
    
    PAdptArray arr = (PAdptArray)malloc(sizeof(AdptArray));
    if(arr == NULL) return NULL; 
    arr->size = 0;
    arr->pElemArr = NULL;
    arr->DEL_FUNC = del;
    arr->COPY_FUNC = copy;
    arr->PRINT_FUNC = prt;
    return arr;
}

// Adding new element to the cell of the index
Result SetAdptArrayAt(PAdptArray arr, int index, PElement element) {
    printf("%s\n", "Enter?");

    if(index < 0 || arr == NULL || element == NULL){
        printf("%s\n", "Illegal arguments");
        return FAIL;
    }
    
    if(index >= arr->size) {
        
        PElement* newPelemArr = (PElement*)calloc(index+1, sizeof(PElement));
        
        if(newPelemArr == NULL) {
            printf("%s\n", "calloc didn't work");
            return FAIL;
        }

        int i;
        for(i = 0; i < arr->size; i++) {
            if(arr->pElemArr[i] != NULL) {
                newPelemArr[i] = arr->COPY_FUNC(arr->pElemArr[i]);
                // Copy function didn't work
                if(newPelemArr[i] == NULL) {
                    printf("%s\n", "Didn't make copy1");
                    deletePelementArr(arr, newPelemArr, index + 1);
                    return FAIL;
                }
            }
        }

        newPelemArr[index] = arr->COPY_FUNC(element);
        if(newPelemArr[index] == NULL) {
            printf("%s\n", "Didn't make copy2");
            deletePelementArr(arr, newPelemArr, index + 1);
            return FAIL;
        }

        deletePelementArr(arr, arr->pElemArr, arr->size); // Deletes the original array
        // Gets the new array
        arr->pElemArr = newPelemArr;
        arr->size = index + 1;
        return SUCCESS;
    }
    else {
            PElement temp = NULL;
            if(arr->pElemArr[index] != NULL) {
                temp = arr->COPY_FUNC(arr->pElemArr[index]); // Temporary for saving the data if need it
                if(!temp) {
                    printf("%s\n", "Didn't make copy3");
                    return FAIL;
                }
                arr->DEL_FUNC(arr->pElemArr[index]);
            }
            arr->pElemArr[index] = arr->COPY_FUNC(element);
            if(arr->pElemArr[index] == NULL) {
                printf("%s\n", "Didn't make copy4");
                arr->pElemArr[index] = temp; // Returns the copy of the object before delete
                return FAIL;
            }
        
        return SUCCESS;
    }
}
// Deletes the inner array
void deletePelementArr(PAdptArray adpArr,PElement* arr, int size) {
    int i;
    for(i = 0; i < size; i++) {
        if(arr[i] != NULL) {
            adpArr->DEL_FUNC(arr[i]);
            arr[i] = NULL;
        }
    }
    free(arr);
    arr = NULL;
}

void DeleteAdptArray(PAdptArray arr) {
    int i;
    // Deletes all the objects
    for(i = 0; i< arr->size; i++) {
        if (arr->pElemArr[i] != NULL) {
            arr->DEL_FUNC(arr->pElemArr[i]);
            arr->pElemArr[i] = NULL;
        }
    }
    free(arr->pElemArr);
    arr->pElemArr = NULL;
    free(arr);
    arr = NULL;
}

// Gets index and retruns copy of the element in the index cell
PElement GetAdptArrayAt(PAdptArray arr, int index) {
    if(index >= arr->size || index < 0 || arr == NULL) return NULL; // Checks legall index and array
    PElement copy = NULL;
    if(arr->pElemArr[index] == NULL) return NULL; // Checks if can do copy
    copy = arr->COPY_FUNC(arr->pElemArr[index]);
    return copy;
}

int GetAdptArraySize(PAdptArray arr) {
    if (arr == NULL) return -1;
    else return arr->size;
}

void PrintDB(PAdptArray arr) {
    if(arr != NULL) {
        int i;
        for(i = 0; i < arr->size; i++) {
            if(arr->pElemArr[i] != NULL)
                arr->PRINT_FUNC(arr->pElemArr[i]);
        }
    }
}

