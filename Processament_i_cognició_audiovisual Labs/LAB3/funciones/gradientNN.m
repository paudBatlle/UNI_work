function [W1_grad, W2_grad] = gradientNN( W1, W2, x, y )
% Toma como parametros los pesos de la primera y segunda capa, los inputs y
% los targets.
% Devuelve el gradiente correspondiente a los pesos de la primera y segunda
% capa.

% Inputs:
% W1: matriz entre la capa de entrada y la capa oculta (cuidado: W1 incluye
% el sesgo)
% W2: matriz entre la capa oculta y la salida (W2 incluye el sesgo)
% x: vector de inputs (1 muestra)
% y: etiqueta

% =========================================================================

% calcula activacion para las neuronas ocultas:
z_h = W1*x;  % suma antes de la sigmoide
a_h = sigmoid(z_h);  % activaci�n de las neuronas ocultas
a_h = [ones(size(y)); a_h];  % agrega la neurona cuyo valor es siempre igual a uno y que sirve de sesgo a la siguiente capa

% calcula activacion de la neurona de salida:
z_out = W2*a_h;  % suma antes de la funci�n de respuesta de la neurona de salida
a_out = z_out;  % calcula respuesta neurona de salida (regresion: identidad -- clasificacion: sigmoide logistica)

% Calculamos el termino delta de la neurona de salida 
d_out = (a_out - y);  

% Propagamos el delta de la salida hacia la calpa oculta
d_h = (W2(2:end)'*d_out) .* sigmoidGradient(z_h);  % termino delta capa oculta

% Gradiente:
%-----------
% En vez de obtener el gradiente en forma de vector, vamos a escribirlo
% con las dimensiones de W1 y W2. Por ejemplo:
%
%           dE/dw11  dE/dw12
%           dE/dw21  dE/dw22
% W1_Grad = dE/dw31  dE/dw32
%           dE/dw41  dE/dw42
% 

% Calcula el gradiente con respecto a W2:
W2_grad = a_h'*d_out ;
% Calcula el gradiente con respecto a W1:
W1_grad = [x(1)*d_h , x(2)*d_h] ;

% -------------------------------------------------------------

% =========================================================================

end
