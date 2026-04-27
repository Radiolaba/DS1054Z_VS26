clc; close all; clear;
files = {"ave3000A9point1.txt", "ave3000A9point2.txt", ...
    "ave3000A9point3.txt", "ave3000A9point4.txt", "ave3000A9point5.txt", ...
    "ave3000A9point6.txt"};
res = oscil_sig_spec(files, 50000, 3000, 0, 3, 1000, 195, 230, 200, 150, 16, 1.2, 5)


% res = oscil_sig_spec(files, Nticks, empty_ticks, fmin, fmax, Nf, head, tail, pic_x, pic_y, font, line, marker)