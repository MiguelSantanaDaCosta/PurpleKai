#!/usr/bin/env python3
"""
Script de demonstração profissional do PurpleKai - Modo Ao Vivo com Neovim
Autor: Miguel Santana
Descrição: Abre o nvim, digita o código linha por linha (simula codificação ao vivo)
           com indentação adequada e executa o PurpleKai.
"""

import subprocess
import os
import sys
import time
import random
import argparse

# Cores para output
GREEN = "\033[92m"
BLUE = "\033[94m"
YELLOW = "\033[93m"
RED = "\033[91m"
RESET = "\033[0m"

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
║           PurpleKai - Demonstração Ao Vivo (nvim)           ║
║                     v0.1.0 - Turing Complete                ║
╚══════════════════════════════════════════════════════════════╝{RESET}
"""

def print_step(msg, status="info"):
    symbols = {"info": "ℹ️", "ok": "✅", "error": "❌", "warning": "⚠️"}
    color = {"info": BLUE, "ok": GREEN, "error": RED, "warning": YELLOW}
    print(f"{color.get(status, RESET)}{symbols.get(status, '•')} {msg}{RESET}")

def encontrar_terminal():
    """Detecta um terminal gráfico (kitty, alacritty, ghostty, etc.)"""
    terminais = ["kitty", "alacritty", "ghostty", "gnome-terminal", "xterm"]
    for term in terminais:
        if subprocess.run(["which", term], capture_output=True).returncode == 0:
            return term
    return None

def digitar_no_nvim(conteudo_linhas, intervalo_base=0.2, variacao=0.1):
    """
    Envia texto linha por linha para o Neovim usando wtype.
    - usa 'Return' para Enter
    - usa 'Escape' ou '\\x1b' para sair do modo de inserção
    - evita opções não suportadas (-kp)
    """
    time.sleep(1.5)
    # Entrar em modo de inserção
    subprocess.run(["wtype", "i"])
    time.sleep(0.5)

    for idx, linha in enumerate(conteudo_linhas):
        if idx > 0:
            pausa = intervalo_base + random.uniform(-variacao, variacao)
            time.sleep(max(0.1, pausa))
        
        if linha.strip() == "":
            # Linha vazia: apenas Enter
            subprocess.run(["wtype", "-k", "Return"])
        else:
            # Digitar o texto da linha
            subprocess.run(["wtype", linha])
            if linha.rstrip().endswith(';'):
                time.sleep(0.15)   # pausa extra após ponto e vírgula
            time.sleep(0.05)
            # Enter para próxima linha
            subprocess.run(["wtype", "-k", "Return"])
    
    # Sair do modo de inserção (Escape)
    time.sleep(0.3)
    subprocess.run(["wtype", "-k", "Escape"])   # se falhar, tente "\\x1b"
    time.sleep(0.5)
    # Salvar e sair do Neovim
    subprocess.run(["wtype", ":wq"])
    time.sleep(0.2)
    subprocess.run(["wtype", "-k", "Return"])
    time.sleep(0.5)

def executar_ao_vivo(velocidade="normal"):
    velocidades = {
        "lento": (0.4, 0.15),
        "normal": (0.2, 0.1),
        "rapido": (0.1, 0.05)
    }
    intervalo_base, variacao = velocidades.get(velocidade, (0.2, 0.1))

    term = encontrar_terminal()
    if not term:
        print_step("Nenhum terminal gráfico encontrado.", "error")
        sys.exit(1)
    print_step(f"Usando terminal: {term} (velocidade: {velocidade})", "info")

    script_path = os.path.join(os.getcwd(), "teste_completo.pk")

    # Conteúdo linha a linha (exatamente como deve ser digitado)
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

    comando = f'bash -c "nvim {script_path}; echo; ./purplekai {script_path}; read -p \'Pressione Enter para fechar...\'"'

    if term == "kitty":
        proc = subprocess.Popen(["kitty", "--working-directory", os.getcwd(), "-e", "bash", "-c", comando])
    elif term == "alacritty":
        proc = subprocess.Popen(["alacritty", "--working-directory", os.getcwd(), "-e", "bash", "-c", comando])
    elif term == "ghostty":
        proc = subprocess.Popen(["ghostty", "--working-directory", os.getcwd(), "--exec", "bash", "-c", comando])
    elif term == "gnome-terminal":
        proc = subprocess.Popen(["gnome-terminal", "--working-directory", os.getcwd(), "--", "bash", "-c", comando])
    else:  # fallback xterm
        proc = subprocess.Popen(["xterm", "-e", "bash", "-c", comando])

    time.sleep(2)  # Aguarda o nvim abrir
    digitar_no_nvim(linhas, intervalo_base, variacao)
    
    try:
        proc.wait()
    except KeyboardInterrupt:
        print_step("Demonstração interrompida.", "warning")

def main():
    parser = argparse.ArgumentParser(description="PurpleKai Demonstração Ao Vivo (digitação em nvim)")
    parser.add_argument("--live", action="store_true", help="Modo ao vivo: abre nvim e digita o código")
    parser.add_argument("--velocidade", type=str, default="normal", choices=["lento", "normal", "rapido"],
                        help="Velocidade da digitação (lento, normal, rapido)")
    args = parser.parse_args()

    print(BANNER)
    if args.live:
        print_step(f"Iniciando demonstração ao vivo com Neovim (velocidade: {args.velocidade})...", "info")
        executar_ao_vivo(args.velocidade)
    else:
        print_step("Use --live para iniciar a demonstração com digitação ao vivo.", "info")

if __name__ == "__main__":
    main()
