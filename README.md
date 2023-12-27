# Car Management System
This program comprises two data structures: an AVL tree and a hash table, to manage a collection of cars. The system enables users to load car information from a file, modify car details, delete cars, and observe the state of the data structures. I just wanted to play with some data structures and learn how do they work.

## Compilation
Compile the program using the provided source code:
```
gcc main.c -o car_management_system
```
## Usage
Run the compiled executable:
```
./car_management_system
```

The system will prompt you with options to choose from:

1. AVL Tree
* Load car information into an AVL tree.
* Allow modification of car masses by their ordinal number.
* Save the updated tree into a file (cars_2.txt).
2. Hash Table
* Load car information into a hash table using rehashed insertion.
* Enable deletion of cars by their index.
* Display the state of the hash table before and after deletions.
3. Exit
* Terminate the program.
## Input file
The input file contains car details in the following format:
> <number_of_cars>
> <car_key> <car_name> <car_mass> <car_country>
> ...
> Example:
> 3
> 1 Toyota 1200 Japan
> 2 Honda 1500 USA
> 3 Ford 1400 USA
## Output File (cars_2.txt)
The output file will store the AVL tree in an in-order traversal format:
> <car_key> <car_name> <car_mass> <car_country> <tree_height>
> ...
> Example:
> 1 Toyota 1200 Japan 2
> 2 Honda 1500 USA 1
> 3 Ford 1400 USA 1
## Functionality Overview
### AVL Tree
* Load: Load cars into an AVL tree and modify car masses.
* Save: Store the updated tree into a file (cars_2.txt).
### Hash Table
* Load: Load cars into a hash table using rehashed insertion.
* Delete: Remove cars from the hash table by their index.
* Display: Show the state of the hash table before and after deletions.

