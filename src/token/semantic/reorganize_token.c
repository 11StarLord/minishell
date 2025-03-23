#include "minishell.h"

static int is_redirection_token(t_token token) {
    return (!strcmp(token.type, "REDIR_OUT") ||
            !strcmp(token.type, "REDIR_IN") ||
            !strcmp(token.type, "APPEND") ||
            !strcmp(token.type, "HEREDOC"));
}

t_token *reorganize_tokens(t_token *tokens) {
    int i = 0;
    int cmd_found = 0;
    int redir_index = -1;
    t_token temp;
    
    while (tokens[i].str) {
        if (is_redirection_token(tokens[i])) {
            redir_index = i;
        }
        
        if (redir_index != -1 && 
            !strcmp(tokens[i].type, "ARGUMENT") && 
            (strcmp(tokens[i].str, "echo") == 0 )) {
            tokens[i].type = "COMMAND";
            temp = tokens[i];
            
            int j = i;
            while (j > 0) {
                tokens[j] = tokens[j - 1];
                j--;
            }
            
            tokens[0] = temp;
            cmd_found = 1;
            
            if (redir_index < i) {
                redir_index++;
            }
            i = 0;
            continue;
        }
        i++;
    }
    
    i = 0;
    while (tokens[i].str) {
        if (is_redirection_token(tokens[i]) && tokens[i+1].str) {
            tokens[i+1].type = "FILENAME";
        }
        i++;
    }
    
    if (!cmd_found) {
        i = 0;
        while (tokens[i].str) {
            if (!strcmp(tokens[i].type, "ARGUMENT") && 
                (strcmp(tokens[i].str, "echo") == 0)) {
                tokens[i].type = "COMMAND";
                break;
            }
            i++;
        }
    }
    
    printf("\nTokens reorganizados:\n");
    i = 0;
    while (tokens[i].str) {
        printf("%d\t%s\t%s\n", i + 1, tokens[i].str, tokens[i].type);
        i++;
    }
    printf("\n");
    
    return tokens;
}