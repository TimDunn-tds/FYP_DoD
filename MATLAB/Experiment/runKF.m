function [mup,Pp] = runKF(muptm1, u, y, Pptm1, KF, plantModel)
% Extrat parameters
Cm      = plantModel.Cm;
Dm      = plantModel.Dm;
Bod     = KF.Bod;
Qo      = KF.Qo;
Ro      = KF.Ro;
Aod     = KF.Aod;

% Update with new measurement
muf = muptm1 + Pptm1*Cm'*((Cm*Pptm1*Cm' + Ro)\(y - Cm*muptm1 - Dm*u));
Pf = Pptm1 - Pptm1*Cm'*((Cm*Pptm1*Cm' + Ro)\(Cm*Pptm1));

% Predict Forward
mup = Aod*muf + Bod*u;
Pp = Aod*Pf*Aod' + Qo;