#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NUM 100              // Requirement - MAX_file_number     : 100
#define NUM_BLOCKS 1024               // Requirement - MAX_num_block       : 1024
#define BLOCK_SIZE 32                 // Requirement - block_size(byte)    : 32
#define MAX_FILE_NAME 100             // Requirement - MAX_file_name       : 100
#define FS_STAT "fs_state.dat"        // DISK FILE

// File Entry
typedef struct {
    char filename[MAX_FILE_NAME];
    int start_block;
    int size;
} FileEnt;

// File system structure
typedef struct {
    int fat_table[NUM_BLOCKS];        // FAT Table
    FileEnt directory[MAX_FILE_NUM]; // FAT Table
    char data_area[NUM_BLOCKS * BLOCK_SIZE]; // 1024blocks * 32byte
} FileSystem;

FileSystem myfat;                    // File System Instance

/*===================================================================*/
/*                             CONTROL API                           */
/*===================================================================*/
int create_file(const char *filename);          // Create & allocate block
int write_file(const char *filename, const char *data); // Write data to file & link blocks in the FAT table
int read_file(const char *filename);            // Read data from file, follow linked blocks
int delete_file(const char *filename);          // Delete the file, release blocks in the FAT table
void list_files(void);                          // Display a list of all files in the system.
/* End of API */

void save_file_system(void);                    // Save the File System to Disk
void load_file_system(void);                    // Restore the File System from Disk

// Save the File System to Disk
void save_file_system(void) {
    FILE *f = fopen(FS_STAT, "wb");
    if (f == NULL) {
        printf("Error: Could not save file system state.\n");
        return;
    }
    fwrite(&myfat, sizeof(FileSystem), 1, f);
    fclose(f);
}

// Restore the File System from Disk
void load_file_system(void) {
    FILE *f = fopen(FS_STAT, "rb");
    if (f == NULL) {
        printf("Warning: No saved state found. Starting fresh.\n");
        memset(&myfat, 0, sizeof(FileSystem));
        return;
    }
    fread(&myfat, sizeof(FileSystem), 1, f);
    fclose(f);
}

// File Creation
int create_file(const char *filename) {
    // check file name
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (strcmp(myfat.directory[i].filename, filename) == 0) {
            printf("File '%s' already exists.\n", filename);
            return -1;
        }
    }

    // allocate new file
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (myfat.directory[i].filename[0] == '\0') {
            for (int j = 1; j < NUM_BLOCKS; j++) {
                if (myfat.fat_table[j] == 0) {
                    myfat.fat_table[j] = -1;

                    strcpy(myfat.directory[i].filename, filename);
                    myfat.directory[i].start_block = j;
                    myfat.directory[i].size = 0;
                    printf("File '%s' created.\n", filename);
                    return 0;
                }
            }
        }
    }
    printf("File system full. Cannot create more files.\n");
    return -1;
}

// File Write
int write_file(const char *filename, const char *data) {
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (strcmp(myfat.directory[i].filename, filename) == 0) {
            int start_block = myfat.directory[i].start_block;
            int current_block = start_block;
            int total_size = myfat.directory[i].size;

            int remaining_data = strlen(data);
            int data_offset = 0;

            if (start_block == -1) {
                // File Not Initialized
                printf("File '%s' not initialized. Use create command first.\n", filename);
                return -1;
            }

            while(myfat.fat_table[current_block] != -1)
                current_block = myfat.fat_table[current_block];

            int block_end = total_size % BLOCK_SIZE;

            // Check the data size
            if (remaining_data <= BLOCK_SIZE - block_end) {
                strncpy(&myfat.data_area[current_block * BLOCK_SIZE + block_end], &data[data_offset], remaining_data);
            } else {
                int read_size = BLOCK_SIZE - block_end;
                strncpy(&myfat.data_area[current_block * BLOCK_SIZE + block_end], &data[data_offset], read_size);

                block_end = 0;
                data_offset += read_size;
                remaining_data -= read_size;

                while(remaining_data > 0)
                {
                    int j;
                    int found = 0;
                    for(j = 1; j < NUM_BLOCKS; j++)
                    {
                        if(myfat.fat_table[j] == 0){
                            found = 1;
                            break;
                        }
                    }
                    if(!found){
                        printf("there's no more block!");
                        return -1;
                    }
                    myfat.fat_table[current_block] = j;
                    myfat.fat_table[j] = -1;
                    current_block = j;


                    int read_size = (remaining_data < BLOCK_SIZE) ? remaining_data : BLOCK_SIZE;
                    strncpy(&myfat.data_area[current_block * BLOCK_SIZE + block_end], &data[data_offset], read_size);
                    data_offset += read_size;
                    remaining_data -= read_size;
                }
            }

            myfat.directory[i].size += strlen(data);
            printf("Data written to '%s'.\n", filename);
            return 0;
        }
    }
    printf("File '%s' not found.\n", filename);
    return -1;
}

// Read Files
int read_file(const char *filename) {
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (strcmp(myfat.directory[i].filename, filename) == 0) {
            int start_block = myfat.directory[i].start_block;
            int block = start_block;
            int total_size = myfat.directory[i].size;
            int bytes_read = 0;

            if (start_block == -1) {
                printf("File '%s' not initialized. Use create command first.\n", filename);
                return -1;
            }

            printf("Content of '%s': ", filename);

            while(bytes_read < total_size)
            {
                int remaining_bytes_in_block = total_size - bytes_read;
                int read_size = (remaining_bytes_in_block < BLOCK_SIZE) ? remaining_bytes_in_block : BLOCK_SIZE;
                printf("%.*s", read_size, &myfat.data_area[block * BLOCK_SIZE]);
                
                block = myfat.fat_table[block];
                bytes_read += read_size;
            }
            

            printf("\n");
            return 0;
        }
    }

    printf("File '%s' not found.\n", filename);
    return -1;
}

// File Deletion
int delete_file(const char *filename) {
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (strcmp(myfat.directory[i].filename, filename) == 0) {
            int start_block = myfat.directory[i].start_block;
            int block = start_block;

            // Release Cluster at FAT Table
            while (block != -1) {
                int next_block = myfat.fat_table[block];
                myfat.fat_table[block] = 0; // Release Cluster
                block = next_block;
            }

            // Remove file at Directory
            myfat.directory[i].filename[0] = '\0';
            printf("File '%s' deleted.\n", filename);

            return 0;
        }
    }
    printf("File '%s' not found.\n", filename);
    return -1;
}

// Display a list of all files in the system.
void list_files() {
    printf("Files in the file system:\n");
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (myfat.directory[i].filename[0] != '\0') {
            printf("File: %s, Size: %d bytes\n", myfat.directory[i].filename, myfat.directory[i].size);
        }
    }
}

void execute_cmd(char *cmd, char *filename, char *data, int num) {
    if (strcmp(cmd, "create") == 0) {
        if (num != 3)
            printf("Usage: create <filename>\n");
        else
            create_file(filename);
    } else if (strcmp(cmd, "write") == 0) {
        if (num != 4)
            printf("Usage: write <filename> <data>\n");
        else
            write_file(filename, data);
    } else if (strcmp(cmd, "read") == 0) {
        if (num != 3)
            printf("Usage: read <filename>\n");
        else
            read_file(filename);
    } else if (strcmp(cmd, "delete") == 0) {
        if (num != 3)
            printf("Usage: delete <filename>\n");
        else
            delete_file(filename);
    } else if (strcmp(cmd, "list") == 0) {
        list_files();
    } else {
        printf("Invalid command.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("USAGE: ./fat <COMMAND> [ARGS]...\n");
        exit(1);
    }

    load_file_system();                             // Restore File System from disk
    execute_cmd(argv[1], argv[2], argv[3], argc);   // Execute command
    save_file_system();                             // Save File System to Disk
    exit(0);
}
