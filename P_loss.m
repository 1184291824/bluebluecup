%����
V_in = 6;
V_out = 3.3;
%V_f = 450e-3;
f_s = 250e3;

%������
I_out_group = [0.1,0.2,0.5,1,1.2,1.5];
P_diode_group = [21.95e-3, 34.94e-3, 94.11e-3, 197.78e-3, 239.85e-3, 302.54e-3];
Duty_cycle_group = [43.89e-2,	58.34e-2,	59.24e-2,	60.68e-2,	61.3e-2, 62.35e-2,];
i_pp_group = [263.98e-3,	353.39e-3,	358.86e-3,	367.57e-3,	371.35e-3,	377.66e-3,];
% Theoretical_efficiency_group = ones(1,6);

%����
%I_out = 1.2;
%P_diode = 239.85e-3;        %Diode power dissipation
%Duty_cycle = 61.30e-2;      %Duty cycle
%i_pp = 371.35e-3;           %Peak-to-peak inductor ripple current

i=6;
for i = 1:6
    I_out = I_out_group(i);
    P_diode = P_diode_group(i);        %Diode power dissipation
    Duty_cycle = Duty_cycle_group(i);      %Duty cycle
    i_pp = i_pp_group(i);           %Peak-to-peak inductor ripple current

    %����
    L = 18e-6;
    R_ds = 180e-3;
    a_sw = 0.25e-9;
    Q_g = 3e-9;
    V_dr = 6;
    ESR_L = 80e-3;
    K_1 = 0.261;
    x = 1.21;
    y = 2.01;
    K_2 = 0.92;
    ESR_Cin = 5e-3;
    ESR_Cout = 25e-3;
    I_u = 116e-6;


    %D = V_out/V_in;
    Duty_cycle_2 = 1-Duty_cycle;
    %i_pp = V_out * D2/(f_s * L);
    a_pp = 1 + ((i_pp/I_out)^2)/12;
    t_sw = a_sw * V_in;



    P_MOS_c(i) = R_ds * Duty_cycle * I_out^2 * a_pp;
    P_MOS_sw(i) = V_in * I_out * f_s * t_sw;
    P_MOS_g(i) = Q_g * V_dr * f_s;
    %P_diode = V_f * D2 * I_out;     %D2 mines D',D'=1-D
    P_L_w(i) = ESR_L * I_out^2 * a_pp;
    P_L_c(i) = (K_1 * (f_s/1000)^x * (K_2 * i_pp)^y)/1000;    % P_L_c��mWΪ��λ������
    P_Cin(i) = ESR_Cin * I_out^2 * Duty_cycle_2 * Duty_cycle;
    P_Cout(i) = 1/12 * ESR_Cout * i_pp^2;
    P_IC(i) = V_in * I_u;

    Ploss(i) = P_MOS_c(i) + P_MOS_sw(i) + P_MOS_g(i) + P_diode + P_L_w(i) + P_L_c(i) + P_Cin(i) + P_Cout(i) + P_IC(i);
    Pout(i) = V_out * I_out;
    Theoretical_efficiency =  Pout(i) / (Pout(i) + Ploss(i)) * 100;
    Theoretical_efficiency_group(i) = Theoretical_efficiency;
end

 

