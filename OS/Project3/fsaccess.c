/**
 *
 * Project 3 : Operating System Concepts
 *
 * FileName : fsaccess.c
 *
 * Team Memebers : Varadhan Ramamoorthy (vrr180003@utdallas.edu) and Humayoon Akthat Qaimkhani (hxq190001)
 *
 * UTD ID : 2021480952 and 2021505334
 *
 * Class : OS 5348.001
 *
 */

/**
 *
 * ----------------   BUGS --------------------------------------------
 *
 *
 *  1) Alignment padding of super block struct causing extra bytes assigned
 *
 *  2) Missing a pair paranthesis for File Descriptor will cause different behaviour
 */




/***********************************************************************
 
 
 
 This program allows user to do two things: 
   1. initfs: Initilizes the file system and redesigning the Unix file system to accept large 
      files of up tp 4GB, expands the free array to 152 elements, expands the i-node array to 
      200 elemnts, doubles the i-node size to 64 bytes and other new features as well.
   2. Quit: save all work and exit the program.
   
 User Input:
     - initfs (file path) (# of total system blocks) (# of System i-nodes)
     - q
     
 File name is limited to 14 characters.
 ***********************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

#define FREE_SIZE 152  
#define I_SIZE 200
#define BLOCK_SIZE 1024 // block size is 1024   
#define ADDR_SIZE 11    // addr array of size 11
#define INPUT_SIZE 256


// Superblock Structure
/**
 *
 * A Structure to represent the Superblock of the file system
 * Fixed Alignment padding by reordering the fields
 *
 */
typedef struct {
  char flock;
  char ilock;
  unsigned short isize;
  unsigned short fsize;
  unsigned short nfree;
  unsigned short ninode;
  unsigned short fmod;
  unsigned short time[2];
  unsigned short inode[I_SIZE];
  unsigned int free[FREE_SIZE];
} superblock_type;

superblock_type superBlock;

// I-Node Structure
/**
 *
 * A Structure to represent the I-Node of the file system
 * size of inode : 64 bytes
 *
 */
typedef struct {
unsigned short flags;
unsigned short nlinks;
unsigned short uid;
unsigned short gid;
unsigned int size;
unsigned int addr[ADDR_SIZE];
unsigned short actime[2];
unsigned short modtime[2];
} inode_type; 

inode_type inode;


/**
 *
 * A Structure to represent directory in the file system
 *
 */
typedef struct {
  unsigned short inode;
  unsigned char filename[14];
} dir_type;

dir_type root;

int fileDescriptor ;		//file descriptor 
// inode is free/not allocates
const unsigned short inode_alloc_flag = 0100000;
// A flag to check if the inode is directory or not
const unsigned short dir_flag = 040000;
// A flag for large file
const unsigned short dir_large_file = 010000;
// A flag for permission privileges, 777 means the access is given to all i.e, Users, Grops and World
const unsigned short dir_access_rights = 000777; // User, Group, & World have all access privileges 
// i_node of size 64 byte long
const unsigned short INODE_SIZE = 64; // inode has been doubled

/**
 * Function Declarations
 * 
 */
int initfs(char* path, unsigned short total_blcks,unsigned short total_inodes);
void add_block_to_free_list( int blocknumber , unsigned int *empty_buffer );
void create_root();

/**
 * Driver function for the program
 */
int main() {
 
  char input[INPUT_SIZE];
  char *splitter;
  unsigned int numBlocks = 0, numInodes = 0;
  char *filepath;
  printf("Size of super block = %d , size of i-node = %d\n",sizeof(superBlock),sizeof(inode));
  printf("Enter command:\n");
  
  /**
   * A while loop to get user command
   *
   * if q return 0 , exit from mai program
   *
   * if initfs , Initialize v6 file system
   *
   */
  while(1) {
  
    scanf(" %[^\n]s", input);
    
    // split the input command by space
    splitter = strtok(input," ");
   
    // If command is initfs

    if(strcmp(splitter, "initfs") == 0){
    
        preInitialization();
        splitter = NULL;

    // If command is q, write the file descriptor before exiting from main                   
    } else if (strcmp(splitter, "q") == 0) {
       // reposition the pointer of the file descriptor to block size offset
       lseek(fileDescriptor, BLOCK_SIZE, 0);
       // write the super block to the first block
       write(fileDescriptor, &superBlock, BLOCK_SIZE);
       return 0;
     
    } 
  }
}


/**
 *
 * Initialize V6 File System with number of block and number of i_nodes
 *
 * Contains Validation for File path existence
 *
 */
int preInitialization(){

  char *n1, *n2;
  unsigned int numBlocks = 0, numInodes = 0;
  char *filepath;

  // Retrieve v6 File Path name, number of block, number of i_nodes
  filepath = strtok(NULL, " ");
  n1 = strtok(NULL, " ");
  n2 = strtok(NULL, " ");

 // printf("%s",filepath);
         
  // Check the existence of the file path and open it if it exist   
  if(access(filepath, F_OK) != -1) {
      
      // open the file with read/write access mode
      // Bug Fix
      if((fileDescriptor = open(filepath, O_RDWR, 0600)) == -1){
      
         printf("\n filesystem already exists but open() failed with error [%s]\n", strerror(errno));
         return 1;
      }
      printf("filesystem already exists and the same will be used.\n");
  
  } else {
  		//the file does not exist

        	if (!n1 || !n2)
              printf(" All arguments(path, number of inodes and total number of blocks) have not been entered\n");
            
       		else {
          		numBlocks = atoi(n1);
          		numInodes = atoi(n2);
          		
			//Initialize the v6 file system with given file path , number of blocks and number of i_nodes
          		if( initfs(filepath,numBlocks, numInodes )){
          		  printf("The file system is initialized\n");	
          		} else {
            		printf("Error initializing file system. Exiting... \n");
            		return 1;
          		}
       		}
  }
}


/**
 *
 * Function to iniatize the v6 file system with number of blocks and number of inodes
 *
 */
int initfs(char* path, unsigned short blocks,unsigned short inodes) {

   unsigned int buffer[BLOCK_SIZE/4];
   int bytes_written;
   
   unsigned short i = 0;
   // fsize of super block, beyond that it is illegal block number
   superBlock.fsize = blocks;
   // # of inodes that can be accomodated per block
   unsigned short inodes_per_block= BLOCK_SIZE/INODE_SIZE;
   
   //calculate the isize of super block
   if((inodes%inodes_per_block) == 0)
   superBlock.isize = inodes/inodes_per_block;
   else
   superBlock.isize = (inodes/inodes_per_block) + 1;
   
   // Create a file with name path with read,write and execute permissions
   if((fileDescriptor = open(path,O_RDWR|O_CREAT,0700))== -1)
       {
           printf("\n open() failed with the following error [%s]\n",strerror(errno));
           return 0;
       }
   
   // Initialize the free array
   for (i = 0; i < FREE_SIZE; i++)
      superBlock.free[i] =  0;			//initializing free array to 0 to remove junk data. free array will be stored with data block numbers shortly.
    
   superBlock.nfree = 0;
   superBlock.ninode = I_SIZE;
   
   // inode are contiguous arranged, so we need to maintain array of inode number for inode access
   for (i = 0; i < I_SIZE; i++)
	    superBlock.inode[i] = i + 1;		//Initializing the inode array to inode numbers
   
  
   superBlock.flock = 'a'; 					//flock,ilock and fmode are not used.
   superBlock.ilock = 'b';					
   superBlock.fmod = 0;
   superBlock.time[0] = 0;
   superBlock.time[1] = 1970;


   // reposition the file descriptor to the first block
   lseek(fileDescriptor, BLOCK_SIZE, SEEK_SET);
   // write the super block to the first vlock
   write(fileDescriptor, &superBlock, BLOCK_SIZE); // writing superblock to file system
   
   // writing zeroes to all inodes in ilist
   for (i = 0; i < BLOCK_SIZE/4; i++) 
   	  buffer[i] = 0;
   // For each inodes, write zeros     
   for (i = 0; i < superBlock.isize; i++)
   	  write(fileDescriptor, buffer, BLOCK_SIZE);
   
   // First Data block is (total number of blocks - 2 - total number of blocks for inode)
   int data_blocks = blocks - 2 - superBlock.isize;
   int data_blocks_for_free_list = data_blocks - 1;
   
   // Create root directory
   create_root();

   // add all the free data blocks to free list 
   for ( i = 2 + superBlock.isize + 1; i < data_blocks_for_free_list; i++ ) {
      add_block_to_free_list(i , buffer);
   }
   
   return 1;
}

// Add Data blocks to free list
void add_block_to_free_list(int block_number,  unsigned int *empty_buffer){

  if ( superBlock.nfree == FREE_SIZE ) {

    int free_list_data[BLOCK_SIZE / 4], i;
    free_list_data[0] = FREE_SIZE;
    
    for ( i = 0; i < BLOCK_SIZE / 4; i++ ) {
       if ( i < FREE_SIZE ) {
         free_list_data[i + 1] = superBlock.free[i];
       } else {
         free_list_data[i + 1] = 0; // getting rid of junk data in the remaining unused bytes of header block
       }
    }
    
    lseek( fileDescriptor, (block_number) * BLOCK_SIZE, 0 );
    write( fileDescriptor, free_list_data, BLOCK_SIZE ); // Writing free list to header block
    
    superBlock.nfree = 0;
    
  } else {
    // Reposition file descriptor to the free block and write 0 to it	
    lseek( fileDescriptor, (block_number) * BLOCK_SIZE, 0 );
    write( fileDescriptor, empty_buffer, BLOCK_SIZE );  // writing 0 to remaining data blocks to get rid of junk data
  }

  superBlock.free[superBlock.nfree] = block_number;  // Assigning blocks to free array
  // Increment nfree after assigning blocks to free list
  ++superBlock.nfree;
}

// Create root directory
void create_root() {
  
  int root_data_block = 2 + superBlock.isize; // Allocating first data block to root directory
  int i;
  
  root.inode = 1;   // root directory's inode number is 1.
 
  root.filename[0] = '.';
  root.filename[1] = '\0';

  //set inode as allocated, is a directory, is a large file and access to all
  inode.flags = inode_alloc_flag | dir_flag | dir_large_file | dir_access_rights;   		// flag for root directory 
  inode.nlinks = 0; 
  inode.uid = 0;
  inode.gid = 0;
  inode.size = INODE_SIZE;
  inode.addr[0] = root_data_block;
  
  //Initialize data blocks to 0
  for( i = 1; i < ADDR_SIZE; i++ ) {
    inode.addr[i] = 0;
  }
  //set access time and modification time
  inode.actime[0] = 0;
  inode.modtime[0] = 0;
  inode.modtime[1] = 0;
  
  //Reposition the file descriptor and write the root inode to the first inode of the second block
  lseek(fileDescriptor, 2 * BLOCK_SIZE, 0);
  write(fileDescriptor, &inode, INODE_SIZE);   // 
  // Reposition the file descriptor to the root data block and write root's 16 byte content  
  lseek(fileDescriptor, root_data_block, 0);
  write(fileDescriptor, &root, 16);
  
  // The first and the second filename for root are root itself 
  root.filename[0] = '.';
  root.filename[1] = '.';
  root.filename[2] = '\0';
  
  write(fileDescriptor, &root, 16);
 
}
