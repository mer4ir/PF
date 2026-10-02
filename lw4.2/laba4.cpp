#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NAME_LEN 50

typedef struct 
{
    char lastName[MAX_NAME_LEN];
    char firstName[MAX_NAME_LEN];
    char middleName[MAX_NAME_LEN];
    int groupNumber;
    char language[MAX_NAME_LEN];
} Student;

Student* students = NULL;
int studentCount = 0;
int currentRecord = 0; 

void loadFromFile(const char* fileName) 
{
    FILE* file = fopen(fileName, "r");
    if (!file) 
    {
        printf("Ошибка открытия файла %s для чтения.\n", fileName);
        return;
    }

    studentCount = 0;
    students = (Student*)malloc(10 * sizeof(Student));
    if (!students) 
    {
        printf("Ошибка выделения памяти!\n");
        fclose(file);
        return;
    }

    int capacity = 10;
    while (fscanf(file, "%s %s %s %d %s", 
                  students[studentCount].lastName, 
                  students[studentCount].firstName, 
                  students[studentCount].middleName, 
                  &students[studentCount].groupNumber, 
                  students[studentCount].language) == 5) 
                  {
        studentCount++;
        if (studentCount >= capacity) 
        {
            capacity *= 2;
            Student* temp = (Student*)realloc(students, capacity * sizeof(Student));
            if (!temp) 
            {
                printf("Ошибка увеличения памяти!\n");
                free(students);
                fclose(file);
                return;
            }
            students = temp;
        }
    }
    fclose(file);
    printf("Данные успешно загружены. Загружено записей: %d\n", studentCount);
}

void saveToFile(const char* fileName, Student* students, int studentCount) 
{
    FILE* file = fopen(fileName, "w");
    if (!file) 
    {
        printf("Ошибка открытия файла %s для записи.\n", fileName);
        return;
    }

    for (int i = 0; i < studentCount; i++) 
    {
        fprintf(file, "%s %s %s %d %s\n",
                students[i].lastName, students[i].firstName, students[i].middleName,
                students[i].groupNumber, students[i].language);
    }
    fclose(file);
    printf("Данные успешно сохранены в файл %s.\n", fileName);
}

void sortStudents(Student* arr, int count) 
{
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = i + 1; j < count; j++) 
        {
            if (strcmp(arr[i].lastName, arr[j].lastName) > 0 || 
                (strcmp(arr[i].lastName, arr[j].lastName) == 0 && strcmp(arr[i].firstName, arr[j].firstName) > 0) ||
                (strcmp(arr[i].lastName, arr[j].lastName) == 0 && strcmp(arr[i].firstName, arr[j].firstName) == 0 && strcmp(arr[i].middleName, arr[j].middleName) > 0)) 
                {
                Student temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void navigateRecords() 
{
    char command;
    while (1) {
        printf("\nТекущая запись [%d из %d]:\n", currentRecord + 1, studentCount);
        printf("%s %s %s, группа %d, язык: %s\n", 
               students[currentRecord].lastName, 
               students[currentRecord].firstName, 
               students[currentRecord].middleName, 
               students[currentRecord].groupNumber, 
               students[currentRecord].language);
        printf("Команды: [n] - следующая, [m] - предыдущая, [q] - выход: ");
        scanf(" %c", &command);

        if (command == 'n') 
        {
            if (currentRecord < studentCount - 1) currentRecord++;
            else printf("Это последняя запись.\n");
        } else if (command == 'm') 
        {
            if (currentRecord > 0) currentRecord--;
            else printf("Это первая запись.\n");
        } else if (command == 'q') 
        {
            break;
        } else 
        {
            printf("Неверная команда.\n");
        }
    }
}

void addRecord() 
{
    Student newStudent;
    printf("Введите данные нового студента (Фамилия Имя Отчество Группа Язык): ");
    scanf("%s %s %s %d %s", 
          newStudent.lastName, newStudent.firstName, newStudent.middleName, 
          &newStudent.groupNumber, newStudent.language);

    Student* temp = (Student*)realloc(students, (studentCount + 1) * sizeof(Student));
    if (!temp) 
    {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    students = temp;
    students[studentCount] = newStudent;
    studentCount++;
    printf("Запись успешно добавлена.\n");
}

void editRecord() 
{
    printf("Введите номер записи для редактирования (1 - %d): ", studentCount);
    int recordIndex;
    scanf("%d", &recordIndex);
    if (recordIndex < 1 || recordIndex > studentCount) 
    {
        printf("Неверный номер записи.\n");
        return;
    }
    recordIndex--;

    printf("Текущие данные: %s %s %s, группа %d, язык: %s\n",
           students[recordIndex].lastName, students[recordIndex].firstName,
           students[recordIndex].middleName, students[recordIndex].groupNumber,
           students[recordIndex].language);

    printf("Введите новые данные (Фамилия Имя Отчество Группа Язык): ");
    scanf("%s %s %s %d %s", 
          students[recordIndex].lastName, students[recordIndex].firstName,
          students[recordIndex].middleName, &students[recordIndex].groupNumber,
          students[recordIndex].language);

    printf("Запись успешно обновлена.\n");
}

void deleteRecord() 
{
    printf("Введите номер записи для удаления (1 - %d): ", studentCount);
    int recordIndex;
    scanf("%d", &recordIndex);
    if (recordIndex < 1 || recordIndex > studentCount) 
    {
        printf("Неверный номер записи.\n");
        return;
    }
    recordIndex--;

    for (int i = recordIndex; i < studentCount - 1; i++) 
    {
        students[i] = students[i + 1];
    }
    studentCount--;

    Student* temp = (Student*)realloc(students, studentCount * sizeof(Student));
    if (!temp && studentCount > 0) 
    {
        printf("Ошибка уменьшения памяти!\n");
        return;
    }
    students = temp;
    printf("Запись успешно удалена.\n");
}

void processRecords() 
{
    Student* englishStudents = NULL;
    Student* frenchStudents = NULL;
    Student* germanStudents = NULL;
    int englishCount = 0, frenchCount = 0, germanCount = 0;

    for (int i = 0; i < studentCount; i++) 
    {
        if (strcmp(students[i].language, "English") == 0) 
        {
            englishStudents = (Student*)realloc(englishStudents, (englishCount + 1) * sizeof(Student));
            englishStudents[englishCount++] = students[i];
        } else if (strcmp(students[i].language, "French") == 0) 
        {
            frenchStudents = (Student*)realloc(frenchStudents, (frenchCount + 1) * sizeof(Student));
            frenchStudents[frenchCount++] = students[i];
        } else if (strcmp(students[i].language, "German") == 0) 
        {
            germanStudents = (Student*)realloc(germanStudents, (germanCount + 1) * sizeof(Student));
            germanStudents[germanCount++] = students[i];
        }
    }

    sortStudents(englishStudents, englishCount);
    sortStudents(frenchStudents, frenchCount);
    sortStudents(germanStudents, germanCount);

    saveToFile("English.txt", englishStudents, englishCount);
    saveToFile("French.txt", frenchStudents, frenchCount);
    saveToFile("German.txt", germanStudents, germanCount);

    free(englishStudents);
    free(frenchStudents);
    free(germanStudents);
}

int main() 
{
    setlocale(LC_ALL, "");

    int choice;
    while (1) 
    {
        printf("\nМеню:\n");
        printf("1. Загрузить данные из файла\n");
        printf("2. Сохранить данные в файл\n");
        printf("3. Навигация по записям\n");
        printf("4. Добавить запись\n");
        printf("5. Редактировать запись\n");
        printf("6. Удалить запись\n");
        printf("7. Обработать данные\n");
        printf("8. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1: loadFromFile("students.txt"); break;
            case 2: saveToFile("students.txt", students, studentCount); break;
            case 3: navigateRecords(); break;
            case 4: addRecord(); break;
            case 5: editRecord(); break;
            case 6: deleteRecord(); break;
            case 7: processRecords(); break;
            case 8: 
                free(students); 
                return 0;
            default: printf("Неверный выбор!\n");
        }
    }
}