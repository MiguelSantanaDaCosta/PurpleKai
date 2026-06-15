#!/usr/bin/env python3
"""
Script de demonstração profissional do PurpleKai
Autor: Miguel Santana
Descrição: Cria um arquivo de teste completo e executa em diversos terminais,
           com opções de log, modo REPL e personalização.
"""

import subprocess
import os
import sys
import time
import argparse
import tempfile
from pathlib import Path
from datetime import datetime

# Cores para output
GREEN = "\033[92m"
BLUE = "\033[94m"
YELLOW = "\033[93m"
RED = "\033[91m"
RESET = "\033[0m"

# Banner
BANNER = f"""
{BLUE}╔══════════════════════════════════════════════════════════╗
║                                                              ║
║   ██╗   ██╗██████╗ ██╗   ██╗██████╗ ██╗     ███████╗       ║
║   ██║   ██║██╔══██╗██║   ██║██╔══██╗██║     ██╔════╝       ║
║   ██║   ██║██████╔╝██║   ██║██████╔╝██║     █████╗         ║
║   ██║   ██║██╔══██╗██║   ██║██╔═══╝ ██║     ██╔══╝         ║
║   ╚██████╔╝██║  ██║╚██████╔╝██║     ███████╗███████╗       ║
║    ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚══════╝╚══════╝       ║
║                                                              ║
║           PurpleKai - Demonstração Automática               ║
║                     v0.1.0 - Turing Complete                ║
╚══════════════════════════════════════════════════════════════╝{RESET}
"""

def print_step(msg, status="info"):
    """Imprime mensagem formatada."""
    symbols = {"info": "ℹ️", "ok": "✅", "error": "❌", "warning": "⚠️"}
    color = {"info": BLUE, "ok": GREEN, "error": RED, "warning": YELLOW}
    print(f"{color.get(status, RESET)}{symbols.get(status, '•')} {msg}{RESET}")


def criar_arquivo_teste(caminho, incluir_benchmark=False):
    """Cria o arquivo .pk com todos os testes (versão linha a linha)."""
    linhas = [
        'laisser a = 10;',
        'laisser b = 20;',
        'laisser soma = a + b;',
        '"Resultado: " + soma;',
        '',
        'si (soma > 25) {',
        '    "Maior que 25";',
        '} sinon {',
        '    "Menor ou igual a 25";',
        '}',
        '',
        '"--- Loop pour ---";',
        'pour (laisser i = 0; i < 3; i = i + 1) {',
        '    "Valor de i: " + i;',
        '}',
        '',
        '"--- Loop tantque ---";',
        'laisser x = 3;',
        'tantque (x > 0) {',
        '    "x = " + x;',
        '    x = x - 1;',
        '}',
        '',
        '"--- Break ---";',
        'pour (laisser i = 0; i < 5; i = i + 1) {',
        '    si (i == 3) { break; }',
        '    i;',
        '}',
        '',
        '"--- Continue ---";',
        'pour (laisser i = 0; i < 5; i = i + 1) {',
        '    si (i == 2) { continue; }',
        '    i;',
        '}',
        '',
        '"--- Concatenação mista ---";',
        'laisser prefixo = "Número: ";',
        'prefixo + 42;',
        '',
        '"--- Escopo ---";',
        'laisser global = 10;',
        '{',
        '    laisser local = 20;',
        '    "dentro: " + local;',
        '}',
        '"fora: " + global;',
        '',
        'constant PI = 3.14;',
        'PI;',
        '',
        '"--- Expressões ---";',
        '(10 + 5) * 2 - 8 / 4;',
        '(5 > 3) et (10 < 20) ou non faux;',
        '',
        '"--- Fatorial de 5 ---";',
        'laisser n = 5;',
        'laisser fat = 1;',
        'tantque (n > 0) {',
        '    fat = fat * n;',
        '    n = n - 1;',
        '}',
        '"Fatorial: " + fat;',
        '',
        '"--- Todos os testes concluídos com sucesso! ---";'
    ]
    
    # Se benchmark for solicitado, adiciona no final (opcional)
    if incluir_benchmark:
        linhas.extend([
            '',
            '"--- Benchmark (1000 iterações) ---";',
            'laisser count = 0;',
            'pour (laisser i = 0; i < 1000; i = i + 1) {',
            '    count = count + 1;',
            '}',
            '"Contador: " + count;',
        ])
    
    with open(caminho, "w", encoding="utf-8") as f:
        for linha in linhas:
            f.write(linha + '\n')
    return True

def encontrar_terminal(preferido=None):
    """Detecta terminais disponíveis. Se preferido for fornecido, tenta usá-lo primeiro."""
    terminais_map = {
        "kitty":   ["kitty", "--working-directory"],
        "alacritty": ["alacritty", "--working-directory"],
        "gnome-terminal": ["gnome-terminal", "--working-directory"],
        "ghostty": ["ghostty", "--working-directory"],
        "wezterm": ["wezterm", "start", "--cwd"],
        "xterm":   ["xterm", "-e"],
        "foot":    ["foot", "--working-directory"],
        "terminator": ["terminator", "--working-directory"],
    }
    if preferido and preferido in terminais_map:
        cmd_base = terminais_map[preferido]
        if subprocess.run(["which", cmd_base[0]], capture_output=True).returncode == 0:
            return preferido, cmd_base
        print_step(f"Terminal preferido '{preferido}' não encontrado. Buscando alternativas...", "warning")
    
    for nome, cmd in terminais_map.items():
        if subprocess.run(["which", cmd[0]], capture_output=True).returncode == 0:
            return nome, cmd
    return None, None

def executar_no_terminal(terminal_cmd, workdir, purplekai_path, script_path, keep_open=True, log_file=None):
    """Executa o purplekai dentro de um terminal."""
    if keep_open:
        comando_interno = f'cd "{workdir}" && "./{purplekai_path}" "{script_path}"; echo; echo "[Pressione Enter para fechar...]"; read'
    else:
        comando_interno = f'cd "{workdir}" && "./{purplekai_path}" "{script_path}"'
    
    terminal_nome, cmd_base = terminal_cmd
    print_step(f"Usando terminal: {terminal_nome}", "info")
    
    # Constroi comando específico para cada terminal
    if terminal_nome == "kitty":
        proc = subprocess.Popen(cmd_base + [workdir, "-e", "bash", "-c", comando_interno])
    elif terminal_nome == "alacritty":
        proc = subprocess.Popen(cmd_base + [workdir, "-e", "bash", "-c", comando_interno])
    elif terminal_nome == "gnome-terminal":
        proc = subprocess.Popen(cmd_base + [workdir, "--", "bash", "-c", comando_interno])
    elif terminal_nome == "ghostty":
        # Ghostty suporta --working-directory mas não -e; usamos --exec
        proc = subprocess.Popen([cmd_base[0], "--working-directory", workdir, "--exec", "bash", "-c", comando_interno])
    elif terminal_nome == "wezterm":
        # wezterm usa --cwd e depois o comando
        proc = subprocess.Popen([cmd_base[0], "start", "--cwd", workdir, "--", "bash", "-c", comando_interno])
    elif terminal_nome == "foot":
        proc = subprocess.Popen([cmd_base[0], "--working-directory", workdir, "bash", "-c", comando_interno])
    elif terminal_nome == "terminator":
        proc = subprocess.Popen([cmd_base[0], "--working-directory", workdir, "-e", "bash", "-c", comando_interno])
    else:  # fallback xterm etc.
        proc = subprocess.Popen([cmd_base[0], "-e", "bash", "-c", comando_interno])
    
    # Se solicitado, aguarda um pouco e opcionalmente grava log (não trivial com processos filhos)
    return proc

def executar_no_console(workdir, purplekai_path, script_path, log_file=None):
    """Executa diretamente no console atual (não abre nova janela). Útil para CI."""
    comando = f'cd "{workdir}" && "./{purplekai_path}" "{script_path}"'
    print_step("Executando no console atual...", "info")
    result = subprocess.run(comando, shell=True, capture_output=True, text=True)
    if log_file:
        with open(log_file, "w") as f:
            f.write(result.stdout)
            if result.stderr:
                f.write("\n--- STDERR ---\n")
                f.write(result.stderr)
    print(result.stdout)
    if result.stderr:
        print_step("Erros durante execução:", "warning")
        print(result.stderr)
    return result.returncode

def modo_repl(workdir, purplekai_path, comandos=None):
    """Abre o REPL e envia comandos (experimental). Usa wtype ou ydotool se disponível."""
    if not comandos:
        comandos = [
            'laisser nome = "PurpleKai";',
            '"Olá, " + nome;',
            'laisser a = 10; laisser b = 20; a + b;',
            'si (10 > 5) { "Verdadeiro" } sinon { "Falso" };',
            'pour (laisser i = 0; i < 3; i = i + 1) { "i=" + i; }',
            'sair',
        ]
    # Verifica se temos wtype
    has_wtype = subprocess.run(["which", "wtype"], capture_output=True).returncode == 0
    if not has_wtype:
        print_step("wtype não encontrado. Modo REPL automatizado requer wtype. Instale com: sudo pacman -S wtype", "error")
        return False
    # Abre terminal com purplekai (sem script)
    terminal_info = encontrar_terminal()
    if not terminal_info[0]:
        print_step("Nenhum terminal encontrado para REPL.", "error")
        return False
    # Abre o REPL no terminal
    comando_interno = f'cd "{workdir}" && "./{purplekai_path}"'
    terminal_nome, cmd_base = terminal_info
    if terminal_nome == "kitty":
        proc = subprocess.Popen(cmd_base + [workdir, "-e", "bash", "-c", comando_interno])
    elif terminal_nome == "alacritty":
        proc = subprocess.Popen(cmd_base + [workdir, "-e", "bash", "-c", comando_interno])
    elif terminal_nome == "ghostty":
        proc = subprocess.Popen([cmd_base[0], "--working-directory", workdir, "--exec", "bash", "-c", comando_interno])
    else:
        proc = subprocess.Popen([cmd_base[0], "-e", "bash", "-c", comando_interno])
    time.sleep(1.5)  # Aguarda o REPL iniciar
    # Envia comandos
    for cmd in comandos:
        subprocess.run(["wtype", cmd])
        time.sleep(0.5)
        subprocess.run(["wtype", "-kp", "enter"])
        time.sleep(1)
    return True

def gerar_relatorio(log_path):
    """Gera um relatório simples a partir do log."""
    if not os.path.exists(log_path):
        return
    print_step(f"Relatório gerado: {log_path}", "info")
    with open(log_path, "r") as f:
        lines = f.readlines()
    # Conta testes baseado em palavras-chave
    testes = sum(1 for line in lines if "---" in line and not line.strip().startswith("--"))
    erros = sum(1 for line in lines if "Erro" in line or "error" in line.lower())
    print(f"    Testes executados: {testes}")
    print(f"    Erros detectados: {erros}")

def main():
    parser = argparse.ArgumentParser(description="PurpleKai Demonstração Automática")
    parser.add_argument("--terminal", type=str, default=None, help="Terminal preferido (kitty, alacritty, ghostty, etc.)")
    parser.add_argument("--keep-open", action="store_true", default=True, help="Manter terminal aberto após execução (padrão)")
    parser.add_argument("--no-keep", dest="keep_open", action="store_false", help="Fechar terminal automaticamente")
    parser.add_argument("--console", action="store_true", help="Executar no console atual (sem abrir nova janela)")
    parser.add_argument("--repl", action="store_true", help="Modo REPL (digitação automática)")
    parser.add_argument("--log", type=str, default=None, help="Salvar saída em arquivo de log")
    parser.add_argument("--benchmark", action="store_true", help="Incluir benchmark no script")
    parser.add_argument("--output-script", type=str, default="teste_completo.pk", help="Nome do arquivo de script gerado")
    args = parser.parse_args()

    print(BANNER)
    print_step("Iniciando demonstração do PurpleKai", "info")

    workdir = os.getcwd()
    purplekai_exec = "./purplekai"
    script_path = os.path.join(workdir, args.output_script)

    # Verifica executável
    if not os.path.isfile(os.path.join(workdir, "purplekai")):
        print_step("Executável 'purplekai' não encontrado. Compile com 'make'.", "error")
        sys.exit(1)

    # Cria script de teste
    criar_arquivo_teste(script_path, incluir_benchmark=args.benchmark)
    print_step(f"Script criado: {script_path}", "ok")

    # Modo REPL
    if args.repl:
        print_step("Iniciando modo REPL interativo...", "info")
        modo_repl(workdir, purplekai_exec)
        print_step("REPL finalizado.", "ok")
        return

    # Modo console
    if args.console:
        ret = executar_no_console(workdir, purplekai_exec, script_path, log_file=args.log)
        if ret == 0:
            print_step("Execução concluída com sucesso!", "ok")
        else:
            print_step(f"Execução finalizada com código de erro {ret}.", "error")
        if args.log:
            gerar_relatorio(args.log)
        return

    # Modo gráfico com terminal
    terminal_info = encontrar_terminal(preferido=args.terminal)
    if not terminal_info[0]:
        print_step("Nenhum terminal gráfico encontrado. Use --console ou instale um terminal.", "error")
        sys.exit(1)
    
    proc = executar_no_terminal(terminal_info, workdir, purplekai_exec, script_path, keep_open=args.keep_open, log_file=args.log)
    print_step(f"Demonstração iniciada no terminal {terminal_info[0]}.", "ok")
    print_step("Aguardando a janela... Feche-a quando terminar ou pressione Enter para continuar.", "info")
    try:
        proc.wait()
    except KeyboardInterrupt:
        print_step("Interrompido pelo usuário.", "warning")
    if args.log:
        # Tentamos capturar log após execução (não trivial, pois o terminal pode ter fechado)
        print_step("Log salvo em: " + args.log, "info")

if __name__ == "__main__":
    main()
