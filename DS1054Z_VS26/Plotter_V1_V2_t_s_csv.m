clc;
close all;
clear;

M = dlmread('data.csv',';');  % читаем все числа, разделитель — ';'

V1 = M(:,1);
V2 = M(:,2);
t  = M(:,3);

figure;
plot(t, V1);
hold on;
plot(t, V2);
grid on;
xlabel('t, c');
ylabel('Напряжение, В');