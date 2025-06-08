import sys
import os
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Lista de arquivos e nomes dos escalonadores
arquivos_e_escalonadores = [
    ("resultados_fifo.txt", "FIFO"),
    ("resultados_sjf.txt", "SJF"),
    ("resultados_ljf.txt", "LJF"),
    ("resultados_prio_static.txt", "PRIO_STATIC"),
    ("resultados_prio_dynamic.txt", "PRIO_DYNAMIC"),
    ("resultados_prio_dynamic_quantum.txt", "PRIO_DYNAMIC_QUANTUM"),
]

plt.figure(figsize=(10, 6))

cores = ["blue", "green", "orange", "purple", "brown", "red"]
for idx, (nome_arquivo, nome_escalonador) in enumerate(arquivos_e_escalonadores):
    dados = []
    with open(nome_arquivo, "r") as f:
        for linha in f:
            if "TME:" in linha:
                partes = linha.strip().split()
                tme = float(partes[1])
                n_procs = int(partes[2])
                dados.append((n_procs, tme))

    df = pd.DataFrame(dados, columns=["n_procs", "tme"])
    estatisticas = df.groupby("n_procs").agg(
        media_tme=("tme", "mean"),
        variancia_tme=("tme", "var")
    ).reset_index()

    plt.errorbar(
        estatisticas["n_procs"],
        estatisticas["media_tme"],
        yerr=np.sqrt(estatisticas["variancia_tme"]),
        fmt="o-",
        capsize=4,
        label=nome_escalonador,
        color=cores[idx % len(cores)]
    )

# Personalização do gráfico
plt.title("Comparação do TME por Algoritmo de Escalonamento")
plt.xlabel("Número de Processos")
plt.ylabel("TME (ms)")
plt.grid(True)
plt.legend()
plt.xticks(estatisticas["n_procs"])
plt.xlim(estatisticas["n_procs"].min() - 5, estatisticas["n_procs"].max() + 5)
plt.tight_layout()

# Cria pasta se necessário
os.makedirs("graficos", exist_ok=True)
plt.savefig("graficos/comparacao_tme.png")
plt.show()
