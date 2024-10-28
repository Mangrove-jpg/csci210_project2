#include "types.h"
#include <stdio.h>

extern struct NODE* root;
extern struct NODE* cwd;

void attatchNodeToParent(struct NODE* parent, const char* name, char type) {
    /*Create space for new node in the heap*/
    struct NODE* newNode;
    newNode = (struct NODE*)malloc(sizeof(struct NODE));

    /*Initialize the child node*/
        strcpy(newNode->name, name);
        newNode->fileType = type;
        newNode->childPtr = NULL;
        newNode->siblingPtr = NULL;
        newNode->parentPtr = parent;
    
    /*Attach the child to the parent*/
    struct NODE* child;
    child = parent->childPtr;
    if (child == NULL) {/*Case where parent doesn't have child*/
        parent->childPtr = newNode;
    }
    else { /*The parent has one or more children*/
        while (child->siblingPtr != NULL)
        {
            child = child->siblingPtr;
        }
        child->siblingPtr = newNode;
    }
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    // Find Whether path is absolute or relative
    char path_type = pathName[0];
    struct NODE* directory;
    if (path_type == '/')
    {
        strcat(dirName, "/");
        directory = root;
    }
    else
    {
        directory = cwd;
    }

    // Check if path is only "/"
    if (strcmp(pathName, "/") == 0)
    {
        return directory;
    }

    // Initialize array of C strings
    char path_strings[100][64] = {""};

    // Find initial token
    char* token = strtok(pathName, "/");
    int current_token_index = 0;

    // Place all tokens into array
    while(token != NULL && current_token_index < 100)
    {
        strcpy(path_strings[current_token_index], token);
        token = strtok(NULL, "/");
        current_token_index += 1;
    }

    // Check all directories exist on path
    for (int i = 0; i < current_token_index - 1; i++)
    {

        // Check if current directory has child directories
        struct NODE* child_directory;
        if (directory->childPtr != NULL)
            {
                child_directory = directory->childPtr;
            }
            else
            {
                printf("ERROR: directory %s does not exist\n", path_strings[i]);
                return NULL;
            }

        // Check if any of the siblings have the same name as the current token
        while(child_directory != NULL)
        {
            // If child exists, set it to directory and break
            if (strcmp(path_strings[i], child_directory->name) == 0)
            {
                directory = child_directory;
                break;
            }
            else
            {
                child_directory = child_directory->siblingPtr;
            }
        }
        // If child doesn't exist, send directory doesn't exist error
        if (child_directory == NULL)
        {
            printf("ERROR: directory %s does not exist\n", path_strings[i]);
            return NULL;
            break;
        }

    }

    // Arrange dirname and basename
    for (int i = 0; i < current_token_index -1; i++)
    {
        strcat(dirName, path_strings[i]);
        // Don't add / if it's last one
        if (i != current_token_index - 2)
        {
            strcat(dirName, "/");
        }
    }
    strcpy(baseName, path_strings[current_token_index-1]);
    return directory;
}

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    char basename [64];
    char dirname [64];
    char pathStore[64];
    basename[0] = '\0';
    dirname[0] = '\0';
    pathStore[0] = '\0';
    strcpy(pathStore, pathName);

    // Use splitpath to find dirname and basename.
    struct NODE* directory;
    directory = splitPath(pathName, basename, dirname);

    // Check if directory is null
    if (directory == NULL)
    {
        return;
    }

    //Check if directory already exists
    struct NODE* child_directory;
    child_directory = directory->childPtr;
    while (child_directory != NULL)
    {
        if (strcmp(child_directory->name, basename) == 0)
        {
            printf("MKDIR ERROR: directory %s already exists\n", basename);
            return;
        }
        child_directory = child_directory->siblingPtr;
    }

    // Check if no input was provided
    if (strcmp(pathName, "/") == 0)
    {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    // add node to parent directory
    attatchNodeToParent(directory, basename, 'D');
    printf("MKDIR SUCCESS: node %s successfully created\n", pathStore);

    return;
}
