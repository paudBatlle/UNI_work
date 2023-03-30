function E = GetError(W1,W2,x,y)
    
num_pattern = size(x, 2);
e = zeros(1,num_pattern);

for p = 1:num_pattern  % pattern loop       
    xp = x(:,p); % entrada para la muestra p
    yp = y(p);   % target para la muestra p
    
    % calcula activacion para las neuronas ocultas:
    z_h = W1*xp;  % suma antes de la sigmoide
    a_h = sigmoid(z_h);  % activaci�n de las neuronas ocultas
    a_h = [ones(size(yp)); a_h];  % agrega la neurona cuyo valor es siempre igual a uno y que sirve de sesgo a la siguiente capa

    % calcula activacion de la neurona de salida:
    z_out = W2*a_h;  % suma antes de la funci�n de respuesta de la neurona de salida
    a_out = z_out;  % calcula respuesta neurona de salida (regresion: identidad -- clasificacion: sigmoide)

    % Calculamos la diferencia cuadrada para esta muestra:
    e(p) = sum((a_out-yp).^2);    % En este caso MSE como en P2, para clasificacion sera diferente
end  
    
    
E = 1/2*mean(e);
