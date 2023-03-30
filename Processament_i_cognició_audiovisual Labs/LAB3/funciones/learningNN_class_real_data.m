function [E, E_val, W1, W2, ct] = learningNN_class_real_data( x, y, x_val, y_val, hidden_units, alpha, max_iter )
tic
output = 1;  % number of output units
% initialization of the parameters
W1 = 10*randn(hidden_units,size(x,1)); % weights of the layer 1
W2 = 10*randn(output,hidden_units+1); % weights of the layer 2

E = zeros(1,max_iter);
E_val = zeros(1,max_iter);

num_pattern = size(x, 2);
minibatch_size = num_pattern;
flag=0;
for i = 1:max_iter  % loop through iterations
    
    rnd_order = randperm(num_pattern);
    x = x(:,rnd_order);  % randomize order at each step
    y = y(rnd_order);  % need to randomize also y to preserve correspondence

    for p = 1:minibatch_size  % loop through the samples (or patterns)
        [DW1, DW2] = gradientNN_class_real_data(W1, W2, x(:,p), y(p));
        if any(isnan([DW1(:);DW2(:)])), sprintf('break!'), flag=1; break; end
        % Gradient Descent
        W1 = W1 - alpha * DW1; % actualisation of the weights
        W2 = W2 - alpha * DW2; % actualisation of the weights
    end
    
    if flag==1 
        display(x(:,p)), display(y(p)), break,
    end
    % Calculate the error for trainset and testset:    
    E(i) = GetError_class(W1,W2,x,y);
    E_val(i) = GetError_class(W1,W2,x_val,y_val);
    if E(i)<0.0001
    sprintf('stop after %g iterations (err < tol)',i);    
    break
    end
end

ct = toc;



