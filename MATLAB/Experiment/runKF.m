function [mup,Pp] = runKF(mup, u, y, Pp, KF, t)
% Extrat parameters
Cm      = KF.Cm;
Dm      = KF.Dm;
Bod     = KF.Bod;
Qo      = KF.Qo;
Ro      = KF.Ro;
Aod     = KF.Aod;

% Update with new measurement
mup(:,t)    = mup(:,t-1) + Pp(:,:,t-1)*Cm'*((Cm*Pp(:,:,t-1)*Cm' + Ro)\(y - Cm*mup(:,t-1) - Dm*u));
Pp(:,:,t)   = Pp(:,:,t-1) - Pp(:,:,t-1)*Cm'*((Cm*Pp(:,:,t-1)*Cm' + Ro)\(Cm*Pp(:,:,t-1)));

% Predict Forward
mup(:,t+1) = Aod*mup(:,t) + Bod*u;
Pp(:,:,t+1) = Aod*Pp(:,:,t)*Aod' + Qo;


% function [mup,Pp] = runKF(muptm1, u, y, Pptm1, KF, plantModel)
% % Extrat parameters
% Cm      = plantModel.Cm;
% Dm      = plantModel.Dm;
% Bod     = KF.Bod;
% Qo      = KF.Qo;
% Ro      = KF.Ro;
% Aod     = KF.Aod;
% 
% % Update with new measurement
% muf = muptm1 + Pptm1*Cm'*((Cm*Pptm1*Cm' + Ro)\(y - Cm*muptm1 - Dm*u));
% Pf = Pptm1 - Pptm1*Cm'*((Cm*Pptm1*Cm' + Ro)\(Cm*Pptm1));
% 
% % Predict Forward
% mup = Aod*muf + Bod*u;
% Pp = Aod*Pf*Aod' + Qo;