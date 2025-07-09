import pandas as pd
import matplotlib.pyplot as plt

# Lê os dados (tab-separated)
df = pd.read_csv("resultados.txt", sep="\t")

# Converte colunas para os tipos corretos
df["frames"] = df["frames"].astype(int)
df["mem_refs"] = df["mem_refs"].astype(int)
df["page_faults"] = df["page_faults"].astype(int)
df["pages_written"] = df["pages_written"].astype(int)

# Paleta de cores (opcional)
cores = {
    "fifo": "blue",
    "lru": "green",
    "sc": "orange",
    "lfu": "purple",
    "mfu": "red",
    "random": "gray"
}

# Frames ordenados como strings
frames = sorted(df["frames"].unique())
frames_str = [str(f) for f in frames]
algoritmos = df["algoritmo"].unique()
n_alg = len(algoritmos)

# Gera gráficos para cada trace
for trace in df["trace"].unique():
    df_trace = df[df["trace"] == trace]

    # PAGE FAULTS
    plt.figure(figsize=(12, 6))
    for i, alg in enumerate(algoritmos):
        dados = df_trace[df_trace["algoritmo"] == alg]
        x_indices = [frames_str.index(str(f)) + i * (0.8 / n_alg) for f in dados["frames"]]
        plt.bar(x_indices, dados["page_faults"], width=0.8 / n_alg,
                label=alg, color=cores.get(alg, None))

    plt.title(f"Page Faults - {trace}")
    plt.xlabel("Frames")
    plt.ylabel("Page Faults")
    plt.xticks(ticks=range(len(frames)), labels=frames_str)
    plt.legend(title="Algoritmo")
    plt.tight_layout()
    plt.savefig(f"page_faults_{trace}.png")
    plt.close()

    # PAGES WRITTEN
    plt.figure(figsize=(12, 6))
    for i, alg in enumerate(algoritmos):
        dados = df_trace[df_trace["algoritmo"] == alg]
        x_indices = [frames_str.index(str(f)) + i * (0.8 / n_alg) for f in dados["frames"]]
        plt.bar(x_indices, dados["pages_written"], width=0.8 / n_alg,
                label=alg, color=cores.get(alg, None))

    plt.title(f"Pages Written - {trace}")
    plt.xlabel("Frames")
    plt.ylabel("Pages Written on Disk")
    plt.xticks(ticks=range(len(frames)), labels=frames_str)
    plt.legend(title="Algoritmo")
    plt.tight_layout()
    plt.savefig(f"pages_written_{trace}.png")
    plt.close()

print("Gráficos salvos com sucesso!")
