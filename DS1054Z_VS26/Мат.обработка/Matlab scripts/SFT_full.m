% SFT_full.m Ч сплайн-спектр по двум каналам, с окном Tukey и нормировкой
clc;
close all;
clear;

% „итаем CSV: 3 столбца (V1, V2, t), разделитель Ч ';'
M = dlmread('data.csv', ';');
V1 = M(:,1);      % ¬
V2 = M(:,2);      % ¬
t  = M(:,3);      % с

% »нтересующий диапазон частот (можно мен€ть)
f_min = 20000;      % √ц
f_max = 200000;     % √ц
Nf    = 600;        % число точек по частоте
fs_vec = linspace(f_min, f_max, Nf);

% --- окно Tukey (опционально: можно отключить, задав alpha = 0) ---
alpha = 0.2;        % 0 Ч пр€моугольное, 1 Ч почти Hann
Nt = length(t);
if alpha > 0
    w_t = tukeywin(Nt, alpha);   % окно по времени
else
    w_t = ones(Nt,1);            % пр€моугольное окно
end

% --- комплексный сплайн-спектр дл€ каждого канала ---
dt = t(2) - t(1);   % шаг дискретизации, с
ci = 1i;
ws = 2*pi*fs_vec;   % угловые частоты

% канал 1: V1
spec_V1 = zeros(1, Nf);
ps1 = V1 .* w_t;        % оконный сигнал
for i = 1:Nf
    s = 0;
    for j = 1:Nt
        s = s + ps1(j) * 2*exp(ci*ws(i)*t(j))/(dt*ws(i)^2)*(1 - cos(dt*ws(i)));
    end
    spec_V1(i) = s;
end

% канал 2: V2
spec_V2 = zeros(1, Nf);
ps2 = V2 .* w_t;        % оконный сигнал
for i = 1:Nf
    s = 0;
    for j = 1:Nt
        s = s + ps2(j) * 2*exp(ci*ws(i)*t(j))/(dt*ws(i)^2)*(1 - cos(dt*ws(i)));
    end
    spec_V2(i) = s;
end

% --- Ќќ–ћ»–ќ¬ ј ---
% ѕриводим спектры к масштабу Ђ¬ольтыї, как в классическом ƒѕ‘:
% интеграл ~ sum(x_j * exp(-i w t_j)) * dt => делим на Nt, чтобы
% амплитуда гармоники была ~ амплитуде сигнала.
norm_factor = dt / Nt;   % dt/Nt: учитываем dt как в интеграле и 1/Nt как в ƒѕ‘ [web:51][web:72]

spec_V1 = spec_V1 * norm_factor;
spec_V2 = spec_V2 * norm_factor;

% --- отображение каналов во времени ---
figure;
plot(t, V1, 'b');
grid on;
xlabel('t, c');
ylabel('U_1, ¬');
title('Channel 1 (time)');

figure;
plot(t, V2, 'r');
grid on;
xlabel('t, c');
ylabel('U_2, ¬');
title('Channel 2 (time)');

% --- фигура дл€ канала 1: |S|, arg(S), Re(S), Im(S) ---
figure;

subplot(4,1,1);
plot(fs_vec, abs(spec_V1), 'b');
grid on;
xlabel('f, √ц');
ylabel('|S_1(f)|, ¬');
title('Channel 1 Ч модуль спектра');

subplot(4,1,2);
plot(fs_vec, angle(spec_V1), 'b');
grid on;
xlabel('f, √ц');
ylabel('arg(S_1(f)), рад');
title('Channel 1 Ч аргумент');

subplot(4,1,3);
plot(fs_vec, real(spec_V1), 'b');
grid on;
xlabel('f, √ц');
ylabel('Re(S_1(f)), ¬');
title('Channel 1 Ч действительна€ часть');

subplot(4,1,4);
plot(fs_vec, imag(spec_V1), 'b');
grid on;
xlabel('f, √ц');
ylabel('Im(S_1(f)), ¬');
title('Channel 1 Ч мнима€ часть');

% --- фигура дл€ канала 2: |S|, arg(S), Re(S), Im(S) ---
figure;

subplot(4,1,1);
plot(fs_vec, abs(spec_V2), 'r');
grid on;
xlabel('f, √ц');
ylabel('|S_2(f)|, ¬');
title('Channel 2 Ч модуль спектра');

subplot(4,1,2);
plot(fs_vec, angle(spec_V2), 'r');
grid on;
xlabel('f, √ц');
ylabel('arg(S_2(f)), рад');
title('Channel 2 Ч аргумент');

subplot(4,1,3);
plot(fs_vec, real(spec_V2), 'r');
grid on;
xlabel('f, √ц');
ylabel('Re(S_2(f)), ¬');
title('Channel 2 Ч действительна€ часть');

subplot(4,1,4);
plot(fs_vec, imag(spec_V2), 'r');
grid on;
xlabel('f, √ц');
ylabel('Im(S_2(f)), ¬');
title('Channel 2 Ч мнима€ часть');
