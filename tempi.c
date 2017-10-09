#include <stdio.h>
#include <string.h>
#include<stdlib.h>

typedef struct lnode lnode;
typedef struct list list;

void printlist(list *lst);

struct lnode
{
  enum { is_int, is_float, is_char,is_list} type;
  union{
  int ival;
  char *cval;
  list *lval;
  }val;
  
};

struct list{
    int length;
    struct lnode *item;
    
    
    
};

list *createlist(){
    
list *l=(list*)malloc(sizeof(list));
l->length=0;
l->item=(lnode*) malloc (sizeof (lnode));
return l;
    
}

void additem(list *l,char *mem){
    
    int index=l->length;
    if(index>0){
        int newlen=index+1;
        l->item=(lnode *)realloc(l->item, sizeof(lnode)*newlen);
    }

    if(isdigit(mem[0])!=0){
       l->item[index].type=is_int;
       l->item[index].val.ival=atoi(mem);
    }
    
    else if(isalpha(mem[0])){
        l->item[index].type=is_char;
        l->item[index].val.cval=mem;
    }
	
	 else{
        l->item[index].type=is_char;
        l->item[index].val.cval=mem;
        
    }
    l->length+=1;
    
}
void additem_list(list *l, list *mem){
    
       int index=l->length;
    if(index>0){
        int newlen=index+1;
        l->item=(lnode *)realloc(l->item, sizeof(lnode)*newlen);
    }
    
    
    
    
    l->item[index].type=is_list;
    l->item[index].val.lval=mem;
    
      l->length+=1;

}


void *print_litem(list *lst,int ind){
    if(lst->item[ind].type==is_int)
    {printf("%d",lst->item[ind].val.ival);
    }
    else if(lst->item[ind].type==is_char)
    {
      printf("'%s'",lst->item[ind].val.cval);  
    }
    else{
        printlist(lst->item[ind].val.lval);
    }

}

void printlist(list *lst)
{
    int lng=lst->length;
    printf("[");
    for(int i=0;i<lng;i++){
        print_litem(lst,i);
        if(i!=lng-1)
         printf(",");
    }
    printf("]");
}












// str replace - learned from sf thread.
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


list *tokenize(char *str){
    char *str1="(";
    char *str2=" ( ";
    char *str3=")";
    char *str4=" ) ";
    
    
    
    str=replace_str(str, str1, str2);
    
 
    str=replace_str(str,str3,str4);
    
    
     
    
    
    str=strstrip(str);
    char *token;
    char *rest = str;
    list *tokenlist=createlist();
    while ((token = strtok_r(rest, " ", &rest)))
    {    additem(tokenlist,token);
        printf("%s\n", token);
    }
    return tokenlist;      

}


//i to a function :

char *itoa (int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

lnode *getitem(list *lst,int i)
{
    return &lst->item[i];
}


void addto_plist(list *lst,lnode *token){

if(token->type==is_char){
additem(lst,token->val.cval);
}
else if(token->type==is_list){
additem_list(lst,token->val.lval);

}
else if(token->type==is_int){
char *tmpint=(char*)malloc(sizeof(int));
additem(lst,itoa(token->val.ival,tmpint,10));
}


}


lnode *parse(list *tokens,int *tknum){
    
    
    if(tokens->length==0){
        printf("syntax error");
    }
    
    lnode *token=(lnode*)malloc(sizeof(lnode));
    
    token=getitem(tokens,*tknum);
    
    if(token->type==is_char&&strcmp(token->val.cval,"(")==0)
     {   list *parsedlist =createlist();
        
         while((*tknum)<tokens->length-1){
             
             
             if(!(getitem(tokens,(*tknum)+1)->type==is_char&&strcmp(getitem(tokens,(*tknum)+1)->val.cval,")")==0))
             {(*tknum)++;
             addto_plist(parsedlist,parse(tokens,tknum));}
             else if(getitem(tokens,(*tknum)+1)->type==is_char&&strcmp(getitem(tokens,(*tknum)+1)->val.cval,")")==0)
             { (*tknum)++; 
               break;  
             }
             
            
         }
        
        
         lnode *tokenf=(lnode*)malloc(sizeof(lnode));
         tokenf->type=is_list;
         tokenf->val.lval=parsedlist;
         
         return tokenf;
     }
     else{
         if(token->type==is_char&&strcmp(token->val.cval,")")==0)
          {printf("syntax error");}
         else
         {return token;}
         
     }
    
    
    
}





int main()
{   
    
    char *str="(begin (define r 10) (* pi (* r r)))";
    
   


    
   //splitting tokens test
   list *tokens=tokenize(str);
   printlist(tokens);
   int tnum=0;
   lnode *pnode=(lnode*)malloc(sizeof(lnode));
   list *parsedl=createlist();
   pnode=parse(tokens,&tnum);
   parsedl=pnode->val.lval;
   printf("\n"); 
   printlist(parsedl);
    
    
    
    return 0;
}



