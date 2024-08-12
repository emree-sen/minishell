#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    while (1) {
        // Kullanıcıdan girdi almak için readline çağrısı
        input = readline("minishell> ");
        
        // Girdi boş değilse ve sadece Enter'a basılmamışsa, girdi ekle
        if (input && *input) {
            add_history(input);
            printf("Kullanıcı girişi: %s\n", input);
        }
        
        // Bellek sızıntısını önlemek için girdiyi serbest bırak
        free(input);
    }

    return 0;
}

