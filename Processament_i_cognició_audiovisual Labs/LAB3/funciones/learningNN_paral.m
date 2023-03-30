function [E, E_val, W1, W2, ct] = learningNN_paral( x, y, x_val, y_val, hidden_units, alpha, max_iter )

% Actualizaremos los pesos y calcularemos el error para cada iteracion utilizando un bucle,
% asi como para cada patron utilizando un segundo bucle anidado. 
tic

output = 1;  % number of output units

% 1. Random initialisation of the weights in each layer
% Same principles for parallel, serial and classification
% learning
W1 = 5*randn(hidden_units,size(x,1)); % weights of layer 1
W2 = 5*randn(output,hidden_units+1); % weights of layer 2

E = zeros(1,max_iter); % initialise the variable to store training error
E_val = zeros(1,max_iter); % initialise the variable to store validation error

num_pattern = size(x, 2);

%2.Iteration loop - will be different for parallel, serial and classification
% for loop going through all the iterations
for i = 1:max_iter  % loop through iterations
    incr1 = zeros(size(W1));
    incr2 = zeros(size(W2));

    %3. Pattern loop (MEAN over the iterations)
    % for loop going through all the patterns
    % In practice 2, we calculated the gradient for all the patterns at
    % once, but it's better to go one by one
    for p = 1:num_pattern  
        [DW1, DW2] = gradientNN(W1, W2, x(:,p), y(p));
        incr1 = incr1 + DW1;  % sum of the gradient in respect of W1 for each pattern
        incr2 = incr2 + DW2;  % sum of the gradient in respect of W2 for each pattern
    end
    
    % 4.Gradient Descent:
    W1 = W1 - alpha*incr1/num_pattern; % actualizaci�n de los pesos usando las dos matrices que hemos calculado y alpha
    W2 = W2 - alpha*incr2/num_pattern; % actualizaci�n de los pesos
    
    % calcula el error para trainset y testset:
    % lo mismo para parallel, classification y serial learning pero depende
    % del tipo de calcError function (ques diferente para classification)
    E(i) = GetError(W1,W2,x,y);
    E_val(i) = GetError(W1,W2,x_val,y_val);
        
end

ct = toc;

