#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "huff.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the nodes of the Huffman tree and other data have
 * been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */
int block_counter = 0;
int global_counter = 0;
int output = 0;
int path = 0;
int path_finder =0;
int power(int base,int power)
{
    int fin = 1;
    while(power>0)
    {
        fin = fin*base;
        power--;
    }
    return fin;
}
int clear_node()
{
    int i = num_nodes;
    NODE *node_ptr = nodes;
    for(int j = 0; j<i; j++)
    {
        (*node_ptr).symbol = 257;
        (*node_ptr).weight = 0;
        (*node_ptr).left = NULL;
        (*node_ptr).right = NULL;
        (*node_ptr).parent = NULL;
        node_ptr++;
    }
    return 1;
}
/**
 * @brief Emits a description of the Huffman tree used to compress the current block.
 * @details This function emits, to the standard output, a description of the
 * Huffman tree used to compress the current block.  Refer to the assignment handout
 * for a detailed specification of the format of this description.
 */
int swap_node(NODE *node_ptr, NODE *temp_node_ptr )
{
    if((node_ptr!=NULL) & (temp_node_ptr!=NULL)){
        NODE temp_of_temp = *temp_node_ptr;
        (*temp_node_ptr).left = (*node_ptr).left;
        (*temp_node_ptr).right = (*node_ptr).right;
        (*temp_node_ptr).symbol = (*node_ptr).symbol;
        (*temp_node_ptr).weight = (*node_ptr).weight;
        (*node_ptr).symbol = (temp_of_temp).symbol;
        (*node_ptr).weight = (temp_of_temp).weight;
        (*node_ptr).right = (temp_of_temp).right;
        (*node_ptr).left = (temp_of_temp).left;
        return 1;
    }
    return 0;

}

int node_sort( int length)
{
    NODE *node_ptr = nodes;
    for(int i = 0; i<length-1;i++)
    {
        NODE *temp_node_ptr = nodes;
        for(int j =0;j<length-1;j++)
        {
            int weight = (*temp_node_ptr).weight;
            temp_node_ptr++;
            if(weight < (*temp_node_ptr).weight)
                swap_node(temp_node_ptr-1,temp_node_ptr);
        }
    }
    return 1;
}

void post_order_print(NODE *node)
{
    // call left subtree
    if((*node).left!=NULL)
        post_order_print((*node).left);
     // call right subtree
    if((*node).right!=NULL)
        post_order_print((*node).right);
     // now deal with the node
     // if node is leaf
     if(((*node).left == NULL) & (((*node).right)==NULL)) {
        output = output*2;
        global_counter++;
        //printf("leaf found with char: %c\n",(*node).symbol );
        if(global_counter >= 8) {
            //printf("Putting in char: %x\n",(output)/2 );
            putchar((output)/2);
            output = 0;
            global_counter = 0;
        }

     }
     // if node is not leaf
     else  {
        if((*node).weight>0){
            output = (output+1)*2;
            global_counter++;
            //printf("Internal node found\n" );
            if(global_counter >= 8) {
                //printf("Putting in char: %x\n",(output)/2 );
                putchar((output)/2);
                output = 0;
                global_counter = 0;
            }
        }
     }

}
void post_order_leaf_print(NODE *node)
{
    if((*node).left!=NULL)
        post_order_leaf_print((*node).left);
    if((*node).right!=NULL)
        post_order_leaf_print((*node).right);
    if((*node).symbol<257) {
        if((*node).symbol==256)
        {
            putchar(255);
            putchar(0);
        }
        else
        putchar((*node).symbol);
    }
}
int end_symbol_exception = 0;
void post_order_decompress(NODE *node)
{
    if(node==NULL)
        return;
    if((*node).left!=NULL)
        post_order_decompress((*node).left);
    if((*node).right!=NULL)
        post_order_decompress((*node).right);
    if(((*node).left==NULL)&((*node).right==NULL))
    {
        if(end_symbol_exception!=0)
            (*node).symbol = end_symbol_exception;
        else
        {
            short temp = (short)getchar();
            (*node).symbol = temp;
            //printf("Char:%i (%c)\n",(*node).symbol,(char)(*node).symbol );
            if(temp==255)
            {
                end_symbol_exception = getchar();
            }
            else
            {
                end_symbol_exception = 0;
            }
        }
    }
}

void emit_huffman_tree()
{
    NODE *node_ptr = nodes;

    // Post order traversal
    node_ptr = nodes;
    //printf("Num Nodes:%i\n", num_nodes);
    putchar(num_nodes>>8);
    putchar(num_nodes);
    post_order_print(node_ptr);
    if(global_counter!=0) {
        for(int i = 0; i<8-num_nodes%8;i++) {
            output = output*2;
         //   printf("HELLO\n");
        }
    putchar(output/2);
    }
    global_counter = 0;
    output = 0;
    node_ptr = nodes;
    post_order_leaf_print(node_ptr);
    // Assigning pointers in node_for_symbol array
    NODE **node_for_symbol_ptr = node_for_symbol;
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        node_ptr = nodes;
        for (int j = 0; j < num_nodes; ++j)
        {
            if((int)(*node_ptr).symbol==i)
            {
                (*node_for_symbol_ptr) = node_ptr;
                break;
            }
            node_ptr++;
        }
        node_for_symbol_ptr++;
    }
    // Assigning Parents
    node_ptr = nodes;
    for(int i =0;i<num_nodes;i++)
    {
        if((*node_ptr).left!=NULL)
            (*((*node_ptr).left)).parent = node_ptr;
        if((*node_ptr).right!=NULL)
            (*((*node_ptr).right)).parent = node_ptr;
        node_ptr++;
    }

    node_ptr = nodes;
    node_for_symbol_ptr = node_for_symbol;
    char unsigned *ptr = current_block;
    NODE *path_ptr = nodes;
    for(int i =0;i<block_counter;i++)
    {
        node_ptr = nodes;
        node_for_symbol_ptr = node_for_symbol;
        short c = (short)*ptr;
        node_for_symbol_ptr+=c;
        node_ptr = (*node_for_symbol_ptr);
        //printf("Char %c: ", (char)(*node_ptr).symbol );
        while(node_ptr!=nodes)
        {
            path_ptr = node_ptr;
            node_ptr = (*node_ptr).parent;
            if(((*node_ptr).left)==path_ptr)
            {
                (*node_ptr).weight = 0;
                //printf("0");
            }
            else if((((*node_ptr).right)==path_ptr)){
                (*node_ptr).weight = 1;
                //printf("1");
            }
        }
        path_ptr = (*node_for_symbol_ptr);
        node_ptr = nodes;
        //printf("\n ");
        while(node_ptr!=path_ptr)
        {
            if((*node_ptr).weight == 0)
            {
                //printf("0");
                node_ptr = (*node_ptr).left;
                path_finder++;
                path = path*2;
                if(path_finder == 8)
                {
                    //printf("Putting in char: %x\n",(path)/2 );
                    putchar((path)/2);
                    path = 0;
                    path_finder = 0;
                }
            }
            else
            {
                //printf("1");
               node_ptr = (*node_ptr).right;
                path_finder++;
                path = (path+1)*2;
                if(path_finder == 8)
                {
                    //printf("Putting in char: %x\n",(path)/2 );
                    putchar((path)/2);
                    path = 0;
                    path_finder = 0;
                }
            }
        }
        //printf("Char:%c\n",(*node_ptr).symbol);
        ptr++;
    }
       // putting in pointer to end symbol
        node_ptr = nodes;
        node_for_symbol_ptr = node_for_symbol;
        short c = (short)256;
        node_for_symbol_ptr+=c;
        node_ptr = (*node_for_symbol_ptr);
        //printf("Char %c: ", (char)(*node_ptr).symbol );
        while(node_ptr!=nodes)
        {
            path_ptr = node_ptr;
            node_ptr = (*node_ptr).parent;
            if(((*node_ptr).left)==path_ptr)
            {
                (*node_ptr).weight = 0;
                //printf("0");
            }
            else if((((*node_ptr).right)==path_ptr)){
                (*node_ptr).weight = 1;
                //printf("1");
            }
        }
        path_ptr = (*node_for_symbol_ptr);
        node_ptr = nodes;
        //printf("\n Char %c: ",(char)(*path_ptr).symbol);
        while(node_ptr!=path_ptr)
        {
            if((*node_ptr).weight == 0)
            {
                //printf("0");
                node_ptr = (*node_ptr).left;
                path_finder++;
                path = path*2;
                if(path_finder >= 8)
                {
                    //printf("Putting in char: %x\n",(path)/2 );
                    putchar((path)/2);
                    path = 0;
                    path_finder = 0;
                }
            }
            else
            {
               // printf("1");
               node_ptr = (*node_ptr).right;
                path_finder++;
                path = (path+1)*2;
                if(path_finder >= 8)
                {
                    //printf("Putting in char: %x\n",(path)/2 );
                    putchar((path)/2);
                    path = 0;
                    path_finder = 0;
                }
            }
        }
        if(path_finder!=0)
        {
            //printf("%x\n",path/2 );
            for(int i = 0; i<8 - path_finder;i++ )
            path = path*2;
        }
        //printf("putting in char %x", path);
       putchar(path/2);
       path = 0;
       path_finder=0;
       return;
}

/**
 * @brief Reads a description of a Huffman tree and reconstructs the tree from
 * the description.
 * @details  This function reads, from the standard input, the description of a
 * Huffman tree in the format produced by emit_huffman_tree(), and it reconstructs
 * the tree from the description.  Refer to the assignment handout for a specification
 * of the format for this description, and a discussion of how the tree can be
 * reconstructed from it.
 *
 * @return 0 if the tree is read and reconstructed without error, otherwise 1
 * if an error occurs.
 */
int read_huffman_tree() {
    int byte_counter =0; // Counter for how many bytes have been used so far
    NODE *node = nodes; // Pointer for node array
    int found_end = 0;
    int char_output =0;
    int char_counter =0;
    while(found_end==0)
    {
        int i = 8;
        int bits = getchar();
        //printf("%x\n",bits);
        int bit_counter =0;
        int current_bit = -1;
        byte_counter++;
        while(bit_counter<i)
        {
            if(bit_counter!=7)
                current_bit =(bits>>(7-bit_counter)) & 1;
            else
                current_bit =bits & 1;
            //printf("Current bit: %i\n", current_bit);0
            if(((*node).left==NULL) & ((*node).right==NULL)){
                if((*node).symbol == 255){
                    found_end=1;
                    break;
                }
                //printf("Char is: %c\n", ((*node).symbol));
                putchar(((*node).symbol));
                node = nodes;

            }
            if(current_bit==1)
            {
                //printf("1\n");
                node = (*node).right;
            }
            else
            {
                node = (*node).left;
                //printf("0\n");

            }
            bit_counter++;
        }
    }
    return 1;
}

/**
 * @brief Reads one block of data from standard input and emits corresponding
 * compressed data to standard output.
 * @details This function reads raw binary data bytes from the standard input
 * until the specified block size has been read or until EOF is reached.
 * It then applies a data compression algorithm to the block and outputs the
 * compressed block to the standard output.  The block size parameter is
 * obtained from the global_options variable.
 *
 * @return 0 if compression completes without error, 1 if an error occurs.
 */
int compress_block()
{
        char c = 0;
        int i = 0;
        char unsigned *ptr = current_block;
        int blocksize = 0;
        //printf("%i\n", block_counter);
        while (i<block_counter)
        {
            c = getchar();
            if(c==-1)
                break;
            *ptr = c;
            blocksize++;
            //printf("Found: %c of value %hi\n", *ptr,(short)*ptr);
            ptr++;
            i++;
        }
        clear_node();
        //printf("%s\n", "calling compress block" );
        if(blocksize==0)
            return 1;
        block_counter = blocksize;
    //  Make Histogram
    //printf("Counter:%i\n", counter);
    ptr = current_block;
    for(int i =0; i<blocksize;i++)
    {
         char c = *ptr;
         //printf("%ith loop, character is: %c \n", i, c );
        short c_cast = (short) c;
        //printf("%c\n", c );
        int found_bool = 0;
        NODE *node_ptr = nodes;
        int count = 0;
        //printf("%s\n", "1");
        while((count < (2*MAX_SYMBOLS -1)) & (found_bool == 0))
        {
            //printf("%s\n", "2");
            short symbol = (*node_ptr).symbol;
            if(symbol == c_cast)
            {
                (*node_ptr).weight++;
                found_bool = 1;
            }
            count++;
            node_ptr++;
        }
        if(found_bool==0)
        {
            //printf("%s\n", "3");
            NODE *node = nodes;
            while(found_bool==0)
            {
                if((*node).weight!=0)
                    node++;
                else
                {
                    (*node).symbol = c_cast;
                    (*node).weight = 1;
                    found_bool = 1;
                }
            }
        }
        ptr++;
    }
    int low_end_counter = 1; // low end counter for nodes array
    int high_end_counter = 0; // high end counter for nodes array
    // Count Unique Char, i.e nodes + 1 for end symbol
    NODE *node_ptr = nodes;
    while(((*node_ptr).symbol!=257) & ((*node_ptr).weight>0))
    {
        node_ptr++;
        low_end_counter++;
    }
    int unique_char = low_end_counter;
    num_nodes = (2*unique_char) -1;
    // Add END Symbol
    (*node_ptr).symbol = 256;
    (*node_ptr).weight = 0;
    // sort the nodes by weight
    node_sort(low_end_counter);
    node_ptr = nodes;
    // Create Huffman Tree
    int parent_pointer = low_end_counter -1;
    while(low_end_counter>1)
    {
        node_ptr = node_ptr + low_end_counter -1;
        swap_node(node_ptr,(node_ptr-low_end_counter+(2*(low_end_counter) - 1))); // swap smallest node with node at n_huff_tree - 1
        node_ptr--;
        high_end_counter++;
        swap_node(node_ptr,(node_ptr-low_end_counter+(2*(low_end_counter) - 1))); // swap 2nd smallest node with node at n_huff_tree - 1
        high_end_counter++;
        // Create Parent
        (*node_ptr).left = (node_ptr-low_end_counter+(2*(low_end_counter) - 1))+1;
        (*node_ptr).right = (node_ptr-low_end_counter+(2*(low_end_counter) - 1));
        (*node_ptr).symbol = 257;
        (*node_ptr).weight = (*((*node_ptr).left)).weight + (*((*node_ptr).right)).weight; // weight = sum of children weight
        node_ptr = nodes; // reset node
        low_end_counter--;
        node_sort(low_end_counter); // sort low end
    }
    emit_huffman_tree();
    return 0;
}


/**
 * @brief Reads one block of compressed data from standard input and writes
 * the corresponding uncompressed data to standard output.
 * @details This function reads one block of compressed data from the standard
 * inputk it decompresses the block, and it outputs the uncompressed data to
 * the standard output.  The input data blocks are assumed to be in the format
 * produced by compress().  If EOF is encountered before a complete block has
 * been read, it is an error.
 *
 * @return 0 if decompression completes without error, 1 if an error occurs.
 */
int decompress_block()
{

    clear_node();
    // Clear the node to remove previous block
    int num_leaves = (num_nodes+1)/2;
    int byte_counter =0; // Counter for how many bytes have been used so far
    NODE *node = nodes; // Pointer for node array
    int low_end_counter = 0;
    int high_end_counter = 0;
    while(byte_counter<(num_nodes/8)+1)
    {

        int i = 8;
        int bits = getchar();
        //printf("%x\n",bits);
        int bit_counter =0;
        int current_bit = -1;
        if(byte_counter==((num_nodes)/8))
        {
            i = num_nodes%8;
        }
        byte_counter++;
        while(bit_counter<i)
        {
            if(bit_counter!=7){
                current_bit =(bits>>(7-bit_counter)) & 1;
            }
            else{
                current_bit =bits & 1;
            }
            //printf("Current bit: %i\n", current_bit);
            if(current_bit==1)
            {
                //printf("Making parent\n");
                swap_node(node-1,nodes+(num_nodes-high_end_counter-1));
                high_end_counter++;
                low_end_counter--;
                node--;
                swap_node(node-1,nodes+(num_nodes-high_end_counter-1));
                high_end_counter++;
                low_end_counter--;
                node--;
                (*node).right = (nodes+num_nodes-high_end_counter+1);
                (*node).left = (nodes+num_nodes-high_end_counter);
                node++;
                //When 1 is scanned, pop two nodes, call the first node popped "R"
                //and the second "L", create a new node "P" having L as its left child and R
                //as its right child, and push P onto the stack.
            }
            else
            {
                //printf("Making leaf\n");
                //  When 0 is scanned, create a new leaf node and push it on the stack
                node++;
                low_end_counter++;
                (*node).weight =257;
            }
            bit_counter++;
        }
    }
    node = nodes;
    post_order_decompress(node);
    read_huffman_tree();
    //printf("everything is guicci - your segmentation fault is in decompress and you can fix it later\n");
    return 0;
}

/**
 * @brief Reads raw data from standard input, writes compressed data to
 * standard output.
 * @details This function reads raw binary data bytes from the standard input in
 * blocks of up to a specified maximum number of bytes or until EOF is reached,
 * it applies a data compression algorithm to each block, and it outputs the
 * compressed blocks to standard output.  The block size parameter is obtained
 * from the global_options variable.
 *
 * @return 0 if compression completes without error, 1 if an error occurs.
 */
int compress()
{
    global_options = global_options - 0x2;
    block_counter = (int)(((unsigned int)global_options >> 16)+1);
    //printf("%i\n", block_counter);
    int c = 0;
    while(c!=1){
        c = compress_block();
        //printf("Return value is: %i\n", c);
    }

    return 0;
}

/**
 * @brief Reads compressed data from standard input, writes uncompressed
 * data to standard output.
 * @details This function reads blocks of compressed data from the standard
 * input until EOF is reached, it decompresses each block, and it outputs
 * the uncompressed data to the standard output.  The input data blocks
 * are assumed to be in the format produced by compress().
 *
 * @return 0 if decompression completes without error, 1 if an error occurs.
 */
int decompress() {
    // To be implemented.
    char c =0;
    while(c!=-1) {
        c = getchar();
        num_nodes = (int)(c<<8);
        num_nodes+= (int)getchar();
        if( num_nodes>2*MAX_SYMBOLS - 1 || num_nodes<1)
            return 1;
        //printf("%i\n",num_nodes );
        //printf("%i\n", c);
        if(c==-1)
            break;
        int val = decompress_block();
        if(val==1)
            return 1;
    }
    return 0;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and 1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and 1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    argv++;
    char *temp = *argv;

    /*No flag*/
    if(argc == 1) {
        return EXIT_FAILURE;
        /*do no flag stuff*/
    }
    if(*temp != '-') {
        return EXIT_FAILURE;
    }
    temp++;
    /* -h flag*/
     if(*temp == 'h') {
        temp++;
        if(*temp) {
            return EXIT_FAILURE;
        }
        global_options = 1;
        return EXIT_SUCCESS;
    }
    /* -c flag with -b*/
    else if(*temp == 'c') {
        if(argc == 2) {
            temp++;
            if(*temp) {
                return EXIT_FAILURE;
            }
            global_options = 0xffff0002;
            return EXIT_SUCCESS;
        }
        else if(argc == 4) {
            temp++;
            if(*temp) {
                return EXIT_FAILURE;
            }
            argv++;
            temp = *argv;
            temp++;
            if(*temp == 'b') {
                temp++;
                if(*temp) {
                    return EXIT_FAILURE;
                }
                argv++;
                temp = *argv;
                if(*temp) {
                    int len = 0;
                    while(*temp) {
                        len++;
                        temp++;
                    }
                    if (len==0)
                        return EXIT_FAILURE;
                    temp = temp-len;
                    int blocksize = 0;
                    while(len>0) {
                        int c = (int) (*temp - 48);
                        if (c>10) {
                            return EXIT_FAILURE;
                        }
                        int templen = len;
                        blocksize += c*power(10, len-1);
                        temp++;
                        len--;
                    }
                if( ((blocksize<1024)) || ((blocksize>65536)) )
                    return EXIT_FAILURE;
                blocksize-=1;
                blocksize = blocksize << 16;
                global_options = global_options | blocksize;
                global_options = global_options | 0x2;
                return EXIT_SUCCESS;
            }

            }
        }
        else {
            return EXIT_FAILURE;
        }


    }
    /* -d flag */
    else if(*temp =='d') {
        if (argc!=2){
            return EXIT_FAILURE;
        }
        temp++;
        if(*temp) {
            return EXIT_FAILURE;
        }
        global_options = 0xFFFF0004;
        return EXIT_SUCCESS;

    }
        return EXIT_FAILURE;
}


