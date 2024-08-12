#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>


int main() {
    char *input;
    char *args[10]; // Maksimum 10 argümanlık bir dizi
    int i;

    while (1) {
        // Kullanıcıdan girdi almak için readline çağrısı
        input = readline("minishell> ");
        
        // Girdi boş değilse ve sadece Enter'a basılmamışsa
        if (input && *input) {
            add_history(input); // Komut geçmişine ekle
            
            // Girdiyi parçala
            i = 0;
            args[i] = strtok(input, " ");
            while (args[i] != NULL) {
                i++;
                args[i] = strtok(NULL, " ");
            }
            
            // Yeni bir işlem (child process) oluştur
            pid_t pid = fork();
            
            if (pid == 0) { // Child process
                execv(args[0], args);
                perror("execv failed"); // execv başarısız olursa hata mesajı göster
                exit(EXIT_FAILURE);
            } else if (pid < 0) { // fork başarısız olursa
                perror("fork failed");
            } else { // Parent process
                wait(NULL); // Child process bitene kadar bekle
            }
        }
        
        free(input); // Belleği serbest bırak
    }

    return 0;
}
