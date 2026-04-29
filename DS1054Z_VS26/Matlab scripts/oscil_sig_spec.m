function res = oscil_sig_spec(files, Nticks, empty_ticks, fmin, fmax, Nf, head, tail, pic_x, pic_y, font, line, marker)
    freqs = linspace(fmin, fmax, Nf);
    Nfiles = max(size(files));
    legends = files;
    colors = IMMIcolorsGPT();
    dt = 16*1d-3; % in microseconds

    IMMI2024(pic_x, pic_y, font, line, marker);
    set(gcf,'color','w');
    for j = 1: Nfiles
        d = load(files{j});
        for i = 1:Nticks
            dd(i,1) = dt*i;
            dd(i,2) = d(empty_ticks + i);
        end
        
        dd_tail_rows = dd(:,1) > head & dd(:,1) < tail;
        dd_tail = dd(dd_tail_rows, :);
        
        dd_spec = spline_spectrum(dd_tail(:,1), dd_tail(:,2), freqs);
        plot(freqs, abs(dd_spec)/max(abs(dd_spec)), 'color', colors{j})
        hold on;
    end
    legend(legends)
    hold off;
    figure();
    plot(dd(:,1), dd(:,2), dd_tail(:,1), dd_tail(:,2), '--');
    res = 'done';
end