clc;
close all;
clear all;

L = 7.5; % [cm]
n = 8; % number of sections

dx = 0.001;

% Input Points
%     x_points = [0 2   4 6 10 18];
%     y_points = [8 7.5 7 6 4 0];
    
%     x_points = [0:0.01:7.5];
%     y_points = (7.5^2 - x_points.^2).^0.5;
    
    x_points = [0:0.01:L];
    y_points = (L^2 - (x_points-L).^2).^0.5;
    
x_ = 0:dx:L;
y_ = interp1(x_points,y_points,x_,'spline');

figure;
    plot(x_,y_);
    xlabel('x [cm]');
    ylabel('y [cm]');
    
%% S
yp_ = diff(y_)./diff(x_);
yp_(end+1) = yp_(end);
ds_ = dx*(1+yp_.^2).^0.5;
s_ = cumsum(ds_);

figure;
    plot(x_,s_); 
    xlabel('x [cm]');
    ylabel('s [cm]');

%% Section Width
w_ = 2*y_*sin(pi/n);

figure;
    plot(s_,w_/2,s_,-w_/2);
    xlabel('s [cm]');
    ylabel('w [cm]');
    
%% Making the flower
    SW_Up = [s_; w_/2];
    SW_Down = [s_; -w_/2];
    for i = 1:n
        R = [cos((i-1)*2*pi/n) sin((i-1)*2*pi/n); -sin((i-1)*2*pi/n) cos((i-1)*2*pi/n)];
        curveUp(:,:,i) = R*SW_Up;
        curveDown(:,:,i) = R*SW_Down;
    end
    
    figure;
        hold on;
        for i = 1:n
            plot(curveUp(1,:,i),curveUp(2,:,i));
            plot(curveDown(1,:,i),curveDown(2,:,i));
            plot([curveUp(1,end,i),curveDown(1,end,i)],[curveUp(2,end,i),curveDown(2,end,i)]);
            gcf= plot([curveUp(1,end,i),curveDown(1,end,i)],[curveUp(2,end,i),curveDown(2,end,i)]);
            exportgraphics(gcf,'vectorfig.pdf','ContentType','vector')
            axis square
        end
        hold off;
            
        
    

