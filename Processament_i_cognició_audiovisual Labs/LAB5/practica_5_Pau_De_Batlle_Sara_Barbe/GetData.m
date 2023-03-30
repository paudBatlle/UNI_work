function [data,data_val,t,t_val] = GetData(num1,num2)

digits=load('hand_written_digits');
data_all = digits.data.training.inputs;
numbers = eye(10);
for i=1:size(data_all,2)
    for j=1:size(numbers,1)
        if digits.data.training.targets(:,i)==numbers(:,j)
            t_all(i) = j-1;
        end
    end
end
data_val_all = digits.data.validation.inputs;
for i=1:size(data_all,2)
    for j=1:size(numbers,1)
        if digits.data.validation.targets(:,i)==numbers(:,j)
            t_val_all(i) = j-1;
        end
    end
end

% training set
t = t_all(:,t_all==num1 | t_all==num2);
data = data_all(:,t_all==num1 | t_all==num2);
data = data';
t(t==num1)=1; % attention!
t(t==num2)=0;
t = t';

% validation set
t_val = t_val_all(:,t_val_all==num1 | t_val_all==num2);
data_val = data_val_all(:,t_val_all==num1 | t_val_all==num2);
data_val = data_val';
t_val(t_val==num1)=1;  %attention!
t_val(t_val==num2)=0;
t_val = t_val';

