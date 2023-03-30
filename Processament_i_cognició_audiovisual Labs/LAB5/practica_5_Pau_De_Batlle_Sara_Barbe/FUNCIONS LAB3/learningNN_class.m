function [E, E_val, W1, W2, ct] = learningNN_class( x, y, x_val, y_val, hidden_units, alpha, max_iter )
tic
output = 1;  % number of output units

%%%%%%%% CLASSIFICATION (same principle than SERIAL) %%%%%%%%%

% 1. Random initialisation of the weights in each layer
% Same principles for parallel, serial and classification
% learning
W1 = 10*randn(hidden_units,size(x,1)); % weights of layer 1
W2 = 10*randn(output,hidden_units+1); % weights of layer 2

E = zeros(1,max_iter); % initialise the variable to store training error
E_val = zeros(1,max_iter); % initialise the variable to store validation error

num_pattern = size(x, 2);
minibatch_size = num_pattern/10; %MINIBATCH
flag=0;
%2.Iteration loop - will be different for parallel, serial and classification
% for loop going through all the iterations
for i = 1:max_iter   
    
    % randomly disrupt the patterns in each iteration (like serial)
    rnd_order = randperm(num_pattern);
    x = x(:,rnd_order);  % RANDOMIZE order at each step
    y = y(rnd_order);  % need to RANDOMIZE also y to preserve correspondence
    
    %3. Pattern loop
    % W1 and W2 be updated for each pattern with the values of its gradient (instead of using the average as we have done so far)
    % for loop going through all the patterns
    % In practice 2, we calculated the gradient for all the patterns at
    % once, but it's better to go one by one
    for p = 1:minibatch_size  
        [DW1, DW2] = gradientNN_class(W1, W2, x(:,p), y(p));
        if any(isnan([DW1(:);DW2(:)])), sprintf('break!'), flag=1; break; end
        % 4. Gradient Descent:
        W1 = W1 - alpha * DW1; % actualisation of the weights
        W2 = W2 - alpha * DW2; % actualisation of the weights
    end
    
    if flag==1 
        display(x(:,p)), display(y(p)), break,
    end
    % Calculate the error for trainset and testset:     
    E(i) = GetError_class(W1,W2,x,y); %weights and x,y from training set
    E_val(i) = GetError_class(W1,W2,x_val,y_val); %weights and x,y from validation set
    if E(i)<0.0001
    sprintf('stop after %g iterations (err < tol)',i);   
    break
    end
end

ct = toc;

