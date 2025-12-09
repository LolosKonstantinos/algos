//
// Created by Constantin on 15/10/2025.
//
#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//todo: !!!!!!!!!IMPORTANT!!!!!!!!VITAL
//TODO: THE SECOND LEVEL HASH TABLE MUST HAVE A SIZE OF N^2, WHERE N IS THE NUMBER OF ELEMENTS IN THE 2ND LEVEL HT
//TODO: THE FIRST LEVEL CAN HAVE SOME MORE NODES THAN K, WHERE K THE TOTAL NUMBER OF ELEMENTS (USE 1.5K)
//TODO: RESIZING THE HASH TABLE MEANS THAT WE RECONSTRUCT THE WHOLE TABLE, BOTH LEVELS


struct first_level_ht_node {
    hashFunction hash;                   //the hash function used for this hash table
    void *second_level_hash_table;       //the second level hash table
    size_t bucket_count; //the number of buckets in the second level
    size_t hash_code;                    //the hash code that corresponds to this hash table //todo: kinda useless
    uint8_t hash_bit_length;             //the length in bits of the hash code
};

struct dynamic_perfect_hash_table_priv {
    hashFunction hash;
    first_level_ht_node *hash_table;
    size_t cell_count;
    size_t cell_size;
    size_t bucket_count;                //the total amount of elements stored
    uint8_t hash_bit_length;
    uint32_t key_length;
};


dpHashTable *new_dynamic_perfect_hash_table(size_t cell_size, size_t key_count,uint32_t max_key_length) {
    //key length longer than 8 bytes is treated as a string (doesn't have to be null terminated)
    dpHashTable *hash_table = malloc(sizeof(dpHashTable));
    if (hash_table == NULL) {
        return NULL;
    }
    //this is ceil(log2(key_count)), but faster
    hash_table->priv->hash_bit_length = (8 * sizeof(int)) - __builtin_clz(key_count -1);

    hash_table->priv = malloc((1<<hash_table->priv->hash_bit_length) * sizeof(first_level_ht_node));
    if (hash_table->priv == NULL) {
        free(hash_table);
        return NULL;
    }
    hash_table->priv->cell_count = key_count;
    hash_table->priv->cell_size = cell_size;
    hash_table->priv->key_length = max_key_length;
    hash_table->priv->hash = MurMurHash;

    //allocate space for the first hash table, we allocate 1.5 x key_count
    hash_table->priv->hash_table = calloc(key_count + (key_count>>1),sizeof(first_level_ht_node));
    if (hash_table->priv->hash_table == NULL) {
        free(hash_table->priv);
        free(hash_table);
        return NULL;
    }

    return hash_table;
}


int dpht_insert(dpHashTable *table, const char *key, size_t key_size, const char *value) {
    uint32_t hash_code;
    if (table == NULL|| key == NULL || value == NULL) return -1;
    //hash and keep the bits we care about
    {
        //the key cannot be zero
        void *temp = NULL;
        if (key_size <= sizeof(void *) && memcmp(key,temp,key_size) == 0)
            return -1;
    }
    hash_code = table->priv->hash(key,key_size);
    hash_code &= table->priv->hash_bit_length - 1; //use the hash_bit_length least significant bits

    if((table->priv->hash_table)[hash_code].second_level_hash_table == NULL) {
        if (new_second_level_hash_table((table->priv->hash_table) + hash_code,
                                                table->priv->cell_size,
                                                table->priv->key_length)) {
            return 1;
        }
    }

    table->priv->bucket_count++;
    //second level must be n^2, we always reconstruct


}


int new_second_level_hash_table(first_level_ht_node *node, size_t cell_size, uint32_t key_length) {
    //key length longer than 8 bytes is treated as a string (doesn't have to be null terminated)
    if (key_length > sizeof(void *)) key_length = sizeof(void *);
    //each cell has space for the key and the value
    node->second_level_hash_table = calloc(2, (cell_size + key_length));
    if (node->second_level_hash_table == NULL) {
        return 1;
    }
    node->hash = FastHash;
    node->bucket_count = 2;
    node->hash_bit_length = 1;
    return 0;
}

int expand_second_level_hash_table(first_level_ht_node *node, size_t cell_size, uint32_t key_length,uint32_t new_size) {
    void *temp;
    void *stack, *top = NULL;
    uint8_t old_bit_length;
    uint32_t hash_code, true_key_length = key_length;

    if (!node) return 1;
    if (node->second_level_hash_table == NULL) return 1;
    if (node->bucket_count >= new_size) return 1;
    if (key_length > sizeof(void *)) key_length = sizeof(void *);

    old_bit_length = node->hash_bit_length;
    node->hash_bit_length = (8 * sizeof(int)) - __builtin_clz(new_size * new_size);

    temp = realloc(node->second_level_hash_table,(1<<node->hash_bit_length) * (key_length + cell_size) );
    if (temp == NULL) {
        return 1;
    }
    node->second_level_hash_table = temp;
    memset(temp + ((1<<old_bit_length) * (key_length + cell_size)), 0,(1<<node->hash_bit_length - 1<<old_bit_length) * (key_length + cell_size));

    //now reconstruct the second level, we need to recalculate the hashes
    //create a stack to handle cases where a bucket is moved into a bucket that is being used
    stack = malloc(node->bucket_count * sizeof(first_level_ht_node));
    if (stack == NULL) {
        return 1;
    }
    top = NULL;

    temp = NULL;
    //todo: algorith to reconstruct the table
    //allocate the new table and rehash, then free the current table
}

int resize_first_level_hash_table(dpHashTable *table) {
    void *temp;
    if (table == NULL) return -1;

    ++(table->priv->hash_bit_length);
    temp = realloc(table->priv->hash_table, (1<<table->priv->hash_bit_length) * sizeof(first_level_ht_node));
    if (temp == NULL) {
        return 1;
    }
    table->priv->hash_table = temp;

    //move all active buckets to the new valid locations
    for (size_t i = 0; i < (1<<table->priv->hash_bit_length); ++i) {
        if (table->priv->hash_table[i].second_level_hash_table) {
            //there is a case the new bucket location is occupied by a bucket that must be relocated.
            //relocate the next bucket and then the initial one, in the worst case all buckets need to be relocated.
            //use a stack of the first_level_nodes.
            //also no need to check if the node is already in the stack
            //in all cases there cant be a relocation up, all buckets go down (0 can stay where it was in some cases)
            //so there is no case where two buckets exchange bucket
            //the hash code goes through new_hash = (2 * prev_hash) (+ 1 or +0) new_hash >= prev_hash
        }
    }
}

