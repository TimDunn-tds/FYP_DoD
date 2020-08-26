function Tm = motorTorque(Ia)

% tauM_p =@(Ia) (-0.4482 + sqrt(4.816.*Ia - 0.034860))./2.408;
% tauM_n =@(Ia) -(-1.353 + sqrt(-1.322.*Ia + 1.8276))./0.661;
% 
% if Va>0
%     Tm = tauM_p(Ia);
% elseif Va<0
%     Tm = tauM_n(Ia);
% else
%     Tm = 0;
% end

Tm = 0.04629.*Ia.^4 + 0.2721.*Ia.^3 + 0.06547.*Ia.^2 + 1.417.*Ia - 0.082;









% persistent Ia_prev
% if isempty(Ia_prev)
%     Ia_prev = 0;
% end

% if (Ia>= 0 && Ia<= 1 || (sign(Ia)~=sign(Ia_prev))) && abs(w_m)<=0.15 
%     Tm = 0;
% elseif (Ia<0 && Ia>-0.8 || (sign(Ia)~=sign(Ia_prev))) && abs(w_m)<=0.15
%     Tm = 0;
% else
%     Tm = Kt.*Ia - B.*w_m;
% end
% Ia_prev = Ia;


% if (Ia>= 0 && Ia<= 1 && abs(w_m)<=0.15) || abs(Va)<0.4
%     Tm = 0;
% elseif (Ia<0 && Ia>-0.8  && abs(w_m)<=0.15) || abs(Va)<0.4
%     Tm = 0;
% else
%     Tm = Kt.*Ia - B.*w_m;
% end

