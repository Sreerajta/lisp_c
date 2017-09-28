#include <stdio.h>
#include <string.h>


char *replace_str(char *orig, char *rep, char *with) {
    char *result; 
    char *ins;    
    char *tmp;    
    int len_rep;  
    int len_with; 
    int len_front;
    int count;    

    
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; 
    if (!with)
        with = "";
    len_with = strlen(with);

    
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

   
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; 
    }
    strcpy(tmp, orig);
    return result;
}


char *strstrip(char *s)
{
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
        s++;

    return s;
}

int main()
{   
    
    char *str="(define pi 3.14)";
    
    char *str1="(";
    char *str2=" ( ";
    char *str3=")";
    char *str4=" ) ";
    
    
    
    str=replace_str(str, str1, str2);
    puts(str);
 
    str=replace_str(str,str3,str4);
    puts(str);
    
     
    
    
    str=strstrip(str);
    puts(str);

    
   //splitting tokens test
   
   char *token;
    char *rest = str;
 
    while ((token = strtok_r(rest, " ", &rest)))
        printf("%s\n", token);
    
        return 0;
}



