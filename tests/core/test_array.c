/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <at/core.h>

#define TEST(array,dim,num_elements,elemsize,num_bytes,contiguous, empty, size,step) do{\
  assert_non_null(array);\
  assert_int_equal(at_array_get_dim(array), dim);\
  assert_int_equal(at_array_get_num_elements(array), num_elements);\
  assert_int_equal(at_array_get_elemsize(array), elemsize);\
  assert_int_equal(at_array_get_num_bytes(array),num_bytes);\
  assert_##contiguous(at_array_get_contiguous(array));\
  assert_##empty(at_array_is_empty(array));\
  for(i = 0; i < dim; i++){\
    assert_int_equal(at_array_get_size(array, i), size[i]);\
    assert_int_equal(at_array_get_step(array, i), step[i]);\
  }\
}while(0)

static void
test_array_new(void** state){
  uint64_t i;

  // Empty array
  g_autoptr(AtArray(uint8_t)) array = NULL;
  at_array_new(&array);

  uint64_t size[1] = {0};
  uint64_t step[1] = {0};
  TEST(array,0,0,1,0,true, true, size, step);

  // 1D array
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  at_array_new(&array2, 2);
  uint64_t size2[1] = {2};
  uint64_t step2[1] = {1};
  TEST(array2,1,2,1,2,true, false, size2,step2);

  // 2D array
  g_autoptr(AtArray(uint8_t)) array3 = NULL;
  at_array_new(&array3, 2, 3);
  uint64_t size3[2] = {2,3};
  uint64_t step3[2] = {3,1};
  TEST(array3,2,6,1,6,true, false, size3, step3);

  // 3D array
  g_autoptr(AtArray(uint8_t)) array4 = NULL;
  at_array_new(&array4, 2, 3, 5);
  uint64_t size4[3] = {2,3,5};
  uint64_t step4[3] = {15,5,1};
  TEST(array4,3,30,1,30,true, false, size4, step4);

  // 4D array
  g_autoptr(AtArray(uint8_t)) array5 = NULL;
  uint64_t size5[4] = {2,3,5,10};
  uint64_t step5[4] = {150,50,10,1};
  at_array_new(&array5, 2, 3, 5, 10);
  TEST(array5,4,300,1,300,true, false, size5, step5);

  // Array from data
  uint8_t data[10] = {1,2,3,4,5,6,7,8,9,10};
  uint64_t size6[2] = {2,5};
  uint64_t step6[2] = {5,1};
  g_autoptr(AtArray(uint8_t)) array6 = NULL;
  at_array_new(&array6,2,size6,data);
  TEST(array6,2,10,1,10,true, false, size6, step6);
}

#define TEST1(array,height,width,data) do{\
  assert_non_null(array);\
  assert_int_equal(at_array_get_size(array,0),height);\
  assert_int_equal(at_array_get_size(array,1),width);\
  for(i = 0; i < width*height; i++)\
    assert_int_equal(at_array_get(array,i),data[i]);\
}while(0)
static void test_array_add(void** state){
  uint64_t i;
  uint8_t  data[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t data2[10] = {10,20,30,40,50,60,70,80,90,100};
  uint64_t size1[2] = {5,2};
  uint64_t size2[2] = {1,2};
  uint64_t size3[2] = {5,1};
  g_autoptr(AtArray(uint8_t)) array1 = NULL;
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  g_autoptr(AtArray(uint8_t)) row1   = NULL;
  g_autoptr(AtArray(uint8_t)) row2   = NULL;
  g_autoptr(AtArray(uint8_t)) column2= NULL;
  at_array_new(&array1 ,2,size1  ,data );
  at_array_new(&array2 ,2,size1  ,data2);
  at_array_new(&row1   ,1,size2+1,data2);
  at_array_new(&row2   ,2,size2  ,data2);
  at_array_new(&column2,2,size3  ,data2);

  // Sum same size
  // ..third output
  g_autoptr(AtArray(uint8_t)) sum1   = NULL;
  uint8_t data_sum1[10] = {11,22,33,44,55,66,77,88,99,110};
  at_array_add(array1,array2,&sum1);
  TEST1(sum1,5,2,data_sum1);

  // array + scalar
  g_autoptr(AtArray(uint8_t)) sum2   = NULL;
  uint8_t data_sum2[10] = {11,12,13,14,15,16,17,18,19,20};
  at_array_add(array1,10,&sum2);
  TEST1(sum2,5,2,data_sum2);

  // array + row
  g_autoptr(AtArray(uint8_t)) sum31  = NULL;
  uint8_t data_sum31[10] = {11,22,13,24,15,26,17,28,19,30};
  at_array_add(array1,row1,&sum31);
  TEST1(sum31,5,2,data_sum31);

  g_autoptr(AtArray(uint8_t)) sum32  = NULL;
  at_array_add(array1,row2,&sum32);
  TEST1(sum32,5,2,data_sum31);

  // array + column
  uint8_t data_sum41[10] = {11,12,23,24,35,36,47,48,59,60};
  g_autoptr(AtArray(uint8_t)) sum42   = NULL;
  at_array_add(array1,column2,&sum42);
  TEST1(sum42,5,2,data_sum41);

  // row + column
  g_autoptr(AtArray(uint8_t)) sum5   = NULL;
  uint8_t data_sum5[10] = {20,30,30,40,40,50,50,60,60,70};
  at_array_add(row2,column2,&sum5);
  TEST1(sum5,5,2,data_sum5);
}

static void test_array_subtract(void** state){
  uint64_t i;
  uint8_t  data[10] = {10,20,30,40,50,60,70,80,90,100};
  uint8_t data2[10] = {1,2,3,4,5,6,7,8,9,10};
  uint64_t size1[2] = {5,2};
  uint64_t size2[2] = {1,2};
  uint64_t size3[2] = {5,1};
  g_autoptr(AtArray(uint8_t)) array1 = NULL;
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  g_autoptr(AtArray(uint8_t)) row1   = NULL;
  g_autoptr(AtArray(uint8_t)) row2   = NULL;
  g_autoptr(AtArray(uint8_t)) column2= NULL;
  at_array_new(&array1 ,2,size1  ,data );
  at_array_new(&array2 ,2,size1  ,data2);
  at_array_new(&row1   ,1,size2+1,data2);
  at_array_new(&row2   ,2,size2  ,data2);
  at_array_new(&column2,2,size3  ,data2);

  // Subtract same size
  // ..third output
  g_autoptr(AtArray(uint8_t)) sub1   = NULL;
  uint8_t data_sub1[10] = {9,18,27,36,45,54,63,72,81,90};
  at_array_subtract(array1,array2,&sub1);
  TEST1(sub1,5,2,data_sub1);

  // array - scalar
  g_autoptr(AtArray(uint8_t)) sub2   = NULL;
  uint8_t data_sub2[10] = {0,10,20,30,40,50,60,70,80,90};
  at_array_subtract(array1,10,&sub2);
  TEST1(sub2,5,2,data_sub2);

  // array - row
  g_autoptr(AtArray(uint8_t)) sub31  = NULL;
  uint8_t data_sub31[10] = {9,18,29,38,49,58,69,78,89,98};
  at_array_subtract(array1,row1,&sub31);
  TEST1(sub31,5,2,data_sub31);

  g_autoptr(AtArray(uint8_t)) sub32  = NULL;
  at_array_subtract(array1,row2,&sub32);
  TEST1(sub32,5,2,data_sub31);

  // array - column
  uint8_t data_sub41[10] = {9,19,28,38,47,57,66,76,85,95};
  g_autoptr(AtArray(uint8_t)) sub42   = NULL;
  at_array_subtract(array1,column2,&sub42);
  TEST1(sub42,5,2,data_sub41);

  // row - column
  g_autoptr(AtArray(uint8_t)) sub5   = NULL;
  uint8_t data_sub5[10] = {0,1,-1,0,-2,-1,-3,-2,-4,-3};
  at_array_subtract(row2,column2,&sub5);
  TEST1(sub5,5,2,data_sub5);
}

static void test_array_mult(void** state){
  uint64_t i;
  uint8_t  data[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t data2[10] = {1,2,3,4,5,6,7,8,9,10};
  uint64_t size1[2] = {5,2};
  uint64_t size2[2] = {1,2};
  uint64_t size3[2] = {5,1};
  g_autoptr(AtArray(uint8_t)) array1 = NULL;
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  g_autoptr(AtArray(uint8_t)) row1   = NULL;
  g_autoptr(AtArray(uint8_t)) row2   = NULL;
  g_autoptr(AtArray(uint8_t)) column2= NULL;
  at_array_new(&array1 ,2,size1  ,data );
  at_array_new(&array2 ,2,size1  ,data2);
  at_array_new(&row1   ,1,size2+1,data2);
  at_array_new(&row2   ,2,size2  ,data2);
  at_array_new(&column2,2,size3  ,data2);

  // Subtract same size
  // ..third output
  g_autoptr(AtArray(uint8_t)) sub1   = NULL;
  uint8_t data_sub1[10] = {1,4,9,16,25,36,49,64,81,100};
  at_array_mult(array1,array2,&sub1);
  TEST1(sub1,5,2,data_sub1);

  // array - scalar
  g_autoptr(AtArray(uint8_t)) sub2   = NULL;
  uint8_t data_sub2[10] = {10,20,30,40,50,60,70,80,90,100};
  at_array_mult(array1,10,&sub2);
  TEST1(sub2,5,2,data_sub2);

  // array - row
  g_autoptr(AtArray(uint8_t)) sub31  = NULL;
  uint8_t data_sub31[10] = {1,4,3,8,5,12,7,16,9,20};
  at_array_mult(array1,row1,&sub31);
  TEST1(sub31,5,2,data_sub31);

  g_autoptr(AtArray(uint8_t)) sub32  = NULL;
  at_array_mult(array1,row2,&sub32);
  TEST1(sub32,5,2,data_sub31);

  // array - column
  uint8_t data_sub41[10] = {1,2,6,8,15,18,28,32,45,50};
  g_autoptr(AtArray(uint8_t)) sub42   = NULL;
  at_array_mult(array1,column2,&sub42);
  TEST1(sub42,5,2,data_sub41);

  // row - column
  g_autoptr(AtArray(uint8_t)) sub5   = NULL;
  uint8_t data_sub5[10] = {1,2,2,4,3,6,4,8,5,10};
  at_array_mult(row2,column2,&sub5);
  TEST1(sub5,5,2,data_sub5);
}


static void test_array_divide(void** state){
  uint64_t i;
  uint8_t  data[10] = {10,20,30,40,50,60,70,80,90,100};
  uint8_t data2[10] = {1,2,3,4,5,6,7,8,9,10};
  uint64_t size1[2] = {5,2};
  uint64_t size2[2] = {1,2};
  uint64_t size3[2] = {5,1};
  g_autoptr(AtArray(uint8_t)) array1 = NULL;
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  g_autoptr(AtArray(uint8_t)) row1   = NULL;
  g_autoptr(AtArray(uint8_t)) row2   = NULL;
  g_autoptr(AtArray(uint8_t)) column2= NULL;
  at_array_new(&array1 ,2,size1  ,data );
  at_array_new(&array2 ,2,size1  ,data2);
  at_array_new(&row1   ,1,size2+1,data2);
  at_array_new(&row2   ,2,size2  ,data2);
  at_array_new(&column2,2,size3  ,data2);

  // Subtract same size
  // ..third output
  g_autoptr(AtArray(uint8_t)) sub1   = NULL;
  uint8_t data_sub1[10] = {10,10,10,10,10,10,10,10,10,10};
  at_array_divide(array1,array2,&sub1);
  TEST1(sub1,5,2,data_sub1);

  // array - scalar
  g_autoptr(AtArray(uint8_t)) sub2   = NULL;
  uint8_t data_sub2[10] = {1,2,3,4,5,6,7,8,9,10};
  at_array_divide(array1,10,&sub2);
  TEST1(sub2,5,2,data_sub2);

  // array - row
  g_autoptr(AtArray(uint8_t)) sub31  = NULL;
  uint8_t data_sub31[10] = {10,10,30,20,50,30,70,40,90,50};
  at_array_divide(array1,row1,&sub31);
  TEST1(sub31,5,2,data_sub31);

  g_autoptr(AtArray(uint8_t)) sub32  = NULL;
  at_array_divide(array1,row2,&sub32);
  TEST1(sub32,5,2,data_sub31);

  // array - column
  uint8_t data_sub41[10] = {10,20,15,20,16,20,17,20,18,20};
  g_autoptr(AtArray(uint8_t)) sub42   = NULL;
  at_array_divide(array1,column2,&sub42);
  TEST1(sub42,5,2,data_sub41);

  // row - column
  g_autoptr(AtArray(uint8_t)) sub5   = NULL;
  uint8_t data_sub5[10] = {1,2,0,1,0,0,0,0,0,0};
  at_array_divide(row2,column2,&sub5);
  TEST1(sub5,5,2,data_sub5);
}
#define TEST3(array, data) do{\
  for(i = 0; i < at_array_get_num_elements(array); i++)\
    assert_int_equal(data[i],at_array_get(array,i));\
}while(0)
static void test_array_set(void** state){
  uint64_t i;
  g_autoptr(AtArray(uint8_t)) array  = NULL;
  g_autoptr(AtArray(uint8_t)) array1 = NULL;
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  g_autoptr(AtArray(uint8_t)) row    = NULL;
  g_autoptr(AtArray(uint8_t)) column = NULL;
  uint64_t size_array[2]  = {5,2};
  uint64_t size_row[2]    = {1,2};
  uint64_t size_column[2] = {5,1};
  uint8_t data_array[10] = {10,20,30,40,50,60,70,80,90,100};
  uint8_t data_array2[10]= {1,2,3,4,5,6,7,8,9,10};
  uint8_t data_row[2]    = {255,254};
  uint8_t data_column[5] = {255,254,253,252,251};

  at_array_new(&array ,2,size_array , data_array);
  at_array_new(&array2,2,size_array , data_array2);
  at_array_new(&row   ,2,size_row   , data_row);
  at_array_new(&column,2,size_column, data_column);
  at_array_copy(array, &array1);

  // Set an array to another
  at_array_set(array,array2);
  uint8_t data_set1[10] = {1,2,3,4,5,6,7,8,9,10};
  TEST3(array,data_set1);

  // Setting a scalar
  at_array_set(array,127);
  uint8_t data_set2[10] = {127,127,127,127,127,127,127,127,127,127};
  TEST3(array,data_set2);

  // Setting a broadcasted row
  at_array_set(array,row);
  uint8_t data_set3[10] = {255,254,255,254,255,254,255,254,255,254};
  TEST3(array,data_set3);

  // Setting a complete array again
  at_array_set(array,array1);
  uint8_t data_set4[10] = {10,20,30,40,50,60,70,80,90,100};
  TEST3(array,data_set4);

  // Setting a broadcasted column
  at_array_set(array,column);
  uint8_t data_set5[10] = {255,255,254,254,253,253,252,252,251,251};
  TEST3(array,data_set5);
  at_array_set(array,array1);

}

static void
test_array_sub(void** state){
  g_autoptr(AtArray(uint8_t)) array    = NULL;
  g_autoptr(AtArray(uint8_t)) subarray1 = NULL;
  g_autoptr(AtArray(uint8_t)) subarray2 = NULL;

  uint8_t data[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  uint8_t data_subarray[4] = {9,10,13,14};
  uint64_t size[2] = {4,4};

  at_array_new(&array,2,size,data);

  // Creating a subarray
  AtRange range[2] = {at_range_from(2), at_range_to(2)};
  at_array_sub(array,range, &subarray1);
  uint64_t i;

  assert_int_equal(at_array_get_dim(subarray1), 2);
  assert_int_equal(at_array_get_size(subarray1,0),2);
  assert_int_equal(at_array_get_size(subarray1,1),2);
  assert_int_equal(at_array_get_parent(subarray1),array);
  assert_false(at_array_get_contiguous(subarray1));
  assert_int_equal(at_array_get_elemsize(subarray1),1);
  assert_int_equal(at_array_get_num_elements(subarray1),4);
  for(i = 0; i < at_array_get_num_elements(subarray1); i++)
    assert_int_equal(at_array_get(subarray1,i),data_subarray[i]);

  // Modifying a subarray with set
  at_array_set(subarray1, 5);
  uint8_t data2[16] = {1,2,3,4,5,6,7,8,5,5,11,12,5,5,15,16};
  for(i = 0; i < at_array_get_num_elements(array); i++)
    assert_int_equal(at_array_get(array,i),data2[i]);

  // Mathematical operations with subarray
  at_array_add(subarray1, 10);
  uint8_t data3[16] = {1,2,3,4,5,6,7,8,15,15,11,12,15,15,15,16};
  for(i = 0; i < at_array_get_num_elements(array); i++)
    assert_int_equal(at_array_get(array,i),data3[i]);

  at_array_copy(subarray1,&subarray2);
  assert_int_equal(at_array_get_dim(subarray2),2);
  assert_int_equal(at_array_get_size(subarray2,0),2);
  assert_int_equal(at_array_get_size(subarray2,1),2);
  assert_null(at_array_base_get_parent(AT_ARRAY_BASE(subarray2)));
  assert_true(at_array_get_contiguous(subarray2));
  assert_int_equal(at_array_get_elemsize(subarray2),1);
  assert_int_equal(at_array_get_num_elements(subarray2),4);
  uint8_t data_subarray2[4] = {15,15,15,15};
  for(i = 0; i < at_array_get_num_elements(subarray2); i++)
    assert_int_equal(at_array_get(subarray2,i),data_subarray2[i]);
}

static void
test_array_conversion(void** state){
  g_autoptr(AtArray(uint8_t))  array  = NULL;
  g_autoptr(AtArray(uint16_t)) array2 = NULL;
  uint8_t  data[8] = {1,2,3,4,5,6,7,8};
  uint64_t size[3] = {2,2,2};
  at_array_new(&array,3,size,data);
  at_array_copy(array,&array2);
  assert_int_equal(at_array_get_dim(array2),at_array_get_dim(array));
  assert_int_equal(at_array_get_size(array2,0),at_array_get_size(array,0));
  assert_int_equal(at_array_get_size(array2,1),at_array_get_size(array,1));
  uint64_t i;
  for(i = 0; i < 8; i++){
    assert_int_equal(at_array_get(array,i),at_array_get(array2,i));
  }

  // Converting a subarray
  g_autoptr(AtArray(uint8_t)) subarray1 = NULL;
  g_autoptr(AtArray(float))   subarray2 = NULL;
  AtRange ranges[3] = {at_range_from(1),at_range_from(1),at_range_from(1)};
  at_array_sub(array, ranges, &subarray1);
  at_array_copy(subarray1, &subarray2);
  at_array_squeeze(subarray2);
  assert_int_equal(at_array_get_dim(subarray2),1);
  assert_true(at_array_get(subarray2,0) == 8);
}

uint8_t
test_array_map_func(AtArray(uint8_t)* array, uint64_t index, uint8_t value){
  return value << 1;
}


static void
test_array_map(void** state){
  g_autoptr(AtArray(uint8_t)) array = NULL;
  g_autoptr(AtArray(uint8_t)) array_map = NULL;
  uint8_t  data[8] = {1,2,3,4,5,6,7,8};
  uint8_t  data_mapped[8] = {2,4,6,8,10,12,14,16};
  uint8_t  data_subarray[8] = {2,8,6,16,10,12,14,16};
  uint64_t size[3] = {2,2,2};
  at_array_new(&array,3,size,data);
  uint64_t i;

  // out-of-place mapping
  at_array_map(array, test_array_map_func, &array_map);
  for(i = 0; i < at_array_get_num_elements(array); i++){
    assert_int_equal(at_array_get(array_map,i),data_mapped[i]);
    assert_int_equal(at_array_get(array,i),data[i]);
  }

  // in-place mapping
  at_array_map(array, test_array_map_func);
  for(i = 0; i < at_array_get_num_elements(array); i++){
    assert_int_equal(at_array_get(array,i),data_mapped[i]);
  }

  // in-place mapping within a submatrix
  g_autoptr(AtArray(uint8_t)) subarray = NULL;
  AtRange ranges[3] = {at_range_to(1), at_range_all(), at_range_from(1)};
  at_array_sub(array, ranges, &subarray);
  at_array_map(subarray, test_array_map_func);
  for(i = 0; i < at_array_get_num_elements(array); i++){
    assert_int_equal(at_array_get(array,i),data_subarray[i]);
  }
}

static uint8_t
reduce_sum(uint8_t data1, uint8_t data2){
  return data1 + data2;
}
static uint8_t
reduce_max(uint8_t data1, uint8_t data2){
  return max(data1, data2);
}
static uint8_t
reduce_min(uint8_t data1, uint8_t data2){
  return min(data1 ,data2);
}

static void
test_array_reduce(void** state){
  g_autoptr(AtArray(uint8_t)) array = NULL;
  g_autoptr(AtArray(uint8_t)) array_reduced1 = NULL;
  g_autoptr(AtArray(uint8_t)) array_reduced2 = NULL;
  g_autoptr(AtArray(uint8_t)) array_reduced3 = NULL;

  uint8_t  data[8]          = {1,2,3,4,5,6,7,8};
  uint8_t  data_reduced1[4] = {6,8,10,12};
  uint8_t  data_reduced2[4] = {3,4,7,8};
  uint8_t  data_reduced3[2] = {1,3};
  uint8_t  data_reduced_sub[2] = {6,10};
  uint64_t size[3]          = {2,2,2};
  uint64_t i;

  at_array_new(&array, 3, size, data);

  // Reduction in Z (sum)
  at_array_reduce(array, reduce_sum, 0, &array_reduced1,0);
  assert_int_equal(at_array_get_dim(array_reduced1), 2);
  assert_int_equal(at_array_get_size(array_reduced1,0),2);
  assert_int_equal(at_array_get_size(array_reduced1,1),2);

  for(i = 0; i < at_array_get_num_elements(array_reduced1); i++){
    assert_int_equal(at_array_get(array_reduced1,i),data_reduced1[i]);
  }

  // Reduction in Y (max)
  at_array_reduce(array, reduce_max, 1, &array_reduced2,0);
  assert_int_equal(at_array_get_dim (array_reduced2  ), 2);
  assert_int_equal(at_array_get_size(array_reduced2,0), 2);
  assert_int_equal(at_array_get_size(array_reduced2,1), 2);

  for(i = 0; i < at_array_get_num_elements(array_reduced2); i++){
    assert_int_equal(at_array_get(array_reduced2,i),data_reduced2[i]);
  }

  // Reduction in Z and X (min)
  uint16_t dims_reduced[2] = {0,2};
  at_array_reduce(array, reduce_min, 2, dims_reduced, &array_reduced3, 255);
  assert_int_equal(at_array_get_dim(array_reduced3), 1);
  assert_int_equal(at_array_get_size(array_reduced3,0),2);
  for(i = 0; i < at_array_get_num_elements(array_reduced3); i++){
    assert_int_equal(at_array_get(array_reduced3,i),data_reduced3[i]);
  }

  // Reduction in Y in a submatrix
  g_autoptr(AtArray(uint8_t)) subarray         = NULL;
  g_autoptr(AtArray(uint8_t)) subarray_reduced = NULL;
  AtRange ranges[3] = {at_range_all(),at_range_all(),at_range_to(1)};
  at_array_sub(array,ranges,&subarray);
  at_array_squeeze(subarray); // 2x2
  at_array_reduce(subarray, reduce_sum, 0, &subarray_reduced, 0);
  assert_int_equal(at_array_get_dim(subarray_reduced),1);
  assert_int_equal(at_array_get_size(subarray_reduced,0),2);
  for(i = 0; i < at_array_get_num_elements(subarray_reduced);i++){
    assert_int_equal(at_array_get(subarray_reduced,i),data_reduced_sub[i]);
  }
}

// zeros
// ones
// fill
// range
//

#define TEST4(array,dim,num_elements,elemsize,num_bytes,contiguous, empty, size,step,elem) do{\
  assert_non_null(array);\
  assert_int_equal(at_array_get_dim(array), dim);\
  assert_int_equal(at_array_get_num_elements(array), num_elements);\
  assert_int_equal(at_array_get_elemsize(array), elemsize);\
  assert_int_equal(at_array_get_num_bytes(array),num_bytes);\
  assert_##contiguous(at_array_get_contiguous(array));\
  assert_##empty(at_array_is_empty(array));\
  for(i = 0; i < dim; i++){\
    assert_int_equal(at_array_get_size(array, i), size[i]);\
    assert_int_equal(at_array_get_step(array, i), step[i]);\
  }\
  for(i = 0; i < num_elements; i++){\
    assert_int_equal(at_array_get(array,i),elem);\
  }\
}while(0)

static void
test_array_zeros(void** state){
  uint64_t i;
  g_autoptr(AtArray(uint8_t)) array = NULL;

  // 1D array of zeros
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  at_array_zeros(&array2, 2);
  uint64_t size2[1] = {2};
  uint64_t step2[1] = {1};
  TEST4(array2,1,2,1,2,true, false, size2,step2,0);

  // 2D array of zeros
  g_autoptr(AtArray(uint8_t)) array3 = NULL;
  at_array_zeros(&array3, 2, 3);
  uint64_t size3[2] = {2,3};
  uint64_t step3[2] = {3,1};
  TEST4(array3,2,6,1,6,true, false, size3, step3,0);

  // 3D array of zeros
  g_autoptr(AtArray(uint8_t)) array4 = NULL;
  at_array_zeros(&array4, 2, 3, 5);
  uint64_t size4[3] = {2,3,5};
  uint64_t step4[3] = {15,5,1};
  TEST4(array4,3,30,1,30,true, false, size4, step4,0);

  // 4D array of zeros
  g_autoptr(AtArray(uint8_t)) array5 = NULL;
  uint64_t size5[4] = {2,3,5,10};
  uint64_t step5[4] = {150,50,10,1};
  at_array_zeros(&array5, 2, 3, 5, 10);
  TEST4(array5,4,300,1,300,true, false, size5, step5,0);

  // ND array of zeros
  g_autoptr(AtArray(uint8_t)) array6 = NULL;
  uint64_t size6[5] = {3,2,5,2,4};
  uint64_t step6[5] = {80,40,8,4,1};
  at_array_zeros(&array6, 5, size6);
  TEST4(array6,5,240,1,240,true, false, size6, step6,0);
}

static void
test_array_ones(void** state){
  uint64_t i;
  g_autoptr(AtArray(uint8_t)) array = NULL;

  // 1D array of ones
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  at_array_ones(&array2, 2);
  uint64_t size2[1] = {2};
  uint64_t step2[1] = {1};
  TEST4(array2,1,2,1,2,true, false, size2,step2,1);

  // 2D array of ones
  g_autoptr(AtArray(uint8_t)) array3 = NULL;
  at_array_ones(&array3, 2, 3);
  uint64_t size3[2] = {2,3};
  uint64_t step3[2] = {3,1};
  TEST4(array3,2,6,1,6,true, false, size3, step3,1);

  // 3D array of ones
  g_autoptr(AtArray(uint8_t)) array4 = NULL;
  at_array_ones(&array4, 2, 3, 5);
  uint64_t size4[3] = {2,3,5};
  uint64_t step4[3] = {15,5,1};
  TEST4(array4,3,30,1,30,true, false, size4, step4,1);

  // 4D array of ones
  g_autoptr(AtArray(uint8_t)) array5 = NULL;
  uint64_t size5[4] = {2,3,5,10};
  uint64_t step5[4] = {150,50,10,1};
  at_array_ones(&array5, 2, 3, 5, 10);
  TEST4(array5,4,300,1,300,true, false, size5, step5,1);

  // ND array of ones
  g_autoptr(AtArray(uint8_t)) array6 = NULL;
  uint64_t size6[5] = {3,2,5,2,4};
  uint64_t step6[5] = {80,40,8,4,1};
  at_array_ones(&array6, 5, size6);
  TEST4(array6,5,240,1,240,true, false, size6, step6,1);
}

static void
test_array_max(void** state){
  g_autoptr(AtArray(uint8_t)) array = NULL;
  uint8_t                      max1  = 0;
  g_autoptr(AtArray(uint8_t)) max2  = NULL;
  g_autoptr(AtArray(uint8_t)) max3  = NULL;
  uint8_t                      max4  = 0;
  uint64_t i;

  uint8_t  data[8]  = {1,2,3,4,5,6,7,8};
  uint64_t size[3]  = {2,2,2};
  uint8_t  data1    = 8;
  uint8_t  data2[4] = {5,6,7,8};
  uint8_t  data3[2] = {6,8};
  uint8_t  data4    = 4;

  at_array_new(&array,3,size,data);

  // Scalar value
  at_array_max(array, &max1);
  assert_int_equal(max1,data1);

  // Reduction in 1 dimension
  at_array_max(array, 0, &max2);
  for(i = 0; i < at_array_get_num_elements(max2);i++){
    assert_int_equal(at_array_get(max2,i),data2[i]);
  }

  // Reduction in several dimensions
  uint16_t dims[2] = {0,2};
  at_array_max(array, 2, dims, &max3);
  for(i = 0; i < at_array_get_num_elements(max3);i++){
    assert_int_equal(at_array_get(max3,i),data3[i]);
  }

  // Max in subarray
  g_autoptr(AtArray(uint8_t)) subarray = NULL;
  // (1,2,2)
  AtRange ranges[3] = {at_range_to(1), at_range_all(),at_range_all()};
  at_array_sub(array, ranges, &subarray);
  at_array_squeeze(subarray); // (1,2,2) => (2,2)
  at_array_max(subarray, &max4);
  assert_int_equal(max4, data4);
}
static void
test_array_min(void** state){
  g_autoptr(AtArray(uint8_t)) array = NULL;
  uint8_t                      min1  = 0;
  g_autoptr(AtArray(uint8_t)) min2  = NULL;
  g_autoptr(AtArray(uint8_t)) min3  = NULL;
  uint64_t i;

  uint8_t  data[8]  = {1,2,3,4,5,6,7,8};
  uint64_t size[3]  = {2,2,2};
  uint8_t  data1    = 1;
  uint8_t  data2[4] = {1,2,3,4};
  uint8_t  data3[2] = {1,3};

  at_array_new(&array,3,size,data);

  // Scalar value
  at_array_min(array, &min1);
  assert_int_equal(min1,data1);

  // Reduction in 1 dimension
  at_array_min(array, 0, &min2);
  for(i = 0; i < at_array_get_num_elements(min2);i++){
    assert_int_equal(at_array_get(min2,i),data2[i]);
  }

  // Reduction in several dimensions
  uint16_t dims[2] = {0,2};
  at_array_min(array, 2, dims, &min3);
  for(i = 0; i < at_array_get_num_elements(min3);i++){
    assert_int_equal(at_array_get(min3,i),data3[i]);
  }
}

static void
test_array_prod(void** state){
  g_autoptr(AtArray(uint8_t)) array = NULL;
  uint8_t                      prod1  = 0;
  g_autoptr(AtArray(uint8_t)) prod2  = NULL;
  g_autoptr(AtArray(uint8_t)) prod3  = NULL;
  uint64_t i;

  uint8_t  data[8]  = {1,2,3,4,5,6,7,8};
  uint64_t size[3]  = {2,2,2};
  uint8_t  data1    = 128;
  uint8_t  data2[4] = {5,12,21,32};
  uint8_t  data3[2] = {60,160};

  at_array_new(&array,3,size,data);

  // Scalar value
  at_array_prod(array, &prod1);
  assert_int_equal(prod1,data1);

  // Reduction in 1 dimension
  at_array_prod(array, 0, &prod2);
  for(i = 0; i < at_array_get_num_elements(prod2);i++){
    assert_int_equal(at_array_get(prod2,i),data2[i]);
  }

  // Reduction in several dimensions
  uint16_t dims[2] = {0,2};
  at_array_prod(array, 2, dims, &prod3);
  for(i = 0; i < at_array_get_num_elements(prod3);i++){
    assert_int_equal(at_array_get(prod3,i),data3[i]);
  }
}

static void
test_array_range(void** state){
  g_autoptr(AtArray(uint8_t)) array  = NULL;
  g_autoptr(AtArray(uint8_t)) array2 = NULL;
  g_autoptr(AtArray(uint8_t)) array3 = NULL;
  // Just with stop
  at_array_range(&array, 10);
  assert_int_equal(at_array_get_dim(array),1);
  assert_int_equal(at_array_get_size(array,0),10);
  uint64_t i;
  for(i = 0; i < 10; i++)
    assert_int_equal(at_array_get(array,i),i);

  // With start,stop
  at_array_range(&array2, 10, 20);
  assert_int_equal(at_array_get_dim(array2),1);
  assert_int_equal(at_array_get_size(array2,0),10);
  for(i = 0; i < 10; i++)
    assert_int_equal(at_array_get(array2,i),i+10);

  // With start,stop,step
  at_array_range(&array3, 10, 20, 3);
  assert_int_equal(at_array_get_dim(array3),1);
  assert_int_equal(at_array_get_size(array3,0),4);
  for(i = 0; i < 4; i++)
    assert_int_equal(at_array_get(array3,i),i*3+10);
}

static void
test_array_bitwise_ops(void** state){
  g_autoptr(AtArray(uint8_t)) array   = NULL;
  g_autoptr(AtArray(uint8_t)) array2  = NULL;
  g_autoptr(AtArray(uint8_t)) output  = NULL;
  g_autoptr(AtArray(uint8_t)) output2 = NULL;
  g_autoptr(AtArray(uint8_t)) output3 = NULL;
  g_autoptr(AtArray(uint8_t)) output4 = NULL;
  g_autoptr(AtArray(uint8_t)) row     = NULL;
  uint8_t  data[8]   = {1,2,3,4,5,6,7,8};
  uint8_t  data2[8]  = {67,12,43,53,15,67,34,93};
  uint8_t  data3[8]  = {67 & 1,12 & 2,43 & 3,53 & 4,15 & 5,67 & 6,34 & 7,93 & 8};
  uint8_t  data4[8]  = {1 ^ 5,2 ^ 5,3 ^ 5,4 ^ 5,5 ^ 5,6 ^ 5,7 ^ 5,8 ^ 5};
  uint8_t  data5[8]  = {1|67 , 2|12 , 3|67 , 4|12 , 5|67 , 6|12 , 7|67 , 8|12};
  uint8_t  data6[8]  = {~1,~2,~3,~4,~5,~6,~7,~8};
  uint64_t size[3]   = {2,2,2};
  uint64_t i;
  at_array_new(&array,3,size,data);

  // Between arrays
  at_array_new(&array2,3,size,data2);
  at_array_bitwise_and(array, array2, &output);
  for(i = 0; i < 8; i++)
    assert_int_equal(at_array_get(output,i),data3[i]);

  // Array x scalar
  at_array_bitwise_xor(array, 5, &output2);
  for(i = 0; i < 8; i++)
    assert_int_equal(at_array_get(output2,i),data4[i]);

  // Array x row
  AtRange ranges[3] = {at_range_to(1),at_range_to(1),at_range_all()};
  at_array_sub(array2, ranges, &row);
  at_array_squeeze(row);
  at_array_bitwise_or(array, row, &output3);
  for(i = 0; i < 8; i++)
    assert_int_equal(at_array_get(output3,i),data5[i]);

  // Array negation
  at_array_bitwise_not(array, &output4);
  for(i = 0; i < 8; i++)
    assert_int_equal(at_array_get(output4,i),data6[i]);

}

static void
test_array_alloc(void** state){
  g_autoptr(AtArray(uint8_t)) array = NULL;
  at_array_new(&array);
  assert_true(at_array_is_empty(array));
  uint64_t size[3] = {10,10,10};
  at_array_alloc_data(array,3,size);
  assert_false(at_array_is_empty(array));
  at_array_dealloc_data(array);
  assert_true(at_array_is_empty(array));
}

static void
test_array_normalize(void** state){
  uint8_t data[9] = {253,253,253,254,254,254,255,255,255};
  uint8_t data_gt[9] = {0,0,0,3,3,3,6,6,6};
  uint64_t size[2] = {3,3};
  g_autoptr(AtArray(uint8_t)) array = NULL;
  g_autoptr(AtArray(uint8_t)) array_norm = NULL;
  at_array_new(&array, 2, size, data);

  at_array_normalize(array, &array_norm, 0, 6, AT_NORM_MINMAX, NULL);
  uint64_t i;
  for(i = 0; i < 9; i++)
    assert_int_equal(at_array_get(array_norm, i), data_gt[i]);
}

int
main(){
  const struct CMUnitTest tests[19]={
    cmocka_unit_test(test_array_new),
    cmocka_unit_test(test_array_add),
    cmocka_unit_test(test_array_subtract),
    cmocka_unit_test(test_array_mult),
    cmocka_unit_test(test_array_divide),
    cmocka_unit_test(test_array_set),
    cmocka_unit_test(test_array_sub),
    cmocka_unit_test(test_array_conversion),
    cmocka_unit_test(test_array_map),
    cmocka_unit_test(test_array_reduce),

    cmocka_unit_test(test_array_zeros),
    cmocka_unit_test(test_array_ones),
    cmocka_unit_test(test_array_max),
    cmocka_unit_test(test_array_min),
    cmocka_unit_test(test_array_prod),
    cmocka_unit_test(test_array_range),
    cmocka_unit_test(test_array_bitwise_ops),
    cmocka_unit_test(test_array_alloc),
    cmocka_unit_test(test_array_normalize),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
