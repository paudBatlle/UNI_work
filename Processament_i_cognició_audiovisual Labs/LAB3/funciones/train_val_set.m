% Esta funci�n separa los datos en un set de entrenamiento y un set de
% test (o validaci�n)
%
% inputs:
% X : datos de entrada 
%    -filas: num. neuronas de entrada (incluyendo
%    aquellas entradas de unos
%    -columnas: n�mero de muestras totales
% Y : etiquetas (targets) de los datos
%    -filas: n�mero de caracteristicas de las etiquetas
%    -columnas: n�mero de muestras totales
% frac_train es la fraction de datos que ser�n utilizados para
% el entrenamiento (por ej. 0.7 = 70% de los datos van al set de
% entrenamiento)


function [Xtrain, Ytrain, Xval, Yval] = train_val_set(X,Y, frac_train)


num_pattern = size(X, 2);  % n�mero de muestras totales

% primero aleatorizamos el orden de los datos :
rs = randperm(num_pattern);  % randomiza los indices de las muestras
X = X(:,rs);  % reescribe X con esos indices aleatorios
Y = Y(:,rs);  % reescribe Y con esos indices aleatorios

pattern_train = floor(num_pattern * frac_train);  % calcula el numero de muestras de entrenamiento

Xtrain = X(:,1:pattern_train);  % datos de entrada del set de entrenamiento
Ytrain = Y(:,1:pattern_train);  % etiquetas (targets) de entrenamiento

Xval = X(:,pattern_train+1:end);  % datos de entrada del set de test
Yval = Y(:,pattern_train+1:end);  % etiquetas (targets) de test

