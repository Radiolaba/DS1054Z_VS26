clc;
close all;
clear;

% частотное окно отображения
f_min = 20000;         % начальная частота, Гц
f_max = 80000;         % конечная частота, Гц
Nf    = 400;           % число точек по частоте
fs_vec = linspace(f_min, f_max, Nf); % ЧАСТОТЫ, по которым БЕЖИМ

% чтение данных
M  = dlmread('chirp_small_100_cut.csv',';');   % [V1, V2, t]
V1 = M(:,1);
t  = M(:,3);

dt = mean(diff(t));
Fs = 1/dt;
N  = length(V1);

% при необходимости убрать DC
% V1 = V1 - mean(V1);

ci = 1i;
spec_freq_hann = zeros(1, Nf);  % сюда пишем S(f_k)

% центр временного интервала
t0  = (t(1) + t(end))/2;
tau = t - t0;   % симметричное время относительно центра

% нормировка как интеграл ~ sum * dt, плюс 1/N, как в ДПФ
norm_factor = dt / N;

for k = 1:Nf    % ВНЕШНИЙ ЦИКЛ ПО ЧАСТОТАМ f_k
    f = fs_vec(k);
    w = 2*pi*f;

    % ---- Hann-окно ДЛЯ ЭТОЙ ЧАСТОТЫ f ----
    % задаём число периодов в окне:
    cycles = 5;            % можно менять
    T_win  = cycles / f;   % длительность окна по времени

    % безразмерное время относительно окна
    u = tau / T_win;       % u = -0.5..0.5 внутри окна

    % форма Hann: 0.5*(1+cos(2*pi*u)) на [-0.5, 0.5], иначе 0
    w_k = 0.5*(1 + cos(2*pi*u));
    w_k(abs(u) > 0.5) = 0;

    % оконный сигнал для этой частоты
    x_win = V1 .* w_k;

    % ---- СПЕКТР ДЛЯ ЭТОЙ ЧАСТОТЫ ----
    s = sum( x_win .* exp(-ci*w*t) );  % сумма по j (времени)
    spec_freq_hann(k) = s * norm_factor;
end

A_hann = abs(spec_freq_hann);   % амплитудный спектр в В

figure;
plot(fs_vec, A_hann, 'LineWidth', 1);
grid on;
xlabel('f, Гц');
ylabel('|S(f)|, В');
title('Канал 1: спектр с частотно-зависимым окном Hann');
xlim([f_min f_max]);

figure;
plot(t, V1);
grid on;
xlabel('t, s');
ylabel('U_1, V');
title('Channel 1');