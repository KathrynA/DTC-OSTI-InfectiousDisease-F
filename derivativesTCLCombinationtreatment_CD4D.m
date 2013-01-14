function [ dy ] = derivativesTCLCombinationtreatment_CD4D( t, y, param )
%Calculates the derivatives for each equation in the target cell limited model.
%In the model cells are sujected to a combination of protease (PI) and reverse
%transcriptase treatment (RTT). Protease treatment results in the
%production of infectious virus particles (VI) and non-infectious virus
%particles (VNI).CD4D means that CD4 cells will die at a certain rate (dr) due to the drug
% treatment
%To put the model into a suitable form for MATLAB, each variable has been
%listed as a component in the solution vector y as follows:
%
% T = y(1) = Concentration of target cells
% I = y(2) = Concentration of infected cells
% VI = y(3) = Serum virus concentration (infectious)
% VNI = y(3) = Serum virus concentration (non-infectious)

dy=zeros(4,1);
dy(1) = param.s - (param.dr + param.d)*y(1)- (1-param.RTT)*param.beta*y(1)*y(3);
dy(2) = (1-param.RTT)*param.beta*y(1)*y(3)-(param.dr + param.delta)*y(2);
dy(3) = (1 - param.PI)*param.delta*param.p*y(2)-param.c*y(3);
dy(4) = param.PI*param.delta*param.p*y(2)-param.c*y(4);

end