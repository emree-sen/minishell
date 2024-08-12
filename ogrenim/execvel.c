#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char **environ; // Ortam değişkenlerini kullanmak için

int main() {
    char *input;
    char *args[10]; // Maksimum 10 argüman için yer ayırdık
    char *path = "/bin/"; // Komutların çoğu /bin dizininde yer alır
    char full_path[100]; // Tam yolu tutacak dizi
	extern char **environ; // Ortam değişkenlerini kullanmak için

	// print environment variables
	for (char **env = environ; *env != 0; env++) {
		char *thisEnv = *env;
		printf("%s\n", thisEnv);
	}
	acces("/bin/ls", F_OK);
    while (1) {
        // Kullanıcıdan komut al
        input = readline("minishell> ");
        if (!input) {
            break; // Kullanıcı Ctrl+D ile çıkarsa programdan çık
        }

        // Boş komut girildiyse devam et
        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        // Komutları ve argümanları ayır
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Tam yolu oluştur (örneğin: /bin/ls)
        snprintf(full_path, sizeof(full_path), "%s%s", path, args[0]);

        // Child process oluştur
        pid_t pid = fork();

        if (pid == 0) {
            // Child process: Komutu çalıştır
            if (execve(full_path, args, environ) == -1) {
                perror("execve failed");
                exit(EXIT_FAILURE); // Hata durumunda child process sonlandır
            }
        } else if (pid < 0) {
            // fork başarısız oldu
            perror("fork failed");
        } else {
            // Parent process: Child process'in bitmesini bekle
            wait(NULL);
        }

        free(input); // Belleği serbest bırak
    }

    return 0;
}
