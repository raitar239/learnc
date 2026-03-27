#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()

#define MAX_NAME  64
#define MAX_LEVEL 16

typedef struct {
    char name[MAX_NAME];
    int  id;
    char level[MAX_LEVEL];
} Employee;

//syscall
static void write_str(const char *s) {
    write(STDOUT_FILENO, s, strlen(s));
}

static void write_err(const char *s) {
    write(STDERR_FILENO, s, strlen(s));
}

// ON 3: Read 
int read_csv(const char *filename, Employee **out_arr, int *out_count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        write_err("Error: failed to open file '");
        write_err(filename);
        write_err("'\n");
        return -1;
    }

    int capacity = 16, count = 0;       // for 16 emp
    Employee *arr = malloc(capacity * sizeof(Employee));
    if (!arr) { close(fd); return -1; }

    char    buf[4096];
    char    line[256];
    int     line_len = 0;
    ssize_t n;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {  //read in parts
        for (ssize_t i = 0; i < n; i++) {           
            char c = buf[i];

            if (c == '\r') continue; 

            if (c != '\n') {
                if (line_len < (int)sizeof(line) - 1)
                    line[line_len++] = c;
                continue;
            }

            line[line_len] = '\0';
            line_len = 0;
            if (line[0] == '\0') continue;

            // strtok
            char *name = line;
            char *sep1 = strchr(name, ';');  // found
            if (!sep1) { write_err("Warning: invalid line skipped.\n"); continue; }
            *sep1 = '\0';                    // cut line
            char *id_s = sep1 + 1;           // id after this
            char *sep2 = strchr(id_s, ';');
            if (!sep2) { write_err("Warning: invalid line skipped.\n"); continue; }
            *sep2 = '\0';
            char *level = sep2 + 1;

            if (count == capacity) {
                capacity *= 2;
                Employee *tmp = realloc(arr, capacity * sizeof(Employee));
                if (!tmp) { free(arr); close(fd); return -1; }
                arr = tmp;
            }

            strncpy(arr[count].name,  name,  MAX_NAME  - 1); // copy n char in stroke
            strncpy(arr[count].level, level, MAX_LEVEL - 1);
            arr[count].name[MAX_NAME   - 1] = '\0';
            arr[count].level[MAX_LEVEL - 1] = '\0';
            arr[count].id = atoi(id_s); // ASCII to int
            count++;
        }
    }

    // last string
    if (line_len > 0) {
        line[line_len] = '\0';
        char *name = line;
        char *sep1 = strchr(name, ';');
        if (sep1) {
            *sep1 = '\0';
            char *id_s = sep1 + 1;
            char *sep2 = strchr(id_s, ';');
            if (sep2) {
                *sep2 = '\0';
                char *level = sep2 + 1;
                if (count == capacity) {
                    capacity *= 2;
                    Employee *tmp = realloc(arr, capacity * sizeof(Employee));
                    if (!tmp) { free(arr); close(fd); return -1; }
                    arr = tmp;
                }
                strncpy(arr[count].name,  name,  MAX_NAME  - 1);
                strncpy(arr[count].level, level, MAX_LEVEL - 1);
                arr[count].name[MAX_NAME   - 1] = '\0';
                arr[count].level[MAX_LEVEL - 1] = '\0';
                arr[count].id = atoi(id_s);
                count++;
            }
        }
    }

    if (n == -1) {
        write_err("Error: read() failed\n");
        free(arr); close(fd); return -1;
    }

    // fclose
    close(fd);
    *out_arr   = arr;
    *out_count = count;
    return 0;
}

// print table "write"
void print_table(const Employee *arr, int count) {
    char row[256];
    const char *sep = "+----------------------+-------+----------+\n";

    write_str("\n");
    write_str(sep);
    snprintf(row, sizeof(row), "| %-20s | %-5s | %-8s |\n", "Name", "ID", "Level");
    write_str(row);
    write_str(sep);
    for (int i = 0; i < count; i++) {
        snprintf(row, sizeof(row), "| %-20s | %-5d | %-8s |\n",
                 arr[i].name, arr[i].id, arr[i].level);
        write_str(row);
    }
    write_str(sep);
    write_str("\n");
}

////////////
// ON 4: DB
int save_database(const char *dbfile, const Employee *arr, int count) {
    FILE *f = fopen(dbfile, "wb"); // write binary, dont text!
    if (!f) {
        fprintf(stderr, "Error: Failed to create '%s'\n", dbfile);
        return -1;
    }
    if (fwrite(&count, sizeof(int), 1, f) != 1 ||        // count emp
        fwrite(arr, sizeof(Employee), count, f) != (size_t)count) {  
        fprintf(stderr, "Database write error\n");
        fclose(f);
        return -1;
    }
    fclose(f);
    printf("Database saved to '%s' (%d entries)\n", dbfile, count);
    return 0;
}

void search_by_id(const char *dbfile) {
    FILE *f = fopen(dbfile, "rb");  // read binary
    if (!f) {
        fprintf(stderr, "Error: Failed to open '%s'\n", dbfile);
        return;
    }
    int count = 0;
    if (fread(&count, sizeof(int), 1, f) != 1) {
        fprintf(stderr, "Database read error\n");
        fclose(f);
        return;
    }
    int target_id;              // type id
    printf("Enter the employee ID to search: ");
    if (scanf("%d", &target_id) != 1) {
        fprintf(stderr, "Invalid input\n");
        fclose(f);
        return;
    }
    Employee emp;
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (fread(&emp, sizeof(Employee), 1, f) != 1) break; // read
        if (emp.id == target_id) {                           // compare
            printf("\n[ The employee has been found ]\n");
            printf("  Name:  %s\n  ID:    %d\n  Level: %s\n\n", emp.name, emp.id, emp.level);
            found = 1;
            break;
        }
    }
    if (!found)
        fprintf(stderr, "Error: Employee with ID=%d not found\n", target_id);
    fclose(f);
}

////////////
// ON 5: RLE           
// replace duplicate bytes to counter + value pair
int compress_rle(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");    // original
    if (!in) { fprintf(stderr, "Opening error '%s'\n", src); return -1; }
    FILE *out = fopen(dst, "wb");   // write rle
    if (!out) { fprintf(stderr, "Creation error '%s'\n", dst); fclose(in); return -1; }

    int ch = fgetc(in);             // first bit
    while (ch != EOF) {
        unsigned char  cur = ch;
        unsigned short cnt = 1;     // cnt 
        while (cnt < 0xFFFF) {
            int next = fgetc(in);
            if (next == EOF || next != cur) {
                if (next != EOF) ungetc(next, in);
                break;
            }
            cnt++;
        }
        if (fwrite(&cnt, sizeof(cnt), 1, out) != 1 ||   // cnt
            fwrite(&cur, sizeof(cur),  1, out) != 1) {  // cur
            fprintf(stderr, "RLE write error\n");
            fclose(in); fclose(out);
            return -1;
        }
        ch = fgetc(in);             // next bit for new
    }
    fclose(in);
    fclose(out);
    return 0;
}

// read couple and turn them back
int decompress_rle(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) { fprintf(stderr, "Opening error '%s'\n", src); return -1; }
    FILE *out = fopen(dst, "wb");
    if (!out) { fprintf(stderr, "Creation error '%s'\n", dst); fclose(in); return -1; }

    unsigned short cnt;
    unsigned char  val;
    while (fread(&cnt, sizeof(cnt), 1, in) == 1 &&
           fread(&val, sizeof(val),  1, in) == 1) {
        for (unsigned short i = 0; i < cnt; i++) {
            if (fputc(val, out) == EOF) {
                fprintf(stderr, "Write error while unpacking\n");
                fclose(in); fclose(out);
                return -1;
            }
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}

long file_size(const char *path) {     // size file
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);      
    long sz = ftell(f);
    fclose(f);
    return sz;
}

void print_compression_report(const char *orig, const char *compressed) {
    long sz_orig = file_size(orig);
    long sz_comp = file_size(compressed);
    if (sz_orig <= 0 || sz_comp <= 0) {
        fprintf(stderr, "File size error\n");
        return;
    }
    double ratio = 100.0 * (1.0 - (double)sz_comp / sz_orig);
    printf("\n Compression Report\n");
    printf("Size before:  %ld bytes (%.2f Kb)\n", sz_orig, sz_orig / 1024.0);
    printf("Size after:   %ld bytes (%.2f Kb)\n", sz_comp, sz_comp / 1024.0);
    printf("Compression:  %.1f%%\n\n", ratio);
}

////////
// MAIN
int main(int argc, char *argv[]) {
    if (argc < 2) {
        write_err("Usage: ");
        write_err(argv[0]);
        write_err(" <staff.csv>\n");
        return 1;
    }

    const char *csv_file = argv[1];
    const char *db_file  = "database.dat";
    const char *rle_file = "database.rle";
    const char *dec_file = "database_restored.dat";

    // 3
    Employee *employees = NULL;
    int count = 0;
    if (read_csv(csv_file, &employees, &count) != 0) return 1;

    char msg[64];
    snprintf(msg, sizeof(msg), "Loaded employees: %d\n", count);
    write_str(msg);
    print_table(employees, count);

    // 4
    if (save_database(db_file, employees, count) != 0) {
        free(employees);
        return 1;
    }
    search_by_id(db_file);

    // 5
    printf("\nCompression '%s' -> '%s'...\n", db_file, rle_file);
    if (compress_rle(db_file, rle_file) == 0)
        printf("Compression completed.\n");
    print_compression_report(db_file, rle_file);

    printf("Unpacking '%s' -> '%s'...\n", rle_file, dec_file);
    if (decompress_rle(rle_file, dec_file) == 0)
        printf("Unpacking completed.\n");

    long sz1 = file_size(db_file);
    long sz2 = file_size(dec_file);
    if (sz1 == sz2)
        printf("Check: files match (%ld bytes) OK\n\n", sz1);
    else
        fprintf(stderr, "Warning: sizes do not match (%ld vs %ld)\n", sz1, sz2);

    free(employees);
    return 0;
}