%Copyright (C) 2005  Martin-Wasserhardt
%This program is free software; you can redistribute it and/or
%modify it under the terms of the GNU General Public License
%as published by the Free Software Foundation; either version 2
%of the License, or (at your option) any later version.

%This program is distributed in the hope that it will be useful,
%but WITHOUT ANY WARRANTY; without even the implied warranty of
%MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%GNU General Public License for more details.


% ## -*- texinfo -*-
% ## @deftypefn {Function File} {@var{L} = } watershed (@var{A},@var{conn})
% ## Find image watershed regions
% ##
% ## L = watershed(A,conn) computes a label matrix identifying the watershed
% ## regions of the input matrix @var{A}, which must be a grayscale 2-D
% ## image. The elements of @var{L} are integer values greater than or equal
% ## to 0. The elements labeled 0 do not belong to a unique watershed
% ## region. These are called watershed pixels. The elements labeled 1
% ## belong to the first watershed region, the elements labeled 2 belong to
% ## the second watershed region, and so on. You must specify a
% ## connectivity @var{conn} which can have the following values :
% ## @var{conn} = 4 : 4-connected neighborhood
% ## @var{conn} = 8 : 8-connected neighborhood
% ## @end deftypefn
% 
% ## Author: Julien MARTIN <martinj@hotmail.fr> & Stephane WASSERHARDT <chewbizz@hotmail.com>
% ## Created: March 2005

function [L]=watershed2(A, conn)

A = round(A);

s=size(A);

L=zeros(s(1),s(2))-1;

error = 0;

% Parameters check
if (size(A,3)==3)
	print('Watershed only work with 2-D grayscale images (and not with intensity images)\n');
	error = 1;
end
if (conn ~= 4)&(conn ~= 8)
	print('Connectivity must be 4 or 8\n');
	error = 1;	
end

if (error == 0)

	couleur=1;
	
	% Get min and max levels of the input matrix
	min_A=min(min(A));
	max_A=max(max(A));
	
	% For each levels in the input image
	for level=min_A:max_A
		
		% Get pixels coordinates in the current level
		[XA YA] = find(A == level);
		
		% Examines each pixel of the current level
		while (size(XA) > 0)
			
			% Get next pixel x and y coordinates
			x = XA(1);
			y = YA(1);
			XA = XA(2:size(XA));
			YA = YA(2:size(YA));
				
			% Pixel state :
			% -1 <=> unknown
			% 0 <=> watershed pixel
			% n > 0 <=> pixel into region #n
			etat = -1;
			
			% Pick up pixel's neighbors
			% With 4-connectivity
			if (conn==4)
				for i=-1:2:1
					if (i~=0)
						if ((x + i > 0)&(x + i < s(1) + 1))
							% Update pixel's state according to the neighbor
							val = L(x + i, y);
							% If the neighbor pixel already has a state
							if (val > 0)
								% If the current pixel has no old state
								if (etat == -1)
									% The current pixel takes the state of the neighbor
									etat = val;
								else
									% If the old state of the pixel is different from the neighbor's state
									if (val ~= etat)
										% It becomes a watershed pixel
										etat = 0;
									end
								end
							end
						end
						if ((y + i > 0)&(y + i < s(2) + 1))
							% Update pixel's state according to the neighbor
							val = L(x, y + i);
							% If the neighbor pixel already has a state
							if (val > 0)
								% If the current pixel has no old state
								if (etat == -1)
									% The current pixel takes the state of the neighbor
									etat = val;
								else
									% If the old state of the pixel is different from the neighbor's state
									if (val ~= etat)
										% It becomes a watershed pixel
										etat = 0;
									end
								end
							end
						end
					end
				end
			% With 8-connectivity
			else
				for i=-1:1
					for j=-1:1
						if ((i~=0)|(j~=0))
							if ((x + i > 0)&(x + i < s(1) + 1)&(y + j > 0)&(y + j < s(2) + 1))
								% Update pixel's state according to the neighbor
								val = L(x + i, y + j);
								% If the neighbor pixel already has a state
								if (val > 0)
									% If the current pixel has no old state
									if (etat == -1)
										% The current pixel takes the state of the neighbor
										etat = val;
									else
										% If the old state of the pixel is different from the neighbor's state
										if (val ~= etat)
											% It becomes a watershed pixel
											etat = 0;
										end
									end
								end
							end
						end
					end
				end
			end
				
			% Update label matrix according pixel's state :
			if (etat >= 0)
				% The pixel belongs to an existing region or is a watershed pixel
				L(x, y) = etat;
			else
				% The pixel belongs to a new region
				L(x, y) = couleur;
				couleur = couleur + 1;
			end
	
		end       
	
	end

end
