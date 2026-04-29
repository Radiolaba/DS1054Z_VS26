clc;
close all;
clear;

% Параметры диапазона частот
f_min = 5000;   % Начальная частота, Гц
f_max = 100000;  % Конечная частота, Гц

% Читаем CSV: 3 столбца (V1, V2, t), разделитель — ';'
M  = dlmread('data.csv',';');
V1 = M(:,1);  % Сигнал воздействия (вход делителя)
V2 = M(:,2);  % Сигнал реакции (с пьезопреобразователя)
t  = M(:,3);  % Время

% Вычисляем частоту дискретизации из данных
Fs = 1/mean(diff(t));
N = length(V1);

% Вычисляем БПФ для обоих сигналов (комплексные спектры)
X1 = fft(V1) / N;
X2 = fft(V2) / N;

% Односторонние спектры
X1_single = X1(1:floor(N/2)+1);
X2_single = X2(1:floor(N/2)+1);

% Удваиваем комплексные амплитуды (кроме нулевой и найквистовской частоты)
X1_single(2:end-1) = 2 * X1_single(2:end-1);
X2_single(2:end-1) = 2 * X2_single(2:end-1);

% Ось частот в Гц
f = Fs*(0:floor(N/2))/N;

% Находим индексы для заданного диапазона частот
idx = find(f >= f_min & f <= f_max);

% Выделяем данные в заданном диапазоне
f_range = f(idx);
X1_range = X1_single(idx);
X2_range = X2_single(idx);

% Вычисляем частотную функцию H(f) = X2(f) / X1(f)
H_range = X2_range ./ X1_range;

% Разложение на составляющие для сигнала 1
X1_real = real(X1_range);
X1_imag = imag(X1_range);
X1_mag = abs(X1_range);
X1_phase = unwrap(angle(X1_range));

% Разложение на составляющие для сигнала 2
X2_real = real(X2_range);
X2_imag = imag(X2_range);
X2_mag = abs(X2_range);
X2_phase = unwrap(angle(X2_range));

% Разложение на составляющие для частотной функции
H_real = real(H_range);
H_imag = imag(H_range);
H_mag = abs(H_range);
H_phase = unwrap(angle(H_range));

% --------- Окно 1: Действительная и мнимая части сигнала 1 ---------
figure;
subplot(2,1,1);
plot(f_range, X1_real, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('Re[X_1(f)], В');
title('Действительная часть спектра сигнала воздействия');

subplot(2,1,2);
plot(f_range, X1_imag, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('Im[X_1(f)], В');
title('Мнимая часть спектра сигнала воздействия');

% --------- Окно 2: Модуль и аргумент сигнала 1 ---------
figure;
subplot(2,1,1);
plot(f_range, X1_mag, 'k-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('|X_1(f)|, В');
title('Модуль спектра сигнала воздействия');

subplot(2,1,2);
plot(f_range, X1_phase*180/pi, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('arg[X_1(f)], град');
title('Аргумент (фаза) спектра сигнала воздействия');

% --------- Окно 3: Действительная и мнимая части сигнала 2 ---------
figure;
subplot(2,1,1);
plot(f_range, X2_real, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('Re[X_2(f)], В');
title('Действительная часть спектра сигнала реакции');

subplot(2,1,2);
plot(f_range, X2_imag, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('Im[X_2(f)], В');
title('Мнимая часть спектра сигнала реакции');

% --------- Окно 4: Модуль и аргумент сигнала 2 ---------
figure;
subplot(2,1,1);
plot(f_range, X2_mag, 'k-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('|X_2(f)|, В');
title('Модуль спектра сигнала реакции');

subplot(2,1,2);
plot(f_range, X2_phase*180/pi, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('arg[X_2(f)], град');
title('Аргумент (фаза) спектра сигнала реакции');

% --------- Окно 5: Действительная и мнимая части частотной функции ---------
figure;
subplot(2,1,1);
plot(f_range, H_real, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('Re[H(f)]');
title('Действительная часть частотной функции');

subplot(2,1,2);
plot(f_range, H_imag, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('Im[H(f)]');
title('Мнимая часть частотной функции');

% --------- Окно 6: Модуль и аргумент частотной функции ---------
figure;
subplot(2,1,1);
plot(f_range, H_mag, 'k-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('|H(f)|');
title('Модуль частотной функции');

subplot(2,1,2);
plot(f_range, H_phase*180/pi, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('f, Гц');
ylabel('arg[H(f)], град');
title('Аргумент (фаза) частотной функции');

% Вывод информации
fprintf('Частота дискретизации: %.2f Гц\n', Fs);
fprintf('Диапазон частот: %.2f - %.2f Гц\n', f_min, f_max);
fprintf('Количество точек в диапазоне: %d\n', length(idx));