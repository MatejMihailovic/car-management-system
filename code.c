#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXL1 51
#define MAXL2 16
#define MAXL3 100
#define SIZE 67

FILE *safe_open(char *fileName, char *mode);
void clearInputBuffer();
int getUserInput(); 
//AVL tree implementation
typedef struct{
	int key;
	char name[MAXL1];
	int mass;
	char country[MAXL2];
}car;

struct node{
	car car[1];
	struct node *left;
	struct node *right;
	int height;
};

void loadCars(FILE *pf, car cars[], int n);
int max(int a, int b);
int calculateTreeHeight(struct node *node);
struct node *newNode(car cars[],int index);
struct node *rotateRight(struct node *y);
struct node *rotateLeft(struct node *x);
int calculateTreeBalance(struct node *node);
struct node *avlTreeInsert(struct node *node, car cars[], int index);
int search(struct node *node, int index, int m);
void inOrder(struct node *node, FILE *pf);

//HashTable implementation
typedef struct DataItem DataItem;
struct DataItem{
	car *car;
	int key;
};

typedef struct HashTable HashTable;
struct HashTable{
	DataItem** items;
	int size;
	int counter;
};
int hashFunction(int key, int capacity);
DataItem* createItem(int key, car cars[]);
HashTable* createTable(int size);
void hashTableInsert(HashTable *table, int key, car cars[], int capacity);
void deleteItem(HashTable *table, int key, int capacity);
int isPrimeNumber(int n);
int nextPrimeNumber(int n);
void printTable(HashTable* table);
void freeItem(DataItem* item);
void freeTable(HashTable* table);
void rehashedTableInsert(HashTable **ht, car *cars, int n, int capacity);

int main(){
	int n = 0, i;
	char option;

	do{
		fflush(stdin);
		printf("1. AVL tree\n2. Hash Table\n0. Exit\nChoose an option:");
		option = getUserInput();

		switch(option){
			case '1':{
				puts("------------------------------------------------------------------");
				struct node *node = NULL;
				struct node *temp = NULL;

				int index = 1, mass;

				FILE *pf1 = safe_open("cars.txt","r");
				fscanf(pf1,"%d", &n);

				car *cars = calloc(n,sizeof(car));
				loadCars(pf1,cars,n);
				fclose(pf1);

				for(i = 0; i<n; i++){
					node = avlTreeInsert(node, cars, i);
				}

				while(1){
					printf("Ordinal number of an existing car: ");
					scanf("%d",&index);

					if(index <= 0 || index > n) break;

					printf("New mass: ");
					scanf("%d",&mass);
					int result = search(node, index, mass);

					if(result){
						puts("Mass successfully changed.");
					}else{
						puts("Error while changing mass!");
					}
				}
				
				//Write our tree into a new file
				FILE *pf2 = safe_open("cars_2.txt","w");
				inOrder(node, pf2);

				fclose(pf2);
				free(cars);
				puts("------------------------------------------------------------------");
			}break;
			case '2':{
				puts("------------------------------------------------------------------");
				int index, carNum;

				FILE *pf1 = safe_open("cars.txt","r");
				fscanf(pf1,"%d", &carNum);

				car *cars = calloc(carNum, sizeof(car));
				loadCars(pf1, cars, carNum);

				int capacity = SIZE;

				HashTable *ht = createTable(capacity);

				rehashedTableInsert(&ht, cars, carNum, capacity);

				printf("\t\tTABLE BEFORE DELETION\n");
				printTable(ht);

				while(1){
					printf("Enter the index of the car you want to delete:");
					scanf("%d",&index);

					if(index < 0) break;
					
					deleteItem(ht, index-1, capacity);
				}

				printf("\t\tTABLE AFTER DELETION\n");
				printTable(ht);

				freeTable(ht);
				puts("------------------------------------------------------------------");
			}break;
			case '0': 
			{
				exit(0);
			}
			break;
			default: puts("You entered invalid option!");
		}
	}while(1);

	return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getUserInput() {
    int c;
    do {
        c = getchar();
        clearInputBuffer(); // Clear buffer in case of extra characters
    } while (c == '\n' || c == EOF); // Skip newlines and EOF
    return c;
}

FILE *safe_open(char *fileName,char *mode){
	FILE *pf = fopen(fileName,mode);
	if(pf == NULL){
		puts("File couldn't be opened!");
		exit(1);
	}
	return pf;
}

void loadCars(FILE *pf,car cars[],int n){
	int i;
	for(i=0;i<n;i++){
		fscanf(pf,"%d %s %d %s",&cars[i].key,cars[i].name,&cars[i].mass,cars[i].country);
	}
}

int max(int a, int b){
	return (a > b)? a : b;
}

int calculateTreeHeight(struct node *node){
	if (node == NULL)
		return 0;
	return node->height;
}

struct node* newNode(car cars[], int index){
	struct node* node = (struct node*) malloc(sizeof(struct node));
	node->car[0].key = cars[index].key;
	strcpy(node->car[0].name, cars[index].name);
	node->car[0].mass = cars[index].mass;
	strcpy(node->car[0].country, cars[index].country);
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return(node);
}

struct node *rotateRight(struct node *y){
	struct node *x = y->left;
	struct node *T2 = x->right;
	//Rotation
	x->right = y;
	y->left = T2;

	//Change of height
	y->height = max(calculateTreeHeight(y->left), calculateTreeHeight(y->right))+1;
	x->height = max(calculateTreeHeight(x->left), calculateTreeHeight(x->right))+1;
	
	return x;
}

struct node *rotateLeft(struct node *x){
	struct node *y = x->right;
	struct node *T2 = y->left;
	//Rotation
	y->left = x;
	x->right = T2;

	//Change of height
	x->height = max(calculateTreeHeight(x->left), calculateTreeHeight(x->right))+1;
	y->height = max(calculateTreeHeight(y->left), calculateTreeHeight(y->right))+1;
	
	return y;
}

int calculateTreeBalance(struct node *node){
	if (node == NULL) return 0;

	return calculateTreeHeight(node->left) - calculateTreeHeight(node->right);
}

struct node* avlTreeInsert(struct node* node, car cars[], int index){

	if(node == NULL) return(newNode(cars, index));

	if(cars[index].key < node->car[0].key)
		node->left = avlTreeInsert(node->left, cars, index);
	else if (cars[index].key > node->car[0].key)
		node->right = avlTreeInsert(node->right,cars, index);
	else return node; //equal key values are forbidden in AVL tree

	//Height change of new node
	node->height = 1 + max(calculateTreeHeight(node->left),calculateTreeHeight(node->right));

	//Check if tree is unbalanced 
	int balance = calculateTreeBalance(node);
	
	//Left Left case
	if(balance > 1 && cars[index].key < node->left->car[0].key) return rotateRight(node);

	//Right Right case
	if(balance < -1 && cars[index].key > node->right->car[0].key) return rotateLeft(node);

	//Left Right case
	if(balance > 1 && cars[index].key > node->left->car[0].key){
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}
	//Right Left case
	if(balance < -1 && cars[index].key < node->right->car[0].key){
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	//Return unchanged node
	return node;
}

int search(struct node *node, int key, int m){
	if (node == NULL) {
        return 0;
    }

    if (node->car[0].key == key) {
		printf("Car with key %d: Mass changed from %d to %d\n", key, node->car[0].mass, m);
        node->car[0].mass = m;
        // Key found and modified successfully
        return 1;
    }

    // Search in left subtree
    int foundLeft = search(node->left, key, m);
    if (foundLeft) {
        return 1;
    }

    // Search in right subtree
    int foundRight = search(node->right, key, m);
    return foundRight; 
}

void inOrder(struct node *node, FILE *pf){

	if(node != NULL){
		inOrder(node->left,pf);
		fprintf(pf,"%d %s %d %s %d\n", node->car[0].key,node->car[0].name,node->car[0].mass,node->car[0].country,node->height);
		inOrder(node->right,pf);
	}
}

int hashFunction(int key, int capacity){
	return (key % capacity);
}

DataItem* createItem(int key, car cars[]){
	DataItem *item = (DataItem*) malloc(sizeof(DataItem)); 
	item->car = (car*) malloc(sizeof(car));
	
	item->key = key;
	item->car[0] = cars[key];
	
	return item;
}

HashTable* createTable(int size){
	int i;
	HashTable* table = (HashTable*) malloc(sizeof(HashTable));
	table->size = size;
	table->counter = 0;
	table->items = (DataItem**) calloc(table->size, sizeof(DataItem*));
	
	for(i = 0; i < table->size; i++){
		table->items[i] = NULL;
	}
	return table;
}

void hashTableInsert(HashTable *table, int key, car cars[], int capacity){

	DataItem *item = createItem(key,cars);
	
	int index = hashFunction(key,capacity);
	DataItem *currentItem = table->items[index];
	
	if(currentItem == NULL){
		if(table->counter == table->size){
			printf("Hash table is full\n");
			freeItem(item);
			return;
		}
		table->items[index] = item;
		table->counter++;
	}else{
		//In case of colission
		int originalIndex = index;
		int step = 4; 

		while (table->items[index] != NULL && table->items[index]->key != key) {
    		index = (originalIndex + step) % capacity; // Use modulo to wrap around the table
    		step++; // Increase the step for the next probe
    		if (index == originalIndex) {
				printf("Hash table is full\n");
				freeItem(item);
				return;
    		}
		}	

		table->items[index] = item;
		table->counter++;
	}
}

void deleteItem(HashTable *table, int key, int capacity){
	int index = hashFunction(key, capacity);
	DataItem *item = table->items[index];

	if(item == NULL){
		puts("Element doesn't exist!");
		return;
	}

	if(item->key == key){
		printf("Car deleted: %d %s\n", item->car->key, item->car->name);
		//Delete item and make his index NULL
		table->items[index] = NULL;
		freeItem(item);
		//Decrease number of elements in a hash table
		table->counter--;
		return;
	}
}

int isPrimeNumber(int n){
	int i;
	
	if(n <= 1) return 0;
	if(n <= 3) return 1;
	
	if(n%2 == 0 || n%3 == 0) return 0;
	
	for(i = 5; i * i <= n; i = i + 6){
		if(n % i == 0 || n % (i+2) == 0) return 0;
	}
	return 1;
} 
int nextPrimeNumber(int n){
	//Basic case
	if(n <= 1) return 2;
	
	int temp = n;
	int found = 0;
	
	while(!found){
		temp++;
		
		if(isPrimeNumber(temp)) found = 1;
	}
	return temp;
}

void printTable(HashTable* table){
	int i;
	puts("------------------------------------------------------------------");
	for(i = 0; i < table->size; i++){
		if(table->items[i]){
			printf("Index:%d Name:%s \n",(table->items[i]->key+1),table->items[i]->car[0].name);	
 		} else {
 			printf("Index:%d ~~ FREE SPACE ~~\n", i + 1);
 		}
	}
	puts("------------------------------------------------------------------");
} 

void freeItem(DataItem* item){;
	free(item->car);
	free(item);
}

void freeTable(HashTable* table){
	int i;
	for(i = 0; i < table->size; i++){
		DataItem *item = table->items[i];
		if(item != NULL)
			freeItem(item);
	}
	free(table->items);
	free(table);
}  

void rehashedTableInsert(HashTable **ht, car *cars, int n, int capacity) {
    int newCapacity = SIZE;
    int counter = 1;
    int i, j;

    // We count how many times we need to rehash
    while (newCapacity < n) {
        newCapacity = nextPrimeNumber(newCapacity * 4);
        counter++;
    }

    for (j = 0; j < counter; j++) {
        // Create a new table and transfer data if rehashing is needed
        if ((*ht)->counter > (0.9 * capacity)) {
            HashTable *newTable = createTable(newCapacity);

            // Transfer existing data to the new table
            for (i = 0; i < (*ht)->size; i++) {
                DataItem *item = (*ht)->items[i];
                if (item != NULL) {
                    hashTableInsert(newTable, item->key, cars, newCapacity);
                }
            }

            // Free the old table and update the pointer to the new table
            freeTable(*ht);
            *ht = newTable;
            capacity = newCapacity; // Update capacity for the next rehashing, if needed
        }

        // Insert elements from cars into the current table
        for (i = 0; i < n; i++) {
            hashTableInsert(*ht, cars[i].key - 1, cars, capacity);
        }
    }
}



