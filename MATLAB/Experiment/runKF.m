function [mup,Pp] = runKF(muptm1, u, y, Pptm1, KF, t)

% Extract parameters
Cm      = KF.Cm;
Dm      = KF.Dm;
Bod     = KF.Bod;
Qo      = KF.Qo;
Ro      = KF.Ro;
Aod     = KF.Aod;

% % Update with new measurement
% mup(:,t)    = mup(:,t-1) + Pp(:,:,t-1)*Cm'*((Cm*Pp(:,:,t-1)*Cm' + Ro)\(y - Cm*mup(:,t-1) - Dm*u));
% Pp(:,:,t)   = Pp(:,:,t-1) - Pp(:,:,t-1)*Cm'*((Cm*Pp(:,:,t-1)*Cm' + Ro)\(Cm*Pp(:,:,t-1)));
% 
% % Predict Forward
% mup(:,t+1) = Aod*mup(:,t) + Bod*u;
% Pp(:,:,t+1) = Aod*Pp(:,:,t)*Aod' + Qo;



% n = length(Aod);
% 
% % Optimal Kalman Gain
% Kk = Pp(:,:,t-1)*Cm.'/(Cm*Pp(:,:,t-1)*Cm.' + Ro);
% 
% % Correct prediction using measurement
% % muf = muptm1 + Kk*(y - Cm*muptm1 - Dm*u);
% mup(:,t) = mup(:,t-1) + Kk*(y - Cm*mup(:,t-1));
% 
% % Compute new covariance
% Pp(:,:,t) = (eye(n) - Kk*Cm)*Pp(:,:,t-1)*(eye(n) - Kk*Cm).' + Kk*Ro*Kk.';
% 
% % Predict forward
% mup(:,t+1) = Aod*mup(:,t);
% 
% % Update prediction covariance
% Pp(:,:,t+1) = Aod*Pp(:,:,t)*Aod.' + Qo;


n = length(Aod);

% Optimal Kalman Gain
Kk = Pptm1*Cm.'/(Cm*Pptm1*Cm.' + Ro);

% Correct prediction using measurement
% muf = muptm1 + Kk*(y - Cm*muptm1);
muf = muptm1 + Kk*(y - Cm*muptm1 - Dm*u);

% Compute new covariance
Pf = (eye(n) - Kk*Cm)*Pptm1*(eye(n) - Kk*Cm).' + Kk*Ro*Kk.';

% Predict forward
mup = Aod*muf + Bod*u;
% mup = Aod*muf;

% Update prediction covariance
Pp = Aod*Pf*Aod.' + Qo;



