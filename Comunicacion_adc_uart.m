%% --------------------------------------------------------------------- %%
% Programa que comunica al UART con matlab para graficar 6 se�ales del ADC%                                                   
%                                                                         %  
%                    Jorge Adri�n Saloma Hern�ndez                        %
%                                                                         %
%% --------------------------------------------------------------------- %%
clc; clear; close all;
device = serialport("COM9",9600,'timeout',5);
samples = 10000; % Cantidad de muestras
i = 0;
%% --------------------------- Gr�fica ---------------------------------- %
figure('Name', 'Se�ales de 6 canales del ADC', 'NumberTitle',...
       'on','color',[1 1 1]); grid on; hold on;
title('Voltaje de los 6 canales del ADC','FontName','Times New Roman',...
      'FontSize',15);
xlabel('time [s]', 'FontName', 'Times New Roman','FontSize',12);
ylabel('Amplitud', 'FontName', 'Times New Roman','FontSize',12);
xlim([0 100]);
ylim([0 4]);
%% -------------------------- Comunicaci�n ------------------------------ %
while i < samples
    %Para modificar limite en eje x y hacer una grafica movible 
    if mod(i,100) == 0 %Si i es multiplo de 100
       xlim([i i+100]);
    end
    % Se guardan los valores
    dato = readline(device); 
    % Convierte el string dato a valores num�ricos
    num = str2double(dato);
    % Hacer conversi�n
    voltaje(i) = (num*3.3)/4096;
    % Se grafican los datos 
    plot(voltaje, 'k');
    drawnow;
    i = i+1;  
end



