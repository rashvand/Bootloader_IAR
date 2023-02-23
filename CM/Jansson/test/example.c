
#include "main.h"
#include "jansson.h"



/*
 * Parse text into a JSON object. If text is valid JSON, returns a
 * json_t structure, otherwise prints and error and returns null.
 */
json_t *load_json(const char *text) {
    json_t *root;
    json_error_t error;

    root = json_loads(text, 0, &error);

    if (root) {
        return root;
    } else {
        fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
        return (json_t *)0;
    }
}

/*
 * Print a prompt and return (by reference) a null-terminated line of
 * text.  Returns NULL on eof or some error.
 */
char *read_line(char *line, int max_chars) {
    printf("Type some JSON > ");
    fflush(stdout);
    return fgets(line, max_chars, stdin);
}



static void test_misc(void) {
  json_t *array, *five, *seven, *value;
  size_t i;
  
  array = json_array();
  five = json_integer(5);
  seven = json_integer(7);
  
  if (!array)
    printf("unable to create array");
  if (!five || !seven)
    printf("unable to create integer");
  
  if (json_array_size(array) != 0)
    printf("empty array has nonzero size");
  
  if (!json_array_append(array, NULL))
    printf("able to append NULL");
  
  if (json_array_append(array, five))
    printf("unable to append");
  
  if (json_array_size(array) != 1)
    printf("wrong array size");
  
  value = json_array_get(array, 0);
  if (!value)
    printf("unable to get item");
  if (value != five)
    printf("got wrong value");
  
  if (json_array_append(array, seven))
    printf("unable to append value");
  
  if (json_array_size(array) != 2)
    printf("wrong array size");
  
  value = json_array_get(array, 1);
  if (!value)
    printf("unable to get item");
  if (value != seven)
    printf("got wrong value");
  
  if (json_array_set(array, 0, seven))
    printf("unable to set value");
  
  if (!json_array_set(array, 0, NULL))
    printf("able to set NULL");
  
  if (json_array_size(array) != 2)
    printf("wrong array size");
  
  value = json_array_get(array, 0);
  if (!value)
    printf("unable to get item");
  if (value != seven)
    printf("got wrong value");
  
  if (json_array_get(array, 2) != NULL)
    printf("unable to get value out of bounds");
  
  if (!json_array_set(array, 2, seven))
    printf("unable to set value out of bounds");
  
  for (i = 2; i < 30; i++) {
    if (json_array_append(array, seven))
      printf("unable to append value");
    
    if (json_array_size(array) != i + 1)
      printf("wrong array size");
  }
  
  for (i = 0; i < 30; i++) {
    value = json_array_get(array, i);
    if (!value)
      printf("unable to get item");
    if (value != seven)
      printf("got wrong value");
  }
  
  if (json_array_set_new(array, 15, json_integer(123)))
    printf("unable to set new value");
  
  value = json_array_get(array, 15);
  if (!json_is_integer(value) || json_integer_value(value) != 123)
    printf("json_array_set_new works incorrectly");
  
  if (!json_array_set_new(array, 15, NULL))
    printf("able to set_new NULL value");
  
  if (json_array_append_new(array, json_integer(321)))
    printf("unable to append new value");
  
  value = json_array_get(array, json_array_size(array) - 1);
  if (!json_is_integer(value) || json_integer_value(value) != 321)
    printf("json_array_append_new works incorrectly");
  
  if (!json_array_append_new(array, NULL))
    printf("able to append_new NULL value");
  
  json_decref(five);
  json_decref(seven);
  json_decref(array);
}

static void test_insert(void) {
  json_t *array, *five, *seven, *eleven, *value;
  int i;
  
  array = json_array();
  five = json_integer(5);
  seven = json_integer(7);
  eleven = json_integer(11);
  
  if (!array)
    printf("unable to create array");
  if (!five || !seven || !eleven)
    printf("unable to create integer");
  
  if (!json_array_insert(array, 1, five))
    printf("able to insert value out of bounds");
  
  if (json_array_insert(array, 0, five))
    printf("unable to insert value in an empty array");
  
  if (json_array_get(array, 0) != five)
    printf("json_array_insert works incorrectly");
  
  if (json_array_size(array) != 1)
    printf("array size is invalid after insertion");
  
  if (json_array_insert(array, 1, seven))
    printf("unable to insert value at the end of an array");
  
  if (json_array_get(array, 0) != five)
    printf("json_array_insert works incorrectly");
  
  if (json_array_get(array, 1) != seven)
    printf("json_array_insert works incorrectly");
  
  if (json_array_size(array) != 2)
    printf("array size is invalid after insertion");
  
  if (json_array_insert(array, 1, eleven))
    printf("unable to insert value in the middle of an array");
  
  if (json_array_get(array, 0) != five)
    printf("json_array_insert works incorrectly");
  
  if (json_array_get(array, 1) != eleven)
    printf("json_array_insert works incorrectly");
  
  if (json_array_get(array, 2) != seven)
    printf("json_array_insert works incorrectly");
  
  if (json_array_size(array) != 3)
    printf("array size is invalid after insertion");
  
  if (json_array_insert_new(array, 2, json_integer(123)))
    printf("unable to insert value in the middle of an array");
  
  value = json_array_get(array, 2);
  if (!json_is_integer(value) || json_integer_value(value) != 123)
    printf("json_array_insert_new works incorrectly");
  
  if (json_array_size(array) != 4)
    printf("array size is invalid after insertion");
  
  for (i = 0; i < 20; i++) {
    if (json_array_insert(array, 0, seven))
      printf("unable to insert value at the beginning of an array");
  }
  
  for (i = 0; i < 20; i++) {
    if (json_array_get(array, i) != seven)
      printf("json_aray_insert works incorrectly");
  }
  
  if (json_array_size(array) != 24)
    printf("array size is invalid after loop insertion");
  
  json_decref(five);
  json_decref(seven);
  json_decref(eleven);
  json_decref(array);
}

static void test_remove(void) {
  json_t *array, *five, *seven;
  int i;
  
  array = json_array();
  five = json_integer(5);
  seven = json_integer(7);
  
  if (!array)
    printf("unable to create array");
  if (!five)
    printf("unable to create integer");
  if (!seven)
    printf("unable to create integer");
  
  if (!json_array_remove(array, 0))
    printf("able to remove an unexisting index");
  
  if (json_array_append(array, five))
    printf("unable to append");
  
  if (!json_array_remove(array, 1))
    printf("able to remove an unexisting index");
  
  if (json_array_remove(array, 0))
    printf("unable to remove");
  
  if (json_array_size(array) != 0)
    printf("array size is invalid after removing");
  
  if (json_array_append(array, five) || json_array_append(array, seven) ||
      json_array_append(array, five) || json_array_append(array, seven))
    printf("unable to append");
  
  if (json_array_remove(array, 2))
    printf("unable to remove");
  
  if (json_array_size(array) != 3)
    printf("array size is invalid after removing");
  
  if (json_array_get(array, 0) != five || json_array_get(array, 1) != seven ||
      json_array_get(array, 2) != seven)
    printf("remove works incorrectly");
  
  json_decref(array);
  
  array = json_array();
  for (i = 0; i < 4; i++) {
    json_array_append(array, five);
    json_array_append(array, seven);
  }
  if (json_array_size(array) != 8)
    printf("unable to append 8 items to array");
  
  /* Remove an element from a "full" array. */
  json_array_remove(array, 5);
  
  json_decref(five);
  json_decref(seven);
  json_decref(array);
}

static void test_clear(void) {
  json_t *array, *five, *seven;
  int i;
  
  array = json_array();
  five = json_integer(5);
  seven = json_integer(7);
  
  if (!array)
    printf("unable to create array");
  if (!five || !seven)
    printf("unable to create integer");
  
  for (i = 0; i < 10; i++) {
    if (json_array_append(array, five))
      printf("unable to append");
  }
  for (i = 0; i < 10; i++) {
    if (json_array_append(array, seven))
      printf("unable to append");
  }
  
  if (json_array_size(array) != 20)
    printf("array size is invalid after appending");
  
  if (json_array_clear(array))
    printf("unable to clear");
  
  if (json_array_size(array) != 0)
    printf("array size is invalid after clearing");
  
  json_decref(five);
  json_decref(seven);
  json_decref(array);
}

static void test_extend(void) {
  json_t *array1, *array2, *five, *seven;
  int i;
  
  array1 = json_array();
  array2 = json_array();
  five = json_integer(5);
  seven = json_integer(7);
  
  if (!array1 || !array2)
    printf("unable to create array");
  if (!five || !seven)
    printf("unable to create integer");
  
  for (i = 0; i < 10; i++) {
    if (json_array_append(array1, five))
      printf("unable to append");
  }
  for (i = 0; i < 10; i++) {
    if (json_array_append(array2, seven))
      printf("unable to append");
  }
  
  if (json_array_size(array1) != 10 || json_array_size(array2) != 10)
    printf("array size is invalid after appending");
  
  if (json_array_extend(array1, array2))
    printf("unable to extend");
  
  for (i = 0; i < 10; i++) {
    if (json_array_get(array1, i) != five)
      printf("invalid array contents after extending");
  }
  for (i = 10; i < 20; i++) {
    if (json_array_get(array1, i) != seven)
      printf("invalid array contents after extending");
  }
  
  json_decref(five);
  json_decref(seven);
  json_decref(array1);
  json_decref(array2);
}

static void test_circular() {
  json_t *array1, *array2;
  
  /* the simple cases are checked */
  
  array1 = json_array();
  if (!array1)
    printf("unable to create array");
  
  if (json_array_append(array1, array1) == 0)
    printf("able to append self");
  
  if (json_array_insert(array1, 0, array1) == 0)
    printf("able to insert self");
  
  if (json_array_append_new(array1, json_true()))
    printf("failed to append true");
  
  if (json_array_set(array1, 0, array1) == 0)
    printf("able to set self");
  
  json_decref(array1);
  
  /* create circular references */
  
  array1 = json_array();
  array2 = json_array();
  if (!array1 || !array2)
    printf("unable to create array");
  
  if (json_array_append(array1, array2) || json_array_append(array2, array1))
    printf("unable to append");
  
  /* circularity is detected when dumping */
  if (json_dumps(array1, 0) != NULL)
    printf("unable to dump circulars");
  
  /* decref twice to deal with the circular references */
  json_decref(array1);
  json_decref(array2);
  json_decref(array1);
}

static void test_array_foreach() {
  size_t index;
  json_t *array1, *array2, *value;
  
  array1 = json_pack("[sisisi]", "foo", 1, "bar", 2, "baz", 3);
  array2 = json_array();
  
  json_array_foreach(array1, index, value) { json_array_append(array2, value); }
  
  if (!json_equal(array1, array2))
    printf("json_array_foreach failed to iterate all elements");
  
  json_decref(array1);
  json_decref(array2);
}

static void test_bad_args(void) {
  json_t *arr = json_array();
  json_t *num = json_integer(1);
  
  if (!arr || !num)
    printf("failed to create required objects");
  
  if (json_array_size(NULL) != 0)
    printf("NULL array has nonzero size");
  if (json_array_size(num) != 0)
    printf("non-array has nonzero array size");
  
  if (json_array_get(NULL, 0))
    printf("json_array_get did not return NULL for non-array");
  if (json_array_get(num, 0))
    printf("json_array_get did not return NULL for non-array");
  
  if (!json_array_set_new(NULL, 0, json_incref(num)))
    printf("json_array_set_new did not return error for non-array");
  if (!json_array_set_new(num, 0, json_incref(num)))
    printf("json_array_set_new did not return error for non-array");
  if (!json_array_set_new(arr, 0, NULL))
    printf("json_array_set_new did not return error for NULL value");
  if (!json_array_set_new(arr, 0, json_incref(arr)))
    printf("json_array_set_new did not return error for value == array");
  
  if (!json_array_remove(NULL, 0))
    printf("json_array_remove did not return error for non-array");
  if (!json_array_remove(num, 0))
    printf("json_array_remove did not return error for non-array");
  
  if (!json_array_clear(NULL))
    printf("json_array_clear did not return error for non-array");
  if (!json_array_clear(num))
    printf("json_array_clear did not return error for non-array");
  
  if (!json_array_append_new(NULL, json_incref(num)))
    printf("json_array_append_new did not return error for non-array");
  if (!json_array_append_new(num, json_incref(num)))
    printf("json_array_append_new did not return error for non-array");
  if (!json_array_append_new(arr, NULL))
    printf("json_array_append_new did not return error for NULL value");
  if (!json_array_append_new(arr, json_incref(arr)))
    printf("json_array_append_new did not return error for value == array");
  
  if (!json_array_insert_new(NULL, 0, json_incref(num)))
    printf("json_array_insert_new did not return error for non-array");
  if (!json_array_insert_new(num, 0, json_incref(num)))
    printf("json_array_insert_new did not return error for non-array");
  if (!json_array_insert_new(arr, 0, NULL))
    printf("json_array_insert_new did not return error for NULL value");
  if (!json_array_insert_new(arr, 0, json_incref(arr)))
    printf("json_array_insert_new did not return error for value == array");
  
  if (!json_array_extend(NULL, arr))
    printf("json_array_extend did not return error for first argument "
           "non-array");
  if (!json_array_extend(num, arr))
    printf("json_array_extend did not return error for first argument "
           "non-array");
  if (!json_array_extend(arr, NULL))
    printf("json_array_extend did not return error for second argument "
           "non-array");
  if (!json_array_extend(arr, num))
    printf("json_array_extend did not return error for second argument "
           "non-array");
  
  if (num->refcount != 1)
    printf("unexpected reference count on num");
  if (arr->refcount != 1)
    printf("unexpected reference count on arr");
  
  json_decref(num);
  json_decref(arr);
}

static void run_tests() {
  test_misc();
  test_insert();
  test_remove();
  test_clear();
  test_extend();
  test_circular();
  test_array_foreach();
  test_bad_args();
}



