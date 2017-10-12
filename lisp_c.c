#include<math.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define HASHVAL		1000

typedef struct list list;
typedef struct lnode lnode;

void printlist(list *lst);
lnode *addFun(lnode *(*fptr) (lnode *), char *s);

//math functions:

lnode *add(lnode *a){
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_int;
    result->val.ival=0;
    
    for(int i=0;i<a->val.lval->length;++i){
        result->val.ival+=a->val.lval->item[i].val.ival;
    }
    return result;
}

lnode *sub(lnode *a){
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_int;
    result->val.ival=a->val.lval->item[0].val.ival;
    
    for(int i=1;i<a->val.lval->length;++i){
        result->val.ival-=a->val.lval->item[i].val.ival;
    }
    return result;    
}

lnode *gt(lnode *a){
    int x=a->val.lval->item[0].val.ival;
    int y=a->val.lval->item[1].val.ival;
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_bool;
    if(x>y){
    result->val.cval="t";
    }
    else
    result->val.cval="f";
    return result;
}

lnode *lt(lnode *a){
    int x=a->val.lval->item[0].val.ival;
    int y=a->val.lval->item[1].val.ival;
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_bool;
    if(x<y){
    result->val.cval="t";
    }
    else
    result->val.cval="f";
    return result;
}

lnode *ge(lnode *a){
    int x=a->val.lval->item[0].val.ival;
    int y=a->val.lval->item[1].val.ival;
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_bool;
    if(x>=y){
    result->val.cval="t";
    }
    else
    result->val.cval="f";
    return result;
}

lnode *le(lnode *a){
    int x=a->val.lval->item[0].val.ival;
    int y=a->val.lval->item[1].val.ival;
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_bool;
    if(x<=y){
    result->val.cval="t";
    }
    else
    result->val.cval="f";
    return result;
}

lnode *mul(lnode *a){
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_int;
    result->val.ival=1;
  
    for(int i=0;i<a->val.lval->length;++i){
        result->val.ival*=a->val.lval->item[i].val.ival;
    }
    return result;  
}

lnode *divs(lnode *a){
    lnode *result=(lnode*)malloc(sizeof(lnode));
    result->type=is_int;
    result->val.ival=a->val.lval->item[0].val.ival;
   
    for(int i=0;i<a->val.lval->length;++i){
        result->val.ival*=a->val.lval->item[i].val.ival;
    }
    return result; 
}
//end of math functions

struct lnode{
  enum { 
  is_int, is_float, is_char,is_list,is_bool
  } type;
  union{
  int ival;
  char *cval;
  list *lval;
  }val;
  
  lnode **thisEnv;
  lnode *outerEnv;	
  lnode *parms;	
  lnode *body;	
  lnode *env;	

  char *t;			
  char *s;
  lnode *(*fptr) (lnode *);  
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

void print_result(lnode *res){
	if(res->type==is_int){
		printf("\n%d",res->val.ival);
	}
	else if(res->type==is_char){
		printf("\n%s",res->val.cval);
	}
	else if(res->type==is_bool){
		if(strcmp(res->val.cval,"t")==0)
			printf("\nTrue");
		else
			printf("\nFalse");
	}
}


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
    while ((token = strtok_r(rest, " ", &rest))){    
	   additem(tokenlist,token);
    }
    return tokenlist;      
}

char *itoa (int value, char *result, int base){
	
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}


lnode *getitem(list *lst,int i){
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
    
    if(token->type==is_char&&strcmp(token->val.cval,"(")==0){
		list *parsedlist =createlist();
        
        while((*tknum)<tokens->length-1){
             if(!(getitem(tokens,(*tknum)+1)->type==is_char&&strcmp(getitem(tokens,(*tknum)+1)->val.cval,")")==0)){
				 (*tknum)++;
                 addto_plist(parsedlist,parse(tokens,tknum));
				 }
             else if(getitem(tokens,(*tknum)+1)->type==is_char&&strcmp(getitem(tokens,(*tknum)+1)->val.cval,")")==0){ 
			 (*tknum)++; 
              break;  
             }
             
            }
         
		 lnode *tokenf=(lnode*)malloc(sizeof(lnode));
         tokenf->type=is_list;
         tokenf->val.lval=parsedlist;
         return tokenf;
    }
    
	else{
         if(token->type==is_char&&strcmp(token->val.cval,")")==0){
			 printf("syntax error");
			 }
         else{
			 return token;
			 }
         
        }
}

long hash(char *w){
	int i = 0;
	long h = 0;
	char *s = w;

	while(*w){
		h = h + (*w) * pow(10,i);
		w++;
		i++;
	}
    return h%HASHVAL;
}

lnode *standard_env(void){
	
	lnode *new = (lnode *)malloc(sizeof(lnode));
	lnode **env = (lnode **)calloc(HASHVAL,sizeof(lnode *));

	env[hash("+")] = addFun(add,"+");
	env[hash("-")] = addFun(sub,"-");
	env[hash("*")] = addFun(mul,"*");
	env[hash("/")] = addFun(divs,"/");
    env[hash(">")] = addFun(gt,">");
	env[hash("<")] = addFun(lt,"<");
	env[hash(">=")] = addFun(ge,">=");
	env[hash("<=")] = addFun(le,"<=");

	new->t = "e";
	new->thisEnv = env;
	new->outerEnv = NULL;
	
	return new;
}

lnode *addFun(lnode *(*fptr) (lnode *), char *s){
	lnode *new = (lnode *)malloc(sizeof(lnode));
    new->t = "F";
	new->fptr = fptr;
	new->s = strdup(s);
	
	return new;
}

lnode *create_env(lnode *parms, lnode *args, lnode *outerEnv){
	
    lnode *newEnvNode = (lnode *)malloc(sizeof(lnode));
    newEnvNode->t = "e";
	newEnvNode->thisEnv = (lnode **)calloc(HASHVAL,sizeof( lnode *));
	newEnvNode->outerEnv = outerEnv;
	
	if(parms->val.lval->length!=0){
		
		for(int i=0;i<parms->val.lval->length;i++){
		    	newEnvNode->thisEnv[hash(parms->val.lval->item[i].val.cval)] = &args->val.lval->item[i];    
		}
	
	}
	return newEnvNode;
}

lnode *find_env(char *s,lnode *env){
    if((env->thisEnv[hash(s)]) != NULL){
		    return env;  
	}
    else
	return find_env(s,env->outerEnv);
}

lnode *eval(lnode *x,lnode *env){
    lnode *e_env=(lnode*)malloc(sizeof(lnode));
    if(env==NULL){
     e_env=standard_env();
    }
    else{
     e_env=env;
    }

    if(x->type==is_char){
     return find_env(x->val.cval,e_env)->thisEnv[hash(x->val.cval)];
    }
    else if(x->type==is_int){
	 return x;
    }

    else if(x->type==is_list){
     if(x->val.lval->length==1){
		 eval(x->val.lval->item,e_env);
		}

     else if(strcmp(x->val.lval->item[0].val.cval,"quote")==0){
         return &x->val.lval->item[1];
        }
     
     else if(strcmp(x->val.lval->item[0].val.cval,"if")==0){
        lnode *test=(lnode*)malloc(sizeof(lnode));
        lnode *conseq=(lnode*)malloc(sizeof(lnode));
        lnode *alt=(lnode*)malloc(sizeof(lnode));
        lnode *tempres=(lnode*)malloc(sizeof(lnode));
        lnode *res=(lnode*)malloc(sizeof(lnode));
        test=&x->val.lval->item[1];
        conseq=&x->val.lval->item[2];
        alt=&x->val.lval->item[3]; 
        tempres=eval(test,env);
        if(strcmp(tempres->val.cval,"t")==0)
         res=conseq;
        else
         res=alt;
        return eval(res,e_env); 
        }
     
	 else if(strcmp(x->val.lval->item[0].val.cval,"set!")==0){
      find_env(x->val.lval->item[1].val.cval,e_env)->thisEnv[hash(x->val.lval->item[1].val.cval)]=eval(&x->val.lval->item[2],e_env);
     } 
     
	 else if(strcmp(x->val.lval->item[0].val.cval,"define")==0){
         e_env->thisEnv[hash(x->val.lval->item[1].val.cval)]=eval(&x->val.lval->item[2],e_env);
     }
     
     else if(strcmp(x->val.lval->item[0].val.cval,"lambda")==0){
         lnode *vars=(lnode*)malloc(sizeof(lnode));
         lnode *expr=(lnode*)malloc(sizeof(lnode));
         vars=&x->val.lval->item[1];
         expr=&x->val.lval->item[2];
         lnode *retf=(lnode*)malloc(sizeof(lnode));
         retf->parms=vars;
         retf->body=expr;
         retf->t="p";
         retf->env=e_env;
         return retf;
     }
     
	 else {  
         lnode *proc=(lnode*)malloc(sizeof(lnode));
         proc=eval(&x->val.lval->item[0],e_env);
         int argslen=x->val.lval->length;
         list *arglist=createlist();
         arglist->length=argslen-1;
         arglist->item=(lnode*)malloc((argslen)*sizeof(lnode));
         lnode *args=(lnode*)malloc(sizeof(lnode));
         
         for(int i=1;i<argslen;++i){
          arglist->item[i-1]=*eval(&x->val.lval->item[i],e_env);  
         }
         args->val.lval=arglist;
         
         if(strcmp(proc->t,"p")==0){
             return eval(proc->body,create_env(proc->parms,args,proc->env));
         }
         else{
          return (proc->fptr)(args);
         }
     }     
   }
}

int main()
{      
    lnode *genv=standard_env();
    lnode *result=(lnode*)malloc(sizeof(lnode));
	
	//insert program statements:
	char *str="(define fact (lambda (n) (if (< n 2) 1 (* n (fact (- n 1))))))";
    char *str2="(fact 5)";
    
    //expression 1 goes here:
    list *tokens=tokenize(str);
    int tnum=0;
    lnode *pnode=(lnode*)malloc(sizeof(lnode));
    list *parsedl=createlist();
    pnode=parse(tokens,&tnum);
    eval(pnode,genv);
  
   //expression 2 goes here:
    tokens=tokenize(str2);
    tnum=0;
    pnode=parse(tokens,&tnum);
    result=eval(pnode,genv);
    print_result(result);
    return 0;
}


