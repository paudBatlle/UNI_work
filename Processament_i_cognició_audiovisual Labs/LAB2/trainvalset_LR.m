% toma en input los datos y crea un conjunto de entrenamiento y un conjunto de validación
% data tiene que ser una matriz con los patrones en la primera dimensión y
% las caracteristicas en la segunda dimensión. La última columna de la
% matriz data tiene que ser el target.

function [Xtrain, Ytrain, Xval, Yval] = trainvalset_LR(X,Y, frac_train)

num_pattern = length(X);  % numero de muestras

pattern_train = floor(num_pattern * frac_train);  % calcula el numero de muestras de entrenamiento
indxrandom = randperm(num_pattern); % crea indices en orden aleatorio (entre 1 y num_pattern)
X = X(indxrandom);  % aleatorizamos el orden de los datos 
Y = Y(indxrandom);  % aleatorizamos el orden de los datos 

Xtrain = X(1:pattern_train);  % catacteristicas (features) de los patrones de entrenamiento
Ytrain = Y(1:pattern_train);  % target de entrenamiento
Xval = X(pattern_train+1:end);  % muestras de validacion
Yval = Y(pattern_train+1:end);  % target de validacion