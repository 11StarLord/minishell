#!/bin/bash

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Contadores
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Configurações
MINISHELL_PATH="./minishell"
TEST_LOG="signal_test_results.log"
TEMP_DIR="/tmp/minishell_signal_tests"

# Códigos de saída para sinais (128 + número do sinal)
SIGINT_EXIT=130    # 128 + 2 (SIGINT)
SIGQUIT_EXIT=131   # 128 + 3 (SIGQUIT)
SIGTERM_EXIT=143   # 128 + 15 (SIGTERM)
SIGKILL_EXIT=137   # 128 + 9 (SIGKILL)

# Função para imprimir header
print_header() {
    echo -e "${BLUE}=========================================="
    echo -e "      MINISHELL SIGNAL TESTER"
    echo -e "==========================================${NC}"
    echo -e "${CYAN}Testing signal handling and exit codes${NC}"
    echo
}

# Função para logging
log_test() {
    echo "[$(date '+%H:%M:%S')] $1" >> "$TEST_LOG"
}

# Função para setup
setup_test_env() {
    mkdir -p "$TEMP_DIR"
    > "$TEST_LOG"
    
    # Cria scripts de teste
    cat > "$TEMP_DIR/infinite_loop.sh" << 'EOF'
#!/bin/bash
while true; do
    echo "Running..."
    sleep 1
done
EOF

    cat > "$TEMP_DIR/sleep_long.sh" << 'EOF'
#!/bin/bash
echo "Starting sleep..."
sleep 60
echo "Sleep finished"
EOF

    cat > "$TEMP_DIR/ignore_sigint.sh" << 'EOF'
#!/bin/bash
trap 'echo "SIGINT received but ignored"' INT
echo "Script started, try Ctrl+C..."
sleep 30
echo "Script finished normally"
EOF

    cat > "$TEMP_DIR/handle_signals.sh" << 'EOF'
#!/bin/bash
cleanup() {
    echo "Cleanup function called"
    exit 5
}
trap cleanup INT TERM
echo "Script with signal handler started"
sleep 30
echo "Script finished"
EOF

    chmod +x "$TEMP_DIR"/*.sh
}

# Função para cleanup
cleanup_test_env() {
    rm -rf "$TEMP_DIR"
    # Mata qualquer processo minishell que possa ter ficado rodando
    pkill -f "$MINISHELL_PATH" 2>/dev/null || true
}

# Função para executar teste de sinal
run_signal_test() {
    local test_name="$1"
    local command="$2"
    local signal="$3"
    local expected_status="$4"
    local description="$5"
    local delay="${6:-2}"  # Delay antes de enviar sinal (default 2s)
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "Command: ${BLUE}$command${NC}"
    echo -e "Signal: ${PURPLE}$signal${NC}"
    echo -e "Expected status: $expected_status"
    echo -e "Description: $description"
    
    # Inicia minishell em background
    echo "$command" | $MINISHELL_PATH &
    local minishell_pid=$!
    
    # Aguarda um pouco para o comando começar
    sleep "$delay"
    
    # Envia o sinal
    if [ "$signal" != "NONE" ]; then
        kill -"$signal" $minishell_pid 2>/dev/null
        log_test "Sent $signal to PID $minishell_pid"
    fi
    
    # Aguarda o processo terminar
    wait $minishell_pid 2>/dev/null
    local actual_status=$?
    
    # Verifica resultado
    if [ $actual_status -eq $expected_status ]; then
        echo -e "${GREEN}✓ PASSED${NC} (Status: $actual_status)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        log_test "PASS: $test_name - Expected: $expected_status, Got: $actual_status"
    else
        echo -e "${RED}✗ FAILED${NC} (Expected: $expected_status, Got: $actual_status)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        log_test "FAIL: $test_name - Expected: $expected_status, Got: $actual_status"
    fi
    echo
}

# Função para testar sinal interativo
run_interactive_signal_test() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "Command: ${BLUE}$command${NC}"
    echo -e "Description: $description"
    echo -e "${CYAN}This test requires manual interaction${NC}"
    
    # Pergunta se deve executar o teste interativo
    echo -e "${YELLOW}Do you want to run this interactive test? (y/n/s to skip all interactive): ${NC}"
    read -r response
    
    case $response in
        s|S)
            echo -e "${BLUE}Skipping all remaining interactive tests${NC}"
            return 2  # Código especial para skip all
            ;;
        n|N)
            echo -e "${BLUE}Skipping this test${NC}"
            TOTAL_TESTS=$((TOTAL_TESTS - 1))  # Não conta como teste
            return 1
            ;;
        y|Y|*)
            echo -e "${GREEN}Running interactive test...${NC}"
            echo -e "${CYAN}Instructions: Run the command and test signals manually${NC}"
            echo -e "${CYAN}Press Ctrl+C, Ctrl+\\, or other signals as appropriate${NC}"
            echo -e "${CYAN}Press Enter to start the test...${NC}"
            read -r
            
            # Executa o comando
            echo "$command" | $MINISHELL_PATH
            local status=$?
            
            echo -e "${YELLOW}Test completed with status: $status${NC}"
            echo -e "${YELLOW}Was this the expected behavior? (y/n): ${NC}"
            read -r result
            
            if [[ $result =~ ^[Yy] ]]; then
                echo -e "${GREEN}✓ PASSED${NC} (Manual verification)"
                PASSED_TESTS=$((PASSED_TESTS + 1))
                log_test "PASS: $test_name - Manual verification - Status: $status"
            else
                echo -e "${RED}✗ FAILED${NC} (Manual verification)"
                FAILED_TESTS=$((FAILED_TESTS + 1))
                log_test "FAIL: $test_name - Manual verification - Status: $status"
            fi
            ;;
    esac
    echo
}

# Função para testar sinais com child processes
run_child_signal_test() {
    local test_name="$1"
    local command="$2"
    local signal="$3"
    local expected_status="$4"
    local description="$5"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "Command: ${BLUE}$command${NC}"
    echo -e "Signal: ${PURPLE}$signal${NC}"
    echo -e "Expected status: $expected_status"
    echo -e "Description: $description"
    
    # Executa em background e captura PID
    echo "$command" | $MINISHELL_PATH &
    local minishell_pid=$!
    
    # Aguarda um pouco para o comando iniciar
    sleep 1
    
    # Encontra o processo filho (se existir)
    local child_pid=$(pgrep -P $minishell_pid 2>/dev/null | head -1)
    
    if [ -n "$child_pid" ]; then
        log_test "Found child process: $child_pid"
        sleep 1
        
        # Envia sinal para o minishell (pai)
        kill -"$signal" $minishell_pid 2>/dev/null
        log_test "Sent $signal to minishell PID $minishell_pid"
    else
        log_test "No child process found"
        kill -"$signal" $minishell_pid 2>/dev/null
    fi
    
    # Aguarda terminar
    wait $minishell_pid 2>/dev/null
    local actual_status=$?
    
    # Verifica resultado
    if [ $actual_status -eq $expected_status ]; then
        echo -e "${GREEN}✓ PASSED${NC} (Status: $actual_status)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        log_test "PASS: $test_name - Expected: $expected_status, Got: $actual_status"
    else
        echo -e "${RED}✗ FAILED${NC} (Expected: $expected_status, Got: $actual_status)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        log_test "FAIL: $test_name - Expected: $expected_status, Got: $actual_status"
    fi
    echo
}

# Função principal de testes
run_all_signal_tests() {
    local skip_interactive=false
    
    echo -e "${BLUE}Starting comprehensive signal tests...\n${NC}"
    
    # ===================
    # TESTES BÁSICOS DE SINAIS
    # ===================
    echo -e "${BLUE}=== BASIC SIGNAL TESTS ===${NC}"
    
    run_signal_test "SIGINT on Sleep" "sleep 10" "INT" $SIGINT_EXIT "SIGINT should interrupt sleep command"
    
    run_signal_test "SIGTERM on Sleep" "sleep 10" "TERM" $SIGTERM_EXIT "SIGTERM should terminate sleep command"
    
    run_signal_test "SIGQUIT on Sleep" "sleep 10" "QUIT" $SIGQUIT_EXIT "SIGQUIT should quit sleep command"
    
    # ===================
    # TESTES COM SCRIPTS CUSTOMIZADOS
    # ===================
    echo -e "${BLUE}=== CUSTOM SCRIPT SIGNAL TESTS ===${NC}"
    
    run_signal_test "SIGINT on Infinite Loop" "$TEMP_DIR/infinite_loop.sh" "INT" $SIGINT_EXIT "SIGINT should stop infinite loop"
    
    run_signal_test "SIGTERM on Long Sleep" "$TEMP_DIR/sleep_long.sh" "TERM" $SIGTERM_EXIT "SIGTERM should stop long sleep"
    
    # ===================
    # TESTES DE PROPAGAÇÃO DE SINAIS
    # ===================
    echo -e "${BLUE}=== SIGNAL PROPAGATION TESTS ===${NC}"
    
    run_child_signal_test "SIGINT Propagation" "bash -c 'sleep 10'" "INT" $SIGINT_EXIT "SIGINT should propagate to child processes"
    
    run_child_signal_test "SIGTERM Propagation" "bash -c 'sleep 10'" "TERM" $SIGTERM_EXIT "SIGTERM should propagate to child processes"
    
    # ===================
    # TESTES DE PIPES COM SINAIS
    # ===================
    echo -e "${BLUE}=== PIPE SIGNAL TESTS ===${NC}"
    
    run_signal_test "SIGINT on Pipe" "yes | head -100000" "INT" $SIGINT_EXIT "SIGINT should interrupt pipe operations"
    
    run_signal_test "SIGINT on Complex Pipe" "cat /dev/urandom | base64 | head -1000" "INT" $SIGINT_EXIT "SIGINT should interrupt complex pipes"
    
    # ===================
    # TESTES INTERATIVOS
    # ===================
    echo -e "${BLUE}=== INTERACTIVE SIGNAL TESTS ===${NC}"
    
    local interactive_result
    run_interactive_signal_test "Manual SIGINT Test" "$TEMP_DIR/infinite_loop.sh" "Test Ctrl+C behavior manually"
    interactive_result=$?
    
    if [ $interactive_result -eq 2 ]; then
        skip_interactive=true
    fi
    
    if [ "$skip_interactive" = false ]; then
        run_interactive_signal_test "Manual SIGQUIT Test" "sleep 30" "Test Ctrl+\\ behavior manually"
        
        run_interactive_signal_test "Multiple Signals Test" "$TEMP_DIR/handle_signals.sh" "Test script with signal handlers"
        
        run_interactive_signal_test "Background Job Signal" "sleep 30 &" "Test signals on background jobs"
    fi
    
    # ===================
    # TESTES DE EDGE CASES
    # ===================
    echo -e "${BLUE}=== EDGE CASE SIGNAL TESTS ===${NC}"
    
    # Teste de sinal rápido (antes do comando iniciar completamente)
    run_signal_test "Quick SIGINT" "sleep 5" "INT" $SIGINT_EXIT "SIGINT sent immediately" 0.1
    
    # Teste de múltiplos sinais
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${YELLOW}Test $TOTAL_TESTS: Multiple SIGINT Test${NC}"
    echo -e "Command: ${BLUE}sleep 10${NC}"
    echo -e "Signal: ${PURPLE}Multiple INT${NC}"
    echo -e "Description: Multiple SIGINT signals in sequence"
    
    echo "sleep 10" | $MINISHELL_PATH &
    local minishell_pid=$!
    sleep 1
    
    # Envia múltiplos SIGINT
    kill -INT $minishell_pid 2>/dev/null
    sleep 0.1
    kill -INT $minishell_pid 2>/dev/null
    sleep 0.1
    kill -INT $minishell_pid 2>/dev/null
    
    wait $minishell_pid 2>/dev/null
    local status=$?
    
    if [ $status -eq $SIGINT_EXIT ]; then
        echo -e "${GREEN}✓ PASSED${NC} (Status: $status)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAILED${NC} (Expected: $SIGINT_EXIT, Got: $status)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo
    
    # ===================
    # TESTES DE SINAIS EM BUILT-INS
    # ===================
    echo -e "${BLUE}=== BUILT-IN SIGNAL TESTS ===${NC}"
    
    # Alguns built-ins podem não ser interrompíveis
    run_signal_test "SIGINT on Echo Loop" "bash -c 'for i in {1..1000}; do echo \$i; done'" "INT" $SIGINT_EXIT "SIGINT should interrupt echo loop"
    
    # ===================
    # TESTES DE COMPARAÇÃO COM BASH
    # ===================
    echo -e "${BLUE}=== BASH COMPARISON SIGNAL TESTS ===${NC}"
    
    echo -e "${CYAN}Comparing signal behavior with bash...${NC}"
    
    # Testa comportamento do bash para comparação
    echo "sleep 5" | bash &
    local bash_pid=$!
    sleep 1
    kill -INT $bash_pid 2>/dev/null
    wait $bash_pid 2>/dev/null
    local bash_status=$?
    
    echo "sleep 5" | $MINISHELL_PATH &
    local minishell_pid=$!
    sleep 1
    kill -INT $minishell_pid 2>/dev/null
    wait $minishell_pid 2>/dev/null
    local minishell_status=$?
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${YELLOW}Test $TOTAL_TESTS: Bash Comparison - SIGINT${NC}"
    echo -e "Bash status: $bash_status"
    echo -e "Minishell status: $minishell_status"
    
    if [ $minishell_status -eq $bash_status ]; then
        echo -e "${GREEN}✓ PASSED${NC} (Both returned: $minishell_status)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAILED${NC} (Bash: $bash_status, Minishell: $minishell_status)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo
}

# Função para mostrar sumário
show_summary() {
    echo -e "${BLUE}=========================================="
    echo -e "             SIGNAL TEST SUMMARY"
    echo -e "==========================================${NC}"
    echo -e "Total tests: $TOTAL_TESTS"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 All signal tests passed!${NC}"
    else
        echo -e "\n${RED}❌ Some signal tests failed.${NC}"
        echo -e "${YELLOW}Check $TEST_LOG for detailed logs.${NC}"
    fi
    
    if [ $TOTAL_TESTS -gt 0 ]; then
        local pass_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
        echo -e "Pass rate: ${pass_rate}%"
    fi
    
    echo -e "\n${CYAN}Signal Exit Codes Reference:${NC}"
    echo -e "SIGINT (Ctrl+C): $SIGINT_EXIT"
    echo -e "SIGQUIT (Ctrl+\\): $SIGQUIT_EXIT"
    echo -e "SIGTERM: $SIGTERM_EXIT"
    echo -e "SIGKILL: $SIGKILL_EXIT"
    echo
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

# Função de ajuda
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Signal Tester for Minishell"
    echo
    echo "Options:"
    echo "  -h, --help          Show this help message"
    echo "  -p, --path PATH     Set minishell executable path (default: ./minishell)"
    echo "  -l, --log FILE      Set log file name (default: signal_test_results.log)"
    echo "  -q, --quick         Run only basic signal tests (no interactive)"
    echo "  -i, --interactive   Run only interactive signal tests"
    echo "  --no-cleanup        Don't cleanup temporary files after tests"
    echo
    echo "Signal Exit Codes:"
    echo "  SIGINT (Ctrl+C):    130 (128 + 2)"
    echo "  SIGQUIT (Ctrl+\\):   131 (128 + 3)"
    echo "  SIGTERM:            143 (128 + 15)"
    echo "  SIGKILL:            137 (128 + 9)"
    echo
    echo "Examples:"
    echo "  $0                      # Run all signal tests"
    echo "  $0 --quick              # Run only basic tests"
    echo "  $0 --interactive        # Run only interactive tests"
    echo "  $0 -p ../minishell      # Use different minishell path"
}

# Função para testes rápidos (sem interativos)
run_quick_signal_tests() {
    echo -e "${BLUE}Running quick signal tests (no interactive)...\n${NC}"
    
    run_signal_test "Quick SIGINT" "sleep 5" "INT" $SIGINT_EXIT "Basic SIGINT test"
    run_signal_test "Quick SIGTERM" "sleep 5" "TERM" $SIGTERM_EXIT "Basic SIGTERM test"
    run_signal_test "Quick SIGQUIT" "sleep 5" "QUIT" $SIGQUIT_EXIT "Basic SIGQUIT test"
    run_signal_test "Quick Pipe SIGINT" "yes | head -1000" "INT" $SIGINT_EXIT "SIGINT on pipe"
}

# Função para apenas testes interativos
run_interactive_only() {
    echo -e "${BLUE}Running interactive signal tests only...\n${NC}"
    
    run_interactive_signal_test "Interactive SIGINT" "$TEMP_DIR/infinite_loop.sh" "Test Ctrl+C manually"
    run_interactive_signal_test "Interactive SIGQUIT" "sleep 30" "Test Ctrl+\\ manually"
    run_interactive_signal_test "Interactive Multiple Signals" "$TEMP_DIR/handle_signals.sh" "Test various signals"
}

# MAIN SCRIPT
main() {
    local quick_mode=false
    local interactive_only=false
    local no_cleanup=false
    
    # Parse argumentos
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
            -l|--log)
                TEST_LOG="$2"
                shift 2
                ;;
            -q|--quick)
                quick_mode=true
                shift
                ;;
            -i|--interactive)
                interactive_only=true
                shift
                ;;
            --no-cleanup)
                no_cleanup=true
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
    setup_test_env
    
    echo -e "Using minishell at: ${GREEN}$MINISHELL_PATH${NC}"
    echo -e "Log file: ${GREEN}$TEST_LOG${NC}"
    echo -e "Temp directory: ${GREEN}$TEMP_DIR${NC}\n"
    
    # Trap para cleanup em caso de interrupção
    trap 'echo -e "\n${YELLOW}Tests interrupted. Cleaning up...${NC}"; cleanup_test_env; exit 1' INT TERM
    
    if [ "$quick_mode" = true ]; then
        run_quick_signal_tests
    elif [ "$interactive_only" = true ]; then
        run_interactive_only
    else
        run_all_signal_tests
    fi
    
    show_summary
    
    if [ "$no_cleanup" = false ]; then
        cleanup_test_env
    else
        echo -e "${YELLOW}Temporary files kept at: $TEMP_DIR${NC}"
    fi
}

# Executa o script
main "$@"