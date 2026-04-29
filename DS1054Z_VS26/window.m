clc;
close all;
clear;

% Параметры диапазона частот
f_min = 20000;   % Начальная частота, Гц
f_max = 200000;  % Конечная частота, Гц
m = 10;          % Количество циклов для усреднения

% Читаем CSV
M  = dlmread('data.csv',';');
V1 = M(:,1);
t  = M(:,3);

% Вычисляем частоту дискретизации
Fs = 1/mean(diff(t));
N_total = length(V1);

% Проверяем, достаточно ли данных для усреднения
if N_total < m
    error('Недостаточно данных для усреднения по %d циклам. Доступно точек: %d', m, N_total);
end

% Длина одного цикла для усреднения
N_cycle = floor(N_total / m);

if N_cycle < 2
    error('Слишком много циклов (%d) для усреднения. Уменьшите m.', m);
end

fprintf('Усреднение по %d циклам, длина цикла: %d точек\n', m, N_cycle);

% --------- 1. Усреднение БЕЗ окна ---------
X1_avg_nowindow = zeros(floor(N_cycle/2)+1, 1);

for i = 1:m
    start_idx = (i-1)*N_cycle + 1;
    end_idx = i*N_cycle;
    V1_cycle = V1(start_idx:end_idx);
    
    X1_cycle = fft(V1_cycle) / N_cycle;
    X1_cycle_single = X1_cycle(1:floor(N_cycle/2)+1);
    X1_cycle_single(2:end-1) = 2 * X1_cycle_single(2:end-1);
    
    X1_avg_nowindow = X1_avg_nowindow + abs(X1_cycle_single).^2;
end
X1_avg_nowindow = sqrt(X1_avg_nowindow / m);

% --------- 2. Усреднение С окном Hanning ---------
X1_avg_hann = zeros(floor(N_cycle/2)+1, 1);
hann_window = hanning(N_cycle);

for i = 1:m
    start_idx = (i-1)*N_cycle + 1;
    end_idx = i*N_cycle;
    V1_cycle = V1(start_idx:end_idx);
    
    % Применяем окно ПЕРЕД БПФ
    V1_cycle_windowed = V1_cycle .* hann_window;
    
    X1_cycle = fft(V1_cycle_windowed) / N_cycle;
    X1_cycle_single = X1_cycle(1:floor(N_cycle/2)+1);
    X1_cycle_single(2:end-1) = 2 * X1_cycle_single(2:end-1);
    
    X1_avg_hann = X1_avg_hann + abs(X1_cycle_single).^2;
end
X1_avg_hann = sqrt(X1_avg_hann / m);

% --------- 3. Усреднение С окном Tukey ---------
X1_avg_tukey = zeros(floor(N_cycle/2)+1, 1);
tukey_window = tukeywin(N_cycle, 0.3);

for i = 1:m
    start_idx = (i-1)*N_cycle + 1;
    end_idx = i*N_cycle;
    V1_cycle = V1(start_idx:end_idx);
    
    % Применяем окно ПЕРЕД БПФ
    V1_cycle_windowed = V1_cycle .* tukey_window;
    
    X1_cycle = fft(V1_cycle_windowed) / N_cycle;
    X1_cycle_single = X1_cycle(1:floor(N_cycle/2)+1);
    X1_cycle_single(2:end-1) = 2 * X1_cycle_single(2:end-1);
    
    X1_avg_tukey = X1_avg_tukey + abs(X1_cycle_single).^2;
end
X1_avg_tukey = sqrt(X1_avg_tukey / m);

% Ось частот
f_cycle = Fs*(0:floor(N_cycle/2))/N_cycle;

% Находим индексы для диапазона
idx = find(f_cycle >= f_min & f_cycle <= f_max);
f_range = f_cycle(idx);

X1_nowindow_range = X1_avg_nowindow(idx);
X1_hann_range = X1_avg_hann(idx);
X1_tukey_range = X1_avg_tukey(idx);

% --------- Окно 1: Исходный сигнал и примеры окон ---------
figure;
subplot(4,1,1);
plot(t, V1, 'b-', 'LineWidth', 1.5);
grid on;
ylabel('U_1, В');
title('Исходный сигнал V1');

t_cycle = (0:N_cycle-1) / Fs;
V1_example = V1(1:N_cycle);

subplot(4,1,2);
plot(t_cycle, V1_example, 'k-', 'LineWidth', 1.5);
grid on;
ylabel('U_1, В');
title('Один цикл (без окна)');

subplot(4,1,3);
plot(t_cycle, V1_example .* hann_window, 'r-', 'LineWidth', 1.5);
grid on;
ylabel('U_1, В');
title('Один цикл с окном Hanning');

subplot(4,1,4);
plot(t_cycle, V1_example .* tukey_window, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('t, с');
ylabel('U_1, В');
title('Один цикл с окном Tukey');

% --------- Окно 2: Сравнение спектров ---------
figure;
plot(f_range, X1_nowindow_range, 'k-', 'LineWidth', 2, 'DisplayName', sprintf('Усреднение без окна (%d циклов)', m));
hold on;
plot(f_range, X1_hann_range, 'r-', 'LineWidth', 1.5, 'DisplayName', 'Усреднение + Hanning');
plot(f_range, X1_tukey_range, 'g-', 'LineWidth', 1.5, 'DisplayName', 'Усреднение + Tukey');
grid on;
xlabel('f, Гц');
ylabel('|X_1(f)|, В');
title('Сравнение методов: усреднение с разными окнами');
legend('Location', 'best');
hold off;

% --------- Окно 3: Отдельные графики спектров ---------
figure;
subplot(3,1,1);
plot(f_range, X1_nowindow_range, 'k-', 'LineWidth', 1.5);
grid on;
ylabel('|X_1(f)|, В');
title(sprintf('Спектр: усреднение без окна (%d циклов)', m));

subplot(3,1,2);
plot(f_range, X1_hann_range, 'r-', 'LineWidth', 1.5);
grid on;
ylabel('|X_1(f)|, В');
title('Спектр: усреднение + окно Hanning');

subplot(3,1,3);
plot(f_range, X1_tukey_range, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('|X_1(f)|, В');
title('Спектр: усреднение + окно Tukey');

% Вывод информации
fprintf('Частота дискретизации: %.2f Гц\n', Fs);
fprintf('Диапазон частот: %.2f - %.2f Гц\n', f_min, f_max);
fprintf('Частотное разрешение: %.2f Гц\n', Fs/N_cycle);
fprintf('Длительность одного цикла: %.6f с\n', N_cycle/Fs);