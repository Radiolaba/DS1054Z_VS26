clc;
close all;
clear;

% читаем CSV: 3 столбца (V1, V2, t), разделитель — ';'
M  = dlmread('data.csv',';');
V1 = M(:,1);
dV1=fft(V1);
V2 = M(:,2);
dV2=fft(V2);
t  = M(:,3);



figure;
x = V1;          % сигнал
Fs = 50000000;        % частота дискретизации, Гц
N = length(x);

X = fft(x);
P2 = abs(X/N);            % двухсторонний спектр
P1 = P2(1:floor(N/2)+1);   % односторонний спектр
P1(2:end-1) = 2*P1(2:end-1);

f = Fs*(0:floor(N/2))/N;  % ось частот в Гц

plot(f, P1)
xlabel('f (Hz)')
ylabel('|P1(f)|')
grid on
% --------- График CH1 ---------
figure;
plot(t, V1);
grid on;
xlabel('t, c');
ylabel('U_1, В');
title('Channel 1');

% --------- График CH2 ---------
figure;
plot(t, V2);
grid on;
xlabel('t, c');
ylabel('U_2, В');
title('Channel 2');