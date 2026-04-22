clc;
close all;
clear;

% читаем CSV: 3 столбца (V1, V2, t), разделитель — ';'
M  = dlmread('data.csv',';');
V1 = M(:,1);
V2 = M(:,2);
t  = M(:,3);

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