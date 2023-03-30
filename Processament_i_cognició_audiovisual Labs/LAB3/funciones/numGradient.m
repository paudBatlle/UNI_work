function numDWW = numGradient( WW, hidden_units, x, t )

numDWW = zeros(size(WW));
perturb = zeros(size(WW));
e = 1e-4;
m = size(x,2);
for q=1:length(WW)
    perturb(q)=e;
    WW_sup = WW+perturb;
    WW_inf = WW-perturb;
    W_sup1 = reshape(WW_sup(1:hidden_units * m), m, hidden_units);
    W_sup2 = reshape(WW_sup(1+hidden_units * m:end), hidden_units+1, 1);
    W_inf1 = reshape(WW_inf(1:hidden_units * m), m, hidden_units);
    W_inf2 = reshape(WW_inf(1+hidden_units * m:end), hidden_units+1, 1);
    numDWW(q) = ( calcErrorNN(W_sup1, W_sup2, x, t) - calcErrorNN(W_inf1, W_inf2, x, t) ) / (2*e);
    perturb(q) = 0;
end

end
   