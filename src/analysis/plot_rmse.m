data = readmatrix('../../log.txt');

t_log = data(:,1);
x_hat = data(:,2);
y_hat = data(:,3);

x_true = 100 + 5 * t_log;
y_true = 200 + 3 * t_log;

rmse_instant = sqrt((x_hat - x_true).^2 + (y_hat - y_true).^2);

figure('Position', [300, 200, 1000, 600]);
plot(t_log, rmse_instant, 'm*-', 'LineWidth', 1.5);
xlabel('Zaman (s)');
ylabel('RMSE (metre)');
title('Zamana Göre Anlık RMSE');
grid on;
xlim([0 50]);
