#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BYTE unsigned char

static int stack[128];
//static int functions[16];
static bool terminate = false;
/*enum{
	FP,			//FRAME P
	SP,			//STACK P
	PC,			//PROGRAM COUNTER
	A,B,C,D,E //GENERAL
};*/

struct functions{
		int label;
		int num_arg;
		int return_value;
		int num_instrcution;
};
struct functions *function;

static int count_function=0;

//static int check_print=0;

static int registers[8];

void int_to_binary(unsigned int dec, int count, int* bin)
{
    unsigned int mask = 1U << (count-1);
    for (int a = 0; a < count; a++) {
        bin[a] = (dec & mask) ? 1 : 0;
        dec <<= 1;
    }
}
int to_decimal(int* bit, int length, int pointer){
	int decimal = 0;
	for (int a = pointer; a < pointer+length; a++)
	{decimal = decimal * 2 + bit[a];}
	return decimal;
}

static int temp=0;

int move_pointer(int* file_bin,int pointer,int length, int counter){
	pointer=pointer-temp;
	pointer=pointer-3;
	int op_code=to_decimal(file_bin,length,pointer);
		switch(op_code){
			case 0: {
				if(to_decimal(file_bin,2,pointer-2)==1){
					//temp=15; counter=counter+1;
						if(to_decimal(file_bin,2,pointer-7)==0){
							temp=15; counter=counter+1;
							return pointer; break;
						}
						if(to_decimal(file_bin,2,pointer-7)==1){
							temp=10; counter=counter+1;
							return pointer; break;
						}
						if(to_decimal(file_bin,2,pointer-7)==2){
							temp=14; counter=counter+1;
							return pointer; break;
						}
						if(to_decimal(file_bin,2,pointer-7)==3){
							temp=14; counter=counter+1;
							return pointer; break;
						}
					//return pointer; break;
				}
				if(to_decimal(file_bin,2,pointer-2)==2){
					//temp=19; counter=counter+1; 
					if(to_decimal(file_bin,2,pointer-11)==0){
							temp=19; counter=counter+1;
							return pointer; break;
						}
						if(to_decimal(file_bin,2,pointer-11)==1){
							temp=14; counter=counter+1;
							return pointer; break;
						}
						if(to_decimal(file_bin,2,pointer-11)==2){
							temp=18; counter=counter+1;
							return pointer; break;
						}
						if(to_decimal(file_bin,2,pointer-11)==3){
							temp=18; counter=counter+1;
							return pointer; break;
						}
					//return pointer; break;
				}
				
			}
			case 1: {temp=19; counter=counter+1; return pointer; break;}
			case 2: {temp=9; counter=counter+1; return pointer; break;}
			case 3: {counter=counter+1; return pointer; break;}
			case 4: {temp=10; counter=counter+1; return pointer; break;}
			case 5: {temp=10; counter=counter+1; return pointer; break;}
			case 6: {temp=5; counter=counter+1; return pointer; break;}
			case 7: {temp=5; counter=counter+1; return pointer; break;}
			default: return -1;
		}
	
}

int find_function(int* file_bin, int* op_index, struct functions *func, int instruct_count){
	int op_code=to_decimal(file_bin,3,op_index[instruct_count-1]);
	int pointer=op_index[instruct_count-1];
	switch(op_code){
			case 0: {
				if(to_decimal(file_bin,2,pointer-2)==1){
					
						if(to_decimal(file_bin,2,pointer-7)==0){
							pointer=pointer-23;break;
						}
						if(to_decimal(file_bin,2,pointer-7)==1){
							pointer=pointer-18;break;
						}
						if(to_decimal(file_bin,2,pointer-7)==2){
							pointer=pointer-22;break;
						}
						if(to_decimal(file_bin,2,pointer-7)==3){
							pointer=pointer-22;break;
						}
					
				}
				if(to_decimal(file_bin,2,pointer-2)==2){
					if(to_decimal(file_bin,2,pointer-11)==0){
							pointer=pointer-27;break;
						}
						if(to_decimal(file_bin,2,pointer-11)==1){
							pointer=pointer-22;break;
						}
						if(to_decimal(file_bin,2,pointer-11)==2){
							pointer=pointer-26;break;
						}
						if(to_decimal(file_bin,2,pointer-11)==3){
							pointer=pointer-26;break;
						}
				}
				if(to_decimal(file_bin,2,pointer-2)==3){
					if(to_decimal(file_bin,2,pointer-11)==0){
							pointer=pointer-27;break;
						}
						if(to_decimal(file_bin,2,pointer-11)==1){
							pointer=pointer-22;break;
						}
						if(to_decimal(file_bin,2,pointer-11)==2){
							pointer=pointer-26;break;
						}
						if(to_decimal(file_bin,2,pointer-11)==3){
							pointer=pointer-26;break;
						}
				}
			}
			case 1: {pointer=pointer-27;break;}
			case 2: {break;}
			case 3: {break;}
			case 4: {pointer=pointer-18;break;}
			case 5: {pointer=pointer-18;break;}
			case 6: {pointer=pointer-13;break;}
			case 7: {pointer=pointer-13;break;}
	}
	int function_label=to_decimal(file_bin,4,pointer);
	//func[function_label].num_arg=to_decimal(file_bin,4,pointer+4);
	func[count_function].label=function_label;
	func[count_function].num_arg=to_decimal(file_bin,4,pointer+4);
	if(func[count_function].num_arg==0){stack[1]=3;
										//stack[2]=4;
									   }
	if(func[count_function].num_arg==1){stack[1]=4;
										//stack[2]=5;
									   }
	if(func[count_function].num_arg==2){stack[1]=5;
										//stack[2]=6;
									   }
	//func[function_label].return_value=return_value;
	count_function+=1;
	return pointer;
}

int move_pointer_address(int* file_bin,int op_code,int pointer,int length){
	int address_type=to_decimal(file_bin,length,pointer);
	int address_type_2;
	int operants[2];
	int index=1;
	switch(op_code){
		case 0:{
			for(int i=0; i<2; i++){
				if(index==0){
					pointer=pointer-2;
					address_type_2=to_decimal(file_bin,2,pointer);
					if(address_type_2==0){pointer=pointer-8; operants[index]=to_decimal(file_bin,8,pointer);break;}
					if(address_type_2==1){pointer=pointer-3; operants[index]=registers[to_decimal(file_bin,3,pointer)];break;}
					if(address_type_2==2){pointer=pointer-7; operants[index]=stack[to_decimal(file_bin,7,pointer)];break;}
					if(address_type_2==3){pointer=pointer-7; operants[index]=stack[stack[to_decimal(file_bin,7,pointer)]];break;}
				}
				if(address_type==1&&index==1){
					pointer=pointer-3;
					operants[index]=to_decimal(file_bin,3,pointer);
					index=index-1;
					continue;
				}
				if(address_type==2&&index==1){
					pointer=pointer-7;
					operants[index]=to_decimal(file_bin,7,pointer);
					if(operants[index]==2){
						terminate=true;
						//printf("check");
					}
					index=index-1;
					continue;
				}
				if(address_type==3&&index==1){
					pointer=pointer-7;
					operants[index]=to_decimal(file_bin,7,pointer);
					index=index-1;
					continue;
				}
			}
			
			if(address_type==1){registers[operants[1]]=operants[0];}
			if(address_type==2){stack[operants[1]]=operants[0];
								if(operants[1]!=2){stack[1]+=1;}
							   }
			if(address_type==3){stack[stack[operants[1]]]=operants[0];stack[1]+=1;}
			//printf("%d ",stack[3]);
			if(terminate==true){
				terminate=false;
				return -1;
				break;
			}
			stack[2]+=1;
			return 0;
			break;
	}
		case 1:{
				for(int i=0; i<2; i++){
					if(index==0){
						pointer=pointer-10;
						operants[index]=to_decimal(file_bin,8,pointer);
						break;
					}
					pointer=pointer-7;
					operants[index]=to_decimal(file_bin,7,pointer);
					index-=1;
					continue;
				}
				
				//stack[1]+=1;
				return 0;break;}
		case 2:{pointer=pointer-7;operants[0]=to_decimal(file_bin,7,pointer);
				if(function[0].num_arg==0){stack[3]=stack[operants[0]];}
				if(function[0].num_arg==1){stack[4]=stack[operants[0]];}
				if(function[0].num_arg==2){stack[5]=stack[operants[0]];}
				stack[2]+=1;return 0;break;}
		case 3:{ 
				/*if(count_function==1){
					printf("%d\n",stack[operants[0]]);
					break;
				}
				if(function[check_print].label==0){
				printf("%d\n",stack[operants[0]]);
				}
				else{
					if(function[0].num_arg==0){stack[3]=stack[operants[0]];}
					if(function[0].num_arg==1){stack[4]=stack[operants[0]];}
					if(function[0].num_arg==2){stack[5]=stack[operants[0]];}
				}
				check_print+=1;*/
				return stack[operants[0]];break;}
		case 4:{
			for(int i=0; i<2; i++){
				if(index==0){
					pointer=pointer-5;
					operants[index]=to_decimal(file_bin,3,pointer);
					break;
				}
				pointer=pointer-3;
				operants[index]=to_decimal(file_bin,3,pointer);
				index-=1;
			}
			if(registers[operants[0]]+registers[operants[1]]>255){
				registers[operants[0]]=registers[operants[0]]+registers[operants[1]]-256;
				stack[2]+=1;
				return 0;
				break;
			}
			registers[operants[0]]=registers[operants[0]]+registers[operants[1]];
			stack[2]+=1;
			return 0;break;}
		case 5:{
			for(int i=0; i<2; i++){
				if(index==0){
					pointer=pointer-5;
					operants[index]=to_decimal(file_bin,3,pointer);
				}
				pointer=pointer-3;
				operants[index]=to_decimal(file_bin,3,pointer);
				index-=1;
			}
			int value1[8];
			int value2[8];
			int result[8];
			int_to_binary(registers[operants[0]],8,value1);
			int_to_binary(registers[operants[1]],8,value2);
			for(int i=0;i<8;i++){
				result[i]=value1[i] & value2[i];
			}
			registers[operants[0]]=to_decimal(result,8,0);
			stack[2]+=1;
			return 0;break;}
		case 6:{
			pointer=pointer-3;
			operants[0]=to_decimal(file_bin,3,pointer);
			int value[8];
			int result[8];
			int_to_binary(registers[operants[0]],8,value);
			for(int i=0;i<8;i++){
				result[i]=!value[i];
			}
			registers[operants[0]]=to_decimal(result,8,0);
			stack[2]+=1;
			return 0;break;}
		case 7:{
			pointer=pointer-3;
			operants[0]=to_decimal(file_bin,3,pointer);
			//printf("%d ",operants[0]);
			if(registers[operants[0]]==0){
				registers[operants[0]]=1;
				stack[2]+=1;
				return 0;break;
			}
			if(registers[operants[0]]!=0){
				registers[operants[0]]=0;
				stack[2]+=1;
				return 0;break;
			}
		}
	}
	return 0;
}

int calculator(int* file_bin,int* op_index,int instruction_count){
		int pointer;
		int op_code;
		int return_value;
		int jump=100;
		for(int i=instruction_count-1; i>=0; i--){
			if(i==instruction_count-1-jump){
				continue;
			}
			pointer=op_index[i]-2;
			op_code=to_decimal(file_bin,3,op_index[i]);
			return_value=move_pointer_address(file_bin,op_code,pointer,2);
			if(return_value==-1){
				jump=stack[2];
				stack[2]+=1;
				//printf("%d ",op_index[instruction_count-1]);
				if(i<instruction_count-1-jump){
					i=instruction_count-jump;
					if(stack[1]>127){break;}
				}
			}
		}
	 	if(stack[1]>127){
			return -1;
		}
	return return_value;
}

int instruction_counting=0;
//int* instructions;
//int opcode_index[instruction_counting];

void read_file(int* file_bin, int file_length, int* opcode_index){
	int read_pointer=file_length;
	int i=0;
	//instruction_counting=instruction_counting+to_decimal(file_bin,8,read_pointer);
	read_pointer=read_pointer-8;
	
	int counter=0;
	while(counter<instruction_counting){
		read_pointer=move_pointer(file_bin,read_pointer,3,counter);
		opcode_index[i]=read_pointer;
		i=i+1;
		counter=counter+1;
	}
}

/*
 * main
 * Scaffold code for the COMP2017/9017 assignment 1
 * This code will take a filename and print the numeric value associated
 * with each byte in the file
 * :: int n_args :: The number of arguments passed to the file
 * :: char** vargs :: Stores the arguments passed to this program as cstrings
 * Returns 0
 */
int main(int n_args, char** vargs)
{
	if (n_args <= 1) {
        printf("Error: no input files\n");
        return -1;
    }
    // For readability
    char* filename = vargs[1];
    // A pro-tip to prevent seg-faults for yourself might be to check 
    // if this file actually exists, and if an argument is actually passed...

    // Create a 1 byte buffer. You might want more than one byte yourself
    BYTE buffer;

    // Open our file in 'read binary' mode,
    // I sure do hope that this file actually exists
    FILE *file_ptr = fopen(filename,"rb");
	
	if (!file_ptr) {
        printf("Error: could not read file %s \n", filename);
        return -1;
    }
	
    // Skip to the end of the file
    fseek(file_ptr, 0, SEEK_END); 

    // Find the length of the file
    size_t file_length = ftell(file_ptr);
    
    // Rewind to the start of the file so we can actually read it
    rewind(file_ptr);  

    // For each byte in the file, first write the byte to our buffer
    // then print the integer representation of the byte
	
	int file_in_bin[file_length*8];//store reading byte in binary
	int file_index=0;
	
    for (int i = 0; i < file_length; i++)
    {
       	fread(&buffer, 1, 1, file_ptr); // read 1 byte
       	// printf("%d ", (int)buffer);
	   	int byte[8];
       	int_to_binary((int)buffer, 8, byte);
		for (int a = 0; a < 8; a++)
		{ 	file_in_bin[file_index]=byte[a];
			file_index++;
		}
		/*for (int a = 0; a < 8; a++)
		{	printf("%d", byte[a]); }
		printf("\n");*/
    
    }
	
	
	instruction_counting=to_decimal(file_in_bin,8,file_length*8-8);
	//int num_inst=0;
	//num_inst=num_inst+instruction_counting;
	int opcode_index[instruction_counting];
	int return_value;
	int pointer;
	
	function=(struct functions *)malloc(sizeof(struct functions)*4);
	
	read_file(file_in_bin,file_length*8,opcode_index);
	
	pointer=find_function(file_in_bin, opcode_index, function, instruction_counting);
	
	if(pointer>7){
	int* new_opcode_index;
	//int new_opcode_index[instruction_counting];
	while(pointer>7){
		instruction_counting=to_decimal(file_in_bin,8,pointer-8);
		//num_inst=num_inst+instruction_counting;
		temp=0;
		//int new_opcode_index[instruction_counting];
		new_opcode_index=(int*)malloc(sizeof(int)*instruction_counting);
		
		read_file(file_in_bin,pointer,new_opcode_index);
		
		pointer=find_function(file_in_bin, new_opcode_index, function, instruction_counting);
	
		return_value=calculator(file_in_bin,new_opcode_index,instruction_counting);
		
	}
		free(new_opcode_index);
		
		instruction_counting=to_decimal(file_in_bin,8,file_length*8-8);
		
		return_value=calculator(file_in_bin,opcode_index,instruction_counting);
		if(return_value==-1){
		printf("Stack Overflow!\n");
		return 1;
		}
		printf("%d\n",return_value);
	}
	
	else{
	return_value=calculator(file_in_bin,opcode_index,instruction_counting);
	if(return_value==-1){
		printf("Stack Overflow!\n");
		return 1;
	}
	
	printf("%d\n",return_value);
	}
	/*return_value=calculator(file_in_bin,opcode_index,instruction_counting);
	
	//int pointer=find_function(file_in_bin, opcode_index, function, instruction_counting,return_value);
	//printf("%d ",pointer);
	if(pointer>7){
	int* new_opcode_index;
	//int new_opcode_index[instruction_counting];
	while(pointer>7){
		instruction_counting=to_decimal(file_in_bin,8,pointer-8);
		temp=0;
		//int new_opcode_index[instruction_counting];
		new_opcode_index=(int*)malloc(sizeof(int)*instruction_counting);
		
		read_file(file_in_bin,pointer,new_opcode_index);
		
		pointer=find_function(file_in_bin, new_opcode_index, function, instruction_counting);
	
		return_value=calculator(file_in_bin,new_opcode_index,instruction_counting);
		
	}
		free(new_opcode_index);
	}*/
    return 0;
}
