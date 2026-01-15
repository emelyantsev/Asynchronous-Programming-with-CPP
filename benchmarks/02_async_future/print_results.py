import pandas as pd
import matplotlib.pyplot as plt
import sys

def create_benchmark_plot(csv_file):
    # Google Benchmark в CSV добавляет несколько строк с инфой о системе (Context)
    # Нам нужно найти строку, где начинаются реальные данные
    try:
        # Читаем данные, пропуская строки контекста
        df = pd.read_csv(csv_file, skiprows=9) # Обычно контекст занимает около 10 строк
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        return

    # Очищаем названия (убираем лишние префиксы)
    df['name'] = df['name'].str.replace('BM_', '', regex=False)

    # Конвертируем наносекунды в микросекунды для читаемости
    df['real_time_us'] = df['real_time'] / 1000
    df['cpu_time_us'] = df['cpu_time'] / 1000

    # Настройка стиля
    plt.style.use('seaborn-v0_8-muted')
    fig, ax = plt.subplots(figsize=(8, 5))

    x = range(len(df['name']))
    width = 0.35

    # Рисуем столбцы
    rects1 = ax.bar([i - width/2 for i in x], df['real_time_us'], width, label='Wall Time (μs)', color='#3498db')
    rects2 = ax.bar([i + width/2 for i in x], df['cpu_time_us'], width, label='CPU Time (μs)', color='#e74c3c')

    # Оформление
    ax.set_title('Performance Analysis: Async Patterns vs Thread Pool', fontsize=16, pad=20)
    ax.set_ylabel('Time (microseconds)', fontsize=12)
    ax.set_xticks(x)
    ax.set_xticklabels(df['name'], rotation=15, ha='right')
    ax.legend()
    ax.grid(axis='y', linestyle='--', alpha=0.7)

    # Добавляем цифры над столбцами
    ax.bar_label(rects1, padding=3, fmt='%.0f')
    ax.bar_label(rects2, padding=3, fmt='%.0f')

    plt.tight_layout()
    plt.savefig('benchmark_plot.png', dpi=100)
    print("График успешно сохранен в файл 'benchmark_plot.png'")
    plt.show()

if __name__ == "__main__":
    create_benchmark_plot('results.csv')