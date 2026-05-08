clc;
close all;
clear;

% Ïàğàìåòğû äèàïàçîíà ÷àñòîò
f_min = 20000;   % Íà÷àëüíàÿ ÷àñòîòà, Ãö
f_max = 200000;  % Êîíå÷íàÿ ÷àñòîòà, Ãö

% ×èòàåì CSV: 3 ñòîëáöà (V1, V2, t), ğàçäåëèòåëü — ';'
M  = dlmread('data.csv',';');
V1 = M(:,1);  % Ñèãíàë âîçäåéñòâèÿ (âõîä äåëèòåëÿ)
V2 = M(:,2);  % Ñèãíàë ğåàêöèè (ñ ïüåçîïğåîáğàçîâàòåëÿ)
t  = M(:,3);  % Âğåìÿ

% Âû÷èñëÿåì ÷àñòîòó äèñêğåòèçàöèè èç äàííûõ
Fs = 1/mean(diff(t));
N = length(V1);

% Âû÷èñëÿåì ÁÏÔ äëÿ îáîèõ ñèãíàëîâ (êîìïëåêñíûå ñïåêòğû)
X1 = fft(V1) / N;
X2 = fft(V2) / N;

% Îäíîñòîğîííèå ñïåêòğû
X1_single = X1(1:floor(N/2)+1);
X2_single = X2(1:floor(N/2)+1);

% Óäâàèâàåì êîìïëåêñíûå àìïëèòóäû (êğîìå íóëåâîé è íàéêâèñòîâñêîé ÷àñòîòû)
X1_single(2:end-1) = 2 * X1_single(2:end-1);
X2_single(2:end-1) = 2 * X2_single(2:end-1);

% Îñü ÷àñòîò â Ãö
f = Fs*(0:floor(N/2))/N;

% Íàõîäèì èíäåêñû äëÿ çàäàííîãî äèàïàçîíà ÷àñòîò
idx = find(f >= f_min & f <= f_max);

% Âûäåëÿåì äàííûå â çàäàííîì äèàïàçîíå
f_range = f(idx);
X1_range = X1_single(idx);
X2_range = X2_single(idx);

% Ğàçëîæåíèå íà ñîñòàâëÿşùèå äëÿ ñèãíàëà 1
X1_real = real(X1_range);
X1_imag = imag(X1_range);
X1_mag = abs(X1_range);
X1_phase = unwrap(angle(X1_range));

% Ğàçëîæåíèå íà ñîñòàâëÿşùèå äëÿ ñèãíàëà 2
X2_real = real(X2_range);
X2_imag = imag(X2_range);
X2_mag = abs(X2_range);
X2_phase = unwrap(angle(X2_range));

% --------- Îêíî 1: Äåéñòâèòåëüíàÿ è ìíèìàÿ ÷àñòè ñèãíàëà 1 ---------
figure;
subplot(2,1,1);
plot(f_range, X1_real, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('Re[X_1(f)], Â');
title('Äåéñòâèòåëüíàÿ ÷àñòü ñïåêòğà ñèãíàëà âîçäåéñòâèÿ');

subplot(2,1,2);
plot(f_range, X1_imag, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('Im[X_1(f)], Â');
title('Ìíèìàÿ ÷àñòü ñïåêòğà ñèãíàëà âîçäåéñòâèÿ');

% --------- Îêíî 2: Ìîäóëü è àğãóìåíò ñèãíàëà 1 ---------
figure;
subplot(2,1,1);
plot(f_range, X1_mag, 'k-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('|X_1(f)|, Â');
title('Ìîäóëü ñïåêòğà ñèãíàëà âîçäåéñòâèÿ');

subplot(2,1,2);
plot(f_range, X1_phase*180/pi, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('arg[X_1(f)], ãğàä');
title('Àğãóìåíò (ôàçà) ñïåêòğà ñèãíàëà âîçäåéñòâèÿ');

% --------- Îêíî 3: Äåéñòâèòåëüíàÿ è ìíèìàÿ ÷àñòè ñèãíàëà 2 ---------
figure;
subplot(2,1,1);
plot(f_range, X2_real, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('Re[X_2(f)], Â');
title('Äåéñòâèòåëüíàÿ ÷àñòü ñïåêòğà ñèãíàëà ğåàêöèè');

subplot(2,1,2);
plot(f_range, X2_imag, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('Im[X_2(f)], Â');
title('Ìíèìàÿ ÷àñòü ñïåêòğà ñèãíàëà ğåàêöèè');

% --------- Îêíî 4: Ìîäóëü è àğãóìåíò ñèãíàëà 2 ---------
figure;
subplot(2,1,1);
plot(f_range, X2_mag, 'k-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('|X_2(f)|, Â');
title('Ìîäóëü ñïåêòğà ñèãíàëà ğåàêöèè');

subplot(2,1,2);
plot(f_range, X2_phase*180/pi, 'g-', 'LineWidth', 1.5);
grid on;
xlabel('f, Ãö');
ylabel('arg[X_2(f)], ãğàä');
title('Àğãóìåíò (ôàçà) ñïåêòğà ñèãíàëà ğåàêöèè');

% Âûâîä èíôîğìàöèè
fprintf('×àñòîòà äèñêğåòèçàöèè: %.2f Ãö\n', Fs);
fprintf('Äèàïàçîí ÷àñòîò: %.2f - %.2f Ãö\n', f_min, f_max);
fprintf('Êîëè÷åñòâî òî÷åê â äèàïàçîíå: %d\n', length(idx));