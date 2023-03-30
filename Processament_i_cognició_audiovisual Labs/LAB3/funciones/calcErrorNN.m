function J = calcErrorNN( W1, W2, x, t )

a_h = x*W1;  % calcula activacion para las neuronas ocultas
z_h = sigmoid(a_h);  % calcula respuesta de las neuronas ocultas
z_h = [ones(size(t)), z_h];  % añade bias a z_h
a_out = z_h*W2;  % calcula activacion para la neurona de output
z_out = a_out;  % calcula respuesta neurona de output (regresion: identidad -- clasificacion: sigmoide logistica)

% Ahora calculamos el error en la salida 
e_out = z_out - t;  % termino delta capa de output

J = 0.5*mean((e_out).^2); % first compute the mean over all samples

