#!/bin/bash

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Contadores
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Configurações
MINISHELL_PATH="./minishell"
TIMEOUT_DURATION=5
TEST_LOG="test_results.log"

# Função para imprimir header
print_header() {
    echo -e "${BLUE}=================================="
    echo -e "    MINISHELL STATUS CODE TESTER"
    echo -e "==================================${NC}"
    echo
}

# Função para logging
log_test() {
    echo "$1" >> "$TEST_LOG"
}

# Função para executar teste
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_status="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "Command: ${BLUE}$command${NC}"
    echo -e "Expected status: $expected_status"
    
    # Executa o comando no minishell e captura o status
    echo "$command" | timeout $TIMEOUT_DURATION $MINISHELL_PATH > /dev/null 2>&1
    local actual_status=$?
    
    # Se timeout, considera como falha
    if [ $actual_status -eq 124 ]; then
        echo -e "${RED}✗ TIMEOUT${NC}"
        echo -e "Description: $description\n"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        log_test "TIMEOUT: $test_name - $command"
        return
    fi
    
    # Compara status codes
    if [ $actual_status -eq $expected_status ]; then
        echo -e "${GREEN}✓ PASSED${NC} (Status: $actual_status)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        log_test "PASS: $test_name - Expected: $expected_status, Got: $actual_status"
    else
        echo -e "${RED}✗ FAILED${NC} (Expected: $expected_status, Got: $actual_status)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        log_test "FAIL: $test_name - Expected: $expected_status, Got: $actual_status"
    fi
    
    echo -e "Description: $description\n"
}

# Função para executar teste com bash como referência
run_comparison_test() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "Command: ${BLUE}$command${NC}"
    
    # Executa no bash para obter status esperado
    bash -c "$command" > /dev/null 2>&1
    local bash_status=$?
    
    # Executa no minishell
    echo "$command" | timeout $TIMEOUT_DURATION $MINISHELL_PATH > /dev/null 2>&1
    local minishell_status=$?
    
    echo -e "Bash status: $bash_status"
    echo -e "Minishell status: $minishell_status"
    
    if [ $minishell_status -eq 124 ]; then
        echo -e "${RED}✗ TIMEOUT${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        log_test "TIMEOUT: $test_name - $command"
    elif [ $minishell_status -eq $bash_status ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        log_test "PASS: $test_name - Status: $minishell_status"
    else
        echo -e "${RED}✗ FAILED${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        log_test "FAIL: $test_name - Bash: $bash_status, Minishell: $minishell_status"
    fi
    
    echo -e "Description: $description\n"
}

# Função para verificar se minishell existe
check_minishell() {
    if [ ! -f "$MINISHELL_PATH" ]; then
        echo -e "${RED}Error: Minishell not found at $MINISHELL_PATH${NC}"
        echo "Please make sure minishell is compiled and in the current directory."
        exit 1
    fi
    
    if [ ! -x "$MINISHELL_PATH" ]; then
        echo -e "${RED}Error: Minishell is not executable${NC}"
        exit 1
    fi
}

# Função principal de testes
run_all_tests() {
    echo -e "${BLUE}Starting comprehensive status code tests...\n${NC}"
    
    # Limpa log anterior
    > "$TEST_LOG"
    
    # ===================
    # TESTES BÁSICOS
    # ===================
    echo -e "${BLUE}=== BASIC COMMAND TESTS ===${NC}"
    
    run_test "Success Command" "echo hello" 0 "Simple echo command should return 0"
    run_test "True Command" "true" 0 "Built-in true command should return 0"
    run_test "False Command" "false" 1 "Built-in false command should return 1"
    run_test "Exit 0" "exit 0" 0 "Explicit exit with status 0"
    run_test "Exit 1" "exit 1" 1 "Explicit exit with status 1"
    run_test "Exit 42" "exit 42" 42 "Explicit exit with custom status"
    
    # ===================
    # COMANDOS INEXISTENTES
    # ===================
    echo -e "${BLUE}=== COMMAND NOT FOUND TESTS ===${NC}"
    
    run_test "Command Not Found" "nonexistentcommand" 127 "Non-existent command should return 127"
    run_test "Invalid Path" "/invalid/path/command" 127 "Invalid path should return 127"
    run_test "Empty Command" "" 0 "Empty command should return 0 (no-op)"
    
    # ===================
    # TESTES DE ARQUIVOS
    # ===================
    echo -e "${BLUE}=== FILE ACCESS TESTS ===${NC}"
    
    # Cria arquivo temporário para testes
    echo "#!/bin/bash\nexit 5" > test_script.sh
    chmod +x test_script.sh
    
    run_test "Executable Script" "./test_script.sh" 5 "Executable script with exit 5"
    
    # Remove permissão de execução
    chmod -x test_script.sh
    run_test "Non-executable File" "./test_script.sh" 126 "File exists but not executable should return 126"
    
    # Remove arquivo
    rm -f test_script.sh
    
    # ===================
    # TESTES DE REDIRECIONAMENTO
    # ===================
    echo -e "${BLUE}=== REDIRECTION TESTS ===${NC}"
    
    run_test "Valid Redirection" "echo hello > /tmp/test_output" 0 "Valid output redirection should succeed"
    run_test "Invalid Redirection" "echo hello > /invalid/path/file" 1 "Invalid path redirection should fail"
    run_test "Input from Non-existent" "cat < /nonexistent/file" 1 "Input from non-existent file should fail"
    
    # Limpa arquivo de teste
    rm -f /tmp/test_output
    
    # ===================
    # TESTES DE PIPES
    # ===================
    echo -e "${BLUE}=== PIPE TESTS ===${NC}"
    
    run_test "Simple Pipe Success" "echo hello | cat" 0 "Simple pipe should succeed"
    run_test "Pipe with False" "false | true" 0 "Pipe should return status of last command"
    run_test "Pipe with Failure" "true | false" 1 "Pipe with failing last command"
    run_test "Complex Pipe" "echo hello | grep hello | wc -l" 0 "Multi-stage pipe should work"
    
    # ===================
    # TESTES DE SINAIS
    # ===================
    echo -e "${BLUE}=== SIGNAL TESTS ===${NC}"
    
    # Estes testes podem precisar de ajuste dependendo da implementação
    run_test "Ctrl+C Simulation" "sleep 10" 130 "Interrupted command should return 130 (128+SIGINT)"
    
    # ===================
    # TESTES DE BUILT-INS
    # ===================
    echo -e "${BLUE}=== BUILT-IN TESTS ===${NC}"
    
    run_test "CD Success" "cd /tmp" 0 "CD to valid directory should succeed"
    run_test "CD Failure" "cd /nonexistent/directory" 1 "CD to invalid directory should fail"
    run_test "PWD Command" "pwd" 0 "PWD should always succeed"
    run_test "Echo Built-in" "echo test" 0 "Echo built-in should succeed"
    run_test "Env Command" "env" 0 "Env should succeed"
    
    # ===================
    # TESTES DE COMPARAÇÃO COM BASH
    # ===================
    echo -e "${BLUE}=== BASH COMPARISON TESTS ===${NC}"
    
    run_comparison_test "Comparison: ls" "ls /tmp" "Compare ls command with bash"
    run_comparison_test "Comparison: pwd" "pwd" "Compare pwd with bash"
    run_comparison_test "Comparison: echo" "echo 'test message'" "Compare echo with bash"
    run_comparison_test "Comparison: false" "false" "Compare false command with bash"
    run_comparison_test "Comparison: true" "true" "Compare true command with bash"
}

# Função para mostrar sumário
show_summary() {
    echo -e "${BLUE}=================================="
    echo -e "           TEST SUMMARY"
    echo -e "==================================${NC}"
    echo -e "Total tests: $TOTAL_TESTS"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 All tests passed!${NC}"
    else
        echo -e "\n${RED}❌ Some tests failed. Check $TEST_LOG for details.${NC}"
    fi
    
    local pass_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "Pass rate: ${pass_rate}%\n"
}

# Função de ajuda
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Options:"
    echo "  -h, --help          Show this help message"
    echo "  -p, --path PATH     Set minishell executable path (default: ./minishell)"
    echo "  -t, --timeout SEC   Set timeout duration (default: 5 seconds)"
    echo "  -l, --log FILE      Set log file name (default: test_results.log)"
    echo "  -q, --quick         Run only basic tests"
    echo
    echo "Examples:"
    echo "  $0                          # Run all tests with default settings"
    echo "  $0 -p ../minishell -t 10    # Use different path and timeout"
    echo "  $0 --quick                  # Run only basic tests"
}

# Função para testes rápidos
run_quick_tests() {
    echo -e "${BLUE}Running quick tests only...\n${NC}"
    
    > "$TEST_LOG"
    
    run_test "Echo Test" "echo hello" 0 "Basic echo command"
    run_test "True Test" "true" 0 "True command"
    run_test "False Test" "false" 1 "False command"
    run_test "Exit Test" "exit 42" 42 "Exit with custom code"
    run_test "Not Found" "invalidcommand" 127 "Command not found"
}

# MAIN SCRIPT
main() {
    # Parse argumentos
    QUICK_MODE=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -p|--path)
                MINISHELL_PATH="$2"
                shift 2
                ;;
            -t|--timeout)
                TIMEOUT_DURATION="$2"
                shift 2
                ;;
            -l|--log)
                TEST_LOG="$2"
                shift 2
                ;;
            -q|--quick)
                QUICK_MODE=true
                shift
                ;;
            *)
                echo "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    print_header
    check_minishell
    
    echo -e "Using minishell at: ${GREEN}$MINISHELL_PATH${NC}"
    echo -e "Timeout duration: ${GREEN}${TIMEOUT_DURATION}s${NC}"
    echo -e "Log file: ${GREEN}$TEST_LOG${NC}\n"
    
    if [ "$QUICK_MODE" = true ]; then
        run_quick_tests
    else
        run_all_tests
    fi
    
    show_summary
}

# Executa o script
main "$@"