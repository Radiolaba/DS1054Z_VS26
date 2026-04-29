ch1 = load('ch1.txt');  % Вольты
t   = load('ch2.txt');  % секунды

plot(t, ch1);
xlabel('t, с');
ylabel('Напряжение, В');
grid on;