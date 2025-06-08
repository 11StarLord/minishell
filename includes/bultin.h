#ifndef BULTIN_H
#define BULTIN_H
    void	execute_builtin(t_shell *shell, char **cmd);
    void    ft_echo(char **cmd, t_shell *shell);
    void    ft_exit(char **cmd, t_shell *shell);
    void ft_unset(char **cmd, t_shell *shell);

    char *pwd(t_shell *shell);
    void     ft_pwd(char **cmd, t_shell *shell);
    int without_option(char **cmd, t_shell *shell);
    int	total_arg(char **cmd);
#endif