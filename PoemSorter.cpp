/*!
\file
\brief Главный файл программы
*/
#include "PoemSorter.h"
//#define DEBUG

int main(const int args, const char *argv[]){
	
	std::setlocale(LC_ALL, "ru_RU.UTF-8");

	const char *file_name = "poem.txt";

	#ifndef DEBUG
	if(args < 2){
		fprintf(stderr, "Неверный вызов\nИспользуйсте: %s [имя файла]\n", argv[0]);
		return 0;
	}

	file_name = argv[1];

	#endif

	FILE *file = fopen("sorted_poem.txt", "w");
	if(!file){
		fprintf(stderr, "Not Enough Rights to open %s for writing\n", file);
		return 1;
	}



	int buff_size = -1;
	Elem_t *buff = read_file_to_created_buff(file_name, &buff_size);
	if(buff == nullptr){
		fprintf(stderr, "Bad allocation %s: buff\n", __PRETTY_FUNCTION__);
		return 1;
	}


	int text_size = 0;
	str_ptr text = make_text_must_free(buff, buff_size, &text_size);
	if(text == nullptr){
		fprintf(stderr, "%s: make_text_must_free(): text == nullptr error\n", __PRETTY_FUNCTION__);
		return 1; 
	}

	int answer = quick_sort_poem_with_qsort(text, text_size, compare_str_reverse_shell_qsort);
	switch(answer){
		case 0:

			#ifdef DEBUG
			printf("Qsort1 of text successfull\n");
			#endif
			break;
		default:
			fprintf(stderr, "%s: ERROR: quick_sort_poem() unexpected return\n", __PRETTY_FUNCTION__);
			return 1;
	}
	//print_text(file, text);

	for(int i = 0; i < 100; i++){
		answer = find_rythm_for_entered_line_and_proc(text, text_size);
		switch(answer){
			case 0:
				break;
			case MATCHING_STR_NF:
				printf("No matching line found\n");
				break;

			default:
				fprintf(stderr, "main(): ERROR: quick_sort_poem() unexpected return\n");
				return 1;
		}	
	}

	answer = quick_sort_poem_with_qsort(text, text_size, compare_str_direct_shell_qsort);
	switch(answer){
		case 0:
			
			#ifdef DEBUG
			printf("Qsort2 of text successfull\n");
			#endif
			break;

		default:
			fprintf(stderr, "%s: ERROR: quick_sort_poem() unexpected return\n", __PRETTY_FUNCTION__);
			return 1;
	}
	//print_text(file, text);

	//print_original_and_revive_bN(file, buff, buff_size);


	free(text);
	free(buff);
	fclose(file);

	return 0;
}

Elem_t *read_file_to_created_buff(const char file_name[], int *buff_size){

	assert(file_name[0] != '\0');
	assert(file_name);

	FILE *file = fopen(file_name, "r");
	if(!file){
		fprintf(stderr, "Cannot open file %s\n", file_name);
		return nullptr;
	}
	
	const int file_length = get_file_length(file, DEFAULT);
	if(file_length < 0){
		fprintf(stderr, "get_file_length() return incorrect\n");
		return nullptr;
	}

	*buff_size = file_length + 1;
	Elem_t *buff = (Elem_t *) calloc(*buff_size, sizeof(Elem_t));


	#ifdef USING_WCHAR_T
	Elem_t c = fgetwc(file);
	for(int i = 0; i < file_length; i++){
		
		c = fgetwc(file);
		if(c == EOF){
			*buff_size = i + 1;
			break;
		}

		buff[i] = c;
		fflush(stderr);
	}

	#else
	int num_read = fread(buff, 1, file_length, file);
	if(num_read == -1 || num_read != file_length){	
		return nullptr;
	}
	#endif

	fclose(file);

	return buff;
}

int get_file_length(FILE *file, const int mode){

	assert(file != nullptr);

	int ans = 0;

	if(fseek(file, 0, SEEK_END)){
		fprintf(stderr, "fseek in %s SEEK_END return not zero\n", __PRETTY_FUNCTION__);
	}
	if((ans = ftell(file)) == -1L){
		fprintf(stderr, "ftell in %s return -1L\n", __PRETTY_FUNCTION__);
	}
	if(fseek(file, 0, SEEK_SET)){
		fprintf(stderr, "fseek in %s SEEK_SET return not zero\n", __PRETTY_FUNCTION__);
	}
	
	return ans	;
}

str_ptr make_text_must_free(Elem_t buff[], const int buff_size, int * const text_size){

	assert(buff      != nullptr);
	assert(text_size != nullptr);
	assert(buff_size >= 0);

	int num_lines = 0;

	num_lines = change_ch1_to_ch2_and_count_in_ch_buff(Long_string('\n'), Long_string('\0'), buff, buff_size);

	str_ptr text = nullptr;
	text = (str_ptr) calloc(num_lines + 1, sizeof(str)); // text[num_lines].size = -1
	if(!text){
		fprintf(stderr, "Mem calloc for text in %s problem", __PRETTY_FUNCTION__);
	}

	*text_size = fill_string_split_by_separator_from_ch_buff(text, Long_string('\0'), buff, buff_size);

	text[*text_size].size = -1;
	
	return text;
}

int change_ch1_to_ch2_and_count_in_ch_buff(const Elem_t char1, const Elem_t char2, Elem_t buff[], const int buff_size){

	assert(buff      != nullptr);
	assert(buff_size >= 0      );

	int num_lines = 0;

	for(Elem_t * ptr = (Elem_t *) wmemchr(buff, char1, buff_size);
	    ptr < buff + buff_size;
	    ptr = (Elem_t *) wmemchr(ptr + 1, char1, buff + buff_size - 1 - ptr)){

		if(!ptr)
			break;
	
		num_lines++;
		*ptr = char2;
	}
	
	return num_lines;
}

int fill_string_split_by_separator_and_skip_non_alpha_lines_from_ch_buff(str_ptr text, const Elem_t separator, Elem_t buff[], const int buff_size){
	
	assert(text      != nullptr);
	assert(buff      != nullptr);
	assert(buff_size >= 0      );

	bool found = false;
	bool has_alpha = false;
	int text_ptr = 0;
	int last_i = 0;

	for(int i = 0; i < buff_size; i+= sizeof(buff[0])){

		if(found){
			if(has_alpha){
				text[text_ptr].str = buff + last_i;
				text[text_ptr].size = i - last_i;
				text_ptr++;
				has_alpha = false;
			}
			last_i = i;
			found = false;
		}
		if(buff[i] == separator){
			found = true;
		}
		if(iswalpha(buff[i])){
			has_alpha = true;
		}
		
	}
	
	return text_ptr - 1;
}

int fill_string_split_by_separator_from_ch_buff(str_ptr text, const Elem_t separator, Elem_t * buff, const int buff_size){
	
	assert(text      != nullptr);
	assert(buff      != nullptr);
	assert(buff_size >= 0      );

	int text_ptr = 0;
	text[0].str = buff;
	Elem_t * last_ptr = buff;

	for(Elem_t * ptr = (Elem_t *) wmemchr(buff, separator, buff_size);
	    ptr < buff + buff_size;
	    ptr = (Elem_t *) wmemchr(last_ptr, separator, buff + buff_size - 1 - ptr)){
		
		if(ptr == nullptr)
			break;

		text_ptr++;
		text[text_ptr - 1].str = last_ptr;
		text[text_ptr - 1].size = ptr - last_ptr + 1;
		last_ptr = ptr + 1;
	}

	return text_ptr - 1;
}

int quick_sort_poem(str_ptr text, const int text_size, bool (compare)(const str, const str)){

	std::sort(text, text + text_size, compare);
	return 0;
}

int quick_sort_poem_with_qsort(str_ptr text, const int text_size, int (compare)(const void *, const void *)){

	assert(text != nullptr);
	qsort(text, text_size, sizeof(text[0]), compare);
	return 0;
}

int find_rythm_for_entered_line_and_proc(const str_ptr text, const int text_size){

	Elem_t * line = (Elem_t *) calloc(MAX_LINE_LEN, sizeof(line[0]));

	printf("Enter your line that you what to find rithm to below:\n");

	int counter = 0;
	Elem_t c = fgetwc(stdin);
	while(c != Long_string('\n') && c != EOF){
		
		line[counter] = c;
		counter++;
		if(counter >= MAX_LINE_LEN - 1){
			break;
		}
		c = fgetwc(stdin);
	}

	str line_to_send = {};
	line_to_send.str = line;
	line_to_send.size = wcslen(line);


	str_ptr ans[2*EPS_OKRESTNOST] = {}; //[2*(EPS_OKRESTNOST - 1) + 1 + 1]

	bool found = false;
	find_rythm(ans, &line_to_send, text, text_size, &found);
	if(!found){
		return MATCHING_STR_NF;
	}

	printf("\nYour lines:\n");
	for(int i = 0; i < 2*EPS_OKRESTNOST; i++){
		if(ans[i] != nullptr){
			printf("%" Long_str "\n", ans[i]->str);
		}
	}
	printf("\n");

	free(line);

	return 0;
}

void find_rythm(str_ptr *bsearch_answer, str_ptr line, const str_ptr text, const int text_size, bool *found){

	assert(line      != nullptr);
	assert(text      != nullptr);
	assert(text_size >= 0      );
	
	int right_border = text_size - 1;
	int left_border  = 0;
	int middle       = (right_border + left_border)/2;
	
	for(; right_border - left_border > EPS_OKRESTNOST; middle = (left_border + right_border)/2){

		int compare = compare_str_reverse_shell_qsort(line, text + middle);

		if(compare > 0){
 			left_border  = middle;
		}
		else if(compare < 0){
			right_border = middle;
		}
		else{
			break;
		}
	}

	for(int i = 0; i < EPS_OKRESTNOST; i++){

		if(middle + i < text_size){
			int ans = str_partially_match_reverse(line -> str, text[middle + i].str, line->size, text[middle + i].size);

			if(RYTHM_MIN_IDENTICAL <= ans && ans <= RYTHM_MAX_IDENTICAL){
				bsearch_answer[2*i] = text + middle + i;
				*found = true;
			}
		}
		if(middle - i >= 0){
			int ans = str_partially_match_reverse(line -> str, text[middle - i].str, line->size, text[middle - i].size);

			if(RYTHM_MIN_IDENTICAL <= ans && ans <= RYTHM_MAX_IDENTICAL){
				bsearch_answer[2*i + 1] = text + middle - i;
				*found = true;
			}
		}
	}	

}

int str_partially_match_reverse(const Elem_t str1[], const Elem_t str2[], const int len1, const int len2){
	
	assert(str1 != nullptr);
	assert(str2 != nullptr);

	int counter = 0;

	for(int ptr1 = len1 - 1, ptr2 = len2 - 1; ptr1 >= 0 && ptr2 >= 0; ptr1--, ptr2--){

		if(!iswalpha(str1[ptr1])){
			ptr2++;
			continue;
		}
		if(!iswalpha(str2[ptr2])){
			ptr1++;
			continue;
		}

		if(str1[ptr1] == str2[ptr2])
			counter++;		
		else
			return counter;
	}
	
	return counter;
}

int compare_str_direct_shell_qsort(const void * str1, const void * str2){
	
	str_ptr str3 = (str_ptr) str1;
	str_ptr str4 = (str_ptr) str2;

	assert(str3 != nullptr);
	assert(str4 != nullptr);

	return compare_str_direct(str3->str, str4->str, str3->size, str4->size);
}

int compare_str_reverse_shell_qsort(const void * str1, const void * str2){

	str_ptr str3 = (str_ptr) str1;
	str_ptr str4 = (str_ptr) str2;

	assert(str3 != nullptr);
	assert(str4 != nullptr);

	return compare_str_reverse(str3->str, str4->str, str3->size, str4->size);
}

bool compare_str_reverse_shell(const str str1, const str str2){

	return compare_str_reverse(str1.str, str2.str, str1.size, str2.size) < 0;
}

bool compare_str_direct_shell(const str str1, const str str2){

	return compare_str_direct(str1.str, str2.str, str1.size, str2.size) < 0;
}

int compare_str_direct(const Elem_t str1[], const Elem_t str2[], const int len1, const int len2){

	assert(str1 != nullptr);
	assert(str2 != nullptr);

	int ptr1 = 0, ptr2 = 0;
	
	for(; ptr1 < len1 && ptr2 < len2; ptr1++, ptr2++){
		
		if(!iswalpha(str1[ptr1]) && !iswalpha(str2[ptr2])){
			continue;
		}		
		if(!iswalpha(str1[ptr1])){
			ptr2--;
			continue;
		}
		if(!iswalpha(str2[ptr2])){
			ptr1--;
			continue;
		}
		
		if(str1[ptr1] > str2[ptr2])
			return 	1;		
		else if (str1[ptr1] < str2[ptr2])
			return -1;

	}

	int left = 0, right = 0;

	for(int i = ptr1; i < len1; i++){
		left += iswalpha(str1[i]) ? 1 : 0;
	}

	for(int i = ptr2; i < len2; i++){
		right += iswalpha(str2[i]) ? 1 : 0;
	}

	return (left < right) ? -1 : left > right;

}

int compare_str_reverse(const Elem_t str1[], const Elem_t str2[], const int len1, const int len2){

	assert(str1 != nullptr);
	assert(str2 != nullptr);

	int ptr1 = len1 - 1, ptr2 = len2 - 1;

	for(;ptr1 >= 0 && ptr2 >= 0; ptr1--, ptr2--){

		if(!iswalpha(str1[ptr1]) && !iswalpha(str2[ptr2])){
			continue;
		}

		if(!iswalpha(str1[ptr1])){
			ptr2++;
			continue;
		}
		if(!iswalpha(str2[ptr2])){
			ptr1++;
			continue;
		}


		if(str1[ptr1] > str2[ptr2])
			return 1;		
		else if (str1[ptr1] < str2[ptr2])
			return -1;
	}
	
	int left = 0, right = 0;

	for(int i = ptr1; i >= 0; i--){
		left += iswalpha(str1[i]) ? 1 : 0;
	}

	for(int i = ptr2; i >= 0; i--){
		right += iswalpha(str2[i]) ? 1 : 0;
	}

	
	return (left < right) ? -1 : right < left;
}

void print_text_with_size(FILE *file, const str_ptr text, const int text_size){

	assert(text      != nullptr);
	assert(text_size >= 0);

	fprintf(file, "\n\n");
	for(int i = 0; i < text_size; i++)
		fprintf(file, "%" Long_str "\n", text[i]);
}

void print_text(FILE *file, const str_ptr text){

	assert(text != nullptr);

	fprintf(file, "\n\n");
	for(int i = 0; text[i].size != -1; i++){
		fprintf(file, "%" Long_str "\n", text[i]);
	}
}

void print_original_and_revive_bN(FILE *file, Elem_t buff[], const int buff_size){

	assert(buff != nullptr);

	fprintf(file, "\nNOW COMES UNREPEATABLE ORIGINAL\n");
	if(!change_ch1_to_ch2_and_count_in_ch_buff('\0', '\n', buff, buff_size)){
		fprintf(stderr, "Error: in %" Long_str " : not changed any char\n", __PRETTY_FUNCTION__);
		return;
	}
	fprintf(file, "%" Long_str "\n", buff);
}

