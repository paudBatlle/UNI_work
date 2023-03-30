function [data,data_val,t,t_val] = GetDataimageImported(class1, class2,num_pattern)

cifar=load('data_batch_1.mat');
data_all = double(cifar.data(1:num_pattern,:))';  % transpose
t_all = double(cifar.labels(1:num_pattern,:))';  % transpose
cifar_val=load('data_batch_2.mat');
data_val_all = double(cifar_val.data(1:num_pattern,:))';
t_val_all = double(cifar_val.labels(1:num_pattern,:))';


% training set
t = t_all(t_all==class1 | t_all==class2);
data = data_all(: ,t_all==class1 | t_all==class2);
data = data';
t(t==class1)=1; % attention!
t(t==class2)=0;
t = t';

% validation set
t_val = t_val_all(t_val_all==class1 | t_val_all==class2);
data_val = data_val_all(:, t_val_all==class1 | t_val_all==class2);
data_val = data_val';
t_val(t_val==class1)=1; % attention!
t_val(t_val==class2)=0;
t_val = t_val';


