%STABILITY ANALYSIS: Calculates the Jacobian of the target-cell-limited
%model and determines if eigenvalues are imaginary.
function [R_0, eigVal] = EigenvaluesTCL(param);
param.betadash = (param.beta * param.p)/ param.c;

R_0 = param.betadash*param.s/(param.d*param.delta)
%want to put the value of R on the screen

Jacobian = [-param.betadash*param.s/param.delta, -param.delta; ...
            -param.betadash*param.s/param.delta - param.d, 0];
        
[eigVec, eigVal] = eig(Jacobian);


if(real(eigVal(1,1)) < 0 && real(eigVal(2,2)) < 0)
    stability=1;
    disp('This is a point of stability!');
else
    stability=0;
    disp('This is not a point of stability!');
end

if(imag(eigVal(1,1)) ~= 0)
    disp('(1,1) is an imaginary eigenvalue')
else
    disp('Not imaginary');
end

    
if(imag(eigVal(2,2)) ~= 0)
    disp('(2,2) is an imaginary eigenvalue')
else
    disp('Not imaginary');
end
end

    
    
        

