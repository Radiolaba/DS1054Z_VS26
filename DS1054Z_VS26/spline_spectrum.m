function spec = spline_spectrum(ts, ps, fs)
    Nt = max(size(ts));
    Nf = max(size(fs));
    dt = ts(2) - ts(1);
    ci = complex(0,1);
    ws = 2*pi*fs;
    for i = 1: Nf
        spec(i) = 0;
        for j = 1: Nt
            spec(i) = spec(i) + ps(j)*2*exp(ci*ws(i)*ts(j))/(dt*ws(i)^2)*(1 - cos(dt*ws(i)));
        end
    end
end