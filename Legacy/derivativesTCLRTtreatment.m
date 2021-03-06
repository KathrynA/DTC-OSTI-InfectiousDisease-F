%{
Copyright 2013 Ronja Woloszczuk

This file is part of DTC-OSTI-InfectiousDisease-F.

DTC-OSTI-InfectiousDisease-F is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DTC-OSTI-InfectiousDisease-F is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DTC-OSTI-InfectiousDisease-F. If not, see <http://www.gnu.org/licenses/>.
%}

function [ dy ] = derivativesTCLRTtreatment( t, y, param )
%Calculates the derivatives for each equation in the target cell limited model.
%In the model cells are subjected to reverse transcriptase treatment (RTT).
%To put the model into a suitable form for MATLAB, each variable has been
%listed as a component in the solution vector y as follows:
%
% T = y(1) = Concentration of target cells
% I = y(2) = Concentration of infected cells
% V = y(3) = Serum virus concentration

% equations 17-19 in the report

dy=zeros(3,1);
dy(1) = param.s - param.d*y(1)- (1-param.RTT)*param.beta*y(1)*y(3);
dy(2) = (1-param.RTT)*param.beta*y(1)*y(3)- param.delta*y(2);
dy(3) = param.p*y(2)-param.c*y(3);

end
