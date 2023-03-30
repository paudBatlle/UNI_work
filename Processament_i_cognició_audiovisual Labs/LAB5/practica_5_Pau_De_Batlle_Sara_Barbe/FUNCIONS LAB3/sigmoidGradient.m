function g = sigmoidGradient(z)
%SIGMOIDGRADIENT returns the gradient of the sigmoid function
%evaluated at z
%   g = SIGMOIDGRADIENT(z) calcula la derivada de la funcion sigmoide evaluada
%   en z. Tiene que funcionar independientemente de que z sea un vector o 
%   una matriz, deberia devolver la derivada para cada elemento. 

g = zeros(size(z));

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the gradient of the sigmoid function evaluated at
%               each value of z (z can be a matrix, vector or scalar).
 g = sigmoid(z).*(1-sigmoid(z));


% =============================================================

end
