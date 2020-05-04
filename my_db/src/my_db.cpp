// a journey of a thousand miles begins with a single step


#include "../include/bpt.h"
#include "../include/TextTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <iostream>
#include <fstream>

#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#define CONSUMERS_COUNT 2
#define PRODUCERS_COUNT 2
//定义条件变量与互斥量
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t thread[CONSUMERS_COUNT+PRODUCERS_COUNT];

//临界资源
struct msg{
    struct msg* next;
    //int num;
	char cmd[100];
};

struct parameter 
{  
 	FILE *_fp; 
    int _nThreadId;//第几个线程  
}; 

//创建头结点
struct msg* head = NULL;

using namespace bpt;
using namespace std;


const char *errorMessage = "> your input is invalid,print \".help\" for more infomation!\n";
const char *nextLineHeader ="> ";
const char *exitMessage = "> bye!\n";
const char *dbFileName = "./data/db.bin";


clock_t startTime,finishTime;   

// function prototype
void printHelpMess();
void selectCommand();
int insertRecord(bplus_tree *treePtr,int *key, value_t *values);
int deleteRecord(bplus_tree *treePtr,int *index);
int searchRecord(bplus_tree *treePtr,int *index, value_t *return_val);
int searchAll(bplus_tree *treePtr,int *i_start, int *i_end);
int updateRecord(bplus_tree *treePtr,int *key, value_t *values);
void printTable(int *index, value_t *values);
void intToKeyT(bpt::key_t *a,int *b);
bool is_file_exist(const char *fileName);
double durationTime(clock_t *f,clock_t *s);
void searchFunction(int *keyIndex);
void updateFunction(int *keyIndex,value_t *updateData);
void deleteFunction(int *keyIndex);
void insertFunction(int *keyIndex,value_t *insertData);
bplus_tree *duck_db_ptr;
void printHelpMess(){
	cout << "*********************************************************************************************"<<endl<<endl
		<<" 				Welcome to the my_db\n 				db file locates in \"./data/db.bin\" \n 				Author: acewzj\n 				www.navyberry.com\n 				2019-10-31"<<endl<<endl
		<<"*********************************************************************************************"<<endl
		<<"  .help 							print help message;"<<endl
		<<"  .exit 							exit program;"<<endl
		<<"  .reset 							delete db file;"<<endl
		<<"  insert db {index} {name} {age} {email}; 			insert record;"<<endl
		<<"  delete from db where id={index}; 				delete record;"<<endl
		<<"  update db {name} {age} {email} where id={index}; 		update a record;"<<endl
		<<"  select * from db where id={index}; 				search a record by index;"<<endl
		<<"  select * from db where id in({minIndex},{maxIndex}); 		search records between indexs;"<<endl
		<<"  source [path].sql read; 				        multi-thread read based on a sql file;"<<endl
		<<"*********************************************************************************************"<<endl
		<<endl << nextLineHeader;
}
//消费函数
void* consumer(void *arg){
    struct msg *mp;
    for(;;){
        //加锁
        pthread_mutex_lock(&mutex);
        while(head==NULL){
            //等待
            pthread_cond_wait(&cond,&mutex);
        }      
        mp = head;
		int *keyIndex = new int;
		value_t *updateData = new value_t;
		value_t *insertData = new value_t;
		
		if(sscanf(mp->cmd,"select * from db where id=%d;", keyIndex)>0)
			//cout << "select done!" << endl;
			searchFunction(keyIndex);
		else if(sscanf(mp->cmd,"update db %s %d %s where id=%d;",
		updateData->name,&(updateData->age),updateData->email,keyIndex)>3)
			//cout << "update done!" << endl ;
			updateFunction(keyIndex,updateData);
		else if(sscanf(mp->cmd,"delete from db where id=%d;", keyIndex)>0)	
			//cout << "delete done!" << endl ;
			deleteFunction(keyIndex);
		else if(sscanf(mp->cmd,"insert db %d %s %d %s;", 
	    		keyIndex, insertData->name,&(insertData->age),insertData->email)>3)
			//cout << "insert done!" << endl ;
			insertFunction(keyIndex,insertData);	
		
        head = mp->next;
        //释放锁
        pthread_mutex_unlock(&mutex);
        free(mp);
        sleep(rand()%5);
    }
}
//生产函数
void* producer(void* args){
    struct msg *mp;
    for(;;){
		FILE *fpRead = ((parameter *)args)->_fp;
		int nId = ((parameter *)args)->_nThreadId;

		mp = (struct msg*)malloc(sizeof(struct msg));
		memset(mp,0,sizeof(struct msg)); 		 
		fgets(mp->cmd,100,fpRead); 
		if(*(mp->cmd) == '\0'){
			cout << "read done! thread exit"<< nId << nextLineHeader ;
			//释放锁 
        	pthread_mutex_unlock(&mutex);
			pthread_exit((void*)0);			
		}
        
		//加锁
		pthread_mutex_lock(&mutex);					       
        mp->next = head;
        head = mp;
        //唤醒
        pthread_cond_signal(&cond);
        //释放锁 
        pthread_mutex_unlock(&mutex);
		//fclose(fp); 
        sleep(rand()%5);
    }
}


// initial
void initialSystem(){
	// step 1 : print help message
	printHelpMess();
	// step 2 : initial database from file
	bplus_tree my_db(dbFileName, (!is_file_exist(dbFileName)));
	duck_db_ptr = &my_db;
	// step 3 : REPL select commdand (insert,delete,update,search)
	selectCommand();
	
}
// print help message

void searchFunction(int *keyIndex){
		value_t *return_val = new value_t;
		startTime = clock(); 

		int return_code = searchRecord(duck_db_ptr,keyIndex,return_val);

		finishTime = clock(); 

		if (return_code != 0){
			cout << "> index:"<< *keyIndex << " doesn't exist, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n";
		}else{
			printTable( keyIndex , return_val);
			cout << "> executed search, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n";
			
		}	
}
void updateFunction(int *keyIndex,value_t *updateData){
	
	startTime = clock(); 

	int return_code = updateRecord(duck_db_ptr,keyIndex,updateData);

	finishTime = clock(); 

	if (return_code == 0){
		cout << "> executed update index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n";
	}else{
		cout << "> update Record failed! no index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n";
	}

}
void deleteFunction(int *keyIndex){

	startTime = clock(); 

	int return_code = deleteRecord(duck_db_ptr,keyIndex);

	finishTime = clock(); 

	if (return_code == 0){
		cout << "> executed delete index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n";
	}else if(return_code == -1){
		cout << "> delete Record failed ! no index:"<<   *keyIndex << "\n";
	}else{
		cout << "> delete Record failed!\n";
	}

}
void insertFunction(int *keyIndex,value_t *insertData){

	startTime = clock(); 

	int return_code = insertRecord(duck_db_ptr,keyIndex,insertData);

	finishTime = clock(); 

	if (return_code == 0){
		cout << "> executed insert index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n";
	}else if(return_code == 1){
		cout << "> insert Record failed: already exist index:"<<   *keyIndex << "\n";
	}else{
		cout << "> insert Record failed!\n";
	}

}
// select command
void selectCommand(){

	// REPL
	char *userCommand = new char[256];
	
	while(true){

		cin.getline(userCommand,256);
		
		if(strcmp(userCommand,".exit") == 0){

	    	cout << exitMessage;
			break;

	    }else if(strcmp(userCommand,".help") == 0){

	    	printHelpMess();

	    }else if(strcmp(userCommand,".reset") == 0){
	    	if( remove( dbFileName) != 0 )
	    		cout<< "can't delete file"<< nextLineHeader;
	    	else
				cout << "DB file has been deleted!"<< endl << endl;

	    	initialSystem();

	    }else if(strncmp(userCommand,"insert",6) == 0){

	    	int *keyIndex = new int;
	    	value_t *insertData = new value_t;

	    	int okNum = sscanf(userCommand,"insert db %d %s %d %s;", 
	    		keyIndex, insertData->name,&(insertData->age),insertData->email);

			if(okNum < 3){
				
				cout << errorMessage<< nextLineHeader;

			}else{
				insertFunction(keyIndex,insertData);
			}


	    }else if(strncmp(userCommand,"delete",6) == 0){

	    	int *keyIndex = new int;

	    	int okNum = sscanf(userCommand,"delete from db where id=%d;", keyIndex);

			if(okNum < 1){
				cout << errorMessage<< nextLineHeader;
			}else{
				deleteFunction(keyIndex);
			}


	    }else if(strncmp(userCommand,"select",6) == 0){

	    	if( ! strstr (userCommand,"=")){

	    		int i_start,i_end;

	    		int okNum = sscanf(userCommand,"select * from db where id in(%d,%d);", &i_start,&i_end);

				if(okNum < 2){
					cout << errorMessage<< nextLineHeader;
				}else{
					startTime = clock(); 

					searchAll(duck_db_ptr,&i_start, &i_end);

					finishTime = clock(); 
					cout << "> executed search, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
				}


	    	}else{

		    	int *keyIndex = new int;
		    	int okNum = sscanf(userCommand,"select * from db where id=%d;", keyIndex);

				if(okNum < 1){
					cout << errorMessage<< nextLineHeader;
				}else{
					searchFunction(keyIndex);					
				}
			}


	    }else
		 if(strncmp(userCommand,"source",6) == 0){//multi-thread
			//读取sql文件strtok函数第一次调用时会把s字符串中所有在delim中出现的字符替换为NULL。然后通过依次调用strtok(NULL, delim)得到各部分子串。
			//source read.sql;
			char *pathPtr = strtok(userCommand," ");
			char *parmPtr = NULL;
			if(pathPtr!="NULL"){
				pathPtr=strtok(NULL," ");
				parmPtr=strtok(NULL," ");
			}
			FILE *fp = fopen(pathPtr,"r");
				
				    //创建消费者线程
					for(int i=0;i<CONSUMERS_COUNT;i++){
						pthread_create(&thread[i],NULL,consumer ,(void*)i);
					}
					//创建生产者线程
					//生产者线程将任务从文本中读取出来,然后放到那里,由消费者线程进行读取然后执行
					parameter param;
					for(int i=0;i<PRODUCERS_COUNT;i++){
						param._fp = fp;
						param._nThreadId = i;
						pthread_create(&thread[i+CONSUMERS_COUNT],NULL,producer,&(param));
					}
					//等待生产者线程退出后杀死消费者线程
					for(int i = CONSUMERS_COUNT;i<CONSUMERS_COUNT+PRODUCERS_COUNT;i++){
						pthread_join(thread[i],NULL);						
					}
					//杀死消费者线程
					for(int i = 0;i<CONSUMERS_COUNT;i++){
						pthread_mutex_unlock(&mutex);//如果不排除锁就一直有一个线程停在mutex上面
						pthread_cancel(thread[i]);						
					}								
					cout << "> pthread_join exit!!! " << "\n" << nextLineHeader;
					//销毁
					// pthread_mutex_destroy(&mutex);
					// pthread_cond_destroy(&cond);   

	    }else if(strncmp(userCommand,"update",6) == 0){

	    	int *keyIndex = new int;
	    	value_t *updateData = new value_t;

	    	int okNum = sscanf(userCommand,"update db %s %d %s where id=%d;", 
	    		 updateData->name,&(updateData->age),updateData->email,keyIndex);

			if(okNum < 3){
				cout << errorMessage<< nextLineHeader;
			}else{
				updateFunction(keyIndex,updateData);
			}

	    }
	    else{
	    	cout << errorMessage<< nextLineHeader;
	    }
	}



}

// insert
int insertRecord(bplus_tree *treePtr,int *index, value_t *values){
	
	bpt::key_t key;
	intToKeyT(&key,index);
	return (*treePtr).insert(key, *values);
}

// delete 
int deleteRecord(bplus_tree *treePtr,int *index){
	
	bpt::key_t key;
	intToKeyT(&key,index);

    return (*treePtr).remove(key);
}

// update
int updateRecord(bplus_tree *treePtr,int *index, value_t *values){
	bpt::key_t key;
	intToKeyT(&key,index);
	return (*treePtr).update(key, *values);
}

// search by index
int searchRecord(bplus_tree *treePtr,int *index, value_t *return_val){
	bpt::key_t key;
	intToKeyT(&key,index);//key is key_t ype of char k[16];there is to convert index [int] -> key_t
	return (*treePtr).search(key, return_val); 
}
// search all
int searchAll(bplus_tree *treePtr,int *start, int *end){

	TextTable t( '-', '|', '+' );

    t.add( " id " );
    t.add( " name " );
    t.add( " age " );
    t.add( " email " );
    t.endOfRow();

	bpt::key_t key;
	value_t *return_val = new value_t;

	for (int i = *start; i <= *end; ++i)
	{
		
		intToKeyT(&key,&i);
		
		int return_code = (*treePtr).search(key, return_val);
		switch(return_code){
			case -1:
			// no exist
				break;
			case 0:
			// find
				t.add( to_string(i) );
			    t.add( return_val ->name );
			    t.add( to_string(return_val ->age));
			    t.add( return_val ->email );
			    t.endOfRow();
				break;
			case 1:
			// deleted
				break;
		}
		

	}
	cout << t << endl;
	
}
// print table
void printTable(int *index, value_t *values){


	TextTable t( '-', '|', '+' );

    t.add( " id " );
    t.add( " name " );
    t.add( " age " );
    t.add( " email " );
    t.endOfRow();

    t.add( to_string(*index) );
    t.add( values ->name );
    t.add( to_string(values ->age));
    t.add( values ->email );
    t.endOfRow();

    cout << t << endl;
}
// int to key_t
void intToKeyT(bpt::key_t *a,int *b){
	char key[16] = { 0 };//如果只对数组的一部分进行初始化，则编译器将把其他元素设置为0。因此，当只将第一个元素初始化为0时，编译器会自动将其他元素设为0 (就像前一句说的那样)。
	sprintf(key, "%d", *b);
	*a = key;
}

bool is_file_exist(const char *fileName)
{
    ifstream ifile(fileName);
  	return ifile.good();
}

double durationTime(clock_t *f,clock_t *s){
	return (double)(*f - *s) / CLOCKS_PER_SEC;	
}




int main(int argc, char *argv[])
{
    //初始化
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    int i = 0;
    srand((unsigned int)time(NULL));

	initialSystem();


}
