function [W1_grad, W2_grad] = gradientNN_class( W1, W2, x, y )
% Take as parameters the weights of the first and second layers, the inputs and
% the targets.
% Returns the gradient corresponding to the weights of the first and second
% layer.

% Inputs:
% W1: matrix between the input layer and the hidden layer (watch out: W1 includes
% the bias)
% W2: matrix between the hidden layer and the output (W2 includes bias)
% x: input vector (1 sample)
% y: tags

% W1_grad = zeros(size(W1));
% W2_grad = zeros(size(W2));

% =========================================================================

% Calculate activation for hidden neurons:
z_h = W1*x;  % sum before the sigmoid
a_h = sigmoid(z_h);  % activation of hidden neurons
a_h = [ones(size(y)); a_h];  % add the neuron whose value is always equal to one and that serves as bias to the next layer

% Calculate activation of the output neuron:
z_out = W2*a_h;  % sum before the output neuron response function
a_out = sigmoid(z_out);  % calculate output neuron response (regression: identity - classification: logistic sigmoid)

% We calculate the delta term of the output neuron 
ds = sigmoidGradient(z_out);
dEda = (a_out - y)./(a_out.*(1-a_out));
d_out = dEda.*ds;
d_out(ds==0)=0;  

% We propagate the delta of the output towards the hidden layer
d_h = (W2(2:end)'*d_out) .* sigmoidGradient(z_h);  % delta term hidden layer

% Gradient:
%-----------
% Instead of getting the gradient in vector form, let's write it
% with the dimensions of W1 and W2. For example:
%
%           dE/dw11  dE/dw12
%           dE/dw21  dE/dw22
% W1_Grad = dE/dw31  dE/dw32
%           dE/dw41  dE/dw42
% 

% Calculate the gradient with respect to W2:
W2_grad = a_h'*d_out;
% Calculate the gradient with respect to W1:
W1_grad = d_h*x';% Add terms

% -------------------------------------------------------------

% =========================================================================

end
