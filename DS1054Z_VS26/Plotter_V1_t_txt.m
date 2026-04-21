clc;
close all;
clear;

ch1 = load('ch1.txt');   % Вольты CH1 (N x 1)
t   = load('ch2.txt');   % индексы 0..N-1 (N x 1)

figure;
plot(t, ch1);            % U1(t) в условных единицах времени (номер отсчёта)
xlabel('Отсчёт');
ylabel('Напряжение, В');
grid on;