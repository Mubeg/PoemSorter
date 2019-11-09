/*!
\file 
\brief Заголовочный файл, включающий библиотеки, определения функций и define-ы
*/
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

const int DEFAULT = 0;
const int MEMORY_PROB = 9;
const char DEFAULT_FILE[] = "poem.txt";
const int FNF = 3;
const int ADD_FAILED = 4;
const int FILE_OPER_PROBLEM = 5;
const int NO_RIGHTS = 8;
const int EPS_OKRESTNOST = 3;
const int MAX_LINE_LEN  = 100;
const int MATCHING_STR_NF = 1;

#define USING_WCHAR_T

typedef wchar_t Elem_t;
//#undef USING_WCHAR_T

#ifndef USING_WCHAR_T
#define wmemchr memchr
#define iswalpha isalpha
#define Long_char "c"
#define Long_str "s"
#define Long_string(x) x
#define fgetwc fgetc
#define fwprintf fprintf
#define wcslen strlen

#else
#define Long_char "C"
#define Long_str "S"
#define Long_string(x) L ## x

#endif

/*!
Структура string и указатель на неё str_ptr
*/
typedef struct str * str_ptr;
struct str {
	Elem_t *str;
	int size;
};


/*!
Создаёт одномерный массив и читает в него сырой текст из файла
\param[in] file_name[] Название файла
\param[out] buff_size Размер созданого массива
\return Указатель на массив
*/
Elem_t *read_file_to_created_buff(const char file_name[], int *buff_size);

/*!
Рассчитывает размер файла, указатель файла ставится в начало
\param[in] file - структура FILE * открытого файла
\param[in] mode //none in alpha
\return Размер файла в int
*/
int get_file_length(FILE *file, const int mode); //alpha version

/*!
Создаёт массив указателей на начало строк и заменяет '\n' на '\0'
\param[in] buff Указатель на массив сырого текста
\param[in] buff_size Размер массива
\param[out] text_size Размер созданного массива
\return Указатель на массив
*/
str_ptr make_text_must_free(Elem_t buff[], const int buff_size, int * const text_size);


int change_ch1_to_ch2_and_count_in_ch_buff(const Elem_t char1, const Elem_t char2, Elem_t buff[], const int buff_size);


int fill_string_split_by_separator_and_skip_non_alpha_lines_from_ch_buff(str_ptr text, const Elem_t separator, Elem_t buff[], const int buff_size);


int fill_string_split_by_separator_from_ch_buff(str_ptr text, const Elem_t separator, Elem_t buff[], const int buff_size);

/*!
Сортирует массив указателей с кастомной функцией быстрой сортировкой
\param[in] text Сортиремый массив
\param[in] text_size Размер массива
\param[in] compare Компаратор: 1 = left < right; 0 = left >= right
\return Состояние выполнения
*/
int quick_sort_poem(str_ptr text, const int text_size, bool (compare)(const str, const str));


int quick_sort_poem_with_qsort(str_ptr text, const int text_size, int (compare)(const void *, const void *));


int find_rythm_for_entered_line_and_proc(const str_ptr text, const int text_size);


str_ptr find_rythm(str_ptr line, const str_ptr text, const int text_size);


bool str_partially_match_reverse(const Elem_t str1[], const Elem_t str2[], const int len1, const int len2);


int compare_str_direct_shell_qsort(const void * str1, const void * str2);


int compare_str_reverse_shell_qsort(const void * str1, const void * str2);

/*!
Оболочка для compare_str_reverse для std::sort
\param[in] str1 Строка1
\param[in] str2 Строка2
\return 1 = left < right; 0 = left >= right
*/
bool compare_str_reverse_shell(const str str1, const str str2);

/*!
Оболочка для compare_str_direct для std::sort
\param[in] str1 Строка1
\param[in] str2 Строка2
\return 1 = left < right; 0 = left >= right
*/
bool compare_str_direct_shell(const str str1, const str str2);

/*!
Сравнивает строки с начала, игнорируя неалфавитные символы
\param[in] str1 Строка 1
\param[in] str2 Строка 2
\param[in] len1 Длина строки 1
\param[in] len2 Длина строки 2
\return 1 / 0 / -1 : > / = / <
*/
int compare_str_direct(const Elem_t str1[], const Elem_t str2[], const int len1, const int len2);

/*!
Сравнивает строки с конца, игнорируя неалфавитные символы
\param[in] str1 Строка 1
\param[in] str2 Строка 2
\param[in] len1 Длина строки 1
\param[in] len2 Длина строки 2
\return 1 / 0 / -1 : > / = / <
*/
int compare_str_reverse(const Elem_t str1[], const Elem_t str2[], const int len1, const int len2);


void print_text_with_size(FILE *file, const str_ptr text, const int text_size);

/*!
Печатает в массив указателей файл как строки
\param[in] file Файл, открытый на запись
\param[in] text Указатель на массив
\param[in] text_size Размер массива
*/
void print_text(FILE *file, const str_ptr text);

/*!
Печатает в массив указателей файл как строки, заменяя все '\0' на '\n', кроме последнего
\param[in] file Файл, открытый на запись
\param[in] buff Указатель на массив с сырым текстом
\param[in] text_size Размер массива
*/
void print_original_and_revive_bN(FILE *file, Elem_t buff[], const int buff_size);


