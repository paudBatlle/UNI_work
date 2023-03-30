function E = calculaError(x,w,Y)

E =  0.5*mean((w'*x-Y).^2);