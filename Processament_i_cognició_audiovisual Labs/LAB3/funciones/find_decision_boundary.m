function [X,Y,resp] = find_decision_boundary(limits, dec_fnc, num_points)

xv = linspace(limits(1,1), limits(1,2), num_points);
yv = linspace(limits(2,1), limits(2,2), num_points);
[X, Y] = meshgrid(xv, yv);

resp = dec_fnc([X(:)' ;Y(:)']);
resp = reshape(resp, size(X));