#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'


MINISHELL="./minishell"
TIMEOUT=3
TEST_DIR="minishell_test_temp"
RESULTS_FILE="test.log"
VALGRIND_LOG="valgrind_results.log"


if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Erro: O programa minishell não foi encontrado.${NC}"
    echo -e "${YELLOW}Certifique-se de que o executável está no diretório atual com o nome 'minishell'.${NC}"
    exit 1
fi


mkdir -p $TEST_DIR
touch $RESULTS_FILE

total_tests=0
passed_tests=0


run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local expected_exit_code="$4"
    
    total_tests=$((total_tests + 1))
    
    echo -e "${BLUE}Teste $total_tests: $test_name${NC}"
    echo "Comando: $command"
    
    # Executa o comando no minishell
    echo "$command" > $TEST_DIR/input.txt
    actual_output=$(timeout $TIMEOUT $MINISHELL < $TEST_DIR/input.txt 2>&1)
    actual_exit_code=$?
    
    # Verifica timeout
    if [ $actual_exit_code -eq 124 ]; then
        echo -e "${RED}FALHA: Timeout excedido (${TIMEOUT}s)${NC}"
        echo "===== Teste $total_tests: $test_name =====" >> $RESULTS_FILE
        echo "Comando: $command" >> $RESULTS_FILE
        echo "Status: TIMEOUT" >> $RESULTS_FILE
        echo "" >> $RESULTS_FILE
        return
    fi
    
    # Verifica o resultado
    success=true
    
    # Verifica saída (se especificada)
    if [ -n "$expected_output" ]; then
        if ! echo "$actual_output" | grep -q "$expected_output"; then
            success=false
            echo -e "${RED}Saída não corresponde à esperada${NC}"
            echo "Esperado: $expected_output"
            echo "Recebido: $actual_output"
        fi
    fi
    
    # Verifica código de saída (se especificado)
    if [ -n "$expected_exit_code" ] && [ $actual_exit_code -ne $expected_exit_code ]; then
        success=false
        echo -e "${RED}Código de saída não corresponde ao esperado${NC}"
        echo "Esperado: $expected_exit_code"
        echo "Recebido: $actual_exit_code"
    fi
    
    if $success; then
        passed_tests=$((passed_tests + 1))
        echo -e "${GREEN}PASSOU!${NC}"
    else
        echo -e "${RED}FALHOU!${NC}"
    fi
    
    # Registra resultado no arquivo de log
    echo "===== Teste $total_tests: $test_name =====" >> $RESULTS_FILE
    echo "Comando: $command" >> $RESULTS_FILE
    if $success; then
        echo "Status: PASSOU" >> $RESULTS_FILE
    else
        echo "Status: FALHOU" >> $RESULTS_FILE
        echo "Saída esperada: $expected_output" >> $RESULTS_FILE
        echo "Saída recebida: $actual_output" >> $RESULTS_FILE
        echo "Código esperado: $expected_exit_code" >> $RESULTS_FILE
        echo "Código recebido: $actual_exit_code" >> $RESULTS_FILE
    fi
    echo "" >> $RESULTS_FILE
}


cleanup() {
    echo -e "${BLUE}Limpando arquivos temporários...${NC}"
    rm -rf $TEST_DIR
}


trap cleanup EXIT

echo -e "${YELLOW}======== TESTADOR DE MINISHELL ========${NC}"
echo -e "${YELLOW}Iniciando bateria de testes...${NC}"
echo ""

# ===== TESTES BÁSICOS =====
run_test "Echo simples" "echo hello world" "hello world" 0
run_test "Echo com aspas" "echo \"hello world\"" "hello world" 0
run_test "Comando inexistente" "comando_que_nao_existe" "comando não encontrado" 127
run_test "Pwd" "pwd" "$PWD" 0
run_test "Env" "env" "" 0

# ===== TESTES DE REDIRECIONAMENTO =====
run_test "Redirecionamento de saída" "echo hello > $TEST_DIR/outfile; cat $TEST_DIR/outfile" "hello" 0
run_test "Redirecionamento de entrada" "cat < $TEST_DIR/outfile" "hello" 0
run_test "Redirecionamento de append" "echo world >> $TEST_DIR/outfile; cat $TEST_DIR/outfile" "world" 0
run_test "Redirecionamento com erro" "cat < arquivo_inexistente" "No such file or directory" 1

# ===== TESTES DE PIPE =====
run_test "Pipe simples" "echo hello | grep hello" "hello" 0
run_test "Múltiplos pipes" "echo hello world | grep hello | wc -l" "1" 0
run_test "Pipe com redirecionamento" "echo hello | grep hello > $TEST_DIR/pipefile; cat $TEST_DIR/pipefile" "hello" 0

# ===== TESTES DE VARIÁVEIS DE AMBIENTE =====
run_test "Expansão de variável" "echo \$HOME" "" 0  # Resultado varia, então não verificamos a saída específica
run_test "Variável inexistente" "echo \$VARIAVEL_INEXISTENTE" "" 0
run_test "Export e expansão" "export TEST_VAR=value; echo \$TEST_VAR" "value" 0
run_test "Unset" "export TEST_VAR=value; unset TEST_VAR; echo \$TEST_VAR" "" 0

# ===== TESTES DE QUOTES =====
run_test "Aspas simples" "echo 'hello \$HOME'" "hello \$HOME" 0
run_test "Aspas duplas" "echo \"hello \$HOME\"" "" 0  # Resultado varia, então não verificamos a saída específica
run_test "Sem aspas com variável" "echo hello \$HOME" "" 0  # Resultado varia

# ===== TESTES DE COMANDOS BUILT-IN =====
run_test "cd" "cd $TEST_DIR; pwd" "$PWD/$TEST_DIR" 0
run_test "cd retorno" "cd $TEST_DIR; cd ..; pwd" "$PWD" 0
run_test "cd inválido" "cd diretorio_inexistente" "No such file or directory" 1
run_test "exit" "exit 42" "" 42
run_test "exit sem valor" "exit" "" 0

# ===== TESTES AVANÇADOS =====
run_test "Operador AND" "true && echo success" "success" 0
run_test "Operador AND falha" "false && echo success" "" 1
run_test "Operador OR" "false || echo success" "success" 0
run_test "Operador OR sucesso" "true || echo success" "" 0
run_test "Comando com argumentos" "ls -la" "" 0
run_test "Subshell" "echo \$(echo hello)" "hello" 0


# ===== TESTES DE VERIFICAÇÃO DE MEMÓRIA =====
echo -e "${BLUE}Executando teste de vazamento de memória com Valgrind...${NC}"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$VALGRIND_LOG $MINISHELL -c "echo hello; exit" > /dev/null 2>&1

if [ -f $VALGRIND_LOG ]; then
    if grep -q "no leaks are possible" $VALGRIND_LOG; then
        echo -e "${GREEN}Nenhum vazamento de memória detectado!${NC}"
    else
        leaks=$(grep -A 1 "LEAK SUMMARY" $VALGRIND_LOG | grep "definitely lost" | awk '{print $4}')
        echo -e "${RED}Possíveis vazamentos de memória detectados: $leaks bytes${NC}"
        echo -e "${YELLOW}Consulte o arquivo $VALGRIND_LOG para mais detalhes.${NC}"
    fi
else
    echo -e "${YELLOW}Valgrind não disponível ou erro na execução.${NC}"
fi

# Relatório final
echo ""
echo -e "${YELLOW}======== RELATÓRIO FINAL ========${NC}"
echo -e "Total de testes: $total_tests"
echo -e "Testes aprovados: ${GREEN}$passed_tests${NC}"
echo -e "Testes reprovados: ${RED}$((total_tests - passed_tests))${NC}"
echo -e "Taxa de sucesso: $(( (passed_tests * 100) / total_tests ))%"
echo -e "Detalhes dos testes estão no arquivo: $RESULTS_FILE"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "${GREEN}PARABÉNS! Todos os testes passaram!${NC}"
else
    echo -e "${YELLOW}Você ainda tem alguns problemas para resolver.${NC}"
fi