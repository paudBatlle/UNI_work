function g = normalize(f)
    g=(f-min(f(:)))/(max(f(:))-min(f(:)));
end