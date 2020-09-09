%% Clean up
clc; clear;

%% Load parameters
frictParams = getFrictionParams();
motorParams = getMotorParams();

% Extract Parameters
Ra  = motorParams.Ra;
Kw  = motorParams.Kw;
N   = motorParams.N;
p1  = motorParams.p1;
p2  = motorParams.p2;
p3  = motorParams.p3;

a1 = frictParams.a1;
a2 = frictParams.a2;
a3 = frictParams.a3;
B1 = frictParams.B2;
B2 = frictParams.B2;
B3 = frictParams.B3;
a4 = frictParams.a4;
a5 = frictParams.a5;
a6 = frictParams.a6;
B4 = frictParams.B4;
B5 = frictParams.B5;
B6 = frictParams.B6;

% Write C definitions
writeArrayDef(Ra, 'C:\FYP\STM32\workspace\dod\src\Ra.def');
writeArrayDef(Kw, 'C:\FYP\STM32\workspace\dod\src\Kw.def');
writeArrayDef(N, 'C:\FYP\STM32\workspace\dod\src\N.def');
writeArrayDef(p1, 'C:\FYP\STM32\workspace\dod\src\p1.def');
writeArrayDef(p2, 'C:\FYP\STM32\workspace\dod\src\p2.def');
writeArrayDef(p3, 'C:\FYP\STM32\workspace\dod\src\p3.def');
writeArrayDef(a1, 'C:\FYP\STM32\workspace\dod\src\a1.def');
writeArrayDef(a2, 'C:\FYP\STM32\workspace\dod\src\a2.def');
writeArrayDef(a3, 'C:\FYP\STM32\workspace\dod\src\a3.def');
writeArrayDef(B1, 'C:\FYP\STM32\workspace\dod\src\B1.def');
writeArrayDef(B2, 'C:\FYP\STM32\workspace\dod\src\B2.def');
writeArrayDef(B3, 'C:\FYP\STM32\workspace\dod\src\B3.def');
writeArrayDef(a4, 'C:\FYP\STM32\workspace\dod\src\a4.def');
writeArrayDef(a5, 'C:\FYP\STM32\workspace\dod\src\a5.def');
writeArrayDef(a6, 'C:\FYP\STM32\workspace\dod\src\a6.def');
writeArrayDef(B4, 'C:\FYP\STM32\workspace\dod\src\B4.def');
writeArrayDef(B5, 'C:\FYP\STM32\workspace\dod\src\B5.def');
writeArrayDef(B6, 'C:\FYP\STM32\workspace\dod\src\B6.def');



