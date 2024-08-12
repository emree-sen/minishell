#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mini.h"

extern char **environ;

void ft_strcpy(char *dest, const char *src) {
	while (*src) {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}


void ft_strcat(char *dest, const char *src) {
	while (*dest) {
		dest++;
	}
	ft_strcpy(dest, src);
}


char *get_command_path(char *command) {
    char *path = getenv("PATH");  // PATH çevre değişkenini al
    char full_path[1024]; // Tam yolu tutacak dizi
    char *dir;

    while ((dir = strsep(&path, ":")) != NULL) {
        // Tam yolu oluştur (örneğin: /bin/ls)
        ft_strcpy(full_path, dir);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, command);

        // Eğer dosya mevcutsa ve çalıştırılabilirse, tam yolu döndür
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path);
        }
    }

    return NULL;
}

void execute_command(char *command, char *args[]) {
    char *command_path = get_command_path(command);

    if (command_path) {
        if (execve(command_path, args, environ) == -1) {
            perror("execve failed");
        }
    } else {
        printf("command not found: %s\n", command);
    }
}

int main() {
    char *input;
    char *args[10];  // Maksimum 10 argüman için yer ayırdık

    while (1) {
        // Kullanıcıdan komut al
        input = readline("minishell> ");
        if (!input) {
            break;  // Kullanıcı Ctrl+D ile çıkarsa programdan çık
        }

        // Boş komut girildiyse devam et
        if (ft_strlen(input) == 0) {
            free(input);
            continue;
        }

        // Komut geçmişine ekle
        add_history(input);

        // Komutları ve argümanları ayır
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Child process oluştur
        pid_t pid = fork();

        if (pid == 0) {
            // Child process: Komutu çalıştır
            execute_command(args[0], args);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            // fork başarısız oldu
            perror("fork failed");
        } else {
            // Parent process: Child process'in bitmesini bekle
            wait(NULL);
        }

        free(input);  // Belleği serbest bırak
    }

    return 0;
}
