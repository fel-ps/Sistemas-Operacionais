import sys
import os
import matplotlib.pyplot as plt
import pandas as pd

# Como executar 
# python grafico.py resultados_fifo.txt FIFO
# python grafico.py resultados_sjf.txt SJF

# Verifica se foram passados os argumentos necessários
if len(sys.argv) < 3:
    print("Uso: python grafico.py <arquivo_resultado.txt> <nome_escalonador>")
    sys.exit(1)

nome_arquivo = sys.argv[1]
nome_escalonador = sys.argv[2]

# Lê os dados do arquivo
dados = []
with open(nome_arquivo, "r") as f:
    for linha in f:
        if "TME:" in linha:
            partes = linha.strip().split()
            tme = float(partes[1])
            n_procs = int(partes[2])
            dados.append((n_procs, tme))

# Converte para DataFrame
df = pd.DataFrame(dados, columns=["n_procs", "tme"])

# Agrupa por número de processos e calcula média e variância
estatisticas = df.groupby("n_procs").agg(
    media_tme=("tme", "mean"),
    variancia_tme=("tme", "var")
).reset_index()

# Cria pasta "graficos" se não existir
os.makedirs("graficos", exist_ok=True)

# Plotando com barras de erro (erro = sqrt(variância))
plt.figure(figsize=(10, 6))
plt.errorbar(
    estatisticas["n_procs"],
    estatisticas["media_tme"],
    yerr=estatisticas["variancia_tme"]**0.5,  # Desvio padrão
    fmt="o-", capsize=5, color="blue", ecolor="red", elinewidth=1.5
)

plt.title(f"TME por Quantidade de Processos ({nome_escalonador})")
plt.xlabel("Número de Processos")
plt.ylabel("TME (ms)")
plt.grid(True)
plt.tight_layout()

# Força os ticks do eixo X a serem os valores de número de processos
plt.xticks(estatisticas["n_procs"])

# Define limites do eixo X com um pouco de folga nas pontas
plt.xlim(estatisticas["n_procs"].min() - 5, estatisticas["n_procs"].max() + 5)

# Salva o gráfico
nome_arquivo_saida = f"graficos/grafico_tme_{nome_escalonador.lower()}.png"
plt.savefig(nome_arquivo_saida)
plt.show()
