#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct room {
    char name[50];   // name
    int level;       // difficulty
    int number;      // number
    int res;         // size
} Room;

// Top - doubly
// bottom singly
typedef struct Node {
    Room *data;
    struct Node *next;   // right 
    struct Node *prev;   // left 
    struct Node *down;   // bottom node
} Node;

Node *S = NULL;   // start

// Utils
int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

// name = pre + suf
Room *create_random_room(int number) {
    Room *r = malloc(sizeof(Room));
    if (!r) { perror("malloc room"); exit(1); }
    const char *pre[] = {"Dark","Golden","Cursed","Hidden","Ancient","Frozen","Burning","Silent"};
    const char *suf[] = {"Hall","Chamber","Vault","Dungeon","Crypt","Lair","Sanctuary","Passage"};
    snprintf(r->name, sizeof(r->name), "%s %s", pre[rand_range(0,7)], suf[rand_range(0,7)]);
    r->level = rand_range(1, 10);
    r->number = number;
    r->res = rand_range(10, 100);
    return r;
}

Node *create_node(Room *data) {
    Node *n = malloc(sizeof(Node));
    n->data = data;
    n->next = n->prev = n->down = NULL;
    return n;
}

// doubly=1 -> doubly link(top)
// doubly=0 -> singly link(btm)
Node *build_row(int len, int number_offset, int doubly) {
    Node *head = NULL, *prev = NULL;
    for (int i = 0; i < len; i++) {
        Node *node = create_node(create_random_room(number_offset + i + 1));
        if (prev) {
            prev->next = node;
            if (doubly) node->prev = prev;
        }
        if (!head) head = node;
        prev = node;
    }
    return head;
}

void connect_rows(Node *top, Node *btm) {
    while (top && btm) {
        top->down = btm;
        top = top->next;
        btm = btm->next;
    }
}

void draw_room(Node *node) {
    Room *r = node->data;

    // scale
    int inner_w = 18 + (r->res - 10) * 34 / 90;
    int inner_h = 4  + (r->res - 10) * 8  / 90;
    if (inner_w < 18) inner_w = 18;
    if (inner_h < 4)  inner_h = 4;

    int total_w = inner_w + 2; // +walls

    // top border + num + lvl
    char header[64];
    snprintf(header, sizeof(header), "#%-2d Lv.%d", r->number, r->level);
    int hlen = strlen(header);

    printf("+- %s ", header);
    int dashes = total_w - 2 - 2 - hlen - 2; // +- <header> <dashes> -+
    if (dashes < 1) dashes = 1;
    for (int i = 0; i < dashes; i++) printf("-");
    printf("-+\n");

    // rows
    // name and size lines
    char name_line[64];
    snprintf(name_line, sizeof(name_line), "%s", r->name);
    char size_line[32];
    snprintf(size_line, sizeof(size_line), "[%d x %d]", r->res, r->res);

    int name_row = inner_h / 2 - 1;   // row name 
    int size_row = inner_h / 2;       // row size 

    for (int row = 0; row < inner_h; row++) {
        // left wall and '<'
        if (row == inner_h / 2 && node->prev)
            printf("<");
        else
            printf("|");

        // content
        if (row == name_row) {
            // name is center
            int textlen = (int)strlen(name_line);
            int pad_left  = (inner_w - textlen) / 2;
            int pad_right = inner_w - textlen - pad_left;
            for (int i = 0; i < pad_left;  i++) printf(" ");
            printf("%s", name_line);
            for (int i = 0; i < pad_right; i++) printf(" ");
        } else if (row == size_row) {
            // size in center 
            int textlen = (int)strlen(size_line);
            int pad_left  = (inner_w - textlen) / 2;
            int pad_right = inner_w - textlen - pad_left;
            for (int i = 0; i < pad_left;  i++) printf(" ");
            printf("%s", size_line);
            for (int i = 0; i < pad_right; i++) printf(" ");
        } else {
            for (int i = 0; i < inner_w; i++) printf(" ");
        }

        // right wall, '>'
        if (row == inner_h / 2 && node->next)
            printf(">\n");
        else
            printf("|\n");
    }

    // bottom border, 'v'
    printf("+");
    int left_dashes  = (total_w - 2) / 2;
    int right_dashes = (total_w - 2) - left_dashes - 1;
    for (int i = 0; i < left_dashes; i++)  printf("-");
    if (node->down)
        printf("v");
    else
        printf("-");
    for (int i = 0; i < right_dashes; i++) printf("-");
    printf("+\n");
}

// ===== navigation =====
void navigation() {
    Node *cur = S;
    int in_bottom = 0;   // 0 = top, 1 = bottom
    char cmd;

    printf("\n+===================================================+\n");
    printf("|               DUNGEON NAVIGATION                  |\n");
    printf("|  D/6 - right                  A/4 - left          |\n");
    printf("|  S/2 - down                   W/8 - up (N/A)      |\n");
    printf("|  R   - return to start (S)    Q - quit            |\n");
    printf("+===================================================+\n");

    printf("\nStart (S) - top row, first node:\n");
    draw_room(cur);

    while (1) {
        printf("\nEnter command: ");
        do { cmd = getchar(); } while (cmd == '\n' || cmd == ' ');

        if (cmd == 'Q' || cmd == 'q') {
            printf("Exiting.\n");
            break;

        } else if (cmd == 'D' || cmd == 'd' || cmd == '6') {
            if (cur->next) {
                cur = cur->next;
                printf("-> Moved right\n");
                draw_room(cur);
            } else {
                printf("! End of this row. Return to start? (Y/N): ");
                char ans;
                do { ans = getchar(); } while (ans == '\n' || ans == ' ');
                if (ans == 'Y' || ans == 'y') {
                    cur = S;
                    in_bottom = 0;
                    printf("Returned to start\n");
                    draw_room(cur);
                }
            }

        } else if (cmd == 'A' || cmd == 'a' || cmd == '4') {
            if (cur->prev) {
                cur = cur->prev;
                printf("<- Moved left\n");
                draw_room(cur);
            } else {
                printf("! Left node reached, cannot move left.\n");
            }

        } else if (cmd == 'S' || cmd == 's' || cmd == '2') {
            if (in_bottom) {
                printf("! Already in bottom row, cannot move down.\n");
            } else if (cur->down) {
                cur = cur->down;
                in_bottom = 1;
                printf("v Moved down\n");
                draw_room(cur);
            } else {
                printf("! No bottom node here.\n");
            }

        } else if (cmd == 'W' || cmd == 'w' || cmd == '8') {
            printf("! No upper node here.\n");

        } else if (cmd == 'R' || cmd == 'r') {
            cur = S;
            in_bottom = 0;
            printf("Returned to start\n");
            draw_room(cur);

        } else {
            printf("Unknown command. Use D/A/S/W, 6/4/2/8, R or Q.\n");
        }
    }
}

// Free
void free_row(Node *head) {
    while (head) {
        Node *next = head->next;
        free(head->data);
        free(head);
        head = next;
    }
}

// ==========
int main() {
    srand(time(NULL));

    int n = 0, k = 0;

    printf("Enter N - rooms in top row (N >= 1): ");
    scanf("%d", &n);

    printf("Enter K - rooms in bottom row (K >= 1): ");
    scanf("%d", &k);
    while (getchar() != '\n');

    if (n < 1 || k < 1) {
        printf("N and K must be >= 1.\n");
        return 1;
    }

    printf("\nTop row: %d node(s)   bottom row: %d node(s)\n", n, k);

    Node *top_row = build_row(n, 0, 1);   // doubly linked
    Node *btm_row = build_row(k, n, 0);   // singly linked (prev=NULL)

    connect_rows(top_row, btm_row);

    S = top_row;

    navigation();

    free_row(top_row);
    free_row(btm_row);
    return 0;
}