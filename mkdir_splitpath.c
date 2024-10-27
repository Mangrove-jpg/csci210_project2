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

    // Create a new node that will track the current directory
    struct NODE* directory;
    directory = root;

    // find first token in pathName
    char* token = strtok(pathName, "/");
    char* lastToken = NULL;

    // Loop over the rest of the tokens
    while (token != NULL){

        // Store token in a variable
        lastToken = token;
        token = strtok(NULL, "/");
        // Append token to dirname if it isn't last

        //Check that the Token exists in directory
        if (token != NULL)
        {
            //Check for if token is a child directory
            struct NODE* child_directory;
            if (directory->childPtr != NULL)
            {
                child_directory = directory->childPtr;
            }
            else
            {
                child_directory = directory;
            }
            while(strcmp(lastToken, child_directory->name) != 0)
            {
                //If not, send an error
                if (child_directory->siblingPtr == NULL)
                {
                    printf("ERROR: directory %s does not exist\n", lastToken);
                    return NULL;
                }
                child_directory = child_directory->siblingPtr;
            }

            // Append token to Directory name
            strcat(dirName, "/");
            strcat(dirName, lastToken);

            //Update Directory to correct child
            directory = child_directory;
        }
        else
        {
            strcat(baseName, lastToken); 
        }

        // Make token basename if it is the final token
    }
    // Return Directory that basename is in
    return directory;
}

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    char basename [64];
    char dirname [64];
    basename[0] = '\0';
    dirname[0] = '\0';
    char pathStore[64];
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

    // allocate space for a new  node
    
    attatchNodeToParent(directory, basename, 'D');
    printf("MKDIR SUCCESS: node %s successfully created\n", pathStore);

    return;
}
