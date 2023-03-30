function show_f(f)
    figure;
    imagesc(fftshift(normalize(logvis(f))));
    colormap gray;
end