#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define clock_array_hit 1
#define clock_array_miss 0
#define clock_len 262144

struct node{
	unsigned long long int page;
	int reference;
};

struct node* clock_array[clock_len];
int clock_array_point_index=0;
long hit = 0;
long miss = 0;


void init_clock_array(){
	for(int i=0; i<clock_len; i++){
		clock_array[i] = (struct node*)malloc(sizeof(struct node));
		clock_array[i]->page = 0;
		clock_array[i]->reference = 0;
	}
}

int insert_page(unsigned long long int page){

	// 看page是否已經存在clock_array內,
	int isRemainSpace = 0;
	int i;
	for(i=0; i<clock_len; i++){

		if(clock_array[i]->page == 0){ //還有空間可以放
			isRemainSpace = 1;
			break;
		}
		if(clock_array[i]->page == page){
			clock_array[i]->reference = 1;
			hit++;
			return clock_array_hit;
		}
	}
	// 不在clock_array內
		// 空間還有剩下
	if(isRemainSpace){
		clock_array[i]->page = page;
		clock_array[i]->reference = 1;
		miss++;
		return clock_array_miss;
	}

		// 空間不夠需要置換
	while(clock_array[clock_array_point_index]->reference != 0){
		clock_array[clock_array_point_index]->reference--;
		clock_array_point_index = (clock_array_point_index+1) % clock_len;
	}

	clock_array[clock_array_point_index]->page = page;
	miss++;
	clock_array[clock_array_point_index]->reference = 1;
	return clock_array_miss;

}

int main(int argc, char *argv[]) {
    FILE *file;
    char buffer[20]; 

	unsigned long long int hexValue;

	init_clock_array();

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0';
		sscanf(buffer, "%llx", &hexValue);
        //printf("%s hex: %llx]\n", buffer, hexValue);  //fordebug
		unsigned long long int page = hexValue >> 12; // addr 2 page
		insert_page(page);
    }

    fclose(file);

	long total = miss+hit;
	printf("miss: %ld, hit: %ld, total: %ld\n", miss, hit, total);
	printf("hit ratio %f%%\n", ((double)hit*100.0f / total));
    return 0;
}

