#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <Windows.h>

bool glasnaya(char c)
{
    char vowels[] = "аеёиоуыэюяАЕЁИОУЫЭЮЯ";
    return strchr(vowels, c) != NULL;
}

bool soglasnaya(char c)
{
    return !glasnaya(c) && (c >= 'а' && c <= 'я' || c >= 'А' && c <= 'Я');
}

void slogi(const char* word, char* result)
{
    int len = strlen(word);
    int pos = 0;
    for (int i = 0; i < len; i++)
    {
        result[pos++] = word[i];
        if (glasnaya(word[i]))
        {
            if (i + 1 < len && soglasnaya(word[i + 1]))
            {
                if (i + 2 < len && soglasnaya(word[i + 2]) && !glasnaya(word[i + 2]))
                {
                    result[pos++] = '-';
                }
                else if (i + 2 < len && glasnaya(word[i + 2]))
                {
                    result[pos++] = '-';
                }
            }
        }
    }
    result[pos] = '\0';
}

void process_text(const char* input, char* output)
{
    char word[100];
    char syllables[200];
    int i = 0, j = 0;
    output[0] = '\0';
    while (input[i] != '\0')
    {
        if (isspace((unsigned char)input[i]) || ispunct((unsigned char)input[i]))
        {
            if (j > 0)
            {
                word[j] = '\0';
                slogi(word, syllables);
                strcat(output, syllables);
                j = 0;
            }
            strncat(output, &input[i], 1);
        }
        else
        {
            word[j++] = input[i];
        }
        i++;
    }
    if (j > 0)
    {
        word[j] = '\0';
        slogi(word, syllables);
        strcat(output, syllables);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char input[1000];
    char output[2000];
    printf("Введите текст:\n");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    process_text(input, output);
    printf("Результат:\n%s\n", output);
    return 0;
}
