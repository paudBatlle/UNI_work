function [E, E_val, W1, W2, ct] = learningNN_serial( x, y, x_val, y_val, hidden_units, alpha, max_iter )
tic
output = 1;  % number of output units

%%%%%%%% STOCHASTIC (SERIAL) %%%%%%%%%

% 1. Random initialisation of the weights in each layer
% Same principles for parallel, serial and classification
% learning
% inicializaci�n de los par�metros
W1 = 10*randn(hidden_units,size(x,1)); % pesos de la capa 1
W2 = 10*randn(output,hidden_units+1); % pesos de la capa 2

E = zeros(1,max_iter); % initialise the variable to store training error
E_val = zeros(1,max_iter); % initialise the variable to store validation error

num_pattern = size(x, 2);
minibatch_size = num_pattern/10; %MINIBATCH % Para ir m�s r�pido utilizamos s�lo una fracci�n de muestras totales cada vez

%2.Iteration loop - will be different for parallel, serial and classification
% for loop going through all the iterations
for i = 1:max_iter  % loop a trav�s de las iteraciones
    
    % randomly disrupt the patterns in each iteration, thus implementing the stochastic gradient
    rnd_order = randperm(num_pattern);
    x = x(:,rnd_order);  % RANDOMIZE order at each step
    y = y(rnd_order);  % need to RANDOMIZE also y to preserve correspondence
    
    %3. Pattern loop
    % W1 and W2 be updated for each pattern with the values of its gradient (instead of using the average as we have done so far)
    % for loop going through all the patterns
    % In practice 2, we calculated the gradient for all the patterns at
    % once, but it's better to go one by one
    for p = 1:minibatch_size  % loop a trav�s de las muestras (o patterns)
        [DW1, DW2] = gradientNN(W1, W2, x(:,p), y(p));
        %4. Descenso del gradiente:
        W1 = W1 - alpha*DW1 ; % actualizaci�n de los pesos
        W2 = W2 - alpha*DW2; % actualizaci�n de los pesos
    end
     
    % calcula el error para trainset y testset:    
    E(i) = GetError(W1,W2,x,y);
    E_val(i) = GetError(W1,W2,x_val,y_val);
    
    
end

ct = toc;
