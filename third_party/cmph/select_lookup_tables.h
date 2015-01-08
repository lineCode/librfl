#ifndef SELECT_LOOKUP_TABLES
#define SELECT_LOOKUP_TABLES

#include "cmph_types.h"

/*
rank_lookup_table[i] simply gives the number of bits set to one in the byte of value i.
For example if i = 01010101 in binary then we have :
rank_lookup_table[i] = 4
*/

static cmph_uint8 rank_lookup_table[256] ={
   0 , 1 , 1 , 2 , 1 , 2 , 2 , 3 , 1 , 2 , 2 , 3 , 2 , 3 , 3 , 4
,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 , 2 , 3 , 3 , 4 , 3 , 4 , 4 , 5
,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 , 2 , 3 , 3 , 4 , 3 , 4 , 4 , 5
,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 , 3 , 4 , 4 , 5 , 4 , 5 , 5 , 6
,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 , 2 , 3 , 3 , 4 , 3 , 4 , 4 , 5
,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 , 3 , 4 , 4 , 5 , 4 , 5 , 5 , 6
,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 , 3 , 4 , 4 , 5 , 4 , 5 , 5 , 6
,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 6 , 4 , 5 , 5 , 6 , 5 , 6 , 6 , 7
,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 , 2 , 3 , 3 , 4 , 3 , 4 , 4 , 5
,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 , 3 , 4 , 4 , 5 , 4 , 5 , 5 , 6
,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 , 3 , 4 , 4 , 5 , 4 , 5 , 5 , 6
,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 6 , 4 , 5 , 5 , 6 , 5 , 6 , 6 , 7
,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 , 3 , 4 , 4 , 5 , 4 , 5 , 5 , 6
,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 6 , 4 , 5 , 5 , 6 , 5 , 6 , 6 , 7
,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 6 , 4 , 5 , 5 , 6 , 5 , 6 , 6 , 7
,  4 , 5 , 5 , 6 , 5 , 6 , 6 , 7 , 5 , 6 , 6 , 7 , 6 , 7 , 7 , 8 
 };

/*
select_lookup_table[i][j] simply gives the index of the j'th bit set to one in the byte of value i.
For example if i=01010101 in binary then we have :
select_lookup_table[i][0] = 0,   the first bit set to one is at position 0
select_lookup_table[i][1] = 2,   the second bit set to one is at position 2
select_lookup_table[i][2] = 4,   the third bit set to one is at position 4
select_lookup_table[i][3] = 6,   the fourth bit set to one is at position 6
select_lookup_table[i][4] = 255, there is no more than 4 bits set to one in i, so we return escape value 255. 
*/
static cmph_uint8 select_lookup_table[256][8]={
{ 255 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 2 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 255 , 255 , 255 , 255 , 255 } ,
{ 3 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 255 , 255 , 255 , 255 , 255 } ,
{ 2 , 3 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 255 , 255 , 255 , 255 } ,
{ 4 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 255 , 255 , 255 , 255 , 255 } ,
{ 2 , 4 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 255 , 255 , 255 , 255 } ,
{ 3 , 4 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 255 , 255 , 255 , 255 } ,
{ 2 , 3 , 4 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 255 , 255 , 255 } ,
{ 5 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 5 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 5 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 5 , 255 , 255 , 255 , 255 , 255 } ,
{ 2 , 5 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 5 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 5 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 5 , 255 , 255 , 255 , 255 } ,
{ 3 , 5 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 5 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 5 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 5 , 255 , 255 , 255 , 255 } ,
{ 2 , 3 , 5 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 5 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 5 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 5 , 255 , 255 , 255 } ,
{ 4 , 5 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 5 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 5 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 5 , 255 , 255 , 255 , 255 } ,
{ 2 , 4 , 5 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 5 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 5 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 5 , 255 , 255 , 255 } ,
{ 3 , 4 , 5 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 5 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 5 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 5 , 255 , 255 , 255 } ,
{ 2 , 3 , 4 , 5 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 5 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 5 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 5 , 255 , 255 } ,
{ 6 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 6 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 6 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 6 , 255 , 255 , 255 , 255 , 255 } ,
{ 2 , 6 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 6 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 6 , 255 , 255 , 255 , 255 } ,
{ 3 , 6 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 6 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 6 , 255 , 255 , 255 , 255 } ,
{ 2 , 3 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 6 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 6 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 6 , 255 , 255 , 255 } ,
{ 4 , 6 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 6 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 6 , 255 , 255 , 255 , 255 } ,
{ 2 , 4 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 6 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 6 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 6 , 255 , 255 , 255 } ,
{ 3 , 4 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 6 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 6 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 6 , 255 , 255 , 255 } ,
{ 2 , 3 , 4 , 6 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 6 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 6 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 6 , 255 , 255 } ,
{ 5 , 6 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 5 , 6 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 5 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 5 , 6 , 255 , 255 , 255 , 255 } ,
{ 2 , 5 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 5 , 6 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 5 , 6 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 5 , 6 , 255 , 255 , 255 } ,
{ 3 , 5 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 5 , 6 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 5 , 6 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 5 , 6 , 255 , 255 , 255 } ,
{ 2 , 3 , 5 , 6 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 5 , 6 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 5 , 6 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 5 , 6 , 255 , 255 } ,
{ 4 , 5 , 6 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 5 , 6 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 5 , 6 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 5 , 6 , 255 , 255 , 255 } ,
{ 2 , 4 , 5 , 6 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 5 , 6 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 5 , 6 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 5 , 6 , 255 , 255 } ,
{ 3 , 4 , 5 , 6 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 5 , 6 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 5 , 6 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 5 , 6 , 255 , 255 } ,
{ 2 , 3 , 4 , 5 , 6 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 5 , 6 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 5 , 6 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 255 } ,
{ 7 , 255 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 7 , 255 , 255 , 255 , 255 , 255 } ,
{ 2 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 7 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 7 , 255 , 255 , 255 , 255 } ,
{ 3 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 7 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 7 , 255 , 255 , 255 , 255 } ,
{ 2 , 3 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 7 , 255 , 255 , 255 } ,
{ 4 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 7 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 7 , 255 , 255 , 255 , 255 } ,
{ 2 , 4 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 7 , 255 , 255 , 255 } ,
{ 3 , 4 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 7 , 255 , 255 , 255 } ,
{ 2 , 3 , 4 , 7 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 7 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 7 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 7 , 255 , 255 } ,
{ 5 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 5 , 7 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 5 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 5 , 7 , 255 , 255 , 255 , 255 } ,
{ 2 , 5 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 5 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 5 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 5 , 7 , 255 , 255 , 255 } ,
{ 3 , 5 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 5 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 5 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 5 , 7 , 255 , 255 , 255 } ,
{ 2 , 3 , 5 , 7 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 5 , 7 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 5 , 7 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 5 , 7 , 255 , 255 } ,
{ 4 , 5 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 5 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 5 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 5 , 7 , 255 , 255 , 255 } ,
{ 2 , 4 , 5 , 7 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 5 , 7 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 5 , 7 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 5 , 7 , 255 , 255 } ,
{ 3 , 4 , 5 , 7 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 5 , 7 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 5 , 7 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 5 , 7 , 255 , 255 } ,
{ 2 , 3 , 4 , 5 , 7 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 5 , 7 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 5 , 7 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 5 , 7 , 255 } ,
{ 6 , 7 , 255 , 255 , 255 , 255 , 255 , 255 } , { 0 , 6 , 7 , 255 , 255 , 255 , 255 , 255 } ,
{ 1 , 6 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 1 , 6 , 7 , 255 , 255 , 255 , 255 } ,
{ 2 , 6 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 2 , 6 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 2 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 2 , 6 , 7 , 255 , 255 , 255 } ,
{ 3 , 6 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 3 , 6 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 3 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 3 , 6 , 7 , 255 , 255 , 255 } ,
{ 2 , 3 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 2 , 3 , 6 , 7 , 255 , 255 , 255 } ,
{ 1 , 2 , 3 , 6 , 7 , 255 , 255 , 255 } , { 0 , 1 , 2 , 3 , 6 , 7 , 255 , 255 } ,
{ 4 , 6 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 4 , 6 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 4 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 4 , 6 , 7 , 255 , 255 , 255 } ,
{ 2 , 4 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 2 , 4 , 6 , 7 , 255 , 255 , 255 } ,
{ 1 , 2 , 4 , 6 , 7 , 255 , 255 , 255 } , { 0 , 1 , 2 , 4 , 6 , 7 , 255 , 255 } ,
{ 3 , 4 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 3 , 4 , 6 , 7 , 255 , 255 , 255 } ,
{ 1 , 3 , 4 , 6 , 7 , 255 , 255 , 255 } , { 0 , 1 , 3 , 4 , 6 , 7 , 255 , 255 } ,
{ 2 , 3 , 4 , 6 , 7 , 255 , 255 , 255 } , { 0 , 2 , 3 , 4 , 6 , 7 , 255 , 255 } ,
{ 1 , 2 , 3 , 4 , 6 , 7 , 255 , 255 } , { 0 , 1 , 2 , 3 , 4 , 6 , 7 , 255 } ,
{ 5 , 6 , 7 , 255 , 255 , 255 , 255 , 255 } , { 0 , 5 , 6 , 7 , 255 , 255 , 255 , 255 } ,
{ 1 , 5 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 1 , 5 , 6 , 7 , 255 , 255 , 255 } ,
{ 2 , 5 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 2 , 5 , 6 , 7 , 255 , 255 , 255 } ,
{ 1 , 2 , 5 , 6 , 7 , 255 , 255 , 255 } , { 0 , 1 , 2 , 5 , 6 , 7 , 255 , 255 } ,
{ 3 , 5 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 3 , 5 , 6 , 7 , 255 , 255 , 255 } ,
{ 1 , 3 , 5 , 6 , 7 , 255 , 255 , 255 } , { 0 , 1 , 3 , 5 , 6 , 7 , 255 , 255 } ,
{ 2 , 3 , 5 , 6 , 7 , 255 , 255 , 255 } , { 0 , 2 , 3 , 5 , 6 , 7 , 255 , 255 } ,
{ 1 , 2 , 3 , 5 , 6 , 7 , 255 , 255 } , { 0 , 1 , 2 , 3 , 5 , 6 , 7 , 255 } ,
{ 4 , 5 , 6 , 7 , 255 , 255 , 255 , 255 } , { 0 , 4 , 5 , 6 , 7 , 255 , 255 , 255 } ,
{ 1 , 4 , 5 , 6 , 7 , 255 , 255 , 255 } , { 0 , 1 , 4 , 5 , 6 , 7 , 255 , 255 } ,
{ 2 , 4 , 5 , 6 , 7 , 255 , 255 , 255 } , { 0 , 2 , 4 , 5 , 6 , 7 , 255 , 255 } ,
{ 1 , 2 , 4 , 5 , 6 , 7 , 255 , 255 } , { 0 , 1 , 2 , 4 , 5 , 6 , 7 , 255 } ,
{ 3 , 4 , 5 , 6 , 7 , 255 , 255 , 255 } , { 0 , 3 , 4 , 5 , 6 , 7 , 255 , 255 } ,
{ 1 , 3 , 4 , 5 , 6 , 7 , 255 , 255 } , { 0 , 1 , 3 , 4 , 5 , 6 , 7 , 255 } ,
{ 2 , 3 , 4 , 5 , 6 , 7 , 255 , 255 } , { 0 , 2 , 3 , 4 , 5 , 6 , 7 , 255 } ,
{ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 255 } , { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 } };

#endif
