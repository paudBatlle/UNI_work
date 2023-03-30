function im_output = uniform_quantizer ( im_input, N)

if (N>1)
    min_value = min(min(im_input));
    max_value = max(max(im_input));
    M = double(max_value - min_value);
    g1=(N-1)/M;
    g2=255/(N-1);
    im_output = round( g1 * ( im_input - min_value ) ) * g2 ;
else
    error('The number of levels must be >1');
end
end