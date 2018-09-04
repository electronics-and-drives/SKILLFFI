#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <ffi.h>

typedef struct function
{
    void *funHandle;
    ffi_cif cif;
    ffi_type *rtype;
    int nargs;
    ffi_type **atypes;
} function_t;

typedef struct list
{
    function_t function;
    
    struct list *next;
} list_t;

void push(list_t *head, function_t func)
{
    list_t *current = head;

    while(current->next != NULL)
        {current = current->next;}

    current->next = (list_t *)malloc(sizeof(list_t));
    current->next->function = func;
    current->next->next = NULL;
}

function_t* find(list_t *head, void *funHandle)
{
    list_t *current = head;

    while(current != NULL)
    {
        if(current->function.funHandle == funHandle)
            {return &current->function;}
        current = current->next;
    }
    return NULL;
}

void getInput(char **input)
{
    free(*input);
    char buffer[2048];

    fgets(buffer, 2048, stdin);
    buffer[strlen(buffer) - 1] = '\0';

    *input = (char *)calloc(strlen(buffer), sizeof(char));
    strcat(*input, buffer);
}

ffi_type* getFFItype(char *ffit)
{
    if(strncmp(ffit, "float", sizeof(ffit)) == 0)
        {return &ffi_type_float;} 
    else if(strncmp(ffit, "double", sizeof(ffit)) == 0)
        {return &ffi_type_double;} 
    else if(strncmp(ffit, "int", sizeof(ffit)) == 0)
        {return &ffi_type_sint;} 
    else
        {return &ffi_type_pointer;} 
}

void getValue(char *src, ffi_type *type, void **dst)
{

    if(type == &ffi_type_float)
    {
        float f;
        sscanf(src, "%f", &f);
        *dst = (void *)&f;
    }
}

int printValue(ffi_arg val, ffi_type *type)
{
    if(type == &ffi_type_float)
    {
        printf("%f", (float)val);
    }
}

function_t* getFunction(list_t *head)
{
    function_t *func;
    void *funHandle;
    char *fun = (char *)calloc(1, sizeof(char));

    printf("fun");
    fflush(stdout);
    getInput(&fun);
    sscanf(fun, "%p", (void **)&funHandle);

    if(!funHandle)
        {return NULL;}

    func = find(head, funHandle);

    if(!func)
        {return NULL;}

    free(fun);

    return func;
}

int ffiOpenLib()
{
    void *libHandle = NULL;
    char *ldpath = (char *)calloc(1, sizeof(char));

    printf("open");
    fflush(stdout);

    getInput(&ldpath);
    
    libHandle = dlopen(ldpath, RTLD_LAZY);
    if(!libHandle)
        {return 1;}
    else
        {printf("%p", libHandle);fflush(stdout);}

    free(ldpath);

    return 0;
}

int ffiPrep(list_t *head)
{
    int i;

    function_t func;
    ffi_status status;

    void *libHandle = NULL;
    void *funHandle = NULL;
    char *lib = (char *)calloc(1, sizeof(char));
    char *fun = (char *)calloc(1, sizeof(char));
    char *rt = (char *)calloc(1, sizeof(char));
    char *na = (char *)calloc(1, sizeof(char));
    char *at = (char *)calloc(1, sizeof(char));
    func.nargs = 0;

    printf("func");
    fflush(stdout);
    getInput(&fun);

    printf("lib");
    fflush(stdout);
    getInput(&lib);
    sscanf(lib, "%p", (void **)&libHandle);

    funHandle = dlsym(libHandle, fun);

    if(!funHandle)
        {return 1;}
    
    func.funHandle = funHandle;
        
    printf("rtype");
    fflush(stdout);
    getInput(&rt);
    func.rtype = getFFItype(rt);

    printf("numargs");
    fflush(stdout);
    getInput(&na);
    sscanf(na, "%d", &func.nargs);

    func.atypes = (ffi_type **)calloc(func.nargs, sizeof(ffi_type *));
    
    for(i = 0; i < func.nargs; i++)
    {
        printf("atype");
        fflush(stdout);
        getInput(&at);
        func.atypes[i] = getFFItype(at);
    }

    status = ffi_prep_cif(&func.cif, FFI_DEFAULT_ABI, func.nargs, func.rtype, func.atypes);

    if(status != FFI_OK)
    {
        printf("error");
        fflush(stdout);
        return 2;
    }

    printf("%p", funHandle);
    fflush(stdout);

    push(head, func);

    free(lib);
    free(fun);
    free(rt);
    free(at);
    free(na);

    return 0;
}

int ffiCall(function_t *func)
{
    int n;
    void *funHandle;
    void *avalues[func->nargs];
    char *val = (char *)calloc(1, sizeof(char));
    
    for(n = 0; n < func->nargs; n++)
    {
        printf("val");
        fflush(stdout);
        getInput(&val);

        if(func->atypes[n] == &ffi_type_float)
        {
            float f;
            sscanf(val, "%f", &f);
            avalues[n] = &f;
        }
        else if(func->atypes[n] == &ffi_type_sint)
        {
            int i;
            sscanf(val, "%d", &i);
            avalues[n] = &i;
        }
        else
        {
            char *s;
            sscanf(val, "%s", &s);
            avalues[n] = s;
        }
    }
    
    if(func->rtype == &ffi_type_float)
    {
        float rvalue;
        ffi_call(&func->cif, FFI_FN(func->funHandle), &rvalue, avalues);
        printf("%f", rvalue);
    }
    else if(func->rtype == &ffi_type_sint)
    {
        int rvalue;
        ffi_call(&func->cif, FFI_FN(func->funHandle), &rvalue, avalues);
        printf("%d", rvalue);
    }
    else if(func->rtype == &ffi_type_uint)
    {
        unsigned int rvalue;
        ffi_call(&func->cif, FFI_FN(func->funHandle), &rvalue, avalues);
        printf("%d", rvalue);
    }
    else
    {
        ffi_arg rvalue;
        ffi_call(&func->cif, FFI_FN(func->funHandle), &rvalue, avalues);
        printf("%hhu", rvalue);
    }
    
    fflush(stdout);

    free(val);

    return 0;
}

int ffiClose()
{
    void *libHandle;
    char *lib = (char *)calloc(1, sizeof(char));

    printf("lib");
    fflush(stdout);
    getInput(&lib);
    sscanf(lib, "%p", (void **)&libHandle);

    if(!libHandle)
        {return 1;}

    return dlclose(libHandle);
}

int main(int argc, char **argv)
{
    list_t *head = NULL;
    head = (list_t *)malloc(sizeof(list_t));
    head->next = NULL;

    char *input = (char *)calloc(1, sizeof(char));

    printf("ffi ready");
    fflush(stdout);

    while(1)
    {
        getInput(&input);
        if(strncmp(input, "open", sizeof(input)) == 0)
        {
            if(ffiOpenLib() != 0)
                {printf("not found");fflush(stdout);}
        }
        else if(strncmp(input, "call", sizeof(input)) == 0)
        {
            function_t *func = getFunction(head);
            if(func)
            {
                if(ffiCall(func) != 0)
                    {printf("not callable");fflush(stdout);}
            }
            else
                {printf("not defined");fflush(stdout);}
        }
        else if(strncmp(input, "prep", sizeof(input)) == 0)
        {
            if(ffiPrep(head) != 0)
                {printf("not found");fflush(stdout);}
        }
        else if(strncmp(input, "close", sizeof(input)) == 0)
        {
            if(ffiClose() != 0)
                {printf("not closed");fflush(stdout);}
        }
    }

    free(input);

    return 0;
}
