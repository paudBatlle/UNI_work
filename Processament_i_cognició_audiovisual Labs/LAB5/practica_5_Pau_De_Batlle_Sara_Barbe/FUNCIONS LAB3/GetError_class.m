function E = GetError_class(W1,W2,x,y)
    
num_pattern = size(x, 2);
e = zeros(1,num_pattern);

for p = 1:num_pattern  % pattern loop       
    xp = x(:,p); % sample entry p
    yp = y(p);   % target for the sample p
    
    % Calculate activation for hidden neurons:
    z_h = W1*xp;  % sum before the sigmoid
    a_h = sigmoid(z_h);  % activation of hidden neurons
    a_h = [ones(size(yp)); a_h];  % add the neuron whose value is always equal to one and that serves as bias to the next layer

    % Calculate activation of the output neuron:
    z_out = W2*a_h;  % sum before the output neuron response function
    a_out = sigmoid(z_out);  % calculate output neuron response (regression: identity - classification: logistic sigmoid)
    % We calculate the cross-entropy for this sample:
    e(p) = -mean(yp.*log(a_out)+(1-yp).*log(1-a_out)); %fulfil equation    
end  
    
    
E = nanmean(e);
