/*
 * Include the provided hashtable library
 */
#include "hashtable.h"

/*
 * Include the header file
 */
#include "philspel.h"

/*
 * Standard IO and file routines
 */
#include <stdio.h>

/*
 * General utility routines (including malloc())
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routine
 */
#include <string.h>

/*
 * this hashtable stores the dictionary
 */
HashTable *dictionary;

/*
 * the MAIN routine.  You can safely print debugging information
 * to standard error (stderr) and it will be ignored in the grading
 * process in the same way which this does.
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Specify a dictionary\n");
        return 0;
    }
    /*
     * Allocate a hash table to store the dictionary
     */
    fprintf(stderr, "Creating hashtable\n");
    dictionary = createHashTable(2255, &stringHash, &stringEquals);

    fprintf(stderr, "Loading dictionary %s\n", argv[1]);
    readDictionary(argv[1]);
    fprintf(stderr, "Dictionary loaded\n");

    fprintf(stderr, "Processing stdin\n");
    processInput();

    /* main in C should always return 0 as a way of telling
       whatever program invoked this that everything went OK
       */
    return 0;
}

/*
 * You need to define this function. void *s can be safely casted
 * to a char * (null terminated string) which is done for you here for
 * convenience.
 */
unsigned int stringHash(void *s) {
    // CREDIT : http://stackoverflow.com/questions/7666509/hash-function-for-string
    // CHANGE from long to int
    unsigned int hash = 5381;
    char *string = (char *) s;
    int c;
    while (c = *string++) {
        hash = ((hash << 5) + hash) + c;
        return hash;
    }
    //fprintf(stderr, "Need to define stringHash\n");
    //exit(0);
    return 0;
}

/*
 * You need to define this function.  It should return a nonzero
 * value if the two strings are identical (case sensitive comparison)
 * and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
    // CREDIT: https://www.tutorialspoint.com/c_standard_library/c_function_strcmp.htm
    char *string1 = (char *) s1;
    char *string2 = (char *) s2;
    int idk = strcmp(string1, string2);

    if (idk == 0) {
        return 1;
    } else {
        return 0;
    }
//    fprintf(stderr, "Need to define stringEquals\n");
//    exit(0);
}

/*
 * this function should read in every word in the dictionary and
 * store it in the dictionary.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, exit.  You will need to allocate
 * (using malloc()) space for each word.  As described in the specs, you
 * can initially assume that no word is longer than 70 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You can NOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program. Since the format is one word at a time, with
 * returns in between, you can
 * safely use fscanf() to read in the strings.
 */
void readDictionary(char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    char *str1 = (char *) malloc(70);
    int c;
    int i = 0;
    int total = 70;
    if (fp == NULL) {
        fprintf(stderr, "Error in file opening\n");
        exit(0);
    }
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            str1[i] = '\0';
            char *key = (char *) malloc((strlen(str1) + 1) * sizeof(char));
            strcpy(key, str1);
            if (findData(dictionary, key) == NULL) {
                insertData(dictionary, key, key);
            }
            i = 0;
            total = 70;
            memset(str1, 0, sizeof str1);
            fprintf(stderr, "%s\n", findData(dictionary, key));
            continue;
        }
        if (i == total) {
            char *str2 = (char *) realloc(str1, total * 2);
            str1 = str2;
            total = total * 2;
        }
        str1[i] = (char) c;
        i++;
    }
    free(str1);

//    while (fscanf(fp, "%1023s", str1) != EOF) { // IF CAUSES INFINITE LOOP TRY != -1
//
//        fprintf(stdout, "%s\n", str1);
//
//        //CHECK FOR WORD ALREADY EXISTING INSIDE THE DICTIONARY
//        // Allocate memory of the length of charstring + 1 (for the null terminator)
//        // CREDIT: http://stackoverflow.com/questions/17522327/is-null-character-included-while-allocating-using-malloc
//        char *key = (char *) malloc((strlen(str1) + 1) * sizeof(char));
//        strcpy(key, str1);
//        insertData(dictionary, key, str1);
//
//        fprintf(stdout, "%s\n", findData(dictionary, key));
//    }
// "Super31337-61c" should be treated as a single dictionary entry, that can never be matched as it contains non alphabet characters
//    fprintf(stderr, "Need to define readDictionary\n");
}


/*
 * This should process standard input and copy it to standard output
 * as specified in specs.  EG, if a standard dictionary was used
 * and the string "this is a taest of  this-proGram" was given to
 * standard input, the output to standard output (stdout) should be
 * "this is a teast [sic] of  this-proGram".  All words should be checked
 * against the dictionary as they are input, again with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not being found, by appending " [sic]" after the
 * error.
 *
 * Since we care about preserving whitespace, and pass on all non alphabet
 * characters untouched, and with all non alphabet characters acting as
 * word breaks, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), so you will probably have
 * to get characters from standard input one at a time.
 *
 * As stated in the specs, you can initially assume that no word is longer than
 * 70 characters, but you may have strings of non-alphabetic characters (eg,
 * numbers, punctuation) which are longer than 70 characters. For the final 20%
 * of your grade, you can no longer assume words have a bounded length.
 */
void processInput() {
    char *str1 = (char *) malloc(70);
    char *str2 = (char *) malloc(70);
    char *str3 = (char *) malloc(70);
    int c = 0;
    int i = 0;
    int total = 70;

    while ((c = fgetc(stdin)) != EOF) {

        if (isalpha(c) != 0) {
            if (i == total) {
                char *str4 = (char *) realloc(str1, total * 2);
                char *str5 = (char *) realloc(str2, total * 2);
                char *str6 = (char *) realloc(str3, total * 2);
                str1 = str4;
                str2 = str5;
                str3 = str6;
                total = total * 2;
            }
            str1[i] = (char) c;
            if (i == 0) {
                str2[0] = (char) c;
            } else {
                str2[i] = (char) tolower(c);
            }
            str3[i] = (char) tolower(c);
            i++;

        } else {
            if (c == '\n') {
                if (isalpha(str1[0])) {
                    str1[i] = '\0';
                    str2[i] = '\0';
                    str3[i] = '\0';
                    if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL &&
                        findData(dictionary, str3) == NULL) {
                        fprintf(stdout, "%s [sic]%c", str1, c);
                    } else {
                        if (strlen(str1) != 0) {
                            fprintf(stdout, "%s%c", str1, c);
                        }
                    }
                } else {
                    fprintf(stdout, "%c", c);
                }
            }
            else if (isspace(c)) {
                if (isalpha(str1[0])) {
                    str1[i] = '\0';
                    str2[i] = '\0';
                    str3[i] = '\0';
                    if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL &&
                        findData(dictionary, str3) == NULL) {
                        fprintf(stdout, "%s [sic]", str1);
                    } else {
                        if (strlen(str1) != 0) {
                            fprintf(stdout, "%s", str1);
                        }
                    }
                }
                fprintf(stdout, " ");
            }
            else if (isalpha(c) == 0) {
                if (isalpha(str1[0])) {
                    str1[i] = '\0';
                    str2[i] = '\0';
                    str3[i] = '\0';
                    if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL &&
                        findData(dictionary, str3) == NULL) {
                        fprintf(stdout, "%s [sic]%c", str1, c);
                    } else {
                        if (strlen(str1) != 0) {
                            fprintf(stdout, "%s%c", str1, c);
                            //fprintf(stdout, "\n");
                        }
                    }
                } else {
                    fprintf(stdout, "%c", c);
                }
            }
            i = 0;
            memset(str1, 0, sizeof str1);
            memset(str2, 0, sizeof str2);
            memset(str3, 0, sizeof str3);
        }
    }
    if (isalpha(str1[0])) {
        str1[i] = '\0';
        str2[i] = '\0';
        str3[i] = '\0';
        if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL &&
            findData(dictionary, str3) == NULL) {
            fprintf(stdout, "%s [sic]", str1);
        } else {
            if (strlen(str1) != 0) {
                fprintf(stdout, "%s", str1);
            }
        }
    }


    // 3 Cases
    // Regular input
    // Regular input w/ all letters converted to lowercase but the first
    // All letters converted to lowercase

    free(str1);
    free(str2);
    free(str3);
    //fprintf(stderr, "Need to define processInput\n");
}


//    while ((c = fgetc(stdin)) != EOF) {
//
//        if (isalpha(c) != 0) {
//            str1[i] = (char) c;
//            if (i == 0) {
//                str2[0] = (char) toupper(c);
//            } else{
//                str2[i] = (char) tolower(c);
//            }
//            str3[i] = (char) tolower(c);
//            i++;
//        } else {
//            if (isspace(c)) {
//                if (isalpha(str1[0]) && findData(dictionary, str1) == NULL) {
//                    fprintf(stdout, "%s [sic]", str1);
//                } else {
//                    if (strlen(str1) != 0) {
//                        fprintf(stdout, "%s", str1);
//                    }
//                }
//                i = 0;
//                memset(str1, 0, sizeof str1);
//                fprintf(stdout, " ");
//                continue;
//            }
//            if (c == '\n') {
//                if (isalpha(str1[0]) && findData(dictionary, str1) == NULL) {
//                    fprintf(stdout, "%s [sic]", str1);
//                } else {
//                    if (strlen(str1) != 0) {
//                        fprintf(stdout, "%s", str1);
//                    }
//                }
//                i = 0;
//                memset(str1, 0, sizeof str1);
//                continue;
//            }
//            if (isalpha(c) == 0) {
//                if (isalpha(str1[0]) && findData(dictionary, str1) == NULL) {
//                    fprintf(stdout, "%s [sic]%c", str1, c);
//                } else {
//                    if (strlen(str1) != 0) {
//                        fprintf(stdout, "%s%c", str1, c);
//                    }
//                }
//                i = 0;
//                memset(str1, 0, sizeof str1);
//                continue;
//            }
//        }
//    }


//    while (c = getchar()) {
//
//        if (c == EOF) {
//            break;
//        }
//
//        if (isalpha(c) != 0) {
//            str1[i] = (char) c;
//            i++;
//        } else {
////            if (c == '\n') {
////                continue;
////            }
//            str1[i] = '\0';
//
//            if (findData(dictionary, str1) == NULL) {
//                fprintf(stdout, "%s [sic]", str1);
//            } else {
//                fprintf(stdout, "%s", str1);
//            }
//
//            i = 0;
//            fprintf(stdout, "%c", c);
//        }
//    }

