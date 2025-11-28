**ЛАБОРАТОРНАЯ РАБОТА 9**
GIT
-

<details>
<summary>На 3</summary>

1. Используем git status:
    ```
    On branch main
    Your branch is up to date with 'origin/main'.
    ```
2. Используем git log:
    ```
    commit 8c4c3caef4939cb1651a63cf5f913e7eb41cbbbb (HEAD -> main, origin/main, origin/HEAD)
    Author: Raitar <raitar239@gmail.com>
    Date:   Mon Sep 22 00:09:11 2025 +0700

    Correct error
    ```
3. Создаем [файл](sort.c) с quick сортировкой.
4. Используем git status:
   ```
    Untracked files:
    (use "git add <file>..." to include in what will be committed)
            ./
   ```
5. Добавляем sort.c в область stage:   `git add sort.c`
6. Используем git status: 
   ```
    Changes to be committed:
    (use "git restore --staged <file>..." to unstage)
        new file: sort.c
   ```
7. Коммитим файл: `git commit -m "Create sort.c"`
8. Используем git status (*не отслеживаю readme*): 
   ```
    Untracked files:
    (use "git add <file>..." to include in what will be committed)
            README.md   
    ```
9.  Добавляем комментарий в файл: `// Quick sort numbers`
10. Используем git status: 
    ```
    Changes not staged for commit:
    (use "git add <file>..." to update what will be committed)
    (use "git restore <file>..." to discard changes in working directory)
            modified:   sort.c
    ```
11. Добавляем файл: `git add sort.c`
12. Используем git status: 
    ```
    Changes to be committed:
    (use "git restore --staged <file>..." to unstage)
            modified:   sort.c   
    ```
13. Меняем комментарий: `// Quick sort`
14. Коммитим: `git commit`
15. git status:
    ```
    Changes not staged for commit:
    (use "git add <file>..." to update what will be committed)
    (use "git restore <file>..." to discard changes in working directory)
            modified:   sort.c
    ```
    git log:
    ```
    commit a370e82b495815d6dba3b6d4e329574a88f0abc6 (HEAD -> main, origin/main, origin/HEAD)
    Author: Raitar <raitar239@gmail.com>
    Date:   Fri Nov 21 12:43:42 2025 +0700
        Change comm
    ```
16. Добавляем в stage и коммитим:
     ```
    git add sort.c
    git commit -m "Change comment"
     ```
17. Пушим на githab (*пушил каждый commit*): `git push`
</details>

---    

<details>
<summary>На 3 с ветками</summary>

1. Создаем новую ветку: `git branch mybranch`
2. Смотрим ветки: 
   ```
   * main
     mybranch
   ```
3. Переключаемся на новую ветку: `Switched to branch 'mybranch'`
4. git status: `On branch mybranch`
5. Убедились что находимся на своей ветке
6. Создаем файл [file1.txt](file1.txt)
7. Add and commit
8. Используем `git log --oneline --graph`:
   ```
   * 15e65c0 (HEAD -> mybranch) Create file1.txt
   * 90e4e27 (HEAD -> main, origin/main, origin/HEAD) Commit README.md
   ...
   ```
9. Переходим к main: `git switch main`
10. Используем `git log --oneline --graph`:
    ```
    * 90e4e27 (HEAD -> main, origin/main, origin/HEAD) Commit README.md
    ...
    ```
11. Create, Add, Commit
12. Используем `git log --oneline --graph --all`:
    ```
    * f6bb720 (HEAD -> main) Create file2.txt
    | * 15e65c0 (mybranch) Create file1.txt
    |/
    * 90e4e27 (origin/main, origin/HEAD) Commit README.md
    ```
13. Переключаемся на mybranch: `git switch mybranch`
14. Файл file1.txt пропал
15. Смотрим разницу между ветками `git diff mybranch main`:
    ```
    diff --git a/lab9/file1.txt b/lab9/file2.txt
    similarity index 100%
    rename from lab9/file1.txt
    rename to lab9/file2.txt
    ```
16. Файл `README.md` в ветке mybranch 
17. Коммитим, пушим
</details>

---

<details>
<summary> На 4</summary>

1. Переключаемся на mybranch: `git switch`
2. Пишем функцию main:
   ```c
    int main() {
        int a[] = {4, 2, 0, 7, 1, 9, 3};
        int n = sizeof(a) / sizeof(a[0]);
        
        printf("Исходный массив: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
        
        quickSort(a, 0, n - 1);
        
        printf("Отсортированный массив: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
        
        return 0;
    }
   ```
3. git diff: 
    ```git
    diff --git a/lab9/sort.c b/lab9/sort.c
    index 6cf98da..afc026b 100644
    --- a/lab9/sort.c
    +++ b/lab9/sort.c
   ```
4. git diff --staged пустой
5. git add sort.c
6. git diff sort.c пустой
7. git diff --staged:
    ```git
    diff --git a/lab9/sort.c b/lab9/sort.c
    index 6cf98da..afc026b 100644
    --- a/lab9/sort.c
    +++ b/lab9/sort.c
    ```
8. Удалил число из массива
9. git diff:
    ```git
    int main() {
    -    int a[] = {4, 2, 0, 7, 1, 9, 3};
    +    int a[] = {4, 0, 7, 1, 9, 3};
        int n = sizeof(a) / sizeof(a[0]);
    ```
10. git diff --staged:
    ```git
    diff --git a/lab9/sort.c b/lab9/sort.c
    index 6cf98da..afc026b 100644
    --- a/lab9/sort.c
    +++ b/lab9/sort.c
    ```
11. При изменении в файле меняется git diff. Если добавить изменения в stage, то они появляются в git diff --staged и пропадают из diff.
12. git status:
    ```git
    Changes to be committed:
    (use "git restore --staged <file>..." to unstage)
            modified:   lab9/sort.c

    Changes not staged for commit:
    (use "git add <file>..." to update what will be committed)
    (use "git restore <file>..." to discard changes in working directory)
            modified:   lab9/README.md
            modified:   lab9/sort.c
    ```
13. Отменяем индексацию изменения: `git restore --staged sort.c`
14. git status:
    ```git
    Changes not staged for commit:
    (use "git add <file>..." to update what will be committed)
    (use "git restore <file>..." to discard changes in working directory)
            modified:   lab9/README.md
            modified:   lab9/sort.c
    ```
15. Add & Commit
16. git log:
    ```git
    commit a0c2b398ef87b36490ad49868c02907af2c01079 (HEAD -> mybranch)
    Author: Raitar <raitar239@gmail.com>
    Date:   Mon Nov 24 10:47:34 2025 +0700

        Добавлена функция main
    ```
17. Добавляем `printf("hello git\n");`

18. <details>
    <summary>sort.c</summary>

    ```c
    // Quick sort 
    #include <stdio.h>

    void quickSort(int arr[], int low, int high) {
        if (low < high) {
            int pivot = arr[high];
            int i = low - 1;
            
            for (int j = low; j < high; j++) {
                if (arr[j] < pivot) {
                    i++;
                    int temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
            
            int temp = arr[i + 1];
            arr[i + 1] = arr[high];
            arr[high] = temp;
            
            int pi = i + 1;
            
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    int main() {
        printf("hello git\n");

        int a[] = {4, 0, 7, 1, 9, 3};
        int n = sizeof(a) / sizeof(a[0]);
        
        printf("Исходный массив: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
        
        quickSort(a, 0, n - 1);
        
        printf("Отсортированный массив: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
        
        return 0;
    }
    ```
    </details>
19. git status:
    ```git
    Changes not staged for commit:
    (use "git add <file>..." to update what will be committed)
    (use "git restore <file>..." to discard changes in working directory)
            modified:   lab9/README.md
            modified:   lab9/sort.c
    ```
20. git restore sort.c
21. В файле sort.c пропал printf
22. git status:
    ```git
    Changes not staged for commit:
    (use "git add <file>..." to update what will be committed)
    (use "git restore <file>..." to discard changes in working directory)
            modified:   README.md
    ```
23. Commit & push
</details>

---

<summary>На 4*</summary>

1-2. Create, add, commit.
3. `git branch feature/uppercase`
4. `git switch feature/uppercase`
5. Status: `no changes added to commit (use "git add" and/or "git commit -a")`
6-7. Change, add, commit
8. `git branch`:
```git
* feature/uppercase
  main
  mybranch
```
9. `git log --oneline --graph –all`:
```git
* 120e6cc (HEAD -> feature/uppercase) Change to HELLO
* 92505a8 (mybranch) Add content to greeting.txt
* f4b3552 Add file greeting.txt
```
10-11.  Switch, `type greetings.txt`: hello
12. HELLO/hello
13. 